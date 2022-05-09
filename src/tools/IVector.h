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
  IVector2.h

  Stephane Rehel, April 23rd, 1996
  from pntvect.h, from mathcls.h, etc, ..., from Aug 1992.
*/

#ifndef __IVector_h
#define __IVector_h

#ifndef __Common_h
#include "Common.h"
#endif

#define SCL int

class IPoint;

/////////////////////////////////////////////////////////////////////////////
// IVector class: vector in 2 dimensions
//
class IVector
{
  friend class IPoint;
protected:
  union
    {
    struct
      {
      SCL x, y;
      } s;
    SCL vect[2];
    };

public:
  IVector()
    {}
  IVector( const SCL _x, const SCL _y )
    {
    s.x= _x;
    s.y= _y;
    }
  IVector( const IVector& v )
    {
    s.x= v.s.x;
    s.y= v.s.y;
    }

  const SCL x() const { return s.x; }
  const SCL y() const { return s.y; }
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

  IVector& operator= ( const IVector& v )
    {
    s.x= v.s.x;
    s.y= v.s.y;
    return *this;
    }
  void zero()
    {
    s.x= s.y= SCL(0);
    }

  boolean operator== ( const IVector& v ) const
    {
    return s.x == v.s.x &&
           s.y == v.s.y;
    }
  boolean operator!= ( const IVector& v ) const
    {
    return ! operator== ( v );
    }
  IVector operator+ ( const IVector& v ) const
    {
    return IVector( s.x+v.s.x, s.y+v.s.y );
    }
  IVector& operator+= ( const IVector& v )
    {
    s.x+= v.s.x;
    s.y+= v.s.y;
    return *this;
    }
  IVector operator- () const
    {
    return IVector( -s.x, -s.y );
    }
  IVector operator- ( const IVector& v ) const
    {
    return IVector( s.x-v.s.x, s.y-v.s.y );
    }
  IVector& operator-= ( const IVector& v )
    {
    s.x-= v.s.x;
    s.y-= v.s.y;
    return *this;
    }
  IVector operator* ( const SCL d ) const
    {
    return IVector( s.x*d, s.y*d );
    }
  IVector& operator*= ( const SCL d )
    {
    s.x *= d;
    s.y *= d;
    return *this;
    }
  IVector operator/ ( const SCL d ) const
    {
    return IVector( s.x/d, s.y/d );
    }
  IVector& operator/= ( const SCL d )
    {
    s.x /= d;
    s.y /= d;
    return *this;
    }
  SCL operator| ( const IVector& v ) const
    {
    return s.x*v.s.x + s.y*v.s.y;
    }
  SCL operator ^ ( const IVector& v ) const
    {
    return s.x*v.s.y - v.s.x*s.y;
    }

  boolean isNull() const
    {
    return s.x==SCL(0) && s.y==SCL(0);
    }

  // colinear
  boolean operator / ( const IVector& v ) const
    {
    return (*this^v) == SCL(0);
    }

  // colinear and same orient.
  boolean operator % ( const IVector& v ) const
    {
    return (*this/v) && sameOrient(v);
    }

  // *this and v supposed to be colinear...
  boolean sameOrient( const IVector& v ) const
    {
    return (s.x==SCL(0)) ? sameSign(s.y,v.s.y)
                         : sameSign(s.x,v.s.x);
    }
  SCL absNorm() const
    {
    return ((s.x<0) ? (-s.x) : s.x)
         + ((s.y<0) ? (-s.y) : s.y);
    }

  SCL min() const
    {
    return ::min( s.x, s.y );
    }
  SCL max() const
    {
    return ::max( s.x, s.y );
    }
};

/////////////////////////////////////////////////////////////////////////////

inline IVector operator*( const SCL d, const IVector& v )
{
  return IVector( v.x()*d, v.y()*d );
}


/////////////////////////////////////////////////////////////////////////////
// IPoint class: Integer Point in 2 dimensions
//
class IPoint
{
protected:
  union
    {
    struct
      {
      SCL x, y;
      } s;
    SCL vect[2];
    };

public:
  IPoint()
    {}
  IPoint( const SCL _x, const SCL _y )
    {
    s.x= _x;
    s.y= _y;
    }

  inline operator IVector () const
    {
    return IVector( x(), y() );
    }

  const SCL x() const { return s.x; }
  const SCL y() const { return s.y; }
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

  IPoint& operator= ( const IPoint& p )
    {
    s.x= p.s.x;
    s.y= p.s.y;
    return *this;
    }
  void zero()
    {
    s.x= s.y= SCL(0);
    }

  boolean operator== ( const IPoint& p ) const
    {
    return s.x == p.s.x &&
           s.y == p.s.y;
    }
  boolean operator!= ( const IPoint& p ) const
    {
    return ! operator== ( p );
    }
  IPoint operator+ ( const IPoint& p ) const
    {
    return IPoint( s.x+p.s.x, s.y+p.s.y );
    }
  IPoint& operator+=( const IPoint& p )
    {
    s.x+= p.s.x;
    s.y+= p.s.y;
    return *this;
    }
  IVector operator- ( const IPoint& p ) const
    {
    return IVector( s.x-p.s.x, s.y-p.s.y );
    }
  IPoint operator* ( const SCL k ) const
    {
    return IPoint( s.x*k, s.y*k );
    }
  IPoint operator/ ( const SCL k ) const
    {
    return IPoint( s.x/k, s.y/k );
    }

  IPoint operator + ( const IVector& v ) const
    {
    return IPoint( s.x+v.s.x, s.y+v.s.y );
    }
  IPoint& operator+= ( const IVector& v )
    {
    s.x+= v.s.x;
    s.y+= v.s.y;
    return *this;
    }
  IPoint operator - ( const IVector& v ) const
    {
    return IPoint( s.x-v.s.x, s.y-v.s.y );
    }
  IPoint& operator-= ( const IVector& v )
    {
    s.x-= v.s.x;
    s.y-= v.s.y;
    return *this;
    }
  IPoint& operator*= ( const SCL d )
    {
    s.x *= d;
    s.y *= d;
    return *this;
    }
  IPoint& operator/= ( const SCL d )
    {
    s.x /= d;
    s.y /= d;
    return *this;
    }

  IPoint& addx( const SCL dx )
    {
    s.x+= dx;
    return *this;
    }
  IPoint& addy( const SCL dy )
    {
    s.y+= dy;
    return *this;
    }

  SCL min() const
    {
    return ::min( s.x, s.y );
    }
  SCL max() const
    {
    return ::max( s.x, s.y );
    }
  IPoint min( const IPoint& p ) const
    {
    return IPoint( ::min( s.x, p.s.x ),
                    ::min( s.y, p.s.y ) );
    }
  IPoint max( const IPoint& p ) const
    {
    return IPoint( ::max( s.x, p.s.x ),
                    ::max( s.y, p.s.y ) );
    }
};

/////////////////////////////////////////////////////////////////////////////

IVector min( const IVector& v1, const IVector& v2 );
IVector max( const IVector& v1, const IVector& v2 );
IPoint min( const IPoint& v1, const IPoint& v2 );
IPoint max( const IPoint& v1, const IPoint& v2 );

/////////////////////////////////////////////////////////////////////////////

#undef SCL

#endif // ifndef __IVector_h

