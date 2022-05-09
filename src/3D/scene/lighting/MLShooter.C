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
  MLShooter.C

  Stephane Rehel
  April 5 1997
*/

#include "tools/3D/MLPolygon.h"

#include "scene/MLRayTracer.h"
#include "scene/MLRay.h"
#include "scene/MeshRep.h"

#include "scene/material/MLMaterial.h"

#include "scene/MLObject.h"
#include "scene/MLMesh.h"
#include "scene/MLMeshesIterator.h"
#include "scene/MLChildrenIterator.h"
#include "scene/MLScene.h"

#include "MLLight.h"
#include "MLFormFactor.h"
#include "MLMeshLighting.h"
#include "MLMeshPoolLighting.h"
#include "MLPolygonEnergySource.h"
#include "MLLightingControl.h"

#include "MLShooter.h"

/////////////////////////////////////////////////////////////////////////////

MLShooter::MLShooter( MLScene* _scene, MLLightingControl* _control )
{
  assert( _control != 0 );

  scene= _scene;
  control= _control;
}

/////////////////////////////////////////////////////////////////////////////

MLShooter::~MLShooter()
{}

/////////////////////////////////////////////////////////////////////////////

// return true is completed
boolean MLShooter::shoot( MLLight* light )
{
  if( light == 0 )
    return true;

  if( ! light->globalLight )
    return true;

  return shootHier( light, scene->getRootObject() );

//  return !control->cancelled();
}

/////////////////////////////////////////////////////////////////////////////

