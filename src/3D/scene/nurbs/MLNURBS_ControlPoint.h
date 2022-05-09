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
  MLNURBS_ControlPoint.h

  Stephane Rehel

  January 8 1998
*/

#ifndef __MLNURBS_ControlPoint_h
#define __MLNURBS_ControlPoint_h

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

#ifndef __HVector_h
#include "tools/HVector.h"
#endif

#ifndef __MLNURBS_h
#include "MLNURBS.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLNURBS_ControlPoint
{
public:
  Point point;
  MLNURBS::real w;
  HVector hpoint;

  typedef unsigned int Flag;
  Flag flags;

  enum FLAG
    {
    SELECTED     = (1<<0), // i.e. TAG
    TMP_FLAG     = (1<<1)
    };

  int selected() const
    {
    return (flags & Flag(SELECTED)) != Flag(0);
    }

  MLNURBS_ControlPoint(): point(0,0,0), w(1.), hpoint(0,0,0,1), flags(0)
    {}

  ~MLNURBS_ControlPoint()
    {}

  MLNURBS_ControlPoint& operator = ( const MLNURBS_ControlPoint& cp )
    {
    point= cp.point;
    w= cp.w;
    hpoint= cp.hpoint;
    flags= cp.flags;

    return *this;
    }

  void set( const Point& _point, MLNURBS::real _w );
  void set( const HVector& _hpoint );

  void setPoint( const Point& _point );
  void setWeight( MLNURBS::real _w );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLNURBS_ControlPoint_h
