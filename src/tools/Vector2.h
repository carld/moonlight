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
  Vector2.h

  from mathcls.h

  Math Classes

  Stephane Rehel, SCFR::Kepler, Stephane.Rehel@imag.fr

  Thu 20 Aug 1992 /SCFR::Kepler.
  Revision: 1993
  Revision: (HVector and Matrix4x4) April 1994
  Revision: template versions, June 23rd 1995
  Revision: August 24th 1995
  Revision: June 18, 1996: remove all templates..hum!, split mathcls.h
                           into Vector3.h and Vector2.h

*/

#ifndef __Vector2_h
#define __Vector2_h

#ifndef __Common_h
#include "Common.h"
#endif

#ifndef __CommonMath_h
#include "CommonMath.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// Vector2 class: vector in 2 dimensions
//
class Vector2
{
  friend class Point2;
protected:
  union
    {
    struct
      {
      DBL x, y;
      } s;
    DBL vect[2];
    };

public:
  Vector2()
    {}
  Vector2( const DBL _x, const DBL _y )
    {
    s.x= _x;
    s.y= _y;
    }
  Vector2( const Vector2& v )
    {
    s.x= v.s.x;
    s.y= v.s.y;
    }

  const DBL x() const { return s.x; }
  const DBL y() const { return s.y; }
  const DBL geti( int i ) const
    {
    return vect[i];
    }

  DBL* getCoord() { return vect; }
  const DBL* getConstCoord() const { return vect; }

  DBL& operator [] ( int i )
    {
    return vect[i];
    }

  void setx( const DBL _x ) { s.x= _x; }
  void sety( const DBL _y ) { s.y= _y; }

  Vector2& operator= ( const Vector2& v )
    {
    s.x= v.s.x;
    s.y= v.s.y;
    return *this;
    }
  void zero()
    {
    s.x= s.y= DBL(0);
    }

  boolean operator== ( const Vector2& v ) const
    {
    return s.x == v.s.x &&
           s.y == v.s.y;
    }
  boolean operator!= ( const Vector2& v ) const
    {
    return ! operator== ( v );
    }
  Vector2 operator+ ( const Vector2& v ) const
    {
    return Vector2( s.x+v.s.x, s.y+v.s.y );
    }
  Vector2& operator+= ( const Vector2& v )
    {
    s.x+= v.s.x;
    s.y+= v.s.y;
    return *this;
    }
  Vector2 operator- () const
    {
    return Vector2( -s.x, -s.y );
    }
  Vector2 operator- ( const Vector2& v ) const
    {
    return Vector2( s.x-v.s.x, s.y-v.s.y );
    }
  Vector2& operator-= ( const Vector2& v )
    {
    s.x-= v.s.x;
    s.y-= v.s.y;
    return *this;
    }
  Vector2 operator* ( const DBL d ) const
    {
    return Vector2( s.x*d, s.y*d );
    }
  Vector2& operator*= ( const DBL d )
    {
    s.x *= d;
    s.y *= d;
    return *this;
    }
  Vector2 operator/ ( const DBL d ) const
    {
    return Vector2( s.x/d, s.y/d );
    }
  Vector2& operator/= ( const DBL d )
    {
    s.x /= d;
    s.y /= d;
    return *this;
    }
  DBL operator| ( const Vector2& v ) const
    {
    return s.x*v.s.x + s.y*v.s.y;
    }
  DBL operator ^ ( const Vector2& v ) const
    {
    return s.x*v.s.y - v.s.x*s.y ;
    }

  // *this and v supposed to be colinear...
  boolean sameOrient( const Vector2& v ) const
    {
    return (s.x==DBL(0)) ? sameSign(s.y,v.s.y)
                         : sameSign(s.x,v.s.x);
    }
  DBL norm2() const
    {
    return s.x*s.x + s.y*s.y;
    }
  DBL norm() const
    {
    return (DBL) ::sqrt( sqr(double(s.x)) + sqr(double(s.y)) );
    }
  DBL absNorm() const
    {
    return fabs(s.x) + fabs(s.y);
    }
  Vector2 normalize() const
    {
    return *this / norm();
    }
  Vector2& setNormalized()
    {
    *this /= norm();
    return *this;
    }

  DBL min() const
    {
    return ::min( s.x, s.y );
    }
  DBL max() const
    {
    return ::max( s.x, s.y );
    }

  boolean small() const
    {
    return absNorm() < DBL(2.*distanceTolerance);
    }
};


inline Vector2 operator*( const DBL d, const Vector2& v )
{
  return Vector2( v.x()*d, v.y()*d );
}


