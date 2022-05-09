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
  MeshRepPurge.C

  Stephane Rehel
  April 27 1997
*/

#include "MeshRep.h"
#include "MLHashBox.h"

/////////////////////////////////////////////////////////////////////////////

// reindex vertices[] and remove holes
void MeshRep::reindexVertices()
{
  // first, recount vertices
  int i;
  nVertices= 0;
  for( i= 1; i <= vertices.getSize(); ++i )
    if( vertices[i].allocated() )
      ++nVertices;

  if( nVertices == 0 )
    {
    vertices.clear();
    return;
    }

  if( nVertices == vertices.getSize() )
    return;

  int* replace= new int [ 1 + vertices.getSize() ];
  replace[0]= 0;

  int j= 1;
  for( i= 1; i <= vertices.getSize(); ++i )
    {
    if( ! vertices[i].allocated() )
      {
      replace[i]= 0;
      continue;
      }
    replace[i]= j++;
    }

  for( i= 1; i <= vertices.getSize(); ++i )
    {
    MLVertex& v= vertices[i];
    if( ! v.allocated() )
      continue;

    if( replace[i] == i )
      continue;

    MLVertex& v0= vertices[ replace[i] ];
    v0= v;

    v.unallocate();
    }

  while( vertices.getSize() > nVertices )
    vertices.remove( vertices.getSize() );

  for( i= 1; i <= edges.getSize(); ++i )
    {
    MLEdge& e= edges[i];
    if( ! e.allocated() )
      continue;

    e.v1= replace [ e.v1 ];
    e.v2= replace [ e.v2 ];
    }

  for( i= 1; i <= svertices.getSize(); ++i )
    {
    MLSVertex& sv= svertices[i];
    if( ! sv.allocated() )
      continue;

    sv.vertex= replace [ sv.vertex ];
    }

  for( i= 1; i <= polygons.getSize(); ++i )
    {
    MLPolygon& p= polygons[i];
    if( ! p.allocated() )
      continue;

    for( j= 0; j < p.nVertices; ++j )
      p.elements[j].vertex= replace[ p.elements[j].vertex ];
    }

  delete replace;
}

/////////////////////////////////////////////////////////////////////////////

// reindex edges[] and remove holes
void MeshRep::reindexEdges()
{
  // first, recount edges
  int i;
  nEdges= 0;
  for( i= 1; i <= edges.getSize(); ++i )
    if( edges[i].allocated() )
      ++nEdges;

  if( nEdges == 0 )
    {
    edges.clear();
    return;
    }

  if( nEdges == edges.getSize() )
    return;

  int* replace= new int [ 1 + edges.getSize() ];
  replace[0]= 0;

  int j= 1;
  for( i= 1; i <= edges.getSize(); ++i )
    {
    if( ! edges[i].allocated() )
      {
      replace[i]= 0;
      continue;
      }
    replace[i]= j++;
    }

  for( i= 1; i <= edges.getSize(); ++i )
    {
    MLEdge& e= edges[i];
    if( ! e.allocated() )
      continue;

    if( replace[i] == i )
      continue;

    MLEdge& e0= edges[ replace[i] ];
    e0= e;

    e.unallocate();
    }

  while( edges.getSize() > nEdges )
    edges.remove( edges.getSize() );

  for( i= 1; i <= vertices.getSize(); ++i )
    {
    MLVertex& v= vertices[i];
    if( ! v.allocated() )
      continue;

    for( j= 0; j < v.nEdges; ++j )
      v.edges[j]= replace [ v.edges[j] ];
    }

  for( i= 1; i <= polygons.getSize(); ++i )
    {
    MLPolygon& p= polygons[i];
    if( ! p.allocated() )
      continue;

    for( j= 0; j < p.nVertices; ++j )
      p.elements[j].edge= replace[ p.elements[j].edge ];
    }

  delete replace;
}

/////////////////////////////////////////////////////////////////////////////

