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
  Matrix4x4.h

  Homogeneous matrices classes, from homogen.h

  Stephane Rehel [SCFR::Kepler], Stephane.Rehel@imag.fr

  June 23rd 1995 from mathcls.h

  Revisions:
    + SR, April 22nd, 1996
*/


#ifndef __Matrix4x4_h
#define __Matrix4x4_h

// for memcpy()
#include <string.h>

#ifndef __Vector_h
#include "Vector.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class Matrix4x4
{
private:
  static const double _identity[16];

  double a[16];

  inline double _aij( int i, int j ) const { return a[i+j*4]; }

  inline void set( const double* _a )
    {
    memcpy( (void*) a, (const void*) _a, sizeof(a[0]) * 16 );
    }
public:
  Matrix4x4()
    {}
  ~Matrix4x4()
    {}

  Matrix4x4( const Matrix4x4& m )
    {
    set(m.a);
    }
  Matrix4x4( double a11, double a12, double a13, double a14,
             double a21, double a22, double a23, double a24,
             double a31, double a32, double a33, double a34,
             double a41, double a42, double a43, double a44 )
    {
    a[ 0]= a11; a[ 4]= a12; a[ 8]= a13; a[12]= a14;
    a[ 1]= a21; a[ 5]= a22; a[ 9]= a23; a[13]= a24;
    a[ 2]= a31; a[ 6]= a32; a[10]= a33; a[14]= a34;
    a[ 3]= a41; a[ 7]= a42; a[11]= a43; a[15]= a44;
    }
  Matrix4x4( const DBL* a )
    {
    set(a);
    }
  Matrix4x4& operator= ( const Matrix4x4& m )
    {
    set( m );
    return *this;
    }

  double& aij ( int i, int j )
    {
    return a[i+j*4];
    }
  double operator () ( int i, int j ) const
    {
    return a[i+j*4];
    }

  const double* dump() const
    {
    return a;
    }

  inline void set( const Matrix4x4& m )
    {
    set(m.a);
    }

  void setIdentity()
    {
    set(_identity);
    }
  void setTranslation( const Vector& v );
  void setScaling( const Vector& v );
  void setRotation( int i, const double angle );
  void setRotationX( const double angle );
  void setRotationY( const double angle );
  void setRotationZ( const double angle );
  void setRotation( const Vector& axis, const double angle );
  void setTransformation( const Vector& scale,
                          const Vector& angles,
                          const Vector& translate );
  void getTransformation( Vector& scale,
                          Vector& rotate,
                          Vector& translate );

  Matrix4x4 operator-() const;
  Matrix4x4 operator+( const Matrix4x4& m ) const;
  Matrix4x4 operator-( const Matrix4x4& m ) const;
  Matrix4x4 operator*( const Matrix4x4& m ) const;
  Matrix4x4& operator+=( const Matrix4x4& m );
  Matrix4x4& operator-=( const Matrix4x4& m );
  inline Matrix4x4& operator*=( const Matrix4x4& m );
  inline Matrix4x4& operator&=( const Matrix4x4& m ); // pre-mult

  static Matrix4x4 mult_3x4( const Matrix4x4& a, const Matrix4x4& b );

  Matrix4x4 operator*( const double d ) const;
  Matrix4x4 operator/( double d ) const;
  Matrix4x4& operator*=( const double d );
  Matrix4x4& operator/=( double d );

  inline Point apply( const Point& p ) const;
  inline Vector apply( const Vector& v ) const;

  Matrix4x4 computeInverse() const;

private:
  Matrix4x4 computeInverse_pivot() const;

public:
  double mabs() const;
};

/////////////////////////////////////////////////////////////////////////////

inline Matrix4x4& Matrix4x4::operator*=( const Matrix4x4& m )
{
  Matrix4x4 tmp= operator*(m);
  set(tmp);
  return *this;
}

/////////////////////////////////////////////////////////////////////////////

// pre-mult
inline Matrix4x4& Matrix4x4::operator&=( const Matrix4x4& m )
{
  Matrix4x4 tmp= m.operator*(*this);
  set(tmp);
  return *this;
}

/////////////////////////////////////////////////////////////////////////////

inline Point Matrix4x4::apply( const Point& p ) const
{
  register double h= _aij(3,0) * p.x()
                    +_aij(3,1) * p.y()
                    +_aij(3,2) * p.z()
                    +_aij(3,3);
  h= 1. / h;
  return Point( (_aij(0,0) * p.x()
                +_aij(0,1) * p.y()
                +_aij(0,2) * p.z()
                +_aij(0,3)) * h,

                (_aij(1,0) * p.x()
                +_aij(1,1) * p.y()
                +_aij(1,2) * p.z()
                +_aij(1,3)) * h,

                (_aij(2,0) * p.x()
                +_aij(2,1) * p.y()
                +_aij(2,2) * p.z()
                +_aij(2,3)) * h    );
}

/////////////////////////////////////////////////////////////////////////////

inline Vector Matrix4x4::apply( const Vector& v ) const
{
  return Vector( _aij(0,0) * v.x()
                +_aij(0,1) * v.y()
                +_aij(0,2) * v.z(),

                 _aij(1,0) * v.x()
                +_aij(1,1) * v.y()
                +_aij(1,2) * v.z(),

                 _aij(2,0) * v.x()
                +_aij(2,1) * v.y()
                +_aij(2,2) * v.z() );
}

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __Matrix4x4_h

