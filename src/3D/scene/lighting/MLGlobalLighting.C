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
  MLGlobalLighting.C

  Stephane Rehel

  March 11 1997
*/

#include "tools/BoundingCone.h"
#include "tools/BoundingCylinder.h"
#include "tools/BoundingHalfPlane.h"

#include "MLGlobalLighting.h"

#include "scene/material/MLMaterial.h"

#include "MLLight.h"
#include "MLLightTable.h"
#include "MLLightPool.h"
#include "MLPoolTable.h"
#include "MLMeshLighting.h"

#include "scene/MLMesh.h"
#include "scene/MLMeshesIterator.h"
#include "scene/MLChildrenIterator.h"
#include "scene/MLObject.h"
#include "scene/MLScene.h"

#include "scene/material/MLMaterial.h"

/////////////////////////////////////////////////////////////////////////////

MLGlobalLighting::MLGlobalLighting( MLScene* _scene )
{
  scene= _scene;
  dirty= false;
  update_shadows= true;
}

/////////////////////////////////////////////////////////////////////////////

MLGlobalLighting::~MLGlobalLighting()
{}

/////////////////////////////////////////////////////////////////////////////

void MLGlobalLighting::object_is_transformed( MLObject* object )
{
  if( object == 0 )
    return;

  if( object->getObjectType() == MLObject::LIGHT )
    {
    light_is_transformed( (MLLight*) object );
    return;
    }

  if( object->getObjectType() != MLObject::MESH )
    return;

  MLMesh* mesh= (MLMesh*) object;

  mesh_shadow_has_changed(mesh);

  dirty_mesh_lighting(mesh);
}

/////////////////////////////////////////////////////////////////////////////

void MLGlobalLighting::dirty_mesh_lighting( MLMesh* mesh )
{
  if( mesh == 0 )
    return;

  MLMaterial* m= mesh->getMaterial();

  if( ! m->isRadiosityContributor() )
    return;

  mesh->getLighting()->setDirty();
}

/////////////////////////////////////////////////////////////////////////////

void MLGlobalLighting::reset_mesh_lighting( MLMesh* mesh )
{
  if( mesh == 0 )
    return;

  MLMaterial* m= mesh->getMaterial();

  if( ! m->isRadiosityContributor() )
    return;

  mesh->getLighting()->reset();
  mesh->getLighting()->setDirty();
}

/////////////////////////////////////////////////////////////////////////////

void MLGlobalLighting::mesh_shadow_has_changed( MLMesh* mesh )
{
  if( ! update_shadows )
    return;

  MLMaterial* mat= mesh->getMaterial();

  if( ! mat->generate_shadows )
    return;

  // if this is not a global contributor, it doesn't generate
  // shadows in the radiosity environment
  if( ! mat->isRadiosityContributor() )
    return;

  const BoundingSphere& obs= mesh->getGlobalBoundingSphere();

  MLPoolTable* poolTable= scene->getPoolTable();
  for( int i= 1; i <= poolTable->getNPools(); ++i )
    {
    MLLightPool* pool= poolTable->getPool(i);
    if( pool == 0 )
      continue;

    for( int j= 1; j <= pool->getNLights(); ++j )
      {
      MLLight* light= pool->getLight(j);
      if( light == 0 )
        continue;
      if( ! light->globalLight )
        continue;

      switch(light->lightType)
        {
        case MLLight::POINT:
          {
          // this is the bounding cone (point light,moved object)
          BoundingCone obc( light->worldPosition, obs );

          BoundingHalfPlane bhp;
          Vector v= (light->worldPosition - obs.getCenter());
          double norm= v.norm();
          if( obs.getRadius() > norm )
            bhp.set( Point(0,0,1e30), Vector(0,0,1) );
           else
            {
            v /= norm;
            bhp.set( obs.getCenter() + v * obs.getRadius(), v );
            }

          update_shadowed_objects( scene->getRootObject(), light, obc, bhp );

          break;
          }

        case MLLight::SPOT:
          {
          // this is the bounding cone (point light,moved object)
          BoundingCone obc( light->worldPosition, obs );
          const BoundingCone& light_cone= light->cone;

          if( light_cone.same_origin_intersect(obc) )
            {
            BoundingHalfPlane bhp;
            Vector v= -(light->worldDirection);
            bhp.set( obs.getCenter() + v * obs.getRadius(), v );

            update_shadowed_objects( scene->getRootObject(),
                                     light,
                                     light_cone, bhp );
            }

          break;
          }

        case MLLight::DIRECTIONAL:
          {
          // this is the bounding cylinder (dir light,moved object)
          BoundingCylinder obc( light->worldDirection, obs );

          BoundingHalfPlane bhp;
          Vector v= -(light->worldDirection);
          bhp.set( obs.getCenter() + v * obs.getRadius(), v );

          update_shadowed_objects( scene->getRootObject(), light, obc, bhp );

          break;
          }

        default:
          break;
        }

      } // lights loop
    } // pools loop
}

