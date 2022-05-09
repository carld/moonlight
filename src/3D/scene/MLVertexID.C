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
  MLVertexID.C

  Stephane Rehel

  October 28 1997
*/

#include "tools/3D/MLVertex.h"

#include "MLVertexID.h"
#include "MeshRep.h"
#include "MLMesh.h"
#include "MLScene.h"

/////////////////////////////////////////////////////////////////////////////

MLMesh* MLVertexID::getMesh( MLScene* scene )
{
  if( scene == 0 )
    return 0;

  return (MLMesh*) scene->getObject(object,MLObject::MESH);
}

/////////////////////////////////////////////////////////////////////////////

MLVertex* MLVertexID::getVertex( MLScene* scene )
{
  if( vertex <= 0 )
    return 0;

  MLMesh* mesh= getMesh(scene);
  if( mesh == 0 )
    return 0;

  MeshRep* meshrep= mesh->getMeshRep();

  if( meshrep == 0 )
    return 0;

  if( vertex > meshrep->vertices.getSize() )
    return 0;

  MLVertex& v= meshrep->vertices[vertex];
  if( ! v.allocated() )
    return 0;

  return &v;
}

/////////////////////////////////////////////////////////////////////////////
