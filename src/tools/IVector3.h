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
  IVector3.h

  Stephane Rehel, April 23rd, 1996
  from pntvect.h, from mathcls.h, etc, ..., from Aug 1992.
*/

#ifndef __IVector3_h
#define __IVector3_h

#ifndef __Common_h
#include "Common.h"
#endif

#define SCL int

class IPoint3;

/////////////////////////////////////////////////////////////////////////////
// IVector3 template class class
//
class IVector3
{
protected:
  union
    {
    struct
      {
      SCL x, y, z;
      } s;
    SCL vect[3];
    };

public:

  static const IVector3 X, Y, Z;

  IVector3()
    {}
  IVector3( const SCL x, const SCL y, const SCL z )
    {
    s.x= x;
    s.y= y;
    s.z= z;
    }
  IVector3( const IVector3& v )
    {
    s.x= v.s.x;
    s.y= v.s.y;
    s.z= v.s.z;
    }
  inline operator IPoint3 () const;

  const SCL x() const { return s.x; }
  const SCL y() const { return s.y; }
  const SCL z() const { return s.z; }
  const SCL geti( int i ) const
    {
    return vect[i];
    }

  SCL* getCoord() { return vect; }
  const SCL* getConstCoord() const { return vect; }

  SCL& operator [] ( int i )
    {
    return vect[i];
    }

  void setx( const SCL _x ) { s.x= _x; }
  void sety( const SCL _y ) { s.y= _y; }
  void setz( const SCL _z ) { s.z= _z; }

  IVector3& operator= ( const IVector3& v )
    {
    s.x= v.s.x;
    s.y= v.s.y;
    s.z= v.s.z;
    return *this;
    }
  void zero()
    {
    s.x= s.y= s.z= SCL(0);
    }

  boolean operator== ( const IVector3& v ) const
    {
    return s.x == v.s.x &&
           s.y == v.s.y &&
           s.z == v.s.z;
    }
  boolean operator!= ( const IVector3& v ) const
    {
    return ! operator== ( v );
    }
  IVector3 operator+ ( const IVector3& v ) const
    {
    return IVector3( s.x+v.s.x, s.y+v.s.y, s.z+v.s.z );
    }
  IVector3& operator+= ( const IVector3& v )
    {
    s.x+= v.s.x;
    s.y+= v.s.y;
    s.z+= v.s.z;
    return *this;
    }
  IVector3 operator- ( ) const
    {
    return IVector3( -s.x, -s.y, -s.z );
    }
  IVector3 operator- ( const IVector3& v ) const
    {
    return IVector3( s.x-v.s.x, s.y-v.s.y, s.z-v.s.z );
    }
  IVector3& operator-=( const IVector3& v )
    {
    s.x-= v.s.x;
    s.y-= v.s.y;
    s.z-= v.s.z;
    return *this;
    }
  IVector3 operator* ( const SCL d ) const
    {
    return IVector3( s.x*d, s.y*d, s.z*d );
    }
  IVector3& operator*= ( const SCL d )
    {
    s.x *= d;
    s.y *= d;
    s.z *= d;
    return *this;
    }
  IVector3 operator/ ( const SCL d ) const
    {
    return IVector3( s.x/d, s.y/d, s.z/d );
    }
  IVector3& operator/= ( const SCL d )
    {
    s.x/= d;
    s.y/= d;
    s.z/= d;
    return *this;
    }
  SCL operator| ( const IVector3& v ) const
    {
    return s.x*v.s.x + s.y*v.s.y + s.z*v.s.z;
    }
  IVector3 operator ^ ( const IVector3& v ) const
    {
    return IVector3( s.y*v.s.z - v.s.y*s.z,
                     s.z*v.s.x - v.s.z*s.x,
                     s.x*v.s.y - v.s.x*s.y );
    }

  boolean isNull() const
    {
    return s.x==SCL(0) && s.y==SCL(0) && s.z==SCL(0);
    }

  // colinear
  boolean operator / ( const IVector3& v ) const
    {
    return (*this^v).isNull();
    }

  // colinear and same orient.
  boolean operator % ( const IVector3& v ) const
    {
    return (*this/v) && sameOrient( v );
    }

  // *this and v supposed to be colinear...
  boolean sameOrient( const IVector3& v ) const
    {
    return (s.x==SCL(0)) ? (s.y==SCL(0)) ? sameSign(s.z,v.s.z)
                                         : sameSign(s.y,v.s.y)
                         : sameSign(s.x,v.s.x);
    }

  // multiply each component
  IVector3 operator & ( const IVector3& v ) const
    {
    return IVector3( s.x * v.s.x, s.y * v.s.y, s.z * v.s.z );
    }

  // multiply each component
  IVector3& operator &= ( const IVector3& v )
    {
    s.x *= v.s.x;
    s.y *= v.s.y;
    s.z *= v.s.z;
    return *this;
    }
  SCL absNorm() const
    {
    return ((s.x<0) ? (-s.x) : s.x)
         + ((s.y<0) ? (-s.y) : s.y)
         + ((s.z<0) ? (-s.z) : s.z);
    }

  SCL min() const
    {
    return ::min( s.x, ::min( s.y, s.z ) );
    }
  SCL max() const
    {
    return ::max( s.x, ::max( s.y, s.z ) );
    }

