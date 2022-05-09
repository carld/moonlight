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
  ModelDrawPreview.C

  Stephane Rehel

  September 7 1997
*/

#include "tools/MLGammaCorrection.h"

#include "ModelGL.h"
#include "ModelDrawPreview.h"
#include "ModelDraw.h"

#include "kmodel/ModelFlags.h"
#include "kmodel/ModelModule.h"

#include "scene/MeshRep.h"
#include "scene/MLMesh.h"
#include "scene/MLCamera.h"
#include "scene/MLScene.h"
#include "scene/renderer/MLRendererOptions.h"

#include "scene/lighting/MLLight.h"
#include "scene/lighting/MLLightTable.h"
#include "scene/lighting/MLLightPool.h"
#include "scene/lighting/MLMeshLighting.h"

#include "scene/material/MLMaterial.h"

/////////////////////////////////////////////////////////////////////////////

static inline GLfloat* setColor4( register const Color& color,
                                  register GLfloat* f )
{
  f[0]= color.r();
  f[1]= color.g();
  f[2]= color.b();
  f[3]= 1.;
  return f;
}

/////////////////////////////////////////////////////////////////////////////

static inline GLfloat* setVector4( register const Vector& vector,
                                   register GLfloat* f )
{
  f[0]= vector.x();
  f[1]= vector.y();
  f[2]= vector.z();
  f[3]= 1.;
  return f;
}

/////////////////////////////////////////////////////////////////////////////

static inline GLfloat* setPoint4( register const Point& point,
                                  register GLfloat* f )
{
  f[0]= point.x();
  f[1]= point.y();
  f[2]= point.z();
  f[3]= 1.;
  return f;
}

/////////////////////////////////////////////////////////////////////////////

ModelDrawPreview::ModelDrawPreview( ModelDraw* _modelDraw ):
  ModelDrawDriver(_modelDraw)
{
  GLint max_lights= 0;
  glGetIntegerv( GL_MAX_LIGHTS, &max_lights );
  maxOpenGLLights= max( int(0), int(max_lights) );

  int i;

  opengl_light_is_also_global= new boolean [ maxOpenGLLights + 1 ];
  for( i= 0; i <= maxOpenGLLights; ++i )
    opengl_light_is_also_global[i]= false;

  opengl_lights_diffuse= new float [ (maxOpenGLLights + 1) * 4 ];
  for( i= 0; i <= maxOpenGLLights; ++i )
    {
    opengl_lights_diffuse[i*4+0]= 0.;
    opengl_lights_diffuse[i*4+1]= 0.;
    opengl_lights_diffuse[i*4+2]= 0.;
    opengl_lights_diffuse[i*4+3]= 1.;
    }

  nOpenGLLights= 0;
}

/////////////////////////////////////////////////////////////////////////////

ModelDrawPreview::~ModelDrawPreview()
{
  delete opengl_light_is_also_global;
  opengl_light_is_also_global= 0;

  delete opengl_lights_diffuse;
  opengl_lights_diffuse= 0;
}

/////////////////////////////////////////////////////////////////////////////

