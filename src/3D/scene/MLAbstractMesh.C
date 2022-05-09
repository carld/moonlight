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
  MLAbstractMesh.C

  Stephane Rehel

  December 12 1996
*/

#include <stdio.h>
#include <assert.h>

#include "MLAbstractMesh.h"
#include "MLAbstractMeshAllocator.h"

#include "formal/FormalRep.h"

#include "MeshRep.h"
#include "MLMesh.h"
#include "MLScene.h"
#include "MLObjectAllocator.h"

/////////////////////////////////////////////////////////////////////////////

static const int null_id= -1;

/////////////////////////////////////////////////////////////////////////////

MLAbstractMesh::MLAbstractMesh( MLScene* _scene,
                                FormalRep* _formalRep /* = 0 */ ):
  MLAbstractObject(_scene)
{
  abstract_mesh_id= scene->abstractAllocator->allocID(this);

  name.name= MLString("mesh-") + MLString::valueOf(abstract_mesh_id);

  formalRep= _formalRep;
  meshRep= 0;

//  bbox= Box( Point(0,0,0), Point(1,1,1) * 1e-8 );
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractMesh::~MLAbstractMesh()
{
#ifndef NDEBUG
printf("ABSTRACT MESH '%s' DELETED\n",name.name.get());
#endif

  scene->abstractAllocator->freeID(abstract_mesh_id);
  abstract_mesh_id= null_id;

  delete formalRep; formalRep= 0;
//***BUG!!!  delete meshRep;
   meshRep= 0;
}

/////////////////////////////////////////////////////////////////////////////

int MLAbstractMesh::getSizeOf() const
{
  int s= sizeof(*this) + max_instances * sizeof(instances[0]);

  if( formalRep != 0 )
    s+= formalRep->getSizeOf();

  if( meshRep != 0 )
    s+= meshRep->getSizeOf();

  return s;
}

/////////////////////////////////////////////////////////////////////////////

void MLAbstractMesh::addReference( MLMesh* instance )
{
  if( instance == 0 )
    return;

  if( MLAbstractObject::_addReference(instance) )
    instance->abstract= this;
}

/////////////////////////////////////////////////////////////////////////////

void MLAbstractMesh::removeReference( MLMesh* instance )
{
  if( instance == 0 )
    return;

  if( MLAbstractObject::_removeReference(instance) )
    {
    if( nInstances > 0 )
      instance->abstract= 0;
      // else: dont delete me please, i'm still referenced
    }
   else
    {
    // well, not found
    instance->abstract= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractMesh* MLAbstractMesh::duplicate()
{
  FormalRep* fr= 0;
  if( formalRep != 0 )
    fr= formalRep->duplicate();

  MLAbstractMesh* ao= new MLAbstractMesh(scene,fr);

  if( meshRep != 0 )
    ao->meshRep= meshRep->duplicate();

//  ao->bbox= bbox;

  return ao;
}

/////////////////////////////////////////////////////////////////////////////

void MLAbstractMesh::setFormalRep( FormalRep* _formalRep )
{
  if( formalRep != 0 )
    delete formalRep;

  formalRep= _formalRep;
  delete meshRep;
  meshRep= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLAbstractMesh::generateMesh()
{
  if( meshRep == 0 )
    meshRep= new MeshRep;

  if( formalRep != 0 )
    formalRep->generate(meshRep);

//  bbox= meshRep->bbox;
}

/////////////////////////////////////////////////////////////////////////////

Box MLAbstractMesh::getBBox() const
{
  if( meshRep == 0 )
    return Box( Point(0,0,0), Point(1,1,1) * 1e-8 );

  return meshRep->bbox;
}

/////////////////////////////////////////////////////////////////////////////

void MLAbstractMesh::polygonize()
{
  formalRep= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLAbstractMesh::unpolygonize( FormalRep* _formalRep )
{
  formalRep= _formalRep;
}

/////////////////////////////////////////////////////////////////////////////
