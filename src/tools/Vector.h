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
  Vector.h

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

#ifndef __Vector_h
#define __Vector_h

#ifndef __Common_h
#include "Common.h"
#endif

#ifndef __CommonMath_h
#include "CommonMath.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// Vector
//
class Vector
{
  friend class Point;
protected:
  union
    {
    struct
      {
      DBL x, y, z;
      } s;
    DBL vect[3];
    };

public:

  Vector()
    {}
  Vector( const DBL x, const DBL y, const DBL z )
    {
    s.x= x;
    s.y= y;
    s.z= z;
    }
  Vector( const Vector& v )
    {
    s.x= v.s.x;
    s.y= v.s.y;
    s.z= v.s.z;
    }

  const DBL x() const { return s.x; }
  const DBL y() const { return s.y; }
  const DBL z() const { return s.z; }
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
  void setz( const DBL _z ) { s.z= _z; }

  Vector& operator= ( const Vector& v )
    {
    s.x= v.s.x;
    s.y= v.s.y;
    s.z= v.s.z;
    return *this;
    }
  void zero()
    {
    s.x= s.y= s.z= DBL(0);
    }

  boolean operator== ( const Vector& v ) const
    {
    return s.x == v.s.x &&
           s.y == v.s.y &&
           s.z == v.s.z;
    }
  boolean operator!= ( const Vector& v ) const
    {
    return ! operator== ( v );
    }
  Vector operator+ ( const Vector& v ) const
    {
    return Vector( s.x+v.s.x, s.y+v.s.y, s.z+v.s.z );
    }
  Vector& operator+= ( const Vector& v )
    {
    s.x+= v.s.x;
    s.y+= v.s.y;
    s.z+= v.s.z;
    return *this;
    }
  Vector operator- ( ) const
    {
    return Vector( -s.x, -s.y, -s.z );
    }
  Vector operator- ( const Vector& v ) const
    {
    return Vector( s.x-v.s.x, s.y-v.s.y, s.z-v.s.z );
    }
  Vector& operator-=( const Vector& v )
    {
    s.x-= v.s.x;
    s.y-= v.s.y;
    s.z-= v.s.z;
    return *this;
    }
  Vector operator* ( const DBL d ) const
    {
    return Vector( s.x*d, s.y*d, s.z*d );
    }
  Vector& operator*= ( const DBL d )
    {
    s.x *= d;
    s.y *= d;
    s.z *= d;
    return *this;
    }
  Vector operator/ ( const DBL d ) const
    {
    return Vector( s.x/d, s.y/d, s.z/d );
    }
  Vector& operator/= ( const DBL d )
    {
    s.x/= d;
    s.y/= d;
    s.z/= d;
    return *this;
    }
  DBL operator| ( const Vector& v ) const
    {
    return s.x*v.s.x + s.y*v.s.y + s.z*v.s.z;
    }
  Vector operator ^ ( const Vector& v ) const
    {
    return Vector( s.y*v.s.z - v.s.y*s.z,
                    s.z*v.s.x - v.s.z*s.x,
                    s.x*v.s.y - v.s.x*s.y );
    }
  DBL sum() const
    {
    return s.x + s.y + s.z;
    }

  // *this and v supposed to be colinear...
  boolean sameOrient( const Vector& v ) const
    {
    return (s.x==DBL(0)) ? (s.y==DBL(0)) ? sameSign(s.z,v.s.z)
                                         : sameSign(s.y,v.s.y)
                         : sameSign(s.x,v.s.x);
    }

  // multiply each component
  Vector operator & ( const Vector& v ) const
    {
    return Vector( s.x * v.s.x, s.y * v.s.y, s.z * v.s.z );
    }

  // multiply each component
  Vector& operator &= ( const Vector& v )
    {
    s.x *= v.s.x;
    s.y *= v.s.y;
    s.z *= v.s.z;
    return *this;
    }
  DBL absNorm() const
    {
    return fabs(s.x) + fabs(s.y) + fabs(s.z);
    }

  DBL min() const
    {
    return ::min( s.x, ::min( s.y, s.z ) );
    }
  DBL max() const
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
  int getMaxAbsCoord() const
    {
    if( fabs(s.x) > fabs(s.y) )
      return ( fabs(s.x) > fabs(s.z) )  ? 0 : 2;
     else
      return ( fabs(s.y) > fabs(s.z) ) ? 1 : 2;
    }
  int getMinAbsCoord() const
    {
    if( fabs(s.x) < fabs(s.y) )
      return ( fabs(s.x) < fabs(s.z) )  ? 0 : 2;
     else
      return ( fabs(s.y) < fabs(s.z) ) ? 1 : 2;
    }

  const DBL norm2() const
    {
    return s.x*s.x + s.y*s.y + s.z*s.z;
    }
  const DBL norm() const
    {
    return (DBL) ::sqrt( double(norm2()) );
    }
  Vector getNormalized() const
    {
    return *this / DBL(::sqrt( double(norm2()) ));
    }
  Vector& setNormalized()
    {
    *this /= norm();
    return *this;
    }

  boolean small() const
    {
    return absNorm() < DBL(3.*distanceTolerance);
    }

  // assuming *this as Z vector
  // X and Y are not normalized
  void makeOrthoBase( Vector& X, Vector& Y ) const;
};