// protected
// return true if completed
boolean MLShooter::shootHier( MLLight* light, MLObject* o )
{
  if( o == 0 || light == 0 )
    return true;

  if( o->getNChildren() == 0 )
    {
    if( o->getObjectType() != MLObject::MESH )
      return true;
    return shoot( light, (MLMesh*) o );
    }

  if( ! light->globalLight )
    return true;

  if( o->getNChildren() > 0 )
    {
    // test the object and its hierarchy
    if( light->objectIsUnreachable(o) )
      return true;
    }

  if( o->getObjectType() == MLObject::MESH )
    {
    if( ! shoot( light, (MLMesh*) o ) )
      return false;
    }

  MLChildrenIterator ci(o);
  while( ! ci.eol() )
    {
    if( control->cancelled() )
      return false;

    MLObject* child= ci++;

    shootHier( light, child );
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// not hierarchical
// return true if completed
boolean MLShooter::shoot( MLLight* light, MLMesh* mlmesh )
{
  if( mlmesh == 0 || light == 0 )
    return true;

  if( ! light->globalLight )
    return true;

  if( ! mlmesh->getMaterial()->isRadiosityContributor() )
    return true; // skip this object

  if( light->meshIsUnreachable(mlmesh) )
    return true;

  Spectrum light_energy= light->getEnergy();
  if( light_energy.power() < 1e-30 )
    return true;

  MeshRep* mesh= mlmesh->getMeshRep();
  if( mesh == 0 )
    return true;

//  const Matrix4x4& light_o2w= light->getGlobalXForm().get_o2w();

  MLMeshLighting* lighting= mlmesh->getLighting();
  if( lighting == 0 )
    return true; // ?

  MLMeshPoolLighting* poolLighting= lighting->prepareForShooting(light);

  if( poolLighting == 0 )
    return true; // ??

  const Transform& xform= mlmesh->getGlobalXForm();

  MLSVertexArray& svertices= mesh->svertices;
  int nSVertices= svertices.getSize();

  MLFormFactor* ff= new MLFormFactor [ nSVertices + 1 ];

  MLRay ray;

  MLRayTracer raytracer(scene);

  boolean any_form_factor= false;

  MLMaterial* material= mlmesh->getMaterial();

  boolean do_shadows= light->generate_shadows && material->get_shadows;

  const double four_pi= 4. * M_PI;

  for( int i= 1; i <= nSVertices; ++i )
    {
    if( control->cancelled() )
      {
      delete ff;
      return false;
      }

    MLSVertex& sv= svertices[i];
    if( ! sv.allocated() )
      {
      ff[i]= 0.; // safe
      continue;
      }

    MLVertex& v= mesh->vertices[ sv.vertex ];

    // operating in world coordinates
    Point point= xform.o2w(v.point);

    Vector d;

    if( light->lightType == MLLight::DIRECTIONAL )
      d= light->worldDirection;
     else
      d= point - light->worldPosition;

    Vector normal= xform.normal_o2w(sv.normal);
    double k= -(normal | d);

    if( k <= 0. )
      {
      ff[i]= 0.; // reversed point
      continue;
      }

    double distance2, distance;
    if( light->lightType == MLLight::DIRECTIONAL )
      {
      distance2= distance= 1.;
      }
     else
      {
      distance2= d.norm2();
      distance= sqrt(distance2);

      if( light->has_extent )
        {
        if( distance > light->extent )
          {
          // this vertex is not reached
          ff[i]= 0.;
          continue;
          }
        }
      d /= distance;
      }

    double cos_a= 1.;
    if( light->lightType == MLLight::SPOT )
      {
      cos_a= ( d | light->worldDirection );
      if( cos_a < light->compiled.cos_cutoff )
        {
        ff[i]= 0.;
        continue;
        }
      }

    if( do_shadows )
      {
      ray.origin= point; //light->worldPosition;
      ray.direction= -d;
      ray.skip_reversed= true;
      ray.min_t= MLRAY_SMALL_DIST;

      if( light->lightType == MLLight::DIRECTIONAL )
        ray.max_t= MLRAY_HUGE_DIST;
       else
        ray.max_t= distance - MLRAY_SMALL_DIST;

      ray.radiosityLightRay= true;
      if( raytracer.findBlocking(ray) )
        {
        ff[i]= 0.;
        continue;
        }
      if( ray.light_trans.cabs() < 1e-20 )
        {
        ff[i]= 0.;
        continue;
        }
//***
k *= ray.light_trans.getGray();
      }

    if( light->lightType == MLLight::SPOT )
      k *= pow( cos_a, light->spot_exponent );

    ff[i]= k / (distance * normal.norm() * distance2 * four_pi);
//    if( ff[i] > 1e-20 )
    any_form_factor= true;
    }

  if( any_form_factor )
    poolLighting->directShoot(ff,light_energy);

  delete ff;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return true if completed
boolean MLShooter::shootPES()
{
  MLMeshesIterator mi(scene);

  while( ! mi.eol() )
    if( ! shootPES(mi++) )
      return false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return true if completed
boolean MLShooter::shootPES( MLMesh* mlmesh )
{
  if( mlmesh == 0 )
    return true;

  MLMeshLighting* lighting= mlmesh->getLighting();
  if( lighting == 0 )
    return true;

  MLPolygonEnergySource* pes= lighting->indirectPES;
  if( pes == 0 )
    return true;

  int nSVertices= mlmesh->getSVerticesSize();
  MLPolygon* polygon= pes->getPolygon();

  if( !mlmesh->getMaterial()->isRadiosityContributor() ||
      polygon == 0 ||
      nSVertices < 1 )
    {
    lighting->removeIndirectPES();
    return true;
    }

  Point world_location= pes->world_location;
  Vector world_normal= pes->world_normal;

  if( ! mlmesh->inHalfPlane(world_location,world_normal) )
    {
    lighting->removeIndirectPES();
    return true;
    }

  MeshRep* mesh= mlmesh->getMeshRep();
  if( mesh == 0 )
    return true;

  double area= pes->polygonArea;
  const Transform& xform= mlmesh->getGlobalXForm();

  MLSVertexArray& svertices= mesh->svertices;

  MLFormFactor* ff= new MLFormFactor [ nSVertices + 1 ];

  MLRay ray;
  MLRayTracer raytracer(scene);

  MLMaterial* material= mlmesh->getMaterial();
  boolean transmission_pes= material->transmission.has_transmission;
  boolean do_shadows= material->get_shadows;

  boolean any_form_factor= false;

  for( int i= 1; i <= nSVertices; ++i )
    {
    if( control->cancelled() )
      {
      delete ff;
      return false;
      }
    MLSVertex& sv= svertices[i];
    if( ! sv.allocated() )
      {
      ff[i]= 0.; // safe
      continue;
      }

    MLVertex& v= mesh->vertices[ sv.vertex ];

    Point point= xform.o2w(v.point);
    Vector d= point - world_location;
    double cos1= ( d | world_normal );
    if( cos1 <= 0. )
      {
      if( transmission_pes )
        cos1= -cos1;
       else
        {
        ff[i]= 0.;
        continue;
        }
      }

    Vector normal= xform.normal_o2w(sv.normal);
    double cos2= - ( d | normal );
    if( cos2 <= 0. )
      {
      ff[i]= 0.;
      continue;
      }

    double distance2= d.norm2();

    if( do_shadows )
      {
      register double distance= sqrt(distance2);
      d /= distance;
      ray.origin= point; //world_location;
      ray.direction= -d;
      ray.skip_reversed= true;
      ray.min_t= MLRAY_SMALL_DIST;
      ray.max_t= distance - MLRAY_SMALL_DIST;

      ray.radiosityLightRay= true;
      if( raytracer.findBlocking(ray) )
        {
        ff[i]= 0.;
        continue;
        }
      if( ray.light_ray_blocked || ray.light_trans.cabs() < 1e-20 )
        {
        ff[i]= 0.;
        continue;
        }
//***
cos1 *= ray.light_trans.getGray();
      }

//    double k= area * cos1 * cos2
//             / ( (distance2*M_PI+area) * normal.norm() * distance2 );
    double k=  cos1 * cos2
             / ( (distance2*M_PI+area) * normal.norm() * distance2 );

    ff[i]= k;
    any_form_factor= true;
    }

  if( any_form_factor )
    lighting->indirectShoot(ff,pes);

  delete ff;

  lighting->removeIndirectPES();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return true if completed & a polygon is available
boolean MLShooter::selectShootingPolygon()
{
  MLPolygonEnergySource* pes= new MLPolygonEnergySource(scene);
  pes->mesh= 0;
  ref(pes);

  MLMeshesIterator mi(scene);
  while( ! mi.eol() )
    {
    MLMesh* m= mi++;
    MLMeshLighting* lighting= m->getLighting();
    if( lighting == 0 )
      continue;

    if( control->cancelled() )
      {
      delete pes;
      return false;
      }
    lighting->getShootingPolygon(*pes);
    }

  MLPolygon* polygon= pes->getPolygon();
  if( pes->mesh == 0 || polygon == 0 )
    {
    // no more indirect energy to shoot...
    delete pes;
    return false;
    }

  MLMeshLighting* lighting= pes->mesh->getLighting();

  lighting->retrieveShootingPolygon(pes);

  // set speed-up: world coordinate location & normal of shooting patch
  const Matrix4x4& object_o2w= pes->mesh->getGlobalXForm().get_o2w();
  assert( pes->mesh->getMeshRep() != 0 );
  Point center= polygon->getCenter( pes->mesh->getMeshRep()->vertices.getArray() );
  pes->world_location= object_o2w.apply(center);
  pes->world_normal  = object_o2w.apply(polygon->normal).getNormalized();

  // ok, we've got a shooting polygon
  // let's dispatch it on every object lighting
  mi.reset();
  while( ! mi.eol() )
    {
    MLMesh* m= mi++;
    MLMeshLighting* lighting= m->getLighting();
    if( lighting == 0 )
      continue;

    lighting->setIndirectPES(pes);
    }

  unref(pes);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return true if completed & a polygon has been shot
boolean MLShooter::indirectStep()
{
  if( ! selectShootingPolygon() )
    return false;

  return shootPES();
}

/////////////////////////////////////////////////////////////////////////////

