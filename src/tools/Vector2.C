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
  Vector2.C

  from mathcls.cc

  Math Classes

  Stephane Rehel, SCFR::Kepler, Stephane.Rehel@imag.fr
*/

#include "Vector2.h"

/////////////////////////////////////////////////////////////////////////////

// Return the angle (rads) between vectors v1 and v2
double angle( const Vector2& v1, const Vector2& v2 )
{
  double mag= sqrt( v1.norm2() * v2.norm2() );

  if( mag == 0. )
    return 0.;

  double cos_theta = (v1|v2) / mag;

  if( cos_theta <= -1. )
    return M_PI;

  if( cos_theta >= +1. )
    return 0.;

  double a= acos( cos_theta );

  if( a < DEGtoRAD(0.01) )
    return 0.;

  if( a > DEGtoRAD(179.99) )
    return M_PI;

  return a;
}

/////////////////////////////////////////////////////////////////////////////

// Return the angle (rads) between vectors (p1-p) and (p2-p)
double angle( const Point2& p, const Point2& p1, const Point2& p2 )
{
  return angle( p1 - p, p2 - p );
}

/////////////////////////////////////////////////////////////////////////////

boolean insideTriangle( const Point2& p,
                        const Point2& p1,
                        const Point2& p2,
                        const Point2& p3 )
{
  double sum, cross;

  const Vector2 v1= p - p1;
  const Vector2 v2= p - p2;
  const Vector2 v3= p - p3;

  cross= v1 ^ v2;
  if( cross < 0. )
    sum= -angle(v1,v2);
   else
    sum= angle(v1,v2);

  cross= v2 ^ v3;
  if( cross < 0. )
    sum-= angle(v2,v3);
   else
    sum+= angle(v2,v3);

  cross= v3 ^ v1;
  if( cross < 0. )
    sum-= angle(v3,v1);
   else
    sum+= angle(v3,v1);

  return fabs(sum) > M_PI;
}

/////////////////////////////////////////////////////////////////////////////

