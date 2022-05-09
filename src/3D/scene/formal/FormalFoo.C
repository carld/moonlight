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
   FormalFoo.C

   March 1 1997
   Stephane Rehel
*/

#include "FormalFoo.h"
#include "scene/MeshRep.h"

/////////////////////////////////////////////////////////////////////////////

FormalFoo::FormalFoo( MLScene* _scene ): FormalRep(_scene)
{}

/////////////////////////////////////////////////////////////////////////////

FormalFoo::~FormalFoo()
{}

/////////////////////////////////////////////////////////////////////////////

FormalRep::TYPE FormalFoo::getType() const
{
  return FormalRep::FOO;
}

/////////////////////////////////////////////////////////////////////////////

void FormalFoo::generate( MeshRep* mesh ) const
{
  if( mesh == 0 )
    return;

  mesh->clear();

  int nVertices, nPolygons;
  double* vertices;
  int* polygons;
{
#include "foo.inc"
}

  mesh->alloc( nVertices, 0, nPolygons );
  int i;
  for( i= 0; i < nVertices; ++i )
    {
    Point p= Point( vertices[0], vertices[1], vertices[2] );
    mesh->vertices[i+1].point= p;
    vertices+= 3;
    }

  for( i= 0; i < nPolygons; ++i )
    {
    MLPolygon& p= mesh->polygons[i+1];
    int n= *(polygons++);
    p.alloc(n);

    for( int j= 0; j < n; ++j )
      p.elements[j].vertex= polygons[j];
    polygons+= n;
    }

  assert( *polygons == 0 );

  mesh->findEdges();

//  mesh->mergeVertices();

  mesh->finalizeRep();

  return;
}

/////////////////////////////////////////////////////////////////////////////

FormalRep* FormalFoo::duplicate()
{
  return new FormalFoo(scene);
}

/////////////////////////////////////////////////////////////////////////////
