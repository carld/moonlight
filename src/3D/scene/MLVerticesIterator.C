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
  MLVerticesIterator.C

  Stephane Rehel

  March 13 1998
*/

#include "MLVerticesIterator.h"

#include "MeshRep.h"
#include "MLMesh.h"

/////////////////////////////////////////////////////////////////////////////

MLVerticesIterator::MLVerticesIterator( MLMesh* mesh )
{
  if( mesh == 0 )
    vertices= 0;
   else
    {
    MeshRep* meshrep= mesh->getMeshRep();
    if( meshrep == 0 )
      vertices= 0;
     else
      vertices= &(meshrep->vertices);
    }

  reset();
}

/////////////////////////////////////////////////////////////////////////////

MLVerticesIterator::MLVerticesIterator( MeshRep* meshrep )
{
  if( meshrep == 0 )
    vertices= 0;
   else
    vertices= &(meshrep->vertices);

  reset();
}

/////////////////////////////////////////////////////////////////////////////

MLVerticesIterator::MLVerticesIterator( MLVertexArray& _vertices )
{
  vertices= &_vertices;

  reset();
}

/////////////////////////////////////////////////////////////////////////////

void MLVerticesIterator::reset()
{
  if( vertices == 0 )
    {
    max_index= 0;
    index= 1;
    }
   else
    {
    index= 1;
    max_index= vertices->getSize();
    }
}

/////////////////////////////////////////////////////////////////////////////
