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
  BoundingCone.h

  Stephane Rehel
  Februray 17 1997
*/

#ifndef __BoundingCone_h
#define __BoundingCone_h

#ifndef __boolean_h
#include "boolean.h"
#endif

#ifndef __Vector_h
#include "Vector.h"
#endif

class BoundingSphere;

/////////////////////////////////////////////////////////////////////////////

class BoundingCone
{
public:
  Point origin;
  Vector axis; // normalized
  double cos_alpha; // alpha = half angle

public:
  BoundingCone()
    {
    set( Point(0,0,0), Vector(0,0,1), 1. );
    }

  BoundingCone( const Point& _origin,
                const Vector& _axis, // not normalized
                double _cos_alpha )
    {
    set( _origin, _axis, _cos_alpha );
    }

  BoundingCone( const Point& _origin,
                const Vector& _axis, // not normalized
                const Point& one_point );

  BoundingCone( const Point& _origin, const BoundingSphere& bs );

  BoundingCone( const BoundingCone& c )
    {
    operator = (c);
    }

  ~BoundingCone()
    {}

  BoundingCone& operator = ( const BoundingCone& c )
    {
    origin= c.origin;
    axis= c.axis;
    cos_alpha= c.cos_alpha;

    return *this;
    }

  void set( const Point& _origin,
            const Vector& _axis, // not normalized
            double _cos_alpha );

  const Point& getOrigin() const
    {
    return origin;
    }

  // normalized
  const Vector& getAxis() const
    {
    return axis;
    }

  // alpha = half angle
  double getCosAlpha() const
    {
    return cos_alpha;
    }

  boolean intersectSphere( const BoundingSphere& bsph ) const;

  // Test if *this and c intersect
  // *this and c are to have the same origin
  boolean same_origin_intersect( const BoundingCone& c ) const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __BoundingCone_h
