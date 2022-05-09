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
  poly2tri.C

  From:
    |                               DXF2TRI
    |              DXF polyline/polygon to triangle converter
    |                       by Tim Riley   Aug 1993
    |                              based on
    |              POLY2TRI - a polygon to triangle converter
    |                       by Steve Anger Jan 1993
    |  This program is commited to the public domain. Feel free to use
    |  all or part of this code in your own programs.

  Creation: SR, February 3rd, 1996
  Revisions:

*/

#include "poly2tri.h"

/////////////////////////////////////////////////////////////////////////////

// Determines if the specified point 'p' is inside the polygon.
// Uses a convoluted version of the sum of angles approach
static int poly_inside( Point* poly, int polysize, const Point& p );

// Return the angle (rads) between two vectors
static DBL vect_angle( const Vector& v1, const Vector& v2 );

/////////////////////////////////////////////////////////////////////////////

// Removes a triangle from the specified polygon.
// The size of the polygon is reduced
boolean remove_triangle( Point* poly, int& polysize,
                      Point& v1, Point& v2, Point& v3 )
{
  if( polysize < 3 )
    return false; // No triangle found

  // This simplest case
  if( polysize == 3 )
    {
    v1= poly[0];
    v2= poly[1];
    v3= poly[2];
    polysize= 0;

    return true; // Ok
    }

  for( int i= 0; i < polysize; i++ )
    {
    int a = i;
    int b = (i + 1) % polysize;
    int c = (i + 2) % polysize;

    Point tri[3];
    // Select a candidate triangle
    tri[0]= poly[a];
    tri[1]= poly[b];
    tri[2]= poly[c];

    // Calculate the center of the triangle
    Point center= (tri[0] + tri[1] + tri[2]) / 3.;

    // Is the center of the triangle inside the original polygon?
    // If not skip this triangle
    if( !poly_inside( poly, polysize, center ) )
      continue;

    // Are any of the polygons other vertices inside the triangle
    // If so skip this triangle
    int j;
    for( j= 0; j < polysize; j++ )
      if( j != a && j != b && j != c )
        if( poly_inside( tri, 3, poly[j] ) )
          break;
    if( j >= polysize )
      {
      // This is the one
      v1= tri[0];
      v2= tri[1];
      v3= tri[2];

      // Remove this triangle from the polygon
      --polysize;
      for( j= b; j < polysize; j++ )
        poly[j]= poly[j+1];

      return true; // Ok
      }
    }

  return false; // No triangle found
}

/////////////////////////////////////////////////////////////////////////////

// Determines if the specified point 'p' is inside the polygon.
// Uses a convoluted version of the sum of angles approach
static int poly_inside( Point* poly, int polysize, const Point& p )
{
  Vector sum(0,0,0);

  for( int i= 0; i < polysize; i++ )
    {
    const Vector v1= p -  poly[i];
    const Vector v2= p -  poly[(i+1) % polysize];

    Vector cross= v1 ^ v2;
    DBL magcross= cross.norm();

    if( magcross > 0. )
      cross /= magcross;

    cross *= vect_angle( v1, v2 );
    sum+= cross;
    }

  return sum.norm2() > M_PI*M_PI;
}

/////////////////////////////////////////////////////////////////////////////

// Return the angle (rads) between two vectors
static DBL vect_angle( const Vector& v1, const Vector& v2 )
{
  DBL mag1= v1.norm();
  DBL mag2= v1.norm();

  DBL m= mag1 * mag2;
  if( m < 1e-15 )
    return 0.;

  DBL cos_theta = (v1|v2) / (mag1 * mag2);

  if( cos_theta <= -1.0 )
    return M_PI;
  if( cos_theta >= +1.0 )
    return 0.;

  return acos(cos_theta);
}

/////////////////////////////////////////////////////////////////////////////

