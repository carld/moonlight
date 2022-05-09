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
  BoundingSphere.h

  Stephane Rehel
  February 17 1997
*/

#ifndef __BoundingSphere_h
#define __BoundingSphere_h

#ifndef __Vector_h
#include "Vector.h"
#endif

class Box;

/////////////////////////////////////////////////////////////////////////////

class BoundingSphere
{
protected:
  Point center;
  double radius, radius2;

public:
  BoundingSphere()
    {
    center= Point(0,0,0);
    radius= 1e-8;
    radius2= radius * radius;
    }

  BoundingSphere( const Point& _center, double _radius )
    {
    center= _center;
    radius= _radius;
    if( radius <= 0. ) radius= 1e-8;
    radius2= radius * radius;
    }

  BoundingSphere( const BoundingSphere& bs )
    {
    operator = (bs);
    }

  ~BoundingSphere()
    {}

  BoundingSphere& operator = ( const BoundingSphere& bs )
    {
    center= bs.center;
    radius= bs.radius;
    radius2= bs.radius2;

    return *this;
    }

  void getFromPoints( const Point* points, int nPoints );
  void getFromBox( const Box& box );

  const Point& getCenter() const
    {
    return center;
    }
  double getRadius() const
    {
    return radius;
    }
  double getRadius2() const
    {
    return radius2;
    }

  boolean inHalfPlane( const Point& point,
                     const Vector& normal // normalized
                   ) const
    {
    return ((center-point) | normal) > -radius;
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __BoundingSphere_h