void ModelDrawPreview::clear()
{
  if( model == 0 )
    return;
  MLScene* scene= model->getScene();
  if( scene == 0 )
    return;

  MLGammaCorrection* gc= model->getGammaCorrection();
  MLRendererOptions* options= scene->getRendererOptions();
  double bk_r= gc->do_f(options->backgroundColor.r());
  double bk_g= gc->do_f(options->backgroundColor.g());
  double bk_b= gc->do_f(options->backgroundColor.b());

  glEnable( GL_DEPTH_TEST );
  glDepthMask(GL_TRUE);
  glDisable( GL_LIGHTING );
  glShadeModel(GL_SMOOTH);

  glClearColor( bk_r, bk_g, bk_b, 0. );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

/////////////////////////////////////////////////////////////////////////////

void ModelDrawPreview::setLighting()
{
  nOpenGLLights= 0;

  if( model == 0 )
    return;

  MLScene* scene= model->getScene();

  if( scene == 0 )
    return;

  const MLGammaCorrection* gc= model->getGammaCorrection();

  GLfloat f[4];

  MLLightTable* lightTable= scene->getLightTable();

  if( lightTable->getNLights() == 0 )
    {
    opengl_light_is_also_global[0]= false;

    double default_phi= DEGtoRAD(30.);
    double default_theta= DEGtoRAD(50.);
    Vector default_direction= Vector( cos(default_phi)*cos(default_theta),
                                      sin(default_phi)*cos(default_theta),
                                      sin(default_theta) );
    setVector4(default_direction,f);
    f[3]= 0.;
    glLightfv( GL_LIGHT0, GL_POSITION, f );
    glLightf( GL_LIGHT0, GL_SPOT_CUTOFF, 180. );

    glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION,  1. );
    glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION,    0. );
    glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0. );

    // Color
    f[0]= f[1]= f[2]= 0.;
    f[3]= 1.;
    glLightfv( GL_LIGHT0, GL_AMBIENT, f );

    Color lightColor= Color(1,1,1) * 1.0;

    float* f_diffuse= opengl_lights_diffuse;
    setColor4(lightColor,f_diffuse);
    glLightfv( GL_LIGHT0, GL_DIFFUSE, f_diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, f_diffuse );

    glEnable(GL_LIGHT0);

    nOpenGLLights= 1;
    }
   else
    {
    for( int i= 1; i <= lightTable->getNLights(); ++i )
      {
      if( nOpenGLLights >= maxOpenGLLights )
        break; // unable to set more OpenGL lights!

      MLLight* light= lightTable->getLight(i);
      if( light == 0 )
        continue;
      if( ! light->directLight )
        continue;

      GLenum GL_LIGHTi= GLenum(int(GL_LIGHT0) + nOpenGLLights);

      opengl_light_is_also_global[nOpenGLLights]= light->globalLight;

      const Matrix4x4& o2w= light->getGlobalXForm().get_o2w();

      Point position= o2w.apply(Point(0.,0.,0.));

      MLLight::LightType lt= light->getLightType();

      if( lt == MLLight::DIRECTIONAL )
        {
        Vector direction= o2w.apply(Vector(0., 0.,-1.));
        setVector4(-direction,f);
        f[3]= 0.;
        }
       else
        setPoint4(position,f);
      glLightfv( GL_LIGHTi, GL_POSITION, f );

      if( lt == MLLight::SPOT )
        {
        Vector direction= o2w.apply(Vector(0.,0.,-1.));
        glLightfv( GL_LIGHTi, GL_SPOT_DIRECTION, setVector4(direction,f) );

        glLightf( GL_LIGHTi, GL_SPOT_EXPONENT, light->spot_exponent );
        glLightf( GL_LIGHTi, GL_SPOT_CUTOFF, RADtoDEG(light->spot_cutoff) );
        }
       else
        {
        glLightf( GL_LIGHTi, GL_SPOT_CUTOFF, 180. );
        }

      GLfloat att_c= light->compiled.att_constant;
      GLfloat att_l= light->compiled.att_linear;
      GLfloat att_q= light->compiled.att_quadratic;

      glLightf( GL_LIGHTi, GL_CONSTANT_ATTENUATION,  att_c );
      glLightf( GL_LIGHTi, GL_LINEAR_ATTENUATION,    att_l );
      glLightf( GL_LIGHTi, GL_QUADRATIC_ATTENUATION, att_q );

      // Color
      f[0]= f[1]= f[2]= 0.; f[3]= 1.;
      glLightfv( GL_LIGHTi, GL_AMBIENT, f );

      MLLightPool* pool= light->getPool();
      Color lightColor= light->color & pool->color;
      if( gc->hasCorrection() )
        {
        if( lightColor[0] < 0. ) lightColor[0]= 0.;
        if( lightColor[1] < 0. ) lightColor[1]= 0.;
        if( lightColor[2] < 0. ) lightColor[2]= 0.;
        lightColor[0]= pow( lightColor[0], gc->getOneOverGamma() );
        lightColor[1]= pow( lightColor[1], gc->getOneOverGamma() );
        lightColor[2]= pow( lightColor[2], gc->getOneOverGamma() );
        }
      lightColor *= light->direct_intensity * pool->intensity;

      if( lightColor[0] < 0. ) lightColor[0]= 0.;
      if( lightColor[1] < 0. ) lightColor[1]= 0.;
      if( lightColor[2] < 0. ) lightColor[2]= 0.;
      if( lt == MLLight::DIRECTIONAL )
        lightColor /= 4. * M_PI;

      float* f_diffuse= opengl_lights_diffuse + 4 * nOpenGLLights;
      setColor4(lightColor,f_diffuse);
      glLightfv( GL_LIGHTi, GL_DIFFUSE, f_diffuse );
      glLightfv( GL_LIGHTi, GL_SPECULAR, f_diffuse );

      glEnable(GL_LIGHTi);

      ++nOpenGLLights;
      }
    }

  // This is for combining radiosity results with specular shading
  f[0]= f[1]= f[2]= 1.; f[3]= 1.;
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, f );

  glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER,
