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
  MLAbstractMeshAllocator.C

  Stephane Rehel

  December 12 1996
*/

#include "MLAbstractMeshAllocator.h"

#include "tools/IArray.h"

#include "MLName.h"

#include "MLAbstractMesh.h"

/////////////////////////////////////////////////////////////////////////////

MLAbstractMeshAllocator::MLAbstractMeshAllocator()
{
  objects= new IArray;
  nObjects= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractMeshAllocator::~MLAbstractMeshAllocator()
{
  delete objects;
  objects= 0;
  nObjects= 0;
}

/////////////////////////////////////////////////////////////////////////////

int MLAbstractMeshAllocator::allocID( MLAbstractMesh* object )
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

void MLAbstractMeshAllocator::freeID( int id )
{
  if( id < 1 || id > objects->getSize() )
    return;
  if( (*objects)[id] == 0 )
    return;

  (*objects)[id]= 0;
  --nObjects;
}

/////////////////////////////////////////////////////////////////////////////

void MLAbstractMeshAllocator::freeID( MLAbstractMesh* object )
{
  if( object == 0 )
    return;

  int id= object->abstract_mesh_id;
  if( id < 1 || id > objects->getSize() )
    return;

  for( int i= 1; i <= objects->getSize(); ++i )
    {
    register void*& o= (*objects)[i];
    if( o == (void*) object )
      {
      o= 0;
      --nObjects;
      return;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractMesh* MLAbstractMeshAllocator::getAbstract( int id ) const
{
  if( id < 1 || id > objects->getSize() )
    return 0;

  return (MLAbstractMesh*) (*objects)[id];
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractMesh* MLAbstractMeshAllocator::
getAbstract( const MLName& name ) const
{
  if( name.name.length() == 0 )
    return 0;

  for( int i= 1; i <= objects->getSize(); ++i )
    {
    register MLAbstractMesh* ao= (MLAbstractMesh*) (*objects)[i];
    if( ao == 0 )
      continue;

    if( ao->getName() == name )
      return ao;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractMesh* MLAbstractMeshAllocator::
getAbstractByFilename( const MLString& filename ) const
{
  for( int i= 1; i <= objects->getSize(); ++i )
    {
    register MLAbstractMesh* ao= (MLAbstractMesh*) (*objects)[i];
    if( ao == 0 )
      continue;

    if( ao->getName().filename == filename )
      return ao;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLAbstractMeshAllocator::adjustNameIndices( MLAbstractMesh* newAO )
{
  if( newAO == 0 )
    return;

  const MLName& name= newAO->getName();

  for( int i= 1; i <= objects->getSize(); ++i )
    {
    register MLAbstractMesh* ao= (MLAbstractMesh*) (*objects)[i];
    if( ao == 0 )
      continue;

    MLName& n= ao->name;
    if( n.name != name.name ||
        n.name_index != name.name_index )
      continue;

    if( ao == newAO )
      continue;

    // we found an abstract that points to newAO name
    // Let's change its name_index
    int ni= MLName::default_name_index;
    MLName guess= name;

    for(;;)
      {
      guess.name_index= ni;
      MLAbstractMesh* ao1= getAbstract(guess);
      if( ao1 == 0 )
        break;
      ++ni;
      }

    n.name_index= ni;
    }
}

/////////////////////////////////////////////////////////////////////////////
//#include "MeshRep.h"
void MLAbstractMeshAllocator::clear()
{
  for( int i= 1; i <= objects->getSize(); ++i )
    {
    register MLAbstractMesh* ao= (MLAbstractMesh*) (*objects)[i];
    if( ao == 0 )
      continue;
    (*objects)[i]= 0;
//printf("deleting abstract %s (%d)\n",ao->getName().name.get(),ao->meshRep->nPolygons);

    unref(ao);
    }
//printf( "OK!\n");
  nObjects= 0;
}

/////////////////////////////////////////////////////////////////////////////
