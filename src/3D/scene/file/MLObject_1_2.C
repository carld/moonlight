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
  MLObject_1_2.C

  Stephane Rehel
  October 5 1997

  MLObject* MLObjectIO::read_1_2( MLRFileBlock* block );

  -> don't forget to update the recursive call of read_?_?()
*/

#include "MLObjectIO.h"
#include "MLObjectIDs.h"

#include "tools/file/MLRFile.h"
#include "tools/file/MLRFileBlock.h"

#include "scene/MLObjectAllocator.h"
#include "scene/MLNullObject.h"
#include "scene/MLMesh.h"
#include "scene/lighting/MLLight.h"

#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

MLObject* MLObjectIO::read_1_2( MLObject* father, MLRFileBlock& block )
{
  if( ! block.find(MLObjectID::OBJECT_DEF) )
    return 0;

  MLName sceneName, name;

  int objectType;
  unsigned int flags;

  block.go(MLObjectID::OBJECT_DEF);
    {
    block >> sceneName;
    block >> name;

    block >> objectType;

    block >> flags;
    }

  MLObject* o= 0;
  switch( objectType )
    {
    case MLObject::NULL_OBJECT: o= new MLNullObject(father); break;
    case MLObject::MESH: o= new MLMesh(father,0); break;
    case MLObject::LIGHT: o= new MLLight(father,0); break;
    default: return 0;
    }

  ref(o);
  o->name= name;
  o->flags= MLObject::Flag(flags);

  block.go(MLObjectID::OBJECT_PARAMS);
    {
    block >> o->hierBBox;
    o->bounded= block.getInteger() != 0;
    double sphere_radius;
    Point sphere_center;
    block >> sphere_center;
    block >> sphere_radius;
    o->globalBSphere= BoundingSphere(sphere_center,sphere_radius);
    block >> o->local_xform.preXForm;
    block >> o->local_xform.scale;
    block >> o->local_xform.rotate;
    block >> o->local_xform.translate;
    block >> o->local_xform.transform;
    block >> o->global_xform;
    }

  switch(o->getObjectType())
    {
    case MLObject::LIGHT:
      {
      // in MLLightIO.C
      readLight( (MLLight*) o, block );
      break;
      }
    case MLObject::MESH:
      {
      // in MLMeshIO.C
      readMesh( (MLMesh*) o, sceneName, block );
      break;
      }
    default:
      break;
    }

  block.go(MLObjectID::OBJECT_CHILDREN);
    {
    int nChildren;
    block >> nChildren;

    for( int i= 1; i <= nChildren; ++i )
      {
      MLRFileBlock b;
      b.open(block.getFile());
        if( read_1_2(o,b) == 0 )
          {
          delete o;
          return 0; //***!!!
          }
      b.close();
      }
    }

  // it's the correct filename ... normally
  o->name.filename= getObjectFilename( database, sceneName, o->name );

  // adjust any other object name that poinst to this o->name
  // this prevents other objects to have the same filename
  // when we save next time
  scene->getObjectAllocator()->adjustNameIndices(o);

  scene->getRootObject()->updateTransformations();

  if( o != 0 )
    if( o->getObjectType() == MLObject::LIGHT )
      ((MLLight*)o)->compile();

  return o;
}

/////////////////////////////////////////////////////////////////////////////
