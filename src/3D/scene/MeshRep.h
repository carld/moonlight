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
   MeshRep.h

   November 10 1996
   Stephane Rehel
*/

#ifndef __MeshRep_h
#define __MeshRep_h

#ifndef __Box_h
#include "tools/Box.h"
#endif

#ifndef __BoundingSphere_h
#include "tools/BoundingSphere.h"
#endif

#ifndef __IArray_h
#include "tools/IArray.h"
#endif

#ifndef __MLVertexArray_h
#include "MLVertexArray.h"
#endif

#ifndef __MLEdgeArray_h
#include "MLEdgeArray.h"
#endif

#ifndef __MLPolygonArray_h
#include "MLPolygonArray.h"
#endif

#ifndef __MeshSVertexArray_h
#include "MLSVertexArray.h"
#endif

class MLHashBox;

/////////////////////////////////////////////////////////////////////////////

class MeshRep
{
  friend struct MLPolygon;

public:
  int nVertices;
  MLVertexArray vertices; // of MLVertex

  int nEdges;
  MLEdgeArray edges; // of MLEdges

  // This array may be larger than the vertices array
  int nSVertices;
  MLSVertexArray svertices;

  int nPolygons;
  MLPolygonArray polygons; // of MLPolygon

  float discAngle; // degrees
  float cos_discAngle;
  float smallDistance; // for merging points

  Box bbox;
  BoundingSphere boundingSphere;

  MLHashBox* hashBox;

  boolean dirtyBoundings;
  boolean dirtyHashBox;

//--

  MeshRep();
  virtual ~MeshRep();

  int getSizeOf() const;

  MeshRep* duplicate();

  void clear();

  void alloc( int _nVertices, int _nEdges, int _nPolygons );

  // find edges from vertices[] and polygons.vertices[]
  // -> Build edges[].v{1,2}
  // Cost=O(nEdges*log(nEdges))
  void findEdges();

  // from: vertices[], edges[].v{1,2} and polygons.vertices[]
  // build: vertices.edges[], edges.p{1,2}, polygons.edges[],
  //        svertices[], polygons.svertices[]
  void finalizeRep();

  // Update SVertices of Vertex vi
  void updateSVertices( int vi );

  // Update SVertices of Polygon pi
  // This a recursive function. Can get base/tesselation/refinement
  // polygon
  void updatePolygonSVertices( int pi );

private:
  // Update SVertices of Polygon p
  void _updatePolygonSVertices( MLPolygon& p );

public:
//private:
  // Cost: O(n*log(n)), n= nVertices
  // Updates: vertices[], edges[].v{1,2}, polygons[].vertices[]
  void mergeVertices();

  // bounding box and bounding sphere
  // return false if mesh is unable to bound
  boolean computeBoundings();

  boolean buildNewHashBox();

  // translating a vertex for example
  // also invalidates hashbox
  boolean invalidateBoundings();

  boolean invalidateHashBox();

  MLPolygon& addPolygon();

protected:
  // in MeshRepPolygonTesselate.C
  void tesselatePolygon( int pi );

public:
  void tesselate();

  // reindex vertices[] and remove holes
  void reindexVertices();

  // reindex edges[] and remove holes
  void reindexEdges();

  // reindex svertices[] and remove holes
  void reindexSVertices();

  // reindex polygons[] and remove holes
  void reindexPolygons();

  int computeNBaseEdges() const;

  int computeNBasePolygons() const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MeshRep_h
