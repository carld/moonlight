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
  MLLightPool.C

  Stephane Rehel

  March 11 1997
*/

#include <assert.h>

#include "MLPoolTable.h"
#include "MLLightPool.h"
#include "MLMeshPoolLighting.h"
#include "MLMeshLighting.h"
#include "MLLight.h"

#include "MLLightingControl.h"

#include "scene/MLObject.h"
#include "scene/MLMesh.h"
#include "scene/MLMeshesIterator.h"
#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

// static
const char* MLLightPool::defaultPoolName= "Default";

/////////////////////////////////////////////////////////////////////////////

MLLightPool::MLLightPool( MLScene* _scene )
{
  pool_index= 0;
  scene= _scene;

  color= Color(1,1,1);
  intensity= 1.;

  max_lights= 8;
  lights= new MLLight* [ max_lights + 1 ];
  nLights= 0;
  for( int i= 0; i <= max_lights; ++i )
    lights[i]= 0;
  singleGlobalLight= 0;
  updatePoolType();

  name= "unknown_pool";

  // set pool_index
  scene->getPoolTable()->addPool(this);

  static int static_pool_index= 1;

  name= MLString("pool-") + MLString::valueOf(static_pool_index);
  ++static_pool_index;

  dirty= true;
  dirtyIndirect= true;
}

/////////////////////////////////////////////////////////////////////////////

MLLightPool::~MLLightPool()
{
//  assert( nLights == 0 );

  scene->getPoolTable()->removePool(this);

  delete lights;
  max_lights= 0;
  lights= 0;
  scene= 0;
  name= "";
}

/////////////////////////////////////////////////////////////////////////////

// private
int MLLightPool::getIndexOfLight( MLLight* light )
{
  if( light == 0 )
    return 0;
  if( light->getPoolIndex() != pool_index )
    return 0;

  for( int i= 1; i <= nLights; ++i )
    if( lights[i] == light )
      return i;

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLLightPool::updatePoolType()
{
  if( lights == 0 || nLights == 0 )
    {
    poolType= DIRECT_POOL;
    singleGlobalLight= 0;
    }

  int nGlobal= 0;
  MLLight* the_global_light= 0;

  for( int i= 1; i <= nLights; ++i )
    {
    MLLight* l= lights[i];
    if( l == 0 )
      continue;
    if( l->globalLight )
      {
      ++nGlobal;
      the_global_light= l;
      }
    }

  singleGlobalLight= 0;
  switch(nGlobal)
    {
    case 0:
      {
      poolType= DIRECT_POOL;
      break;
      }
    case 1:
      {
      poolType= SINGLE_GLOBAL_POOL;
      singleGlobalLight= the_global_light;
      break;
      }
    default:
      {
      poolType= GLOBAL_POOL;
      break;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLLightPool::addLight( MLLight* light )
{
  if( light == 0 )
    return;

  int index= getIndexOfLight(light);
  if( index != 0 )
    return; // already have it

  if( nLights == max_lights )
    {
    int new_max_lights= max_lights + max_lights / 2;
    MLLight** new_lights= new MLLight* [ new_max_lights + 1 ];

    int i;
    for( i= 0; i <= max_lights; ++i )
      new_lights[i]= lights[i];

    for( i= max_lights+1; i <= new_max_lights; ++i )
      new_lights[i]= 0;

    delete lights;
    lights= new_lights;
    max_lights= new_max_lights;
    }

  ++nLights;
  lights[nLights]= light;
  light->pool_index= pool_index;

  dirty= true;
  dirtyIndirect= true;

  updatePoolType();
}

/////////////////////////////////////////////////////////////////////////////

void MLLightPool::removeLight( MLLight* light )
{
  int index= getIndexOfLight(light);

  if( index == 0 )
    return; // i dont have this light, sorry

  int i= index + 1;
  while( i <= nLights )
    {
    lights[i-1]= lights[i];
    ++i;
    }

  lights[nLights]= 0;
  --nLights;

  dirty= true;
  dirtyIndirect= true;
  scene->dirtyPoolsLighting= true;
  scene->dirtyFinalLighting= true;

  updatePoolType();

//  if( nLights == 0 )
//    delete this; // sniff
}

/////////////////////////////////////////////////////////////////////////////

void MLLightPool::setName( const MLString& _name )
{
  if( _name.length() == 0 )
    return;

  name= _name;
}

/////////////////////////////////////////////////////////////////////////////

// shoot this pool on each dirty object
boolean MLLightPool::shoot( MLLightingControl& control )
{
  if( !dirty )
    return true;

  if( nLights == 0 )
    {
    MLMeshesIterator mi(scene);
    while( ! mi.eol() )
      {
      MLMesh* m= mi++;

      MLMeshLighting* lighting= m->getLighting();
      if( lighting == 0 )
        continue;

      lighting->resetPool(this);
      lighting->dirtyFinal= true;
      }

    dirty= false;
    return true;
    }

  if( poolType == DIRECT_POOL )
    {
    dirty= false;
    return true;
    }

  MLMeshesIterator mi(scene);
  while( ! mi.eol() )
    {
    MLMesh* m= mi++;

    MLMeshPoolLighting* poolLighting=
                                  m->getLighting()->prepareForShooting(this);
    if( poolLighting == 0 )
      continue;

    if( ! poolLighting->shoot(control) )
      return false; // remains dirty
    }

  dirty= false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLLightPool::setDirty()
{
  if( dirty )
    {
    scene->dirtyPoolsLighting= true;
    dirtyIndirect= true;
    return;
    }

  dirty= true;
  dirtyIndirect= true;

  // reset each object pool lighting
  MLMeshesIterator mi(scene);
  while( ! mi.eol() )
    {
    MLMesh* m= mi++;

    MLMeshLighting* lighting= m->getLighting();
    if( lighting == 0 )
      continue;

    MLMeshPoolLighting* poolLighting= lighting->prepareForShooting(this);
    if( poolLighting == 0 )
      continue;

    poolLighting->reset();
    }

  scene->dirtyPoolsLighting= true;
}

/////////////////////////////////////////////////////////////////////////////

void MLLightPool::setDirtyFinal()
{
  if( dirty )
    {
    scene->dirtyFinalLighting= true;
    return;
    }

  if( poolType == DIRECT_POOL )
    return;

  MLMeshesIterator mi(scene);
  while( ! mi.eol() )
    {
    MLMesh* m= mi++;
    MLMeshLighting* lighting= m->getLighting();
    if( lighting == 0 )
      continue;

    MLMeshPoolLighting* poolLighting= lighting->prepareForShooting(this);
    if( poolLighting == 0 )
      continue;

    if( poolLighting->ffDirect.getSize() == 0 &&
        poolLighting->eDirect.getSize() == 0 &&
        poolLighting->indirect.getSize() == 0 )
      continue; // Speed-up: this object is not lighted by this pool

    lighting->dirtyFinal= true;
    }

  scene->dirtyFinalLighting= true;
}

/////////////////////////////////////////////////////////////////////////////

void MLLightPool::initiateIndirect()
{
  if( poolType == DIRECT_POOL )
    return;

  dirtyIndirect= false;

  MLMeshesIterator mi(scene);
  while( ! mi.eol() )
    {
    MLMesh* m= mi++;

    MLMeshLighting* lighting= m->getLighting();
    if( lighting == 0 )
      continue;

    MLMeshPoolLighting* poolLighting= lighting->prepareForShooting(this);
    if( poolLighting == 0 )
      continue;

    poolLighting->resetIndirect();
    }
}

/////////////////////////////////////////////////////////////////////////////
