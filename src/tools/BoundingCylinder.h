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
  BoundingCylinger.h

  Stephane Rehel

  September 13 1997
*/

#ifndef __BoundingCylinder_h
#define __BoundingCylinder_h

#ifndef __boolean_h
#include "boolean.h"
#endif

#ifndef __Vector_h
#include "Vector.h"
#endif

class BoundingSphere;

/////////////////////////////////////////////////////////////////////////////

class BoundingCylinder
{
public:
  Point origin; // on the axis
  Vector axis; // normalized
  double radius;

public:
  BoundingCylinder()
    {
    set( Point(0,0,0), Vector(0,0,1), 1e-8 );
    }

  BoundingCylinder( const Point& _origin,
                    const Vector& _axis, // not normalized
                    double _radius )

    {
    set( _origin, _axis, _radius );
    }

  BoundingCylinder( const Point& _origin, const BoundingSphere& bs );

  BoundingCylinder( const Vector& _axis, // not normalized
                    const BoundingSphere& bs );

  BoundingCylinder( const BoundingCylinder& c )
    {
    operator = (c);
    }

  ~BoundingCylinder()
    {}

  BoundingCylinder& operator = ( const BoundingCylinder& c )
    {
    origin= c.origin;
    axis= c.axis;
    radius= c.radius;

    return *this;
    }

  void set( const Point& _origin,
            const Vector& _axis, // not normalized
            double _radius );

  const Point& getOrigin() const
    {
    return origin;
    }

  // normalized
  const Vector& getAxis() const
    {
    return axis;
    }

  double getRadius() const
    {
    return radius;
    }

  boolean intersectSphere( const BoundingSphere& bs ) const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __BoundingCylinder_h
