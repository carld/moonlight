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
   MLVertex.h

   November 10 1996
   Stephane Rehel

   Moved to tools/ on April 3 1998
*/

#ifndef __MLVertex_h
#define __MLVertex_h

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

#include "tools/MLSerializable.h"

class MLEdge;
template<class T> class MLDArray;

/////////////////////////////////////////////////////////////////////////////

class MLVertex: public MLSerializable
{
public:
  Point point;

  short nEdges; // base edges, tesselation edges, refinement edges
  int* edges;

  short nPolygons; // CAREFUL: only base polygons
  int* polygons;

  short nSVertices; // base svertices, refinement svertices
  int* svertices;

  typedef unsigned int Flag;
  Flag flags;

  enum FLAG
    {
    ALLOCATED    = (1<<0),
    BASE_VERTEX  = (1<<1), // base vertex
    REFINEMENT   = (1<<2), // refinement vertex
    DISC         = (1<<3), // discontinuity vertex
    SELECTED     = (1<<4), // i.e. TAG
    TMP_FLAG     = (1<<5)
    };

protected:
  void init_vertex()
    {
    point= Point(0,0,0);
    nSVertices= nEdges= nPolygons= 0;
    edges= 0;
    polygons= 0;
    svertices= 0;
    flags= Flag(ALLOCATED);
    }

public:
  MLVertex()
    {
    init_vertex();
    }

  MLVertex( const MLVertex& v )
    {
    init_vertex();
    operator = ( v );
    }

  ~MLVertex()
    {
    delete edges; edges= 0; nEdges= 0;
    delete polygons; polygons= 0; nPolygons= 0;
    delete svertices; svertices= 0; nSVertices= 0;
    flags &= ~Flag(ALLOCATED);
    }

  MLVertex& operator = ( const MLVertex& v );

  int getSizeOf() const;

  int allocated() const
    {
    return (flags & Flag(ALLOCATED)) != Flag(0);
    }

  int selected() const
    {
    return (flags & Flag(SELECTED)) != Flag(0);
    }

  void unallocate();

  void addEdge( int ei );

  // Find edge from this vertex (v1) to vertex v2
  // return edge index (>=1) if found
  // return -1 if not found
  int findEdge( const MLDArray<MLEdge>& all_edges, int v1, int v2 ) const;

  // return false if error
  boolean serialWrite( XDRFile* file );

  // return false if error
  boolean serialRead( XDRFile* file );

  // return the size (in bytes) that will be read/written by
  // serialRead()/serialWrite() on this property
  int serialSize() const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLVertex_h