/////////////////////////////////////////////////////////////////////////////

// set lighting of the light dirty if object is in the cone
void MLGlobalLighting::update_shadowed_objects( MLObject* object,
                                                MLLight* light,
                                                const BoundingCone& cone,
                                                const BoundingHalfPlane& bhp )
{
  if( object->isBounded() )
    {
    const BoundingSphere& object_bs= object->getGlobalBoundingSphere();
    if( ! cone.intersectSphere( object_bs ) )
      return;
    }

  boolean is_dirty= false;

  if( object->getObjectType() != MLObject::MESH )
    goto hier;

  if( ! ((MLMesh*)object)->getMaterial()->get_shadows )
    goto hier;

  if( object->isRoot() )
    goto hier;

//  if( object->getObjectType() == MLObject::MESH )
    {
    MLMesh* m= (MLMesh*) object;
    MLMeshLighting* lighting= m->getLighting();
    if( lighting == 0 )
      goto hier;

    if( ! lighting->hasContributionOfLight(light) )
      goto hier;

    if( ! object->isBounded() )
      is_dirty= true;
     else
      {
      const BoundingSphere& mesh_bs= object->getObjectBoundingSphere();
      if( bhp.intersect(mesh_bs) )
        {
        if( cone.intersectSphere(mesh_bs) )
          is_dirty= true;
        }
      }

    if( is_dirty )
      {
      m->getLighting()->setDirtyPool(light->getPool());
      }
    }

hier:
  MLChildrenIterator ci(object);
  while( ! ci.eol() )
    update_shadowed_objects( ci++, light, cone, bhp );
}

/////////////////////////////////////////////////////////////////////////////

// set lighting of the light dirty if object is in the cylinder
void MLGlobalLighting::update_shadowed_objects( MLObject* object,
                                                MLLight* light,
                                                const BoundingCylinder& cyl,
                                                const BoundingHalfPlane& bhp )
{
  if( object->isBounded() )
    {
    const BoundingSphere& object_bs= object->getGlobalBoundingSphere();
    if( ! cyl.intersectSphere( object_bs ) )
      return;
    }

  boolean is_dirty= false;

  if( !object->isRoot() && object->getObjectType() == MLObject::MESH )
    {
    MLMesh* m= (MLMesh*) object;
    MLMeshLighting* lighting= m->getLighting();
    if( lighting == 0 )
      goto hier;

    if( ! lighting->hasContributionOfLight(light) )
      goto hier;

    if( ! object->isBounded() )
      is_dirty= true;
     else
      {
      const BoundingSphere& mesh_bs= object->getObjectBoundingSphere();
      if( bhp.intersect(mesh_bs) )
        {
        if( cyl.intersectSphere(mesh_bs) )
          is_dirty= true;
        }
      }

    if( is_dirty )
      m->getLighting()->setDirtyPool(light->getPool());
    }

hier:
  MLChildrenIterator ci(object);
  while( ! ci.eol() )
    update_shadowed_objects( ci++, light, cyl, bhp );
}

/////////////////////////////////////////////////////////////////////////////

void MLGlobalLighting::update_light( MLLight* light )
{
  if( light == 0 )
    return;

  if( ! light->globalLight )
    return;

  set_dirty_pool(light->getPool());
}

/////////////////////////////////////////////////////////////////////////////

void MLGlobalLighting::light_is_transformed( MLLight* light )
{
  if( light == 0 )
    return;

  if( ! light->globalLight )
    return;

  update_light(light);
}

/////////////////////////////////////////////////////////////////////////////

