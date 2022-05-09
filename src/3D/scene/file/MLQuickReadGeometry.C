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
  MLQuickReadGeometry.C

  Stephane Rehel
  April 21 1997
*/

#include "tools/Vector.h"
#include "tools/MLString.h"
#include "tools/file/MLRFile.h"
#include "tools/file/MLRFileBlock.h"

#include "tools/3D/MLEdge.h"

#include "MLQuickReadGeometry.h"
#include "MLAbstractMeshIDs.h"

/////////////////////////////////////////////////////////////////////////////

MLQuickReadGeometry::MLQuickReadGeometry()
{
  nVertices= 0;
  vertices= 0;

  nEdges= 0;
  edges= 0;

  bbox= Box( Point(0,0,0), Point(1,1,1) );

  nSVertices= 0;
  nPolygons= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLQuickReadGeometry::~MLQuickReadGeometry()
{
  delete edges;
  edges= 0;
  nEdges= 0;

  delete vertices;
  vertices= 0;
  nVertices= 0;

  nSVertices= 0;
  nPolygons= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLQuickReadGeometry::alloc( int _nVertices, int _nEdges )
{
  delete vertices;
  nVertices= _nVertices;
  if( nVertices < 0 ) nVertices= 0;
  vertices= new Point [ nVertices + 1 ];

  const Point zero( 0., 0., 0. );
  for( int i= 0; i <= nVertices; ++i )
    vertices[i]= zero;

  delete edges;
  nEdges= _nEdges;
  if( nEdges < 0 ) nEdges= 0;
  edges= new Edge [ nEdges + 1 ];
}

/////////////////////////////////////////////////////////////////////////////

// static
MLQuickReadGeometry* MLQuickReadGeometry::read( const MLString& filename )
{
  MLRFile file;

  if( ! file.open(filename) )
    return 0;

  if( ! file.init(MLAbstractMeshID::signature) )
    {
    file.close();
    return 0;
    }

  MLRFileBlock& block= file.getMainBlock();

  if( ! block.go(MLAbstractMeshID::DRIVER_VERSION) )
    return 0;

  int major, minor;
  block >> major;
  block >> minor;

  int version= major * 100 + minor;

  MLQuickReadGeometry* qr= 0;

  switch( version )
    {
    case 100:
    case 101:
      {
      qr= read_1_0(block);
      }
    default:
      break;
    }

  file.close();

  return qr;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLQuickReadGeometry* MLQuickReadGeometry::read_1_0( MLRFileBlock& block )
{
  if( !block.find(MLAbstractMeshID::MESH_PARAMS) ||
      !block.find(MLAbstractMeshID::MESH_VERTICES) ||
      !block.find(MLAbstractMeshID::MESH_EDGES) )
    return 0;

  int nVertices= 0, nEdges= 0, nSVertices= 0, nPolygons= 0;
  Box bbox;

  block.go(MLAbstractMeshID::MESH_PARAMS);
    {
    block >> nVertices;
    block >> nEdges;
    block >> bbox;
    block >> nSVertices;
    block >> nPolygons;
    }

  if( nVertices <= 0 || nEdges <= 0 || nSVertices <= 0 || nPolygons <= 0 )
    return 0;

  MLQuickReadGeometry* qr= new MLQuickReadGeometry;
  qr->alloc(nVertices,nEdges);
  qr->bbox= bbox;
  qr->nSVertices= nSVertices;
  qr->nPolygons= nPolygons;

  block.go(MLAbstractMeshID::MESH_VERTICES);
    {
    Point* vertices= qr->vertices;
    for( int i= 1; i <= nVertices; ++i )
      block >> vertices[i];
    }

  block.go(MLAbstractMeshID::MESH_EDGES);
    {
    Edge* edges= qr->edges;
    for( int i= 1; i <= nEdges; ++i )
      {
      unsigned int flags;
      int polygon1, polygon2;

      int& v1= edges[i].v1;
      int& v2= edges[i].v2;

      block >> flags;

      if( (MLEdge::Flag(flags) & MLEdge::ALLOCATED) == 0 )
        {
        v1= v2= 0;
        continue;
        }

      block >> v1;
      block >> v2;
      block >> polygon1;
      block >> polygon2;

      if( (MLEdge::Flag(flags) & MLEdge::TESSELATION) != 0 ||
          (MLEdge::Flag(flags) & MLEdge::REFINEMENT) != 0 ||
          v1 <= 0 || v1 > nVertices || v2 <= 0 || v2 > nVertices )
        v1= v2= 0;
      }
    }

  return qr;
}

/////////////////////////////////////////////////////////////////////////////
