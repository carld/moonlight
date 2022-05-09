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
   MLSVertex.h

   November 10 1996
   Stephane Rehel

   Moved to tools/ on April 9 1998
*/

#ifndef __MLSVertex_h
#define __MLSVertex_h

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

#ifndef __Vector2_h
#include "tools/Vector2.h"
#endif

#include "tools/MLSerializable.h"

class MLVertex;
class MLPolygon;
template<class T> class MLDArray;

/////////////////////////////////////////////////////////////////////////////

class MLSVertex: public MLSerializable
{
public:
  int vertex; // base vertex, refinement vertex

  typedef unsigned int Flag;
  Flag flags;

  Vector normal;

  int material;
  Vector2 txtCoord;

  enum FLAG
    {
    ALLOCATED    = (1<<0),
    BASE_SVERTEX = (1<<1),
    REFINEMENT   = (1<<2), // refinement SVertex
    TMP_FLAG     = (1<<3)
    };

protected:
  void init_svertex()
    {
    normal= Vector(0,0,1);
    txtCoord= Vector2(0,0);
    vertex= 0;
    material= 0;
    flags= Flag(ALLOCATED);
    }

public:
  MLSVertex()
    {
    init_svertex();
    }

  MLSVertex( const MLSVertex& sv )
    {
    init_svertex();
    operator = ( sv );
    }

  ~MLSVertex()
    {
    vertex= 0;
    flags&= ~Flag(ALLOCATED);
    }

  MLSVertex& operator = ( const MLSVertex& sv );

  int allocated() const
    {
    return (flags & Flag(ALLOCATED)) != Flag(0);
    }

  int getSizeOf() const;

  void updateNormal( const MLDArray<MLVertex>& all_vertices,
                     const MLDArray<MLPolygon>& all_polygons );

  void unallocate();

  // return false if error
  boolean serialWrite( XDRFile* file );

  // return false if error
  boolean serialRead( XDRFile* file );

  // return the size (in bytes) that will be read/written by
  // serialRead()/serialWrite() on this property
  int serialSize() const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLSVertex_h
