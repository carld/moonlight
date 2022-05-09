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
   FormalPolyhedron.C

   March 1 1997
   Stephane Rehel
*/

#include "FormalPolyhedron.h"

#include "scene/MeshRep.h"

#include "tools/file/MLRFileBlock.h"
#include "tools/file/MLWFileBlock.h"

/////////////////////////////////////////////////////////////////////////////

FormalPolyhedron::FormalPolyhedron( MLScene* _scene ): FormalRep(_scene)
{
  polyhedron_type= tetrahedron;
}

/////////////////////////////////////////////////////////////////////////////

FormalPolyhedron::~FormalPolyhedron()
{}

/////////////////////////////////////////////////////////////////////////////

void FormalPolyhedron::setPolyhedronType( PolyhedronType _polyhedron_type )
{
  polyhedron_type= _polyhedron_type;
}

/////////////////////////////////////////////////////////////////////////////

FormalRep::TYPE FormalPolyhedron::getType() const
{
  return FormalRep::POLYHEDRON;
}

/////////////////////////////////////////////////////////////////////////////

FormalRep* FormalPolyhedron::duplicate()
{
  FormalPolyhedron* f= new FormalPolyhedron(scene);
  f->setPolyhedronType(polyhedron_type);

  return f;
}

/////////////////////////////////////////////////////////////////////////////

void FormalPolyhedron::generate( MeshRep* mesh ) const
{
  if( mesh == 0 )
    return;

  mesh->clear();

  int nVertices, nPolygons;
  double* vertices;
  int* polygons;

  switch( polyhedron_type )
    {
    case cube:
      {
#include "polyhedra/cube.inc"
      }
      break;
    case octahedron:
      {
#include "polyhedra/octahedron.inc"
      }
      break;
    case icosahedron:
      {
#include "polyhedra/icosahedron.inc"
      }
      break;
    case dodecahedron:
      {
#include "polyhedra/dodecahedron.inc"
      }
      break;
    case cuboctahedron:
      {
#include "polyhedra/cuboctahedron.inc"
      }
      break;
    case icosidodecahedron:
      {
#include "polyhedra/icosidodecahedron.inc"
      }
      break;
    case rhombicuboctahedron:
      {
#include "polyhedra/rhombicuboctahedron.inc"
      }
      break;
    case rhombicosidodecahedron:
      {
#include "polyhedra/rhombicosidodecahedron.inc"
      }
      break;
    case rhombitruncated_cubeoctahedron:
      {
#include "polyhedra/rhombitruncated_cubeoctahedron.inc"
      }
      break;
    case rhombitruncated_icosidodecahedron:
      {
#include "polyhedra/rhombitruncated_icosidodecahedron.inc"
      }
      break;
    case snub_cube:
      {
#include "polyhedra/snub_cube.inc"
      }
      break;
    case snub_dodecahedron:
      {
#include "polyhedra/snub_dodecahedron.inc"
      }
      break;
    case truncated_tetrahedron:
      {
#include "polyhedra/truncated_tetrahedron.inc"
      }
      break;
    case truncated_cube:
      {
#include "polyhedra/truncated_cube.inc"
      }
      break;
    case truncated_octahedron:
      {
#include "polyhedra/truncated_octahedron.inc"
      }
      break;
    case truncated_icosahedron:
      {
#include "polyhedra/truncated_octahedron.inc"
      }
      break;
    case truncated_dodecahedron:
      {
#include "polyhedra/truncated_dodecahedron.inc"
      }
      break;
    case tetrahedron:
    default:
      {
#include "polyhedra/tetrahedron.inc"
      }
      break;
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

void FormalPolyhedron::_read( MLRFileBlock* block )
{
  int type;
  *block >> type;
  polyhedron_type= PolyhedronType(type);
}

/////////////////////////////////////////////////////////////////////////////

void FormalPolyhedron::_write( MLWFileBlock* block )
{
  *block << int(polyhedron_type);
}

/////////////////////////////////////////////////////////////////////////////
