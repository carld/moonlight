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
  MLNURBS_Curve.h

  Stephane Rehel

  January 7 1998
*/

#ifndef __MLNURBS_Curve_h
#define __MLNURBS_Curve_h

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

/////////////////////////////////////////////////////////////////////////////

class MLNURBS_Curve: public MLNURBS
{
protected:
  MLNURBS_KnotVector kv; // m + 1 = (n + p + 1) + 1 knots

  int n; // n+1 basis functions or control points
  MLNURBS_ControlPoint* cp;

  // buffer for computation
  MLNURBS::real* left_right;
  MLNURBS::real* N_buffer;

public:
  MLNURBS_Curve( int _n, int _degree );

  virtual ~MLNURBS_Curve();

  virtual boolean isCurve() const
    {
    return true;
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
    return kv.getm();
    }

  int get_degree() const
    {
    return getm() - getn() - 1;
    }

  // 0 <= i <= m
  MLNURBS::real getKnot( int i ) const
    {
    assert( i >= 0 );
    assert( i <= getm() );
    return kv.get(i);
    }

  MLNURBS_KnotVector& getKnotVector()
    {
    return kv;
    }

  // 0 <= i <= n
  const MLNURBS_ControlPoint& getConstControlPoint( int i ) const
    {
    assert( i >= 0 );
    assert( i <= n );
    return cp[i];
    }

  // 0 <= i <= n
  MLNURBS_ControlPoint& getControlPoint( int i )
    {
    assert( i >= 0 );
    assert( i <= n );
    return cp[i];
    }

  void setControlPoints( const Point* points,
                         const MLNURBS::real* weights = 0 );

  Point get( MLNURBS::real u ) const;

  void transform( const Matrix4x4& xform );

  void getControlPointsBBox( Point& minPoint, double& minW,
                             Point& maxPoint, double& maxW ) const;

  MLNURBS_Curve* duplicate() const;

  void insertKnot( MLNURBS::real u );

  // ask to remove a knot num times
  // If u has been removed t times, returns t
  int removeKnot( int knto_index, int num );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLNURBS_Curve_h