//GL_FALSE);
                 modelDraw->camera->ortho ? GL_FALSE : GL_TRUE );
  glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );

  if( nOpenGLLights > 0 )
    {
    glEnable(GL_LIGHTING);

    // disable other lights
    for( int i= nOpenGLLights; i < maxOpenGLLights; ++i )
      glDisable( GLenum(int(GL_LIGHT0) + i) );
    }
   else
    glDisable(GL_LIGHTING);
}

/////////////////////////////////////////////////////////////////////////////

// protected
void ModelDrawPreview::drawMesh( MLMesh* mlmesh )
{
  if( mlmesh == 0 )
    return;

  MLAbstractMesh* abstract= mlmesh->getAbstract();

  if( abstract == 0 )
    return;

  if( (mlmesh->flags & MLObject::SHOW_BOX) != 0 )
    return;

  MeshRep* mesh= abstract->getMeshRep();
  if( mesh == 0 )
    return;

  MLMaterial* m= mlmesh->getMaterial();
  boolean direct_shaded= m->direct_shaded;
  boolean global_contributor= m->rad_contributor;

  GLfloat f[4];
  GLfloat f0[4];
  f0[0]= f0[1]= f0[2]= 0.; f0[3]= 1.;
  if( global_contributor )
    {
    glMaterialfv( GL_BACK, GL_AMBIENT, f0 );
    glMaterialfv( GL_BACK, GL_DIFFUSE, f0 );
    glMaterialfv( GL_BACK, GL_EMISSION, f0 );

    // We are going to map radiosity results on the AMBIENT component
    // (remember: ambient color of scene if [1,1,1,1])
    glColorMaterial( GL_FRONT, GL_AMBIENT );
    glMaterialfv( GL_FRONT, GL_EMISSION, f0 );

    glEnable( GL_COLOR_MATERIAL );
    }
   else
    {
    glDisable( GL_COLOR_MATERIAL );

    glMaterialfv( GL_BACK, GL_AMBIENT, f0 );
    glMaterialfv( GL_BACK, GL_DIFFUSE, f0 );
    glMaterialfv( GL_BACK, GL_EMISSION, f0 );

    glMaterialfv( GL_FRONT, GL_AMBIENT, f0 );
//    glMaterialfv( GL_FRONT, GL_DIFFUSE, setColor4(m->diffuse.get(),f) );

    glColor3f(0,0,0);
    }

  boolean some_light_have_been_disabled= false;

  if( direct_shaded )
    {
    glMaterialfv( GL_FRONT, GL_DIFFUSE, setColor4(m->diffuse.get(),f) );

    if( global_contributor )
      {
      // This material is direct *and* global shaded
      // Let's disable diffuse direct lights (opengl lights) that
      // are also global
      for( int i= 0; i < nOpenGLLights; ++i )
        {
        if( opengl_light_is_also_global[i] )
          {
          GLenum GL_LIGHTi= GLenum(int(GL_LIGHT0) + i);

          // Disable diffuse component of this light, since it has
          // already been shot by the radiosity process
          glLightfv( GL_LIGHTi, GL_DIFFUSE, f0 );

          some_light_have_been_disabled= true;
          }
        }
      }
    }
   else
    glMaterialfv( GL_FRONT, GL_DIFFUSE, f0 );

  glMaterialfv( GL_BACK, GL_SPECULAR, f0 );
  glMaterialf ( GL_BACK, GL_SHININESS, 0. );

  if( m->direct_shaded )
    glMaterialfv( GL_FRONT, GL_SPECULAR, setColor4(m->specular.get(),f) );
   else
    glMaterialfv( GL_FRONT, GL_SPECULAR, f0 );

  glMaterialf ( GL_FRONT, GL_SHININESS, m->specular.shininess );

//if( show_rev )
//  glDisable(GL_CULL_FACE);
// else
  {
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
  }

  MLPolygonArray& polygons= mesh->polygons;
  MLVertexArray& vertices= mesh->vertices;
  MLSVertexArray& svertices= mesh->svertices;

  // compute it if necessary
  const MLSpectrumArray& lighting= mlmesh->getLighting()->getFinalLighting();

  MLGammaCorrection* gc= model->getGammaCorrection();
  const unsigned char* gamma_table= gc->getTable();

  // All normals are already normalized
  double normal_scaling= mlmesh->getNormalScaling();
  boolean do_normal_scaling= false;
  if( normal_scaling == 0. )
    glEnable( GL_NORMALIZE );
   else
    {
    if( normal_scaling != 1. )
      do_normal_scaling= true;

    glDisable( GL_NORMALIZE );
    }

  glBegin(GL_TRIANGLES);

  for( int i= polygons.getSize(); i; --i )
    {
    MLPolygon& poly= polygons[i];
    if( ! poly.allocated() )
      continue;
    if( poly.tesselation != 0 )
      continue; // this polygon has been tesselated

    // well, we eliminate polygons that are not triangles
    // (not tesselated yet, or error during tesselation)
    // Question: should we do that??
    if( poly.nVertices != 3 )
      continue;

    const MLPolygon::Element* poly_elements= poly.elements;
    if( poly_elements == 0 )
      continue;
    for( int j= 0; j < poly.nVertices; ++j, ++poly_elements )
      {
      if( poly_elements->svertex == 0 )
        continue; // svertex hasn't been found for this vertex... Hmm..
      register MLSVertex& svertex= svertices [ poly_elements->svertex ];
      register MLVertex& vertex= vertices [ svertex.vertex ];

      if( global_contributor )
        {
        const Spectrum& s= lighting.get(poly_elements->svertex);
        register unsigned char sr, sg, sb;
        register double c;
        c= s.r(); if( c < 0. ) sr= 0; else
                  if( c > 1. ) sr= 255; else
                  sr= gamma_table[ int(c * 255.) ];
        c= s.g(); if( c < 0. ) sg= 0; else
                  if( c > 1. ) sg= 255; else
                  sg= gamma_table[ int(c * 255.) ];
        c= s.b(); if( c < 0. ) sb= 0; else
                  if( c > 1. ) sb= 255; else
                  sb= gamma_table[ int(c * 255.) ];
        glColor3ub(sr,sg,sb);
        }

      if( do_normal_scaling )
        glNormal( svertex.normal * normal_scaling );
       else
        glNormal( svertex.normal );

      glPoint( vertex.point );
      }
    }

  glEnd(); // GL_TRIANGLES

  if( some_light_have_been_disabled )
    {
    // Re-enable disabled diffuse lights
    for( int i= 0; i < nOpenGLLights; ++i )
      {
      if( opengl_light_is_also_global[i] )
        {
        GLenum GL_LIGHTi= GLenum(int(GL_LIGHT0) + i);
        float* f_diffuse= opengl_lights_diffuse + 4 * i;
        glLightfv( GL_LIGHTi, GL_DIFFUSE, f_diffuse );
        }
      }
    }

  glDisable(GL_CULL_FACE);
  glDisable( GL_COLOR_MATERIAL );
  glDisable( GL_NORMALIZE );
}

/////////////////////////////////////////////////////////////////////////////

void ModelDrawPreview::drawCurve( MLCurve* )
{}

/////////////////////////////////////////////////////////////////////////////

void ModelDrawPreview::drawLight( MLLight* )
{}

/////////////////////////////////////////////////////////////////////////////

void ModelDrawPreview::drawNullObject( MLNullObject* )
{}

/////////////////////////////////////////////////////////////////////////////
