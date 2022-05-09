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
  HVector.h

  Homogeneous vectors classes, from homogen.h

  Stephane Rehel [SCFR::Kepler], Stephane.Rehel@imag.fr

  June 23rd 1995 from mathcls.h

  Revisions:
    + SR, April 22nd, 1996
    + SR, January 10, 1998
*/


#ifndef __HVector_h
#define __HVector_h

#ifndef __Vector_h
#include "Vector.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// Homogeneous Vector Class
//
class HVector
{
private:
  union
    {
    struct
      {
      DBL x, y, z, h;
      } s;
    DBL vect[4];
    };

public:
  HVector()
    {}
  HVector( const Vector& v )
    {
    s.x= v.x();
    s.y= v.y();
    s.z= v.z();
    s.h= 0.;
    }
  HVector( const Point& p )
    {
    s.x= p.x();
    s.y= p.y();
    s.z= p.z();
    s.h= 1.;
    }
  HVector( const HVector& v )
    {
    s.x= v.s.x;
    s.y= v.s.y;
    s.z= v.s.z;
    s.h= v.s.h;
    }
  HVector( DBL vx, DBL vy, DBL vz, DBL vh )
    {
    s.x= vx;
    s.y= vy;
    s.z= vz;
    s.h= vh;
    }

  HVector& operator=( const HVector& v )
    {
    s.x= v.s.x;
    s.y= v.s.y;
    s.z= v.s.z;
    s.h= v.s.h;
    return *this;
    }

  operator Point() const
    {
    return Point( s.x, s.y, s.z ) / s.h;
    }
  operator Vector() const
    {
    return Vector( s.x, s.y, s.z );
    }

  DBL x() const { return s.x; }
  DBL y() const { return s.y; }
  DBL z() const { return s.z; }
  DBL h() const { return s.h; }

  DBL geti( int i ) const
    {
    assert( i >= 0 );
    assert( i <= 3 );
    return vect[i];
    }
  DBL operator () ( int i ) const
    {
    assert( i >= 0 );
    assert( i <= 3 );
    return vect[i];
    }

  DBL* getCoord()
    {
    return vect;
    }
  const DBL* getConstCoord() const
    {
    return vect;
    }

  DBL& operator [] ( int i )
    {
    assert( i >= 0 );
    assert( i <= 2 );
    return vect[i];
    }

  HVector operator + ( const HVector& v ) const
    {
    return HVector( s.x+v.s.x, s.y+v.s.y, s.z+v.s.z, s.h+v.s.h );
    }
  HVector& operator += ( const HVector& v )
    {
    s.x += v.s.x;
    s.y += v.s.y;
    s.z += v.s.z;
    s.h += v.s.h;
    return *this;
    }
  HVector operator - ( const HVector& v ) const
    {
    return HVector( s.x-v.s.x, s.y-v.s.y, s.z-v.s.z, s.h-v.s.h );
    }
  HVector& operator -= ( const HVector& v )
    {
    s.x -= v.s.x;
    s.y -= v.s.y;
    s.z -= v.s.z;
    s.h -= v.s.h;
    return *this;
    }
  HVector operator * ( const DBL k ) const
    {
    return HVector( s.x*k, s.y*k, s.z*k, s.h*k );
    }
  HVector& operator *= ( const DBL k )
    {
    s.x *= k;
    s.y *= k;
    s.z *= k;
    s.h *= k;
    return *this;
    }
  HVector operator / ( const DBL k ) const
    {
    return HVector( s.x/k, s.y/k, s.z/k, s.h/k );
    }
  HVector& operator /= ( const DBL k )
    {
    s.x /= k;
    s.y /= k;
    s.z /= k;
    s.h /= k;
    return *this;
    }

  DBL absNorm() const
    {
    register DBL ah= fabs(s.h);
    if( fabs(ah) < 1e-30 )
      ah= 1.;
    return (fabs(s.x) + fabs(s.y) + fabs(s.z)) / ah;
    }
};

/////////////////////////////////////////////////////////////////////////////

inline HVector operator * ( const DBL d, const HVector& v )
{
  return HVector( v.x()*d, v.y()*d, v.z()*d, v.h()*d );
}

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __HVector_h
