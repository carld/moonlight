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
  MLObjectAllocator.C

  Stephane Rehel

  December 11 1996
*/

#include "tools/IArray.h"
#include "MLObjectAllocator.h"

#include "MLObject.h"

/////////////////////////////////////////////////////////////////////////////

MLObjectAllocator::MLObjectAllocator()
{
  objects= new IArray;
  nObjects= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLObjectAllocator::~MLObjectAllocator()
{
  delete objects;
  objects= 0;
  nObjects= 0;
}

/////////////////////////////////////////////////////////////////////////////

int MLObjectAllocator::allocID( MLObject* object )
{
  if( object == 0 )
    return 0;

  int hole= 0;

  for( int i= 1; i <= objects->getSize(); ++i )
    {
    register void* o= (*objects)[i];
    if( o == (void*) object )
      return i; // already allocated
    if( o == 0 && hole == 0 )
      hole= i;
    }

  ++nObjects;
  if( hole == 0 )
    {
    objects->append( (void*) object );
    return objects->getSize();
    }

  (*objects)[ hole ]= (void*) object;

  return hole;
}

/////////////////////////////////////////////////////////////////////////////

void MLObjectAllocator::freeID( int id )
{
  if( id < 1 || id > objects->getSize() )
    return;
  if( (*objects)[id] == 0 )
    return;

  if( ! ((MLObject*)(*objects)[id])->detached.flag )
    --nObjects;

  (*objects)[id]= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLObjectAllocator::detach( int id )
{
  if( id < 1 || id > objects->getSize() )
    return;
  if( (*objects)[id] == 0 )
    return;

  --nObjects;
}

/////////////////////////////////////////////////////////////////////////////

void MLObjectAllocator::attach( int id )
{
  if( id < 1 || id > objects->getSize() )
    return;
  if( (*objects)[id] == 0 )
    return;

  ++nObjects;
}

/////////////////////////////////////////////////////////////////////////////

void MLObjectAllocator::freeID( MLObject* object )
{
  if( object == 0 )
    return;

  int id= object->object_id;
  if( id < 1 || id > objects->getSize() )
    return;

  for( int i= 1; i <= objects->getSize(); ++i )
    {
    register void*& o= (*objects)[i];
    if( o == (void*) object )
      {
      if( ! object->detached.flag )
        --nObjects;
      o= 0;
      return;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

MLObject* MLObjectAllocator::getObject( int id ) const
{
  if( id < 1 || id > objects->getSize() )
    return 0;

  MLObject* o= (MLObject*) (*objects)[id];

  if( o != 0 )
    if( o->detached.flag )
      return 0;

  return o;
}

/////////////////////////////////////////////////////////////////////////////

MLObject* MLObjectAllocator::getDetachedObject( int id ) const
{
  if( id < 1 || id > objects->getSize() )
    return 0;

  MLObject* o= (MLObject*) (*objects)[id];

  if( o != 0 )
    if( ! o->detached.flag )
      o= 0;

  return o;
}

/////////////////////////////////////////////////////////////////////////////

MLObject* MLObjectAllocator::getObject( const MLName& name ) const
{
  for( int i= 1; i <= objects->getSize(); ++i )
    {
    register MLObject* o= (MLObject*) (*objects)[i];
    if( o == 0 )
      continue;

    if( o->getName() == name )
      return o;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

MLObject* MLObjectAllocator::getObjectByFilename
                                            ( const MLString& filename ) const
{
  for( int i= 1; i <= objects->getSize(); ++i )
    {
    register MLObject* o= (MLObject*) (*objects)[i];
    if( o == 0 )
      continue;

    if( o->getName().filename == filename )
      return o;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLObjectAllocator::isValidObject( int index, MLObject* o ) const
{
  if( o == 0 )
    return false;

  MLObject* _o= getObject(index);

  return o == _o;
}

/////////////////////////////////////////////////////////////////////////////

void MLObjectAllocator::adjustNameIndices( MLObject* newO )
{
  if( newO == 0 )
    return;

  const MLName& name= newO->getName();

  for( int i= 1; i <= objects->getSize(); ++i )
    {
    register MLObject* o= (MLObject*) (*objects)[i];
    if( o == 0 )
      continue;

    MLName& n= o->name;
    if( n.name != name.name ||
        n.name_index != name.name_index )
      continue;

    if( o == newO )
      continue;

    // we found an object that points to newO name
    // Let's change its name_index
    int ni= MLName::default_name_index;
    MLName guess= name;

    for(;;)
      {
      guess.name_index= ni;
      MLObject* o1= getObject(guess);
      if( o1 == 0 )
        break;
      ++ni;
      }

    n.name_index= ni;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLObjectAllocator::clear()
{
  for( int i= 1; i <= objects->getSize(); ++i )
    {
    register MLObject* o= (MLObject*) (*objects)[i];
    if( o == 0 )
      continue;
    (*objects)[i]= 0;
//printf("deleting object %s\n",o->getName().name.get());
    delete o;
    }

  nObjects= 0;
}

/////////////////////////////////////////////////////////////////////////////
