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
  MLNURBS_KnotVector.h

  Stephane Rehel

  January 7 1998
*/

#ifndef __MLNURBS_KnotVector_h
#define __MLNURBS_KnotVector_h

#include <assert.h>

#ifndef __MLNURBS_h
#include "MLNURBS.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLNURBS_KnotVector
{
  friend class MLNURBS_Curve;

protected:
  int m; // there are m+1 knots
  MLNURBS::real* U; // m+1 long

public:
  MLNURBS_KnotVector( int _m );

  virtual ~MLNURBS_KnotVector();

protected:
  void alloc();
  void destroy();

public:
  int getm() const
    {
    return m;
    }

  // 0 <= i <= m
  MLNURBS::real& operator [] ( int i )
    {
    assert( i >= 0 );
    assert( i <= m );
    return U[i];
    }

  // 0 <= i <= m
  MLNURBS::real get( int i ) const
    {
    assert( i >= 0 );
    assert( i <= m );
    return U[i];
    }

  void set( const MLNURBS::real* knots );

  // n+1 basis functions or n+1 control points
  // degree = p
  int findSpan( int n, int p, MLNURBS::real u ) const;
  int findSpan( MLNURBS::real u ) const;

  MLNURBS_KnotVector* duplicate() const;

  void insert( MLNURBS::real value, int span, int mul = 1 );

  int getMultiplicity( int span ) const;
  int getMultiplicity( MLNURBS::real u ) const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLNURBS_KnotVector_h