inline Vector operator*( const DBL d, const Vector& v )
{
  return Vector( v.x()*d, v.y()*d, v.z()*d );
}


/////////////////////////////////////////////////////////////////////////////
// Point class
//
class Point
{
protected:
  union
    {
    struct
      {
      DBL x, y, z;
      } s;
    DBL vect[3];
    };

public:
  Point()
    {}
  Point( const DBL x, const DBL y, const DBL z )
    {
    s.x= x;
    s.y= y;
    s.z= z;
    }
  Point( const Point& p )
    {
    s.x= p.s.x;
    s.y= p.s.y;
    s.z= p.s.z;
    }

  inline operator Vector () const
    {
    return Vector( s.x, s.y, s.z );
    }

  const DBL x() const { return s.x; }
  const DBL y() const { return s.y; }
  const DBL z() const { return s.z; }
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
  void setz( const DBL _z ) { s.z= _z; }

  Point& operator= ( const Point& p )
    {
    s.x= p.s.x;
    s.y= p.s.y;
    s.z= p.s.z;
    return *this;
    }
  void zero()
    {
    s.x= s.y= s.z= DBL(0);
    }

  boolean operator== ( const Point& p ) const
    {
    return s.x == p.s.x &&
           s.y == p.s.y &&
           s.z == p.s.z;
    }
  boolean operator!= ( const Point& p ) const
    {
    return ! operator== ( p );
    }
  Point operator+ ( const Point& p ) const
    {
    return Point( s.x+p.s.x, s.y+p.s.y, s.z+p.s.z );
    }
  Point& operator+= ( const Point& p )
    {
    s.x+= p.s.x;
    s.y+= p.s.y;
    s.z+= p.s.z;
    return *this;
    }
  Vector operator- ( const Point& p ) const
    {
    return Vector( s.x-p.s.x, s.y-p.s.y, s.z-p.s.z );
    }
  Point operator* ( const DBL k ) const
    {
    return Point( s.x*k, s.y*k, s.z*k );
    }
  Point operator/ ( const DBL k ) const
    {
    return Point( s.x/k, s.y/k, s.z/k );
    }

  Point operator + ( const Vector& v ) const
    {
    return Point( s.x+v.s.x, s.y+v.s.y, s.z+v.s.z );
    }
  Point& operator+= ( const Vector& v )
    {
    s.x+= v.s.x;
    s.y+= v.s.y;
    s.z+= v.s.z;
    return *this;
    }
  Point operator - ( const Vector& v ) const
    {
    return Point( s.x-v.s.x, s.y-v.s.y, s.z-v.s.z );
    }
  Point& operator-= ( const Vector& v )
    {
    s.x-= v.s.x;
    s.y-= v.s.y;
    s.z-= v.s.z;
    return *this;
    }

  Point& addx( const DBL dx )
    {
    s.x+= dx;
    return *this;
    }
  Point& addy( const DBL dy )
    {
    s.y+= dy;
    return *this;
    }
  Point& addz( const DBL dz )
    {
    s.z+= dz;
    return *this;
    }

  DBL min() const
    {
    return ::min( s.x, ::min( s.y, s.z ) );
    }
  DBL max() const
    {
    return ::max( s.x, ::max( s.y, s.z ) );
    }
  Point min( const Point& p ) const
    {
    return Point( ::min( s.x, p.s.x ),
                   ::min( s.y, p.s.y ),
                   ::min( s.z, p.s.z ) );
    }
  Point max( const Point& p ) const
    {
    return Point( ::max( s.x, p.s.x ),
                   ::max( s.y, p.s.y ),
                   ::max( s.z, p.s.z ) );
    }

  boolean in( const Point& minP, const Point& maxP ) const
    {
    return    s.x >= minP.s.x && s.x <= maxP.s.x
           && s.y >= minP.s.y && s.y <= maxP.s.y
           && s.z >= minP.s.z && s.z <= maxP.s.z;
    }

  // return true if points are 'equal' modulo 'distanceTolerance'
  boolean compare( const Point& p ) const
    {
    return (p - *this).small();
    }
};

/////////////////////////////////////////////////////////////////////////////

inline Vector min( const Vector& v1, const Vector& v2 )
{
  return Vector( min(v1.x(),v2.x()),
                  min(v1.y(),v2.y()),
                  min(v1.z(),v2.z()) );
}


inline Vector max( const Vector& v1, const Vector& v2 )
{
  return Vector( max(v1.x(),v2.x()),
                  max(v1.y(),v2.y()),
                  max(v1.z(),v2.z()) );
}


inline Point min( const Point& v1, const Point& v2 )
{
  return Point( min(v1.x(),v2.x()),
                 min(v1.y(),v2.y()),
                 min(v1.z(),v2.z()) );
}


inline Point max( const Point& v1, const Point& v2 )
{
  return Point( max(v1.x(),v2.x()),
                 max(v1.y(),v2.y()),
                 max(v1.z(),v2.z()) );
}

/////////////////////////////////////////////////////////////////////////////

// global functions
double angle( const Vector& v1, const Vector& v2 );
double angle( const Point& p, const Point& p1, const Point& p2 );
boolean insideTriangle( const Point& p,
                      const Point& p1,
                      const Point& p2,
                      const Point& p3 );

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __Vector_h


