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
  MLScene.C

  Stephane Rehel

  December 8 1996
*/

#include "tools/MLLog.h"

#include "kernel/MLKernel.h"

#include "MLScene.h"
#include "MLObjectAllocator.h"
#include "MLAbstractMeshAllocator.h"
#include "MLAbstractCurveAllocator.h"
#include "MLObjectsIterator.h"
#include "MLMeshesIterator.h"

#include "MeshRep.h"

#include "MLObject.h"
#include "MLMesh.h"
#include "MLNullObject.h"

#include "lighting/MLLight.h"
#include "lighting/MLLightTable.h"
#include "lighting/MLPoolTable.h"
#include "lighting/MLLightPool.h"
#include "lighting/MLGlobalLighting.h"

#include "material/MLMaterial.h"
#include "material/MLMaterialImages.h"
#include "material/MLMaterialAllocator.h"
#include "material/MLMaterialsIterator.h"

#include "renderer/MLRendererOptions.h"

////////////////////////////////////////////////////////////////////////////

MLScene::MLScene( MLKernel* _kernel ): MLAppRoot(_kernel)
{
  name.name= "noname";

  config= 0;
  rc_group= "";

  objectAllocator= 0;
  abstractAllocator= 0;
  abstractCurveAllocator= 0;
  materialAllocator= 0;
  lights= 0;
  poolTable= 0;
  globalLighting= 0;
  materialImages= 0;
  rendererOptions= 0;

  rootObject= 0;
}

////////////////////////////////////////////////////////////////////////////

