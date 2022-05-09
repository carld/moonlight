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
  Matrix3x3.h

  Stephane Rehel [SCFR::Kepler], Stephane.Rehel@imag.fr

  June 23rd 1995 from mathcls.h
*/

#ifndef __Matrix3x3_h
#define __Matrix3x3_h

#ifndef __Vector_h
#include "Vector.h"
#endif


/////////////////////////////////////////////////////////////////////////////
// Matrix3x3 class
// 3x3 matrix
//
class Matrix3x3
{
public:
  DBL a11, a12, a13,
      a21, a22, a23,
      a31, a32, a33;
  Matrix3x3()
    {}
  Matrix3x3( const DBL A11, const DBL A12, const DBL A13,
             const DBL A21, const DBL A22, const DBL A23,
             const DBL A31, const DBL A32, const DBL A33 )
    {
    a11= A11; a12= A12; a13= A13;
    a21= A21; a22= A22; a23= A23;
    a31= A31; a32= A32; a33= A33;
    }
  const DBL getij( int i, int j ) const // i= line
    {                                   // j= row
    return (i==1) ? ((j==1) ? a11 : (j==2) ? a12 : a13) :
           (i==2) ? ((j==1) ? a21 : (j==2) ? a22 : a23) :
                    ((j==1) ? a31 : (j==2) ? a32 : a33);
    }
  void setij( int i, int j, const DBL a ) // i= line
    {                                     // j= row
    (i==1) ? ((j==1) ? (a11=a) : (j==2) ? (a12=a) : (a13=a)) :
    (i==2) ? ((j==1) ? (a21=a) : (j==2) ? (a22=a) : (a23=a)) :
             ((j==1) ? (a31=a) : (j==2) ? (a32=a) : (a33=a));
    }
  boolean operator== ( const Matrix3x3& m ) const
    {
    return a11==m.a11 && a12==m.a12 && a13==m.a13 &&
           a21==m.a21 && a22==m.a22 && a23==m.a23 &&
           a31==m.a31 && a32==m.a32 && a33==m.a33;
    }
  boolean operator!= ( const Matrix3x3& m ) const
    {
    return ! operator==( m );
    }
  Matrix3x3& operator~ () // transpose
    {
    DBL d;
    d= a12; a12= a21; a21= d;
    d= a13; a13= a31; a31= d;
    d= a32; a32= a23; a23= d;
    return *this;
    }
  Matrix3x3& operator- () // inverse sign
    {
    a11= -a11; a12= -a12; a13= -a13;
    a21= -a21; a22= -a22; a23= -a23;
    a31= -a31; a32= -a32; a33= -a33;
    return *this;
    }
  Matrix3x3 operator+ ( const Matrix3x3& m ) const
    {
    return Matrix3x3( a11 + m.a11, a12 + m.a12, a13 + m.a13,
                      a21 + m.a21, a22 + m.a22, a23 + m.a23,
                      a31 + m.a31, a32 + m.a32, a33 + m.a33 );
    }
  Matrix3x3 operator- ( const Matrix3x3& m ) const
    {
    return Matrix3x3( a11 - m.a11, a12 - m.a12, a13 - m.a13,
                      a21 - m.a21, a22 - m.a22, a23 - m.a23,
                      a31 - m.a31, a32 - m.a32, a33 - m.a33 );
    }
  Matrix3x3 operator * ( const DBL d ) const
    {
    return Matrix3x3( a11*d, a12*d, a13*d,
                      a21*d, a22*d, a23*d,
                      a31*d, a22*d, a33*d );
    }

  friend
#ifndef __BORLANDC__
  inline
#endif
  Matrix3x3 operator* ( const DBL d, const Matrix3x3& m );

  Matrix3x3 operator/ ( const DBL d ) const
    {
    return Matrix3x3( a11/d, a12/d, a13/d,
                      a21/d, a22/d, a23/d,
                      a31/d, a22/d, a33/d );
    }

  friend
#ifndef __BORLANDC__
  inline
#endif
  Matrix3x3 operator* ( const Matrix3x3& m,
                        const Matrix3x3& n );

  Vector operator()( const Vector& v ) const
    {
    return Vector( a11*v.x() + a12*v.y() + a13*v.z(),
                   a21*v.x() + a22*v.y() + a23*v.z(),
                   a31*v.x() + a32*v.y() + a33*v.z() );
    }
  Vector operator[]( int i ) const // return column vector
    {
    if( i == 1 ) return Vector( a11, a21, a31 ); else
    if( i == 2 ) return Vector( a12, a22, a32 ); else
                 return Vector( a13, a23, a33 );
    }
  DBL quadrForm( const Vector& v ) const; // if Matrix3x3 is symetric
  DBL cofactor( int i1, int j1 ) const;
  Matrix3x3 com() const;
  DBL det() const;

  // inverse matrix
  Matrix3x3 operator! () const;

  // build a rotation matrix
  void rotation( const Vector& axis, const DBL angle );

  // build a scaling matrix
  void scaling( const Vector& factors );

  Vector operator* ( const Vector& v ) const
    {
    return Vector( a11*v.x() + a12*v.y() + a13*v.z(),
                   a21*v.x() + a22*v.y() + a23*v.z(),
                   a31*v.x() + a32*v.y() + a33*v.z() );
    }
};


Vector cramer( const Matrix3x3& m, const Vector& v );


#ifndef __BORLANDC__
//bug under borland C++ ...???
inline Matrix3x3 operator* ( const DBL d, const Matrix3x3& m )
{
  return Matrix3x3( d*m.a11, d*m.a12, d*m.a13,
                    d*m.a21, d*m.a22, d*m.a23,
                    d*m.a31, d*m.a22, d*m.a33 );
}
#endif


#ifndef __BORLANDC__
//bug under borland C++ ...???
inline Matrix3x3 operator* ( const Matrix3x3& m,
                             const Matrix3x3& n )
{
  return Matrix3x3( m.a11*n.a11 + m.a12*n.a21 + m.a13*n.a31,
                    m.a11*n.a12 + m.a12*n.a22 + m.a13*n.a32,
                    m.a11*n.a13 + m.a12*n.a23 + m.a13*n.a33,
                    m.a21*n.a11 + m.a22*n.a21 + m.a23*n.a31,
                    m.a21*n.a12 + m.a22*n.a22 + m.a23*n.a32,
                    m.a21*n.a13 + m.a22*n.a23 + m.a23*n.a33,
                    m.a31*n.a11 + m.a32*n.a21 + m.a33*n.a31,
                    m.a31*n.a12 + m.a32*n.a22 + m.a33*n.a32,
                    m.a31*n.a13 + m.a32*n.a23 + m.a33*n.a33 );
}
#endif



/*
//////////////////////////////////////////////////////////////////LinearForm/
// LinearForm Class
//
template<class DBL>
class LinearForm
{
private:
  TMVector<DBL> u;
public:
  LinearForm() {}
  LinearForm( const DBL a, const DBL b, const DBL c )
    {
    u= TMVector<DBL>( a, c, b );
    }
  LinearForm( const TMVector<DBL>& _u )
    {
    u= _u;
    }
  DBL operator()( const TMVector<DBL>& v ) const
    {
    return ( u | v );
    }
  TMVector<DBL> getVector() const { return u; }
};
*/

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __Matrix3x3_h