  int getMaxCoord() const
    {
    if( s.x > s.y )
      return ( s.x > s.z )  ? 0 : 2;
     else
      return ( s.y > s.z ) ? 1 : 2;
    }
  int getMinCoord() const
    {
    if( s.x < s.y )
      return ( s.x < s.z )  ? 0 : 2;
     else
      return ( s.y < s.z ) ? 1 : 2;
    }
};

/////////////////////////////////////////////////////////////////////////////

inline IVector3 operator*( const SCL d, const IVector3& v )
{
  return IVector3( v.x()*d, v.y()*d, v.z()*d );
}


/////////////////////////////////////////////////////////////////////////////
// IPoint3 class
//
class IPoint3
{
protected:
  union
    {
    struct
      {
      SCL x, y, z;
      } s;
    SCL vect[3];
    };

public:
  IPoint3()
    {}
  IPoint3( const SCL x, const SCL y, const SCL z )
    {
    s.x= x;
    s.y= y;
    s.z= z;
    }
  IPoint3( const IPoint3& p )
    {
    s.x= p.s.x;
    s.y= p.s.y;
    s.z= p.s.z;
    }

  inline operator IVector3 () const
    {
    return IVector3( s.x, s.y, s.z );
    }

  const SCL x() const { return s.x; }
  const SCL y() const { return s.y; }
  const SCL z() const { return s.z; }
  const SCL geti( int i ) const { return vect[i]; }

  SCL* getCoord() { return vect; }
  const SCL* getConstCoord() const { return vect; }

  SCL& operator [] ( int i )
    {
    return vect[i];
    }

  void setx( const SCL _x ) { s.x= _x; }
  void sety( const SCL _y ) { s.y= _y; }
  void setz( const SCL _z ) { s.z= _z; }

  IPoint3& operator= ( const IPoint3& p )
    {
    s.x= p.s.x;
    s.y= p.s.y;
    s.z= p.s.z;
    return *this;
    }
  void zero()
    {
    s.x= s.y= s.z= SCL(0);
    }

  boolean operator== ( const IPoint3& p ) const
    {
    return s.x == p.s.x &&
           s.y == p.s.y &&
           s.z == p.s.z;
    }
  boolean operator!= ( const IPoint3& p ) const
    {
    return ! operator== ( p );
    }
  IPoint3 operator+ ( const IPoint3& p ) const
    {
    return IPoint3( s.x+p.s.x, s.y+p.s.y, s.z+p.s.z );
    }
  IPoint3& operator+= ( const IPoint3& p )
    {
    s.x+= p.s.x;
    s.y+= p.s.y;
    s.z+= p.s.z;
    return *this;
    }
  IVector3 operator- ( const IPoint3& p ) const
    {
    return IVector3( s.x-p.s.x, s.y-p.s.y, s.z-p.s.z );
    }
  IPoint3 operator* ( const SCL k ) const
    {
    return IPoint3( s.x*k, s.y*k, s.z*k );
    }
  IPoint3 operator/ ( const SCL k ) const
    {
    return IPoint3( s.x/k, s.y/k, s.z/k );
    }

  IPoint3 operator + ( const IVector3& v ) const
    {
    return IPoint3( s.x+v.x(), s.y+v.y(), s.z+v.z() );
    }
  IPoint3& operator+= ( const IVector3& v )
    {
    s.x+= v.x();
    s.y+= v.y();
    s.z+= v.z();
    return *this;
    }
  IPoint3 operator - ( const IVector3& v ) const
    {
    return IPoint3( s.x-v.x(), s.y-v.y(), s.z-v.z() );
    }
  IPoint3& operator-= ( const IVector3& v )
    {
    s.x-= v.x();
    s.y-= v.y();
    s.z-= v.z();
    return *this;
    }

  IPoint3& addx( const SCL dx )
    {
    s.x+= dx;
    return *this;
    }
  IPoint3& addy( const SCL dy )
    {
    s.y+= dy;
    return *this;
    }
  IPoint3& addz( const SCL dz )
    {
    s.z+= dz;
    return *this;
    }

  SCL min() const
    {
    return ::min( s.x, ::min( s.y, s.z ) );
    }
  SCL max() const
    {
    return ::max( s.x, ::max( s.y, s.z ) );
    }
  IPoint3 min( const IPoint3& p ) const
    {
    return IPoint3( ::min( s.x, p.s.x ),
                    ::min( s.y, p.s.y ),
                    ::min( s.z, p.s.z ) );
    }
  IPoint3 max( const IPoint3& p ) const
    {
    return IPoint3( ::max( s.x, p.s.x ),
                    ::max( s.y, p.s.y ),
                    ::max( s.z, p.s.z ) );
    }

  boolean in( const IPoint3& minP, const IPoint3& maxP ) const
    {
    return    s.x >= minP.s.x && s.x <= maxP.s.x
           && s.y >= minP.s.y && s.y <= maxP.s.y
           && s.z >= minP.s.z && s.z <= maxP.s.z;
    }
};

/////////////////////////////////////////////////////////////////////////////

inline IVector3::operator IPoint3 () const
{
  return IPoint3( s.x, s.y, s.z );
}

/////////////////////////////////////////////////////////////////////////////

IVector3 min( const IVector3& v1, const IVector3& v2 );
IVector3 max( const IVector3& v1, const IVector3& v2 );
IPoint3 min( const IPoint3& v1, const IPoint3& v2 );
IPoint3 max( const IPoint3& v1, const IPoint3& v2 );

/////////////////////////////////////////////////////////////////////////////

#undef SCL

#endif // ifndef __IVector3_h

