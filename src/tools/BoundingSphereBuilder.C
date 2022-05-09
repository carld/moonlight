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
  BoundingSphereBuilder.C

  Stephane Rehel

  September 12 1997

  How to use it:
    BoundingSphereBuilder bsb;
    bsb.init(); // optional
    while( iterate on points p )
      bsb.pass1(p);
    bsb.end_pass1();
    while( iterate on points p )
      bsb.pass2(p);
    BoundingSphere bs= bsb.get();

->GGEMS-1: this is an awful algo which is definitly inefficient!
*/

#include "BoundingSphereBuilder.h"

/////////////////////////////////////////////////////////////////////////////

BoundingSphereBuilder::BoundingSphereBuilder()
{
  init();
}

/////////////////////////////////////////////////////////////////////////////

BoundingSphereBuilder::~BoundingSphereBuilder()
{
  nPoints= 0;

  xmin= xmax= 0;
  ymin= ymax= 0;
  zmin= zmax= 0;
}

/////////////////////////////////////////////////////////////////////////////

void BoundingSphereBuilder::init()
{
  nPoints= 0;
  center= Point(0,0,0);
  radius= 1e-8;
  radius2= radius * radius;

  xmin= xmax= 0;
  ymin= ymax= 0;
  zmin= zmax= 0;
}

/////////////////////////////////////////////////////////////////////////////

BoundingSphere BoundingSphereBuilder::get() const
{
  if( nPoints < 1 )
    return BoundingSphere( Point(0,0,0), 1e-8 );

  return BoundingSphere(center,radius);
}

/////////////////////////////////////////////////////////////////////////////

void BoundingSphereBuilder::pass1( const Point& p )
{
  if( nPoints == 0 )
    {
    xmin= &p;
    ymin= &p;
    zmin= &p;
    xmax= &p;
    ymax= &p;
    zmax= &p;
    }
   else
    {
    if( p.x() < xmin->x() ) xmin= &p; else
    if( p.x() > xmax->x() ) xmax= &p;
    if( p.y() < ymin->y() ) ymin= &p; else
    if( p.y() > ymax->y() ) ymax= &p;
    if( p.z() < zmin->z() ) zmin= &p; else
    if( p.z() > zmax->z() ) zmax= &p;
    }

  ++nPoints;
}

/////////////////////////////////////////////////////////////////////////////

void BoundingSphereBuilder::end_pass1()
{
  if( nPoints == 0 )
    return;

  // Set xspan = distance between the 2 points xmin & xmax (squared) */
  double xspan= (*xmax - *xmin).norm2();
  double yspan= (*ymax - *ymin).norm2();
  double zspan= (*zmax - *zmin).norm2();

  // Set points dia1 & dia2 to the maximally seperated pair
  const Point* dia1= xmin;
  const Point* dia2= xmax;
  double maxspan= xspan;
  if( yspan > maxspan )
    {
    maxspan= yspan;
    dia1= ymin;
    dia2= ymax;
    }
  if( zspan > maxspan )
    {
    maxspan= zspan;
    dia1= zmin;
    dia2= zmax;
    }

  if( maxspan < 1e-20 )
    {
    nPoints= 0;
    return;
    }

  // dia1,dia2 is a diameter of initial sphere */
  // calc initial center
  center= (*dia1 + *dia2) * 0.5;
  // calculate initial radius**2 and radius
  radius2= (*dia2 - center).norm2();
  radius= sqrt(radius2);
}

/////////////////////////////////////////////////////////////////////////////

void BoundingSphereBuilder::pass2( const Point& p )
{
  if( nPoints == 0 )
    return;

  // SECOND PASS: increment current sphere

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

/////////////////////////////////////////////////////////////////////////////
