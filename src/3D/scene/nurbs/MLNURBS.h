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
  MLNURBS.h

  Stephane Rehel

  January 7 1998
*/

#ifndef __MLNURBS_h
#define __MLNURBS_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

class Vector;
class Point;
class MLNURBS_Curve;
class MLNURBS_Surface;

/////////////////////////////////////////////////////////////////////////////

// m+1 knots
// n+1 basis functions or control points
// degree = p = m - n -1

class MLNURBS
{
public:
  typedef float real;

  // Curves
  static MLNURBS_Curve* getSegment( const Point& p0, const Point& p1 );
  static MLNURBS_Curve* getSegment4( const Point& p0, const Point& p1, const Point& p2, const Point& p3 );
  static MLNURBS_Curve* getSemiCircle4();
  static MLNURBS_Curve* getCircle9();
  static MLNURBS_Curve* getCircle7Triangle();
  static MLNURBS_Curve* getCircle7Square();

  // Surfaces
  static MLNURBS_Surface* getBilinear( const Point& p00,
                                       const Point& p10,
                                       const Point& p11,
                                       const Point& p01 );
  static MLNURBS_Surface* getGeneralCylinder( MLNURBS_Curve& curve,
                                              const Vector& distance );

  static MLNURBS_Surface* getSurfaceRevolution( MLNURBS_Curve& curve,
                                                double radius );

  MLNURBS()
    {}

  virtual ~MLNURBS()
    {}

  virtual boolean isCurve() const = 0;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLNURBS_h
