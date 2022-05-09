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
  BoundingHalfPlane.C

  Stephane Rehel

  September 13 1997

  Careful: the interior volume (the bounded volume) if behind the
           normal
*/

#include "BoundingHalfPlane.h"
#include "BoundingSphere.h"

/////////////////////////////////////////////////////////////////////////////

BoundingHalfPlane::BoundingHalfPlane( const Point& _origin,
                                      const Vector& _normal // not normalized
                                    )
{
  double norm= _normal.norm();
  if( norm < 1e-20 )
    set( _origin, Vector(0,0,1) ); // Mmmhhh
   else
    set( _origin, _normal / norm );
}

/////////////////////////////////////////////////////////////////////////////

boolean BoundingHalfPlane::intersect( const BoundingSphere& bs ) const
{
  // distance from center to plane
  double distance= (normal | (bs.getCenter()-origin));

  return distance < bs.getRadius();
}

/////////////////////////////////////////////////////////////////////////////
