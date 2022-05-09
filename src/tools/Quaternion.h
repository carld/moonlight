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
  Quaternion.h

  Stephane Rehel

  August 21 1997
*/

#ifndef __Quaternion_h
#define __Quaternion_h

#ifndef __Vector_h
#include "Vector.h"
#endif

class Matrix4x4;

/////////////////////////////////////////////////////////////////////////////

class Quaternion
{
protected:
  double s;
  Vector v;

public:
  Quaternion()
    {
    s= 0;
    v= Vector(0,0,0);
    }

  Quaternion( double _s, const Vector& _v )
    {
    s= _s;
    v= _v;
    }

  ~Quaternion()
    {}

  double gets() const
    {
    return s;
    }

  const Vector& getv() const
    {
    return v;
    }

  // conjuguate
  Quaternion operator ~ () const
    {
    return Quaternion( s, -v );
    }

  Quaternion operator + ( const Quaternion& q ) const
    {
    return Quaternion( s + q.s, v + q.v );
    }
  Quaternion operator - ( const Quaternion& q ) const
    {
    return Quaternion( s - q.s, v - q.v );
    }
  Quaternion operator - () const
    {
    return Quaternion( -s, -v );
    }

  const Quaternion& operator += ( const Quaternion& q )
    {
    s+= q.s;
    v+= q.v;
    return *this;
    }
  const Quaternion& operator -= ( const Quaternion& q )
    {
    s-= q.s;
    v-= q.v;
    return *this;
    }

  Quaternion operator * ( const Quaternion& q ) const
    {
    return Quaternion( s*q.s - (v|q.v),
                       s*q.v + q.s*v + (v^q.v) );
    }

  double norm2() const
    {
    return s*s + v.norm2();
    }

  double norm() const
    {
    return sqrt(norm2());
    }

  void setRotation( double theta, const Vector& axis )
    {
    theta *= 0.5;
    s= cos(theta);
    v= sin(theta) * axis;
    }

  // in case the quaternion is a rotation
  void getRotationMatrix( Matrix4x4& m ) const;

  // m is a rotation matrix
  void makeFromRotationMatrix( const Matrix4x4& m );

  // Spherical interpolation between p and q, parameter t
  static Quaternion slerp( const Quaternion& p,
                           const Quaternion& q,
                           double t );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef  __Quaternion_h