/////////////////////////////////////////////////////////////////////////////
// Point2 class: Point in 2 dimensions
//
class Point2
{
protected:
  union
    {
    struct
      {
      DBL x, y;
      } s;
    DBL vect[2];
    };

public:
  Point2()
    {}
  Point2( const DBL _x, const DBL _y )
    {
    s.x= _x;
    s.y= _y;
    }

  inline operator Vector2 () const
    {
    return Vector2( s.x, s.y );
    }

  const DBL x() const { return s.x; }
  const DBL y() const { return s.y; }
  const DBL geti( int i ) const
    {
    return vect[i];
    }

  DBL* getCoord() { return vect; }
  const DBL* getConstCoord() const { return vect; }

  DBL& operator [] ( int i )
    {
    return vect[i];
    }

  void setx( const DBL _x ) { s.x= _x; }
  void sety( const DBL _y ) { s.y= _y; }

  Point2& operator= ( const Point2& p )
    {
    s.x= p.s.x;
    s.y= p.s.y;
    return *this;
    }
  void zero()
    {
    s.x= s.y= DBL(0);
    }

  boolean operator== ( const Point2& p ) const
    {
    return s.x == p.s.x &&
           s.y == p.s.y;
    }
  boolean operator!= ( const Point2& p ) const
    {
    return ! operator== ( p );
    }
  Point2 operator+ ( const Point2& p ) const
    {
    return Point2( s.x+p.s.x, s.y+p.s.y );
    }
  Point2& operator+=( const Point2& p )
    {
    s.x+= p.s.x;
    s.y+= p.s.y;
    return *this;
    }
  Vector2 operator- ( const Point2& p ) const
    {
    return Vector2( s.x-p.s.x, s.y-p.s.y );
    }
  Point2 operator* ( const DBL k ) const
    {
    return Point2( s.x*k, s.y*k );
    }
  Point2 operator/ ( const DBL k ) const
    {
    return Point2( s.x/k, s.y/k );
    }

  Point2 operator + ( const Vector2& v ) const
    {
    return Point2( s.x+v.s.x, s.y+v.s.y );
    }
  Point2& operator+= ( const Vector2& v )
    {
    s.x+= v.s.x;
    s.y+= v.s.y;
    return *this;
    }
  Point2 operator - ( const Vector2& v ) const
    {
    return Point2( s.x-v.s.x, s.y-v.s.y );
    }
  Point2& operator-= ( const Vector2& v )
    {
    s.x-= v.s.x;
    s.y-= v.s.y;
    return *this;
    }
  Point2& operator*= ( const DBL d )
    {
    s.x *= d;
    s.y *= d;
    return *this;
    }
  Point2& operator/= ( const DBL d )
    {
    s.x /= d;
    s.y /= d;
    return *this;
    }

  Point2& addx( const DBL dx )
    {
    s.x+= dx;
    return *this;
    }
  Point2& addy( const DBL dy )
    {
    s.y+= dy;
    return *this;
    }

  DBL min() const
    {
    return ::min( s.x, s.y );
    }
  DBL max() const
    {
    return ::max( s.x, s.y );
    }
  Point2 min( const Point2& p ) const
    {
    return Point2( ::min( s.x, p.s.x ),
                   ::min( s.y, p.s.y ) );
    }
  Point2 max( const Point2& p ) const
    {
    return Point2( ::max( s.x, p.s.x ),
                   ::max( s.y, p.s.y ) );
    }

  // distance from *this and line (m,v)
  // Caution: v is to be normalized!
  DBL dist( const Point2& m, const Vector2& v ) const
    {
    return fabs( v ^ (*this-m) );
    }

  // return true if points are 'equal' modulo 'distanceTolerance'
  boolean compare( const Point2& p ) const
    {
    return (p - *this).small();
    }
};

/////////////////////////////////////////////////////////////////////////////

inline Vector2 min( const Vector2& v1, const Vector2& v2 )
{
  return Vector2( min(v1.x(),v2.x()), min(v1.y(),v2.y()) );
}


inline Vector2 max( const Vector2& v1, const Vector2& v2 )
{
  return Vector2( max(v1.x(),v2.x()), max(v1.y(),v2.y()) );
}


inline Point2 min( const Point2& v1, const Point2& v2 )
{
  return Point2( min(v1.x(),v2.x()), min(v1.y(),v2.y()) );
}


inline Point2 max( const Point2& v1, const Point2& v2 )
{
  return Point2( max(v1.x(),v2.x()), max(v1.y(),v2.y()) );
}

/////////////////////////////////////////////////////////////////////////////

// global functions
double angle( const Vector2& v1, const Vector2& v2 );
double angle( const Point2& p, const Point2& p1, const Point2& p2 );
boolean insideTriangle( const Point2& p,
                      const Point2& p1,
                      const Point2& p2,
                      const Point2& p3 );

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __Vector2_h
