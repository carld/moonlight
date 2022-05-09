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
  MLNURBS_Surface.h

  Stephane Rehel

  January 8 1998
*/

#ifndef __MLNURBS_Surface_h
#define __MLNURBS_Surface_h

#include <assert.h>

#ifndef __MLNURBS_h
#include "MLNURBS.h"
#endif

#ifndef __MLNURBS_KnotVector_h
#include "MLNURBS_KnotVector.h"
#endif

#ifndef __MLNURBS__ControlPoint_h
#include "MLNURBS_ControlPoint.h"
#endif

class Matrix4x4;
class MLNURBS_ControlPoint;

/////////////////////////////////////////////////////////////////////////////

class MLNURBS_Surface: public MLNURBS
{
protected:
  MLNURBS_KnotVector kvu; // r + 1 = (n + p + 1) + 1 knots in u direction
  MLNURBS_KnotVector kvv; // s + 1 = (m + q + 1) + 1 knots in v direction

  int n; // n+1 basis functions or control points in u direction
  int m; // m+1 basis functions or control points in v direction
  MLNURBS_ControlPoint** cp;

  // buffer for computation
  MLNURBS::real* left_right;
  MLNURBS::real* Nu_buffer;
  MLNURBS::real* Nv_buffer;

public:
  MLNURBS_Surface( int _n, int _u_degree, int _m, int _v_degree );

  virtual ~MLNURBS_Surface();

  virtual boolean isCurve() const
    {
    return false;
    }

protected:
  void alloc();
  void destroy();

public:
  int getn() const
    {
    return n;
    }
  int getm() const
    {
    return m;
    }

  int getr() const
    {
    return kvu.getm();
    }
  int gets() const
    {
    return kvv.getm();
    }

  int get_u_degree() const
    {
    return getr() - getn() - 1;
    }
  int get_v_degree() const
    {
    return gets() - getm() - 1;
    }

  // 0 <= i <= r
  MLNURBS::real getUKnot( int i ) const
    {
    assert( i >= 0 );
    assert( i <= getr() );
    return kvu.get(i);
    }
  // 0 <= i <= s
  MLNURBS::real getVKnot( int i ) const
    {
    assert( i >= 0 );
    assert( i <= gets() );
    return kvv.get(i);
    }

  MLNURBS_KnotVector& getUKnotVector()
    {
    return kvu;
    }
  MLNURBS_KnotVector& getVKnotVector()
    {
    return kvv;
    }

  // 0 <= i <= n
  // 0 <= j <= m
  const MLNURBS_ControlPoint& getConstControlPoint( int i, int j ) const
    {
    assert( i >= 0 );
    assert( i <= n );
    assert( j >= 0 );
    assert( j <= m );
    return cp[i][j];
    }

  // 0 <= i <= n
  // 0 <= j <= m
  MLNURBS_ControlPoint& getControlPoint( int i, int j )
    {
    assert( i >= 0 );
    assert( i <= n );
    assert( j >= 0 );
    assert( j <= m );
    return cp[i][j];
    }

  Point get( MLNURBS::real u, MLNURBS::real v ) const;

  void transform( const Matrix4x4& xform );

  MLNURBS_Surface* duplicate() const;

  void insertUKnot( MLNURBS::real u );
  void insertVKnot( MLNURBS::real v );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLNURBS_Surface_h