// reindex svertices[] and remove holes
void MeshRep::reindexSVertices()
{
  // first, recount svertices
  int i;
  nSVertices= 0;
  for( i= 1; i <= svertices.getSize(); ++i )
    if( svertices[i].allocated() )
      ++nSVertices;

  if( nSVertices == 0 )
    {
    svertices.clear();
    return;
    }

  if( nSVertices == svertices.getSize() )
    return;

  int* replace= new int [ 1 + svertices.getSize() ];
  replace[0]= 0;

  int j= 1;
  for( i= 1; i <= svertices.getSize(); ++i )
    {
    if( ! svertices[i].allocated() )
      {
      replace[i]= 0;
      continue;
      }
    replace[i]= j++;
    }

  for( i= 1; i <= svertices.getSize(); ++i )
    {
    MLSVertex& sv= svertices[i];
    if( ! sv.allocated() )
      continue;

    if( replace[i] == i )
      continue;

    MLSVertex& sv0= svertices[ replace[i] ];
    sv0= sv;

    sv.unallocate();
    }

  while( svertices.getSize() > nSVertices )
    svertices.remove( svertices.getSize() );

  for( i= 1; i <= vertices.getSize(); ++i )
    {
    MLVertex& v= vertices[i];
    if( ! v.allocated() )
      continue;

    for( j= 0; j < v.nSVertices; ++j )
      v.svertices[j]= replace [ v.svertices[j] ];
    }

  for( i= 1; i <= polygons.getSize(); ++i )
    {
    MLPolygon& p= polygons[i];
    if( ! p.allocated() )
      continue;

    for( j= 0; j < p.nVertices; ++j )
      p.elements[j].svertex= replace[ p.elements[j].svertex ];
    }

  delete replace;
}

/////////////////////////////////////////////////////////////////////////////

// reindex polygons[] and remove holes
void MeshRep::reindexPolygons()
{
  // first, recount polygons
  int i;
  nPolygons= 0;
  for( i= 1; i <= polygons.getSize(); ++i )
    if( polygons[i].allocated() )
      ++nPolygons;

  if( nPolygons == 0 )
    {
    polygons.clear();
    return;
    }

  if( nPolygons == polygons.getSize() )
    return;

  int* replace= new int [ 1 + polygons.getSize() ];
  replace[0]= 0;

  int j= 1;
  for( i= 1; i <= polygons.getSize(); ++i )
    {
    if( ! polygons[i].allocated() )
      {
      replace[i]= 0;
      continue;
      }
    replace[i]= j++;
    }

  for( i= 1; i <= polygons.getSize(); ++i )
    {
    MLPolygon& p= polygons[i];
    if( ! p.allocated() )
      continue;

    if( replace[i] == i )
      continue;

    MLPolygon& p0= polygons[ replace[i] ];
    p0= p;
    p0.index= replace[i];

    p.unallocate();
    }

  while( polygons.getSize() > nPolygons )
    polygons.remove( polygons.getSize() );

  for( i= 1; i <= edges.getSize(); ++i )
    {
    MLEdge& e= edges[i];
    if( ! e.allocated() )
      continue;

    e.polygon1= replace [ e.polygon1 ];
    e.polygon2= replace [ e.polygon2 ];
    }

  for( i= 1; i <= vertices.getSize(); ++i )
    {
    MLVertex& v= vertices[i];
    if( ! v.allocated() )
      continue;

    for( j= 0; j < v.nPolygons; ++j )
      v.polygons[j]= replace [ v.polygons[j] ];
    }

/*
  for( i= 1; i <= svertices.getSize(); ++i )
    {
    MLSVertex& sv= svertices[i];
    if( ! sv.allocated() )
      continue;

    sv.ref_polygon= replace [ sv.ref_polygon ];
    }
*/

  for( i= 1; i <= polygons.getSize(); ++i )
    {
    MLPolygon& p= polygons[i];
    if( ! p.allocated() )
      continue;

    if( p.fatherPolygon != 0 )
      p.fatherPolygon= replace [ p.fatherPolygon ];

    if( p.nTessTriangles > 0 && p.tesselation != 0 )
      for( j= 0; j < p.nTessTriangles; ++j )
        p.tesselation[j]= replace [ p.tesselation[j] ];
    }

  if( hashBox != 0 )
    {
    int n= hashBox->getX() * hashBox->getY() * hashBox->getZ();
    for( i= 0; i < n; ++i )
      {
      MLHashBox::Voxel& voxel= (*hashBox)(i);

      if( voxel.polygons != 0 && voxel.nElements > 0 )
        for( j= voxel.nElements-1; j >= 0; --j )
          voxel.polygons[j]= replace [ voxel.polygons[j] ];
      }
    }

  delete replace;
}

/////////////////////////////////////////////////////////////////////////////
