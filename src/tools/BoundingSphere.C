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
  BoundingSphere.C

  Stephane Rehel
  February 17 1997
*/

#include "BoundingSphere.h"
#include "Box.h"

/////////////////////////////////////////////////////////////////////////////

// An Efficient Bounding Sphere
// by Jack Ritter (BoundSphere.c)
// from "Graphics Gems", Academic Press, 1990

// this is an awful algo which is definitly inefficient!

void BoundingSphere::getFromPoints( const Point* points, int nPoints )
{
/*
  if( nPoints < 1 )
    {
    center= Point(0,0,0);
    radius= 1e-8;
    radius2= radius * radius;
    return;
    }

  // FIRST PASS: find 6 minima/maxima points
  int xmini= 0, ymini= 0, zmini= 0;
  int xmaxi= 0, ymaxi= 0, zmaxi= 0;

  int i;
  for( i= 1; i < nPoints; ++i )
    {
    register const Point& p= points[i];
    if( p.x() < points[xmini].x() ) xmini= i; else
    if( p.x() > points[xmaxi].x() ) xmaxi= i;
    if( p.y() < points[ymini].y() ) ymini= i; else
    if( p.y() > points[ymaxi].y() ) ymaxi= i;
    if( p.z() < points[zmini].z() ) zmini= i; else
    if( p.z() > points[zmaxi].z() ) zmaxi= i;
    }

  const Point& xmin= points[xmini];
  const Point& xmax= points[xmaxi];
  const Point& ymin= points[ymini];
  const Point& ymax= points[ymaxi];
  const Point& zmin= points[zmini];
  const Point& zmax= points[zmaxi];

  // Set xspan = distance between the 2 points xmin & xmax (squared)
  double xspan= (xmax-xmin).norm2();
  double yspan= (ymax-ymin).norm2();
  double zspan= (zmax-zmin).norm2();

  // Set points dia1 & dia2 to the maximally seperated pair
  const Point* dia1= &xmin;
  const Point* dia2= &xmax;
  double maxspan= xspan;
  if( yspan > maxspan )
    {
    maxspan= yspan;
    dia1= &ymin;
    dia2= &ymax;
    }
  if( zspan > maxspan )
    {
    maxspan= zspan;
    dia1= &zmin;
    dia2= &zmax;
    }

  if( maxspan < 1e-20 )
    {
    center= Point(0,0,0);
    radius= 1e-8;
    radius2= radius * radius;
    return;
    }

  // dia1,dia2 is a diameter of initial sphere
  // calc initial center
  center= (*dia1+*dia2) * 0.5;
  // calculate initial radius**2 and radius
  radius2= (*dia2-center).norm2();
  radius= sqrt(radius2);

  // SECOND PASS: increment current sphere
  for( i= 0; i < nPoints; ++i )
    {
    const Point& p= points[i];

    double old_to_p_sq= (p-center).norm2();
    if( old_to_p_sq > radius2 ) // do r**2 test first
      {
      // this point is outside of current sphere
      double old_to_p= sqrt(old_to_p_sq);
      // calc radius of new sphere
      radius= (radius + old_to_p) * 0.5;
      radius2= radius * radius; // for next r**2 compare
      double old_to_new= old_to_p - radius;
      // calc center of new sphere
      center= (center*radius + p*old_to_new) / old_to_p;
      }
    }
*/

  if( nPoints <= 0 )
    {
    center= Point(0,0,0);
    radius= 1e-8;
    radius2= radius * radius;
    return;
    }

  Point minP= points[0];
  Point maxP= points[0];
  int i;
  for( i= 1; i < nPoints; ++i )
    {
    const Point& p= points[i];
    minP= min( minP, p );
    maxP= max( maxP, p );
    }
  center= (minP + maxP) * 0.5;
  radius2= sqr(1e-8);
  for( i= 1; i < nPoints; ++i )
    {
    register const Point& p= points[i];
    register double r2= (center - p).norm2();
    if( r2 > radius2 )
      radius2= r2;
    }
  radius= sqrt(radius2);
}

/////////////////////////////////////////////////////////////////////////////

void BoundingSphere::getFromBox( const Box& box )
{
  center= box.getCenter();
  radius2= box.getDiag().norm2() * 0.25;
  radius= sqrt(radius2);
}

/////////////////////////////////////////////////////////////////////////////
