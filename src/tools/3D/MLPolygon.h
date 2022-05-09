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
   MLPolygon.h

   November 10 1996
   Stephane Rehel

   Moved to tools/ on April 3 1998
*/

#ifndef __MLPolygon_h
#define __MLPolygon_h

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

#include "tools/MLSerializable.h"

class MLVertex;
template<class T> class MLDArray;

/////////////////////////////////////////////////////////////////////////////

class MLPolygon: public MLSerializable
{
public:
  int index;

  int nVertices;
  struct Element
    {
    int vertex;
    int edge;
    int svertex;
    };

  Element* elements;

  Vector normal; // normalized
  float area;

  int material;

  typedef unsigned int Flag;
  Flag flags;

  enum FLAG
    {
    ALLOCATED     = (1<<0),
    BASE_POLYGON  = (1<<1), // base polygon
    TESS_TRIANGLE = (1<<2), // this a a tesselation triangle
    REFINEMENT    = (1<<3), // refinement triangle
    SELECTED      = (1<<4),
    TMP_FLAG      = (1<<5)
    };

  int nTessTriangles;
  int* tesselation; // != 0 if nVertices >= 4
  int fatherPolygon; // != 0 if this is a tesselation triangle

  struct Ray_info
    {
    float x1, y1, x2, y2;
    char X, Y;
    } ray_info;

protected:
  void init_polygon()
    {
    index= 0;
    nVertices= 0;
    elements= 0;
    material= 0;
    flags= Flag(ALLOCATED);
    nTessTriangles= 0;
    tesselation= 0;
    fatherPolygon= 0;
    ray_info.X= ray_info.Y= 0;
    ray_info.x1= ray_info.y1= ray_info.x2= ray_info.y2= 0.;
    }
public:
  MLPolygon()
    {
    init_polygon();
    }

  MLPolygon( const MLPolygon& p )
    {
    init_polygon();
    operator = ( p );
    }

  ~MLPolygon()
    {
    delete elements; elements= 0;
    nVertices= 0;
    delete tesselation; tesselation= 0;
    nTessTriangles= 0;
    flags &= ~Flag(ALLOCATED);
    }

  MLPolygon& operator = ( const MLPolygon& p );

  int getSizeOf() const;

  int allocated() const
    {
    return (flags & Flag(ALLOCATED)) != Flag(0);
    }

  int selected() const
    {
    return (flags & Flag(SELECTED)) != Flag(0);
    }

  void alloc( short _nVertices );
  void allocTriangle( int v1, int v2, int v3 );
  void allocQuad( int v1, int v2, int v3, int v4 );

  // return -1 if not found
  int getVertexInternalIndex( int vi ) const
    {
    register int i= nVertices;
    while( --i >= 0 )
      if( elements[i].vertex == vi )
        return i;
    return -1;
    }

  void updateNormalArea( const MLDArray<MLVertex>& all_vertices,
                         MLDArray<MLPolygon>& all_polygons,
                         boolean recurse = false );

//  boolean intersect( MLRay& ray );

  void unallocate();

  boolean isShootingPolygon() const
    {
    return ((flags & BASE_POLYGON) != 0) && allocated();
    }

  Point getCenter( const MLDArray<MLVertex>& all_vertices ) const;

  MLPolygon& getFatherPolygon( MLDArray<MLPolygon>& all_polygons );

  // return false if error
  boolean serialWrite( XDRFile* file );

  // return false if error
  boolean serialRead( XDRFile* file );

  // return the size (in bytes) that will be read/written by
  // serialRead()/serialWrite() on this property
  int serialSize() const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLPolygon_h
