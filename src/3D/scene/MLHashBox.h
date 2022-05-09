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
  MLHashBox.h

  Stephane Rehel
  February 24 1997
*/

#ifndef __MLHashBox_h
#define __MLHashBox_h

#include <assert.h>

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

struct MLPolygon;
class Box;
class IPoint3;
class MeshRep;

/////////////////////////////////////////////////////////////////////////////

class MLHashBox
{
public:
  class Voxel
    {
    public:
      unsigned short nElements;
      unsigned short maxElements;
      int* polygons;

      Voxel()
        {
        nElements= 0;
        maxElements= 0;
        polygons= 0;
        }
      ~Voxel()
        {
        delete polygons;
        polygons= 0;
        }

      boolean add( int id );

      void alloc( int _nElements ); // reading from disk
    };

protected:
  int nx, ny, nz, nVoxels;
  Voxel* raw;
  Voxel*** array;
  Point minP, maxP;
  Vector size, voxelSize;
  int i_incr, j_incr, k_incr;
  double voxelsph_radius;

public:
  MLHashBox( const Box& box );
  virtual ~MLHashBox();

  int getSizeOf() const;

  void destroy();
  void create( int _nx, int _ny, int _nz );

  boolean allocated() const
    {
    return raw != 0;
    }

  const Voxel* get( int index ) const
    {
    assert( index >= 0 );
    assert( index < nx*ny*nz );
    return &(raw[index]);
    }

  const Voxel* get( int i, int j, int k ) const
    {
    return &(array[i][j][k]);
    }

  Voxel& operator () ( int i, int j, int k )
    {
    return array[i][j][k];
    }

  Voxel& operator () ( int index )
    {
    return raw[index];
    }

  int get_i_incr() const { return i_incr; }
  int get_j_incr() const { return j_incr; }
  int get_k_incr() const { return k_incr; }

  int getNVoxels() const { return nVoxels; }
  int getX() const { return nx; }
  int getY() const { return ny; }
  int getZ() const { return nz; }

  const Point& getMin() const { return minP; }
  const Point& getMax() const { return maxP; }
  const Vector& getSize() const { return size; }
  const Vector& getVoxelSize() const { return voxelSize; }

protected:
  void locate( const Point& p, IPoint3& ip );

public:
  void addTriangle( MeshRep* mesh, MLPolygon& polygon );
};

/////////////////////////////////////////////////////////////////////////////

#endif // __MLHashBox_h
