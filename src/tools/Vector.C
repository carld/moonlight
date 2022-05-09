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
  Vector.C

  from mathcls.cc

  Math Classes

  Stephane Rehel, SCFR::Kepler, Stephane.Rehel@imag.fr

*/

#include "Vector.h"

/////////////////////////////////////////////////////////////////////////////

// Return the angle (rads) between vectors v1 and v2
double angle( const Vector& v1, const Vector& v2 )
{
  double mag= sqrt( v1.norm2() * v2.norm2() );

  if( mag == 0. )
    return 0.;

  double cos_theta = (v1 | v2) / mag;

  if( cos_theta <= -0.99 )
    return M_PI;

  if( cos_theta >= +0.99 )
    return 0.;

  return acos( cos_theta );
}


/////////////////////////////////////////////////////////////////////////////

// Return the angle (rads) between vectors (p1-p) and (p2-p)
double angle( const Point& p, const Point& p1, const Point& p2 )
{
  return angle( p1 - p, p2 - p );
}

/////////////////////////////////////////////////////////////////////////////

boolean insideTriangle( const Point& p,
                      const Point& p1,
                      const Point& p2,
                      const Point& p3 )
{
  Vector sum, cross;

  const Vector v1= p - p1;
  const Vector v2= p - p2;
  const Vector v3= p - p3;

  cross= v1 ^ v2;
  sum= cross * (angle(v1,v2) / cross.norm());

  cross= v2 ^ v3;
  sum+= cross * (angle(v2,v3) / cross.norm());

  cross= v3 ^ v1;
  sum+= cross * (angle(v3,v1) / cross.norm());

  return sum.norm2() > M_PI*M_PI;
}

/////////////////////////////////////////////////////////////////////////////

// assuming *this as Z vector
// X and Y are not normalized
void Vector::makeOrthoBase( Vector& X, Vector& Y ) const
{
  if( z() != 0. ) X= Vector(1,0,0); else
  if( y() != 0. ) X= Vector(0,0,1); else
  if( x() != 0. ) X= Vector(0,1,0); else
    {
    // god, *this is null!
    X= Vector(1,0,0);
    Y= Vector(0,1,0);
    return;
    }

  Y= (*this) ^ X;
  X= Y ^ (*this);
}

/////////////////////////////////////////////////////////////////////////////
