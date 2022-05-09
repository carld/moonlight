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
   MLEdge.h

   November 10 1996
   Stephane Rehel

   Moved to tools/ on April 3 1998
*/

#ifndef __MLEdge_h
#define __MLEdge_h

#include "tools/MLSerializable.h"

class MLPolygon;
template<class T> class MLDArray;

/////////////////////////////////////////////////////////////////////////////

//
//               v2
//
//               /|
//                |
//    polygon1    |    polygon2
//                |
//                |
//
//                v1
//
//

class MLEdge: public MLSerializable
{
public:
  // vertices indices
  int v1, v2;              // base vertices and refinement vertices
  int polygon1, polygon2;  // base polygons, tess. polygons, ref. polygons

  typedef unsigned int Flag;
  Flag flags;

  enum FLAG
    {
    ALLOCATED   = (1<<0),

    // when was this edge created ?
    BASE_EDGE   = (1<<1), // base edge
    TESSELATION = (1<<2), // tesselation edge
    REFINEMENT  = (1<<3), // refinement edge

    // edge type
    DISC        = (1<<4), // discontinuity edge
    BORDER      = (1<<5) // border edge (one polygon is missing)
    };

protected:
  void init_edge()
    {
    v1= v2= polygon1= polygon2= 0;
    flags= Flag(ALLOCATED);
    }

public:
  MLEdge()
    {
    init_edge();
    }

  MLEdge( const MLEdge& e )
    {
    init_edge();
    operator = ( e );
    }

  ~MLEdge()
    {
    v1= v2= polygon1= polygon2= 0;
    flags &= ~Flag(ALLOCATED);
    }

  MLEdge& operator = ( const MLEdge& e );

  int getSizeOf() const;

  int allocated() const
    {
    return (flags & Flag(ALLOCATED)) != Flag(0);
    }

  void unallocate();

  void computeDisc( const MLDArray<MLPolygon>& all_polygons,
                    double cos_discAngle );

  // return false if error
  boolean serialWrite( XDRFile* file );

  // return false if error
  boolean serialRead( XDRFile* file );

  // return the size (in bytes) that will be read/written by
  // serialRead()/serialWrite() on this property
  int serialSize() const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLEdge_h
