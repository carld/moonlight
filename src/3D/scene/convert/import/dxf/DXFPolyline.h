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
  DXFPolyline.C

  Revision:
    Stephane Rehel
    April 25 1997
*/

#ifndef __DXFPolyline_h
#define __DXFPolyline_h

#include <stdio.h>

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

class DXFExport;

/////////////////////////////////////////////////////////////////////////////

class DXFPolyline
{
public:
  FILE* fstderr;
  static const int maxVertices;
  static const int nArcSubdivide;
  int nVertices;
  int flags; // = group 70
  int N, M; // N = group 71, M = group 72
  int group75; // attribute for MeshSurface
  double thickness;
  Vector extrusionDir;

  struct Vertex
    {
    Point p;
    int flags; // = group 70
    int indices[4]; // groups from 71 to 74
    float bulge; // bulge = english('arrondi') = group 42
    // flags access inline functions:
    boolean isPolyline3DVertex() const
      { return (flags & 32) != 0; }
    boolean isMeshSurfaceVertex() const
      { return (flags & 64) != 0; }
    boolean isPolyMeshVertex() const
      { return (flags & 128) != 0; }
    }
    *vertices;
  Point* points;
  int maxPoints, nPoints;

  // flags access inline functions:
  boolean isClosed() const
    { return (flags & 1) != 0; }
  boolean isClosedM() const
    { return isClosed(); }
  boolean isClosedN() const
    { return (flags & 32) != 0; }
  boolean isPolyline3D() const
    { return (flags & 8) != 0; }
  boolean isPolyMesh() const
    { return (flags & 64) != 0; }
  boolean isMeshSurface() const
    { return (flags & 16) != 0; }

  boolean addVertex( const Point& thePoint,
                  int theFlags,
                  float theBulge,
                  int index0,
                  int index1,
                  int index2,
                  int index3 );
  Vertex* getVertex( int i )
    { return vertices + i; }
  int getVertexLength( int i ) // return vertex length in *real* points
                               // (translating arcs...)
                               // ->exclude the end point from the list
    {
    return (getVertex(i)->bulge==0.) ? 1 : (nArcSubdivide-1);
    }

  boolean writeVertex( int i );
  int getPolyMeshVertex( int i ); // return an index in vertices[]
  Vertex* getMesh( int i );
  int getMeshSurfaceVertex( int m, int n ); // return vertices[] index

  boolean allocPoints( int n );
  void freePoints();
  void addPoint( const Point& p );

  boolean writePolyline3D( DXFExport* dxfExport );
  boolean writePolyMesh( DXFExport* dxfExport );
  boolean writeMeshSurface( DXFExport* dxfExport );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __DXFPolyline_h

