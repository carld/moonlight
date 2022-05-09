// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

/*
  MLRayRenderer.C

  Stephane Rehel

  January 21 1997
*/

#include <string.h>

#include "tools/MLGammaCorrection.h"

#include "tools/3D/MLSVertex.h"
#include "tools/3D/MLPolygon.h"

#include "MLRayRenderer.h"
#include "MLRendererOptions.h"

#include "scene/MLRay.h"
#include "scene/MLRayTracer.h"

#include "scene/MLCamera.h"

#include "scene/MeshRep.h"
#include "scene/MLObject.h"
#include "scene/MLMesh.h"
#include "scene/MLMeshesIterator.h"

#include "scene/material/MLMaterial.h"
#include "scene/material/MLMaterialsIterator.h"

#include "scene/lighting/MLLight.h"
#include "scene/lighting/MLMeshLighting.h"
#include "scene/lighting/MLLightTable.h"

#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

// For one material, and for one light that is to be actually raytraced
struct MaterialLightSpeedup
{
  MLLight* light;

  boolean compute_diffuse;
  boolean compute_specular;
  boolean use_global_att;

  Color light_diffuse_color;
  Color light_specular_color;

  boolean do_shadows;
};

/////////////////////////////////////////////////////////////////////////////

MLRayRenderer::MLRayRenderer( MLScene* _scene,
                              const MLCamera& _camera,
                              boolean use_camera_resolution /* = false */ )
{
  scene= _scene;
  options= scene->getRendererOptions();

  camera= new MLCamera;
  *camera= _camera;

  own_buffer= true;
  buffer= 0;

  if( use_camera_resolution )
    {
    xres= camera->x_res;
    yres= camera->y_res;
    }
   else
    {
    xres= options->xres;
    yres= options->yres;
    }

  xres= max( 8, xres );
  yres= max( 8, yres );

  if( ! use_camera_resolution )
    {
    camera->x_res= xres;
    camera->y_res= yres;
    }

  camera->compute();

  x1= 0;
  y1= 0;
  x2= xres - 1;
  y2= yres - 1;

  raytracer= new MLRayTracer(scene);

  do_radiosity_shadows= false;

  nRaytracedLights= 0;
  matlights= 0;
  nMatLights= 0;

  gammaCorrection= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLRayRenderer::~MLRayRenderer()
{
  if( scene != 0 && do_radiosity_shadows )
    {
    // if we asked for the indirect final lighting, restore the full
    // lighting (in case we save just before)
    MLMeshesIterator mi(scene);
    while( ! mi.eol() )
      {
      MLMeshLighting* lighting= (mi++)->getLighting();
      lighting->getFinalLighting(false);
      }
    }

  delete raytracer;
  raytracer= 0;

  scene= 0;
  options= 0;

  if( own_buffer )
    delete buffer;
  buffer= 0;

  xres= yres= 0;

  if( matlights != 0 )
    {
    for( int i= 0; i < nMatLights; ++i )
      delete matlights[i];

    delete matlights;
    matlights= 0;
    }

  nMatLights= 0;

  delete nRaytracedLights;
  nRaytracedLights= 0;

  delete gammaCorrection;
  gammaCorrection= 0;
}

/////////////////////////////////////////////////////////////////////////////

// optional
void MLRayRenderer::setRenderedArea( int _x1, int _y1, int _x2, int _y2 )
{
  x1= _x1;
  y1= _y1;
  x2= _x2;
  y2= _y2;

  if( x2 < x1 )
    swap(x2,x1);
  if( y2 < y1 )
    swap(y2,y1);

  x1= max( 0, x1 );
  x2= max( 0, x2 );
  x1= min( xres-1, x1 );
  x2= min( xres-1, x2 );

  y1= max( 0, y1 );
  y2= max( 0, y2 );
  y1= min( yres-1, y1 );
  y2= min( yres-1, y2 );
}

/////////////////////////////////////////////////////////////////////////////

// required
void MLRayRenderer::allocBuffer( unsigned char* _buffer)
{
  if( own_buffer )
    delete buffer;

  if( _buffer != 0 )
    {
    buffer= _buffer;
    own_buffer= false;
    }
   else
    {
    buffer= new unsigned char [ xres * yres * 3 ];
    own_buffer= true;
    }

  memset( (void*) buffer, 0, (x2-x1+1) * (y2-y1+1) * 3 );
}

/////////////////////////////////////////////////////////////////////////////

void MLRayRenderer::init()
{
  delete gammaCorrection;
  gammaCorrection= new MLGammaCorrection( options->getGammaCorrection() );

  scene->compileLights();

  do_radiosity_shadows= (options->trace_shadows &&
                         options->trace_global_shadows);

  MLMeshesIterator mi(scene);
  while( ! mi.eol() )
    {
    MLMeshLighting* lighting= (mi++)->getLighting();

    lighting->getFinalLighting(do_radiosity_shadows);
    }

  int nMaterials= 0;
  MLMaterialsIterator mati(scene);
  while( ! mati.eol() )
    if( mati++ != 0 )
      ++nMaterials;

  delete nRaytracedLights;
  nRaytracedLights= new int [ nMaterials + 1 ];

  int i;
  if( matlights != 0 )
    {
    for( i= 0; i < nMatLights; ++i )
      delete matlights[i];
    }
  delete matlights;
  matlights= new MaterialLightSpeedup* [ nMaterials + 1 ];
  nMatLights= nMaterials + 1;

  for( i= 0; i <= nMaterials; ++i )
    matlights[i]= 0;

  MLLightTable* lightTable= scene->getLightTable();
  int nLights= lightTable->getNLights();

  MaterialLightSpeedup* matl= new MaterialLightSpeedup [ nLights ];

  mati.reset();
  int mat_index= -1;
  while( ! mati.eol() )
    {
    ++mat_index;

    MLMaterial* m= mati++;
    if( m == 0 )
      {
      nRaytracedLights[mat_index]= 0;
      continue;
      }

    m->compile();

    int nLights_to_raytrace= 0;

    // Loop on lights
    for( int light_index= 1; light_index <= nLights; ++light_index )
      {
      MLLight* light= lightTable->getLight(light_index);
      if( light == 0 )
        continue;

      MaterialLightSpeedup& mls= matl[nLights_to_raytrace];

      mls.light= light;
      mls.compute_diffuse= true;
      mls.compute_specular= true;
      mls.use_global_att= false;
      mls.light_diffuse_color= Color(0,0,0);
      mls.light_specular_color= Color(0,0,0);

      if( light->globalLight )
        {
        if( m->rad_contributor )
          {
          // Diffuse component has already been computed by the
          // radiosity process
          // Don't compute it again, unless we have to refine radiosity
          // shadows
          if( ! do_radiosity_shadows )
            mls.compute_diffuse= false;
           else
            {
            mls.light_diffuse_color= light->compiled.global_color;
            mls.use_global_att= true;
            }

          if( light->directLight && m->direct_shaded )
            mls.light_specular_color= light->compiled.direct_color;
           else
            mls.compute_specular= false;
          }
         else
          {
          if( light->directLight && m->direct_shaded )
            {
            mls.light_diffuse_color= light->compiled.direct_color;
            mls.light_specular_color= light->compiled.direct_color;
            }
           else
            {
            mls.compute_diffuse= false;
            mls.compute_specular= false;
            }
          }
        }
       else
        {
        if( m->rad_contributor )
          {
          if( light->directLight )
            {
            if( !m->direct_shaded )
              {
              mls.compute_diffuse= false;
              mls.compute_specular= false;
              }
             else
              {
              mls.light_diffuse_color= light->compiled.direct_color;
              mls.light_specular_color= light->compiled.direct_color;
              }
            }
           else
            {
            mls.compute_diffuse= false;
            mls.compute_specular= false;
            }
          }
         else
          {
          if( light->directLight && m->direct_shaded )
            {
            mls.light_diffuse_color= light->compiled.direct_color;
            mls.light_specular_color= light->compiled.direct_color;
            }
           else
            {
            mls.compute_diffuse= false;
            mls.compute_specular= false;
            }
          }
        }

      mls.light_diffuse_color &= m->diffuse.compiled_color;
      mls.light_specular_color &= m->specular.compiled_color;

      if( mls.compute_diffuse )
        {
        if( mls.light_diffuse_color.cabs() < 1e-20 )
          mls.compute_diffuse= false;
        }

      if( mls.compute_specular )
        {
        if( mls.light_specular_color.cabs() < 1e-20 )
          mls.compute_specular= false;
        }

      if( ! mls.compute_diffuse && ! mls.compute_specular )
        continue; // no need to raytrace this light

      mls.do_shadows= options->trace_shadows &&
                      light->generate_shadows &&
                      m->get_shadows;

      ++nLights_to_raytrace;
      }

    nRaytracedLights[mat_index]= nLights_to_raytrace;

    if( nLights_to_raytrace == 0 )
      matlights[mat_index]= 0;
     else
      {
      matlights[mat_index]= new MaterialLightSpeedup [ nLights_to_raytrace ];

      for( int j= 0; j < nLights_to_raytrace; ++j )
        matlights[mat_index][j]= matl[j];
      }
    }

  delete matl;
  matl= 0;

/*
  nGlobalLights= 0;
  nDirectLights= 0;
  int i;

  MLLightTable* lightTable= scene->getLightTable();
  for( i= 1; i <= lightTable->getNLights(); ++i )
    {
    MLLight* light= lightTable->getLight(i);
    if( light == 0)
      continue;
    if( light->globalLight )
      ++nGlobalLights;
    if( light->directLight )
      ++nDirectLights;
    }

  delete globalLights;
  delete directLights;
  globalLights= new MLLight* [ nGlobalLights + 1 ];
  directLights= new MLLight* [ nDirectLights + 1 ];
  int i1= 1;
  int i2= 1;
  for( i= 1; i <= lightTable->getNLights(); ++i )
    {
    MLLight* light= lightTable->getLight(i);
    if( light == 0)
      continue;
    if( light->globalLight )
      {
      globalLights[i1]= light;
      ++i1;
      }

    if( light->directLight )
      {
      directLights[i2]= light;
      ++i2;
      }
    }
*/
}

/////////////////////////////////////////////////////////////////////////////

void MLRayRenderer::init_trace()
{
  ti.screen_point= Point(0,0,0);

  ti.screen_point[2]= 2*(-camera->nearClippingPlane) - 1.0;

  ti.viewport_width= double(xres) + 1e-3;
  ti.viewport_height= double(yres) + 1e-3;
  ti.x_incr= 2./ti.viewport_width;

  ti.ray.origin= camera->location;
  ti.ray.refract_n= 1.;

  ti.gamma_table= gammaCorrection->getTable();

  ti.width= x2 - x1 + 1;
  ti.height= y2 - y1 + 1;

  ti.y= y2;
  ti.j= ti.height-1;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLRayRenderer::trace_next_line()
{
  if( ti.j < 0 )
    return false;

  ti.screen_point[0]= double(   x1+1)*2./ti.viewport_width  - 1.;
  ti.screen_point[1]= double(ti.y +1)*2./ti.viewport_height - 1.;

  unsigned char* pixel= buffer + ti.j * ti.width * 3;

  for( int i= 0; i < ti.width; ++i, ti.screen_point[0] += ti.x_incr )
    {
    Point I= camera->inv_projection.apply(ti.screen_point);
    ti.ray.direction= (I - camera->location).setNormalized();
    ti.ray.skip_reversed= true;
    ti.ray.min_t= camera->nearClippingPlane;
    ti.ray.max_t= MLRAY_HUGE_DIST;
    ti.ray.ray_index= 1;

    compute_eye_ray(ti.ray);

    register double c;

    c= ti.ray.color.r() * 255.;
    if( c < 0. ) *pixel= 0; else if( c > 255. ) *pixel= 255; else
    *pixel= ti.gamma_table[ int(c+.5) ];

    ++pixel;

    c= ti.ray.color.g() * 255.;
    if( c < 0. ) *pixel= 0; else if( c > 255. ) *pixel= 255; else
    *pixel= ti.gamma_table[ int(c+.5) ];

    ++pixel;

    c= ti.ray.color.b() * 255.;
    if( c < 0. ) *pixel= 0; else if( c > 255. ) *pixel= 255; else
    *pixel= ti.gamma_table[ int(c+.5) ];

    ++pixel;
    }

  --ti.j;
  --ti.y;

  return ti.j >= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLRayRenderer::trace()
{
  init_trace();

  while( trace_next_line() );
}

/////////////////////////////////////////////////////////////////////////////

void MLRayRenderer::compute_eye_ray( MLRay& ray )
{
  ray.mesh= 0;
  ray.polygon= 0;
  ray.lightRay= false;

  raytracer->sendInHier(ray,scene->getRootObject());

  if( ray.mesh== 0 )
    {
    ray.color= options->backgroundColor;
    return;
    }

  // Go to world coordinates
  ray.I= ray.mesh->getGlobalXForm().o2w(ray.I);

  // Compute normal
  {
    register MLPolygon::Element* elements= ray.polygon->elements;
    if( elements[0].svertex != 0 &&
        elements[1].svertex != 0 &&
        elements[2].svertex != 0 )
      {
      assert( ray.mesh->getMeshRep() != 0 );
      register MLSVertexArray& svertices= ray.mesh->getMeshRep()->svertices;
      const Vector& n1= svertices[ elements[0].svertex ].normal;
      const Vector& n2= svertices[ elements[1].svertex ].normal;
      const Vector& n3= svertices[ elements[2].svertex ].normal;

      ray.normal= ray.mesh->getGlobalXForm().normal_o2w
                             ( ray.gamma*n1 + ray.alpha*n2 + ray.beta*n3 );
      }
     else
      ray.normal= ray.mesh->getGlobalXForm().normal_o2w
                                                   ( ray.polygon->normal );
    ray.normal.setNormalized();
  }

  MLMaterial* m= ray.mesh->getMaterial();

  Color& color= ray.color;
  color= m->emittance.compiled_color;
//  color.set((BYTE)(sin(ray.I.x())*256),(BYTE)(cos(ray.I.y())*256),(BYTE)(sin(ray.I.z())*256));

  //FIXME: color has to be replaced by texture if given
  //  - needs coordinates, ray contains them (or should)

  if( m->rad_contributor )
    {
    //register MeshRep* mesh= ray.object->getMeshRep();
    //register MLSVertexArray& svertices= mesh->svertices;
    register MLPolygon::Element* elements= ray.polygon->elements;
    register MLMeshLighting* lighting= ray.mesh->getLighting();
    assert( ray.polygon->nVertices == 3 );
    assert( lighting != 0 );

    if( elements[0].svertex != 0 &&
        elements[1].svertex != 0 &&
        elements[2].svertex != 0 )
      {
      register const Spectrum& s1= lighting->peekFinal( elements[0].svertex );
      register const Spectrum& s2= lighting->peekFinal( elements[1].svertex );
      register const Spectrum& s3= lighting->peekFinal( elements[2].svertex );

      color[0]+= ray.gamma*s1.r() + ray.alpha*s2.r() + ray.beta*s3.r();
      color[1]+= ray.gamma*s1.g() + ray.alpha*s2.g() + ray.beta*s3.g();
      color[2]+= ray.gamma*s1.b() + ray.alpha*s2.b() + ray.beta*s3.b();
      }
    }

  // Loop on lights

  MLRay lightRay;
  lightRay.radiosityLightRay= false;

  const double four_pi= 4. * M_PI;

  int nLights_to_raytrace= nRaytracedLights[m->getID()];

  MaterialLightSpeedup* mls= matlights[m->getID()];

  for( int i= 1; i <= nLights_to_raytrace; ++i, ++mls )
    {
    MLLight* light= mls->light;

    double light_distance;
    double diffuse_att,
           specular_att;

    if( light->lightType == MLLight::DIRECTIONAL )
      {
      lightRay.direction= -(light->worldDirection);
      light_distance= MLRAY_HUGE_DIST;
      specular_att= 1.;
//      if( mls->use_global_att )
        diffuse_att= 1. / four_pi;
//       else
//        diffuse_att= specular_att;
      }
     else
      {
      lightRay.direction= light->worldPosition - ray.I;
      double light_distance2= lightRay.direction.norm2();
      if( light_distance2 < MLRAY_SMALL_DIST )
        continue;

      if( light->has_extent && light_distance2 > light->compiled.extent2 )
        continue;

      light_distance= sqrt(light_distance2);

      specular_att= 1. / ( light->compiled.att_constant
                          +light->compiled.att_linear * light_distance
                          +light->compiled.att_quadratic * light_distance2 );

      if( mls->use_global_att )
        diffuse_att= 1. / (four_pi * light_distance2);
       else
        diffuse_att= specular_att;
      lightRay.direction /= light_distance;
      }

    double spot= 1.;
    if( light->lightType == MLLight::SPOT )
      {
      spot= -(lightRay.direction | light->worldDirection);
      if( spot < light->compiled.cos_cutoff )
        continue; // this point is not in the spot cone

      spot= pow( spot, light->spot_exponent );
      }

    if( mls->do_shadows )
      {
      lightRay.skip_reversed= true;
      lightRay.origin= ray.I;
      lightRay.min_t= MLRAY_SMALL_DIST;
      lightRay.max_t= light_distance - MLRAY_SMALL_DIST;

      if( raytracer->findBlocking(lightRay) )
        continue;

      if( lightRay.light_trans.cabs() < 1e-20 )
        continue;
      }
     else
      lightRay.light_trans= Color(1,1,1);

    double k= (lightRay.direction | ray.normal);
    if( k <= 0. )
      continue;

    // Diffuse illumination
    if( mls->compute_diffuse )
      color+= ((diffuse_att * spot * k) * mls->light_diffuse_color)
              & lightRay.light_trans;

    // Specular illumination
    if( mls->compute_specular )
      {
      Vector VPe= (camera->location-ray.I);
      VPe.setNormalized();
      Vector h= lightRay.direction + VPe;
      double s= (ray.normal | h) / h.norm();

      if( s > 1e-10 )
        color += ((specular_att*spot*pow(s,m->specular.shininess))
                    * mls->light_specular_color)
                 & lightRay.light_trans;
      }
    } // lights loop

  if( ! m->has_reflect_or_trans )
    return;

  if( ray.ray_index >= options->maxRayDepth )
    return;

  ++ray.ray_index;

  MLRay ray2;
  ray2.radiosityLightRay= false;

  double norm_coeff= (ray.direction|ray.normal);

  boolean polygon_is_reversed= (ray.polygon_k_parameter > 0.);

  if( m->reflection.has_reflection &&
      !polygon_is_reversed &&
      options->trace_reflections )
    {
    ray2.origin= ray.I;
    ray2.direction= ray.direction + (-2.*norm_coeff)*ray.normal;
    ray2.skip_reversed= true;
    ray2.min_t= MLRAY_SMALL_DIST;
    ray2.max_t= MLRAY_HUGE_DIST;
    ray2.ray_index= ray.ray_index;
    ray2.refract_n= ray.refract_n;

    compute_eye_ray(ray2);

    ray.color += ray2.color & m->reflection.compiled_color;
    }

  if( m->transmission.has_transmission &&
      options->trace_transmissions )
    {
    ray2.origin= ray.I;

    double alpha;
    if( polygon_is_reversed )
      {
      alpha= m->transmission.trans_n;
      ray2.refract_n= 1.; // assume ouside refraction index is 1.
      }
     else
      {
      alpha= ray.refract_n / m->transmission.trans_n;
      ray2.refract_n= m->transmission.trans_n;
      }

    double beta= 1. - alpha * alpha * (1. - norm_coeff * norm_coeff);

    if( beta > 0. )
      {
      if( polygon_is_reversed )
        {
        ray2.direction= (+sqrt(beta)-alpha*norm_coeff) * ray.normal
                       +(alpha * ray.direction);
        }
       else
        ray2.direction= (-sqrt(beta)-alpha*norm_coeff) * ray.normal
                       +(alpha * ray.direction);

//if( fabs(ray2.direction.norm2() - 1.) > 1e-4 )
//printf("%f\n",ray2.direction.norm2());
//      ray2.direction.setNormalized();

      ray2.skip_reversed= false;
      ray2.min_t= MLRAY_SMALL_DIST;
      ray2.max_t= MLRAY_HUGE_DIST;
      ray2.ray_index= ray.ray_index;

      compute_eye_ray(ray2);

      ray.color += ray2.color & m->transmission.compiled_color;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////
