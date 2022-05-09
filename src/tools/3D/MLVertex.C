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
   MLVertex.C

   Stephane Rehel
   November 11 1996

   Moved to tools/ on April 3 1998
*/

#include <string.h>

#include "tools/MLDArray.h"
#include "tools/xdr/XDRFile.h"

#include "MLVertex.h"
#include "MLEdge.h"

/////////////////////////////////////////////////////////////////////////////

MLVertex& MLVertex::operator = ( const MLVertex& v )
{
  if( &v == this )
    return *this;

  delete edges; edges= 0;
  delete polygons; polygons= 0;
  delete svertices; svertices= 0;

  point= v.point;

  nEdges= v.nEdges;
  if( nEdges > 0 )
    {
    edges= new int [ nEdges ];
    memcpy( (void*) edges,
            (const void*) v.edges,
            sizeof(edges[0]) * nEdges );
    }

  nPolygons= v.nPolygons;
  if( nPolygons > 0 )
    {
    polygons= new int [ nPolygons ];
    memcpy( (void*) polygons,
            (const void*) v.polygons,
            sizeof(polygons[0]) * nPolygons );
    }

  nSVertices= v.nSVertices;
  if( nSVertices > 0 )
    {
    svertices= new int [ nSVertices ];
    memcpy( (void*) svertices,
            (const void*) v.svertices,
            sizeof(svertices[0]) * nSVertices );
    }

  flags= v.flags;

  return *this;
}

/////////////////////////////////////////////////////////////////////////////

int MLVertex::getSizeOf() const
{
  return  sizeof(*this)
        + nEdges * sizeof(edges[0])
        + nPolygons * sizeof(polygons[0])
        + nSVertices * sizeof(svertices[0]);
}

/////////////////////////////////////////////////////////////////////////////

void MLVertex::unallocate()
{
  delete edges; edges= 0; nEdges= 0;
  delete polygons; polygons= 0; nPolygons= 0;
  delete svertices; svertices= 0; nSVertices= 0;

  flags &= ~Flag(ALLOCATED);
}

/////////////////////////////////////////////////////////////////////////////

void MLVertex::addEdge( int ei )
{
  if( ei < 1 )
    return; // crazy?

  int new_nEdges= nEdges + 1;
  int* new_edges= new int [ new_nEdges ];

  if( edges != 0 )
    {
    memcpy( (void*) new_edges,
            (const void*) edges,
            sizeof(int) * nEdges );
    }

  new_edges [ nEdges ] = ei;

  nEdges= new_nEdges;
  edges= new_edges;
}

/////////////////////////////////////////////////////////////////////////////

// Find edge from this vertex (v1) to vertex v2
// return edge index (>=1) if found
// return 0 if not found
int MLVertex::findEdge( const MLDArray<MLEdge>& all_edges,
                        int v1, int v2 ) const
{
  int* ei= edges;
  for( int i= 0; i < nEdges; ++i, ++ei )
    {
    const MLEdge& e= all_edges.get(*ei);
    if( (e.v1==v1 && e.v2==v2) || (e.v1==v2 && e.v2==v1) )
      return *ei;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLVertex::serialWrite( XDRFile* file )
{
  if( file == 0 )
    return false;

#define write_double(r) if( ! file->putDouble(r) ) return false;
#define write_int(it) if( ! file->putInteger(it) ) return false;

  write_int(int(flags));

  write_double(point.x());
  write_double(point.y());
  write_double(point.z());

  int j;
  write_int(nEdges);
  for( j= 0; j < nEdges; ++j )
    write_int(edges[j]);

  write_int(nPolygons);
  for( j= 0; j < nPolygons; ++j )
    write_int(polygons[j]);

  write_int(nSVertices);
  for( j= 0; j < nSVertices; ++j )
    write_int(svertices[j]);
#undef write_double
#undef write_int

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLVertex::serialRead( XDRFile* file )
{
  if( file == 0 )
    return false;

#define read_double(c) { double r; if(!file->getDouble(&r)) return false; c=r; }
#define read_int(it) { int _it; if(!file->getInteger(&_it)) return false; it=_it; }

  read_int(flags);

  read_double(point[0]);
  read_double(point[1]);
  read_double(point[2]);

  read_int(nEdges);
  delete edges;
  edges= 0;
  if( nEdges > 0 )
    {
    edges= new int [ nEdges ];
    for( int j= 0; j < nEdges; ++j )
      read_int(edges[j]);
    }

  read_int(nPolygons);
  delete polygons;
  polygons= 0;
  if( nPolygons > 0 )
    {
    polygons= new int [ nPolygons ];
    for( int j= 0; j < nPolygons; ++j )
      read_int(polygons[j]);
    }

  read_int(nSVertices);
  delete svertices;
  svertices= 0;
  if( nSVertices > 0 )
    {
    svertices= new int [ nSVertices ];
    for( int j= 0; j < nSVertices; ++j )
      read_int(svertices[j]);
    }

#undef read_double
#undef read_int

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return the size (in bytes) that will be read/written by
// serialRead()/serialWrite() on this property
int MLVertex::serialSize() const
{
  int s= 0;

  // flags
  s+= XDROp::getIntegerSize();

  // point
  s+= 3 * XDROp::getDoubleSize();

  // edges
  s+= XDROp::getIntegerSize() * ( 1 + nEdges );

  // polygons
  s+= XDROp::getIntegerSize() * ( 1 + nPolygons );

  // svertices
  s+= XDROp::getIntegerSize() * ( 1 + nSVertices );

  return s;
}

/////////////////////////////////////////////////////////////////////////////
