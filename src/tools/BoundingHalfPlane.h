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
  BoundingHalfPlane.h

  Stephane Rehel

  September 13 1997

  Careful: the interior volume (the bounded volume) if behind the
           normal
*/

#ifndef __BoundingHalfPlane_h
#define __BoundingHalfPlane_h

#ifndef __boolean_h
#include "boolean.h"
#endif

#ifndef __Vector_h
#include "Vector.h"
#endif

class BoundingSphere;

/////////////////////////////////////////////////////////////////////////////

class BoundingHalfPlane
{
protected:
  Point origin;
  Vector normal; // normalized

public:
  BoundingHalfPlane()
    {
    set( Point(0,0,0), Vector(0,0,1) );
    }

  BoundingHalfPlane( const Point& _origin,
                     const Vector& _normal // not normalized
                   );

  void set( const Point& _origin,
            const Vector& _normal // *normalized*
          )
    {
    origin= _origin;
    normal= _normal;
    }

  const Point& getOrigin() const
    {
    return origin;
    }

  // normalized
  const Vector& getNormal() const
    {
    return normal;
    }

  boolean intersect( const BoundingSphere& bs ) const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __BoundingHalfPlane_h
