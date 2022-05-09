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
  MLPoolTable.C

  Stephane Rehel

  March 11 1997
*/

#include "MLPoolTable.h"
#include "MLLightPool.h"
#include "MLLight.h"

#include "scene/MLMeshesIterator.h"
#include "scene/MLObject.h"
#include "scene/MLMesh.h"

#include "MLMeshLighting.h"

/////////////////////////////////////////////////////////////////////////////

MLPoolTable::MLPoolTable( MLScene* _scene )
{
  scene= _scene;

  max_pools= 8;
  pools= new MLLightPool* [ max_pools + 1 ];
  nPools= 0;
  pools[0]= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLPoolTable::~MLPoolTable()
{
  for( int i= 1; i <= max_pools; ++i )
    {
    MLLightPool* p= pools[i];
    if( p == 0 )
      continue;

    pools[i]= 0;
    delete p;
    }

  delete pools;
  pools= 0;
  nPools= 0;
  max_pools= 0;

  scene= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLPoolTable::addPool( MLLightPool* pool )
{
  if( pool == 0 )
    return;

  // First, check out if this pool isn't already in the table
  int i;
  for( i= 1; i <= nPools; ++i )
    if( pools[i] == pool )
      return;

  if( nPools == max_pools )
    {
    int new_max_pools= max_pools + max_pools / 2;
    MLLightPool** new_pools= new MLLightPool* [ new_max_pools + 1 ];

    int i;
    for( i= 0; i <= max_pools; ++i )
      new_pools[i]= pools[i];

    for( i= max_pools+1; i <= new_max_pools; ++i )
      new_pools[i]= 0;

    delete pools;
    pools= new_pools;
    max_pools= new_max_pools;
    }

  ++nPools;
  pools[nPools]= pool;
  pool->pool_index= nPools;

  // now, update MLMeshLighting for every MLObject
  MLMeshesIterator mi(scene);
  while( ! mi.eol() )
    {
    MLMesh* m= mi++;

    MLMeshLighting* lighting= m->getLighting();
    if( lighting == 0 )
      continue; // ???, this could be the root object!

    lighting->addPool(pool);
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLPoolTable::removePool( MLLightPool* pool )
{
  if( pool == 0 )
    return;

  int index= pool->pool_index;
  if( index < 1 || index > nPools )
    return; // ???

  if( pool->getNLights() > 0 )
    return;

  // now, update MLMeshLighting for every MLObject
  MLMeshesIterator mi(scene);
  while( ! mi.eol() )
    {
    MLMesh* m= mi++;

    MLMeshLighting* lighting= m->getLighting();
    if( lighting == 0 )
      continue; // ???, this could be the root object!

    lighting->removePool(pool);
    }

  int i= index+1;
  while( i <= nPools )
    {
    pools[i-1]= pools[i];
    pools[i-1]->pool_index= i-1;

    MLLightPool* p= pools[i-1];
    for( int j= 1; j <= p->getNLights(); ++j )
      {
      MLLight* light= p->getLight(j);
      if( light == 0 )
        continue;
      light->pool_index= p->pool_index;
      }

    ++i;
    }
  pools[nPools]= 0;

  pool->pool_index= 0;
  --nPools;
}

/////////////////////////////////////////////////////////////////////////////

MLLightPool* MLPoolTable::getPool( const MLString& _name ) const
{
  for( int i= 1; i <= getNPools(); ++i )
    {
    MLLightPool* p= getPool(i);
    if( p == 0 )
      continue;
    if( p->name == _name )
      return p;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

// set pool=0 for default pool
void MLPoolTable::addLightToPool( MLLight* light, MLLightPool* pool )
{
  if( light == 0 )
    return;

  if( pool == 0 )
    {
    // find default pool
    MLLightPool* dp= 0;
    for( int i= 1; i <= nPools; ++i )
      if( pools[i]->name == MLLightPool::defaultPoolName )
        {
        dp= pools[i];
        break;
        }

    if( dp == 0 )
      {
      dp= new MLLightPool(scene);
      dp->name= MLLightPool::defaultPoolName;
      }

    pool= dp;
    }

  pool->addLight(light);
}

/////////////////////////////////////////////////////////////////////////////

void MLPoolTable::clear()
{
  if( pools == 0 )
    return;

  for( int i= 1; i <= nPools; ++i )
    {
    MLLightPool* p= pools[i];
    if( p == 0 )
      continue;

    pools[i]= 0;
    delete p;
    }

  nPools= 0;
}

/////////////////////////////////////////////////////////////////////////////
