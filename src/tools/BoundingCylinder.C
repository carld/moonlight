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
  BoundingCylinger.C

  Stephane Rehel

  September 13 1997
*/

#include "BoundingCylinder.h"
#include "BoundingCone.h"
#include "BoundingSphere.h"

/////////////////////////////////////////////////////////////////////////////

BoundingCylinder::BoundingCylinder( const Point& _origin,
                                    const BoundingSphere& bs )
{
  Vector axis= bs.getCenter() - _origin;
  double axis_norm2= axis.norm2();
  double axis_norm= sqrt(axis_norm2);

  if( axis_norm < 1e-20 )
    {
    set( _origin, Vector(0,0,1), 1e30 );
    return;
    }

  set( _origin, axis, bs.getRadius() );
}

/////////////////////////////////////////////////////////////////////////////

BoundingCylinder::BoundingCylinder( const Vector& _axis, // not normalized
                                    const BoundingSphere& bs )
{
  set( bs.getCenter(), _axis, bs.getRadius() );
}

/////////////////////////////////////////////////////////////////////////////

void BoundingCylinder::set( const Point& _origin,
                            const Vector& _axis, // not normalized
                            double _radius )
{
  double axis_norm= _axis.norm();
  if( axis_norm < 1e-20 )
    {
    axis= Vector(0,0,1);
    _radius= 1e30;
    }
   else
    axis= _axis / axis_norm;

  origin= _origin;
  radius= _radius;
  if( radius < 0. )
    radius= 1e-8;
}

/////////////////////////////////////////////////////////////////////////////

boolean BoundingCylinder::intersectSphere( const BoundingSphere& bs ) const
{
  double distance= (axis ^ (bs.getCenter()-origin)).norm() * 0.5;

  return distance < radius + bs.getRadius();
}

/////////////////////////////////////////////////////////////////////////////