void MLGlobalLighting::change_light_pool( MLLight* light,
                                          MLLightPool* new_pool )
{
  if( light == 0 )
    return;

  MLLightPool* old_pool= light->getPool();

  if( old_pool == new_pool )
    return;

  if( light->globalLight )
    set_dirty_pool(old_pool);

  old_pool->removeLight(light);
  new_pool->addLight(light);

  if( light->globalLight )
    set_dirty_pool(new_pool);
}

/////////////////////////////////////////////////////////////////////////////

void MLGlobalLighting::light_energy_is_changed( MLLight* light )
{
  if( light == 0 )
    return;

  if( ! light->globalLight )
    return;

  MLLightPool* pool= light->getPool();
  if( pool == 0 )
    return;

  if( pool->getNLights() == 1 )
    {
    // oh, it's the only light in this pool
    // Dont need to compute form factors again
    pool->setDirtyFinal();
    return;
    }

  set_dirty_pool( light->getPool() );
}

/////////////////////////////////////////////////////////////////////////////

// e.g. cone angle
void MLGlobalLighting::light_params_are_changed( MLLight* light )
{
  if( ! light->globalLight )
    {
    // this light is not global any more
    MLLightPool* pool= light->getPool();
    if( pool == 0 )
      return;
    pool->setDirty();
    dirty= true;
    return;
    }

  update_light(light);
}

/////////////////////////////////////////////////////////////////////////////

void MLGlobalLighting::pool_energy_is_changed( MLLightPool* pool )
{
  if( pool == 0 )
    return;

  pool->setDirtyFinal();
}

/////////////////////////////////////////////////////////////////////////////

void MLGlobalLighting::material_has_changed( MLMaterial* m )
{
  if( m == 0 )
    return;

  MLMeshesIterator mi(scene);
  while( ! mi.eol() )
    {
    MLMesh* mesh= mi++;

    if( mesh->getMaterial() == m )
      {
      MLMeshLighting* lighting= mesh->getLighting();
      if( lighting != 0 )
        lighting->setDirty();
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

// The shadows that are created by this material on the other
// objects have changed (set or unset)
// This is a n**2 algorithm!! (n=nObjects)
void MLGlobalLighting::material_shadow_has_changed( MLMaterial* m )
{
  if( m == 0 )
    return;

  if( ! update_shadows )
    return;

  MLMeshesIterator mi(scene);
  while( ! mi.eol() )
    {
    MLMesh* mesh= mi++;

    if( mesh->getMaterial() == m )
      mesh_shadow_has_changed(mesh);
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLGlobalLighting::set_dirty_pool( MLLightPool* pool )
{
  if( pool == 0 )
    return;

  if( pool->getNLights() == 0 )
    return;

  pool->setDirty();
  dirty= true;
}

/////////////////////////////////////////////////////////////////////////////

void MLGlobalLighting::clear()
{
  dirty= false;
}

/////////////////////////////////////////////////////////////////////////////

Spectrum MLGlobalLighting::getInitialEnergy() const
{
  Spectrum total_energy(0,0,0);

  for( int i= 1; i <= scene->getNLights(); ++i )
    {
    MLLight* light= scene->getLight(i);
    if( light == 0 )
      continue;
    MLLightPool* pool= light->getPool();
    total_energy += light->getEnergy() & pool->getEnergy();
    }

  return total_energy;
}

/////////////////////////////////////////////////////////////////////////////

void MLGlobalLighting::getShotUnshot( Spectrum& direct_shot,
                                      Spectrum& indirect_shot,
                                      Spectrum& unshot )
{
  direct_shot.zero();
  indirect_shot.zero();
  unshot.zero();

  MLMeshesIterator mi(scene);
  while( ! mi.eol() )
    {
    MLMesh* m= mi++;

    MLMeshLighting* lighting= m->getLighting();
    if( lighting == 0 )
      continue;

    Spectrum object_direct_shot;
    Spectrum object_indirect_shot;
    Spectrum object_unshot;
    lighting->getShotUnshot( object_direct_shot,
                             object_indirect_shot,
                             object_unshot );

    direct_shot  += object_direct_shot;
    indirect_shot+= object_indirect_shot;
    unshot       += object_unshot;
    }
}

/////////////////////////////////////////////////////////////////////////////