MLScene::~MLScene()
{
  delete lights;
  lights= 0;

  delete rootObject;
  rootObject= 0;

  delete objectAllocator;
  objectAllocator= 0;

  delete abstractAllocator;
  abstractAllocator= 0;

  delete abstractCurveAllocator;
  abstractCurveAllocator= 0;

  delete materialImages;
  materialImages= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLScene::init()
{
  if( kernel == 0 )
    return false;

  if( ! MLAppRoot::init() )
    return false;

  MLAppRoot::setName("Scene");

  _verbose= kernel->verbose();

  if( objectAllocator != 0 )
    return false; // already initialized

  objectAllocator= new MLObjectAllocator;
  abstractAllocator= new MLAbstractMeshAllocator;
  abstractCurveAllocator= new MLAbstractCurveAllocator;
  materialAllocator= new MLMaterialAllocator(this);
  lights= new MLLightTable(this);
  poolTable= new MLPoolTable(this);
  globalLighting= new MLGlobalLighting(this);
  materialImages= new MLMaterialImages(this);
  rendererOptions= new MLRendererOptions(this);

  createRootObject();

  dirtyPoolsLighting= false;
  dirtyObjectsLighting= false;

  log->printf("Scene successfully initialized.\n");

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLScene::setConfigGroup( const MLString& _rc_group )
{
  config= getKernel()->getConfig();
  rc_group= _rc_group;
}

/////////////////////////////////////////////////////////////////////////////

void MLScene::setName( const MLString& _name )
{
  name.name= _name;
}

/////////////////////////////////////////////////////////////////////////////

void MLScene::setName( const MLName& _name )
{
  name= _name;
}

/////////////////////////////////////////////////////////////////////////////

int MLScene::getNObjects() const
{
  return objectAllocator->getNObjects();
}

/////////////////////////////////////////////////////////////////////////////

MLObject* MLScene::getObject( int id, int object_type /* = 0 */ )
{
  if( objectAllocator == 0 )
    return 0;

  MLObject* o= objectAllocator->getObject(id);
  if( o == 0 )
    return 0;

  if( object_type == 0 )
    return o;

  if( int(o->getObjectType()) != object_type )
    return 0;

  return o;
}

/////////////////////////////////////////////////////////////////////////////

MLObject* MLScene::getDetachedObject( int id )
{
  if( objectAllocator == 0 )
    return 0;

  return objectAllocator->getDetachedObject(id);
}

/////////////////////////////////////////////////////////////////////////////

MLObject* MLScene::getObject( const MLName& name, int object_type /* = 0 */ )
{
  if( objectAllocator == 0 )
    return 0;

  MLObject* o= objectAllocator->getObject(name);
  if( o == 0 )
    return 0;

  if( object_type == 0 )
    return o;

  if( int(o->getObjectType()) != object_type )
    return 0;

  return o;
}

/////////////////////////////////////////////////////////////////////////////

MLObject* MLScene::getObjectByFilename( const MLString& filename ) const
{
  if( objectAllocator == 0 )
    return 0;

  return objectAllocator->getObjectByFilename(filename);
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractMesh* MLScene::getAbstractObject( int id ) const
{
  if( abstractAllocator == 0 )
    return 0;

  return abstractAllocator->getAbstract(id);
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractMesh* MLScene::getAbstractObject( const MLName& name ) const
{
  if( abstractAllocator == 0 )
    return 0;

  return abstractAllocator->getAbstract(name);
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractMesh* MLScene::getAbstractByFilename
                                           ( const MLString& filename ) const
{
  if( abstractAllocator == 0 )
    return 0;

  return abstractAllocator->getAbstractByFilename(filename);
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractCurve* MLScene::getAbstractCurve( int id ) const
{
  if( abstractCurveAllocator == 0 )
    return 0;

  return abstractCurveAllocator->getAbstract(id);
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractCurve* MLScene::getAbstractCurve( const MLName& name ) const
{
  if( abstractCurveAllocator == 0 )
    return 0;

  return abstractCurveAllocator->getAbstract(name);
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractCurve* MLScene::getAbstractCurveByFilename
                                           ( const MLString& filename ) const
{
  if( abstractCurveAllocator == 0 )
    return 0;

  return abstractCurveAllocator->getAbstractByFilename(filename);
}

/////////////////////////////////////////////////////////////////////////////

int MLScene::getNLights() const
{
  return lights->getNLights();
}

/////////////////////////////////////////////////////////////////////////////

MLLight* MLScene::getLight( int light_index ) const
{
  return lights->getLight(light_index);
}

/////////////////////////////////////////////////////////////////////////////

MLLight* MLScene::getLight( const MLName& name ) const
{
  if( lights == 0 || name.name.length() == 0 )
    return 0;

  for( int i= 1; i <= lights->getNLights(); ++i )
    {
    MLLight* l= lights->getLight(i);
    if( l == 0 )
      continue;
    if( l->getName() == name )
      return l;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLScene::compileLights()
{
  if( lights == 0 )
    return;

  lights->compileLights();
}

/////////////////////////////////////////////////////////////////////////////

int MLScene::getNMaterials() const
{
  return materialAllocator->getNMaterials();
}

/////////////////////////////////////////////////////////////////////////////

MLMaterial* MLScene::getMaterial( int id )
{
  return materialAllocator->getMaterial(id);
}

/////////////////////////////////////////////////////////////////////////////

MLMaterial* MLScene::getMaterial( const MLString& name )
{
  if( materialAllocator == 0 || name.length() == 0 )
    return 0;

  MLMaterialsIterator mi(this);
  while( ! mi.eol() )
    {
    MLMaterial* m= mi++;
    if( m == 0 )
      continue;
    if( m->getName().name == name )
      return m;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

MLMaterial* MLScene::getDefaultMaterial() const
{
  return materialAllocator->getDefault();
}

/////////////////////////////////////////////////////////////////////////////

void MLScene::updateTransformations()
{
  if( rootObject != 0 )
    rootObject->updateTransformations();
}

/////////////////////////////////////////////////////////////////////////////

void MLScene::computeFinalLighting()
{
  MLMeshesIterator mi(this);
  while( ! mi.eol() )
    (mi++)->computeFinalLighting();
}

/////////////////////////////////////////////////////////////////////////////

int MLScene::getNPools() const
{
  return lights->getNLights();
}

/////////////////////////////////////////////////////////////////////////////

MLLightPool* MLScene::getPool( int pool_index ) const
{
  return poolTable->getPool(pool_index);
}

/////////////////////////////////////////////////////////////////////////////

MLLightPool* MLScene::getPool( const MLString& name ) const
{
  if( poolTable == 0 || name.length() == 0 )
    return 0;

  return poolTable->getPool(name);
}

/////////////////////////////////////////////////////////////////////////////

void MLScene::clear()
{
  MLName defaultName;
  name= defaultName;
  name.name= "noname";

/*
  if( rootObject != 0 )
    for(;;)
      {
      MLObject* o= rootObject->getFirstChild();
      if( o == 0 )
        break;
      delete o;
      }
*/

  if( globalLighting != 0 )
    globalLighting->clear();

  if( poolTable != 0 )
    poolTable->clear();

  if( lights != 0 )
    lights->clear();

  if( materialAllocator != 0 )
    materialAllocator->clear();

  if( materialImages != 0 )
    materialImages->clear();

  if( objectAllocator != 0 )
    objectAllocator->clear();

  if( abstractAllocator != 0 )
    abstractAllocator->clear();

  if( rendererOptions != 0 )
    rendererOptions->clear();

//  delete rootObject;
  createRootObject();

  dirtyPoolsLighting= false;
  dirtyObjectsLighting= false;
}

/////////////////////////////////////////////////////////////////////////////

// protected
void MLScene::createRootObject()
{
  rootObject= new MLNullObject(0);
  rootObject->scene= this;
  rootObject->object_id= objectAllocator->allocID(rootObject);
}

/////////////////////////////////////////////////////////////////////////////

// Check for boundings/hashboxes to update
void MLScene::updateBoundingHashBox()
{
  boolean found_one= false;

  MLObjectsIterator oi(this);
  while( ! oi.eol() )
    {
    MLObject* o= oi++;
    if( o->getObjectType() == MLObject::MESH )
      {
      MLMesh* mesh= (MLMesh*) o;
      MeshRep* meshrep= mesh->getMeshRep();
      if( meshrep == 0 )
        continue;

      if( meshrep->dirtyHashBox )
        {
        // also update boundings
        meshrep->buildNewHashBox();
        found_one= true;
        }
       else
        {
        if( meshrep->dirtyBoundings )
          {
          meshrep->computeBoundings();
          found_one= true;
          }
        }

      continue;
      }
    }

  if( found_one )
    updateTransformations(); // and hier. b.boxes too
}

/////////////////////////////////////////////////////////////////////////////
