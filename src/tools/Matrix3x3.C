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
  Matrix3x3.C

  Stephane Rehel [SCFR::Kepler], Stephane.Rehel@imag.fr

  June 23rd 1995 from mathcls.h
*/

#include "Matrix3x3.h"


/////////////////////////////////////////////////////////////////////////////

// if Matrix3x3 is symetric
DBL Matrix3x3::quadrForm( const Vector& v ) const
{
  return     a11*v.x()*v.x()
        +    a22*v.y()*v.y()
        +    a33*v.z()*v.z()
        + 2.*a12*v.x()*v.y()
        + 2.*a13*v.x()*v.z()
        + 2.*a23*v.y()*v.z() ;
}


/////////////////////////////////////////////////////////////////////////////

DBL Matrix3x3::cofactor( int i1, int j1 ) const
{
  i1= (i1==3) ? 1 : (i1+1);
  j1= (j1==3) ? 1 : (j1+1);
  int i2= (i1==3) ? 1 : (i1+1),
      j2= (j1==3) ? 1 : (j1+1);
  return getij(i1,i1) * getij(i2,j2) - getij(i1,j2) * getij(i2,j1);
}

/////////////////////////////////////////////////////////////////////////////

Matrix3x3 Matrix3x3::com() const
{
  return Matrix3x3( cofactor(1,1), cofactor(1,2), cofactor(1,3),
                    cofactor(2,1), cofactor(2,2), cofactor(2,3),
                    cofactor(3,1), cofactor(3,2), cofactor(3,3) );
}

/////////////////////////////////////////////////////////////////////////////

DBL Matrix3x3::det() const
{
  return   a11*a22*a33 + a31*a12*a23 + a13*a21*a32
         - a13*a22*a31 - a11*a23*a32 - a33*a12*a21;
}

/////////////////////////////////////////////////////////////////////////////

Vector cramer( const Matrix3x3& m, const Vector& v )
{
  return (!m)( v ); // thanx operators:-)
}

/////////////////////////////////////////////////////////////////////////////

Matrix3x3 Matrix3x3::operator! () const
{
  return (~com()) / det(); // wooo!
}

/////////////////////////////////////////////////////////////////////////////

// build a rotation matrix
void Matrix3x3::rotation( const Vector& axis, const DBL angle )
{
  DBL cosine, sine, one_minus_cosine;

  cosine= cos( angle );
  sine= sin( angle );
  one_minus_cosine= 1. - cosine;

  a11 = sqr(axis.x()) + (1. - sqr(axis.x())) * cosine;
  a12 = axis.x() * axis.y() * one_minus_cosine + axis.z() * sine;
  a13 = axis.x() * axis.z() * one_minus_cosine - axis.y() * sine;

  a21 = axis.x() * axis.y() * one_minus_cosine - axis.z() * sine;
  a22 = sqr(axis.y()) + (1. - sqr(axis.y())) * cosine;
  a23 = axis.y() * axis.z() * one_minus_cosine + axis.x() * sine;

  a31 = axis.x() * axis.z() * one_minus_cosine + axis.y() * sine;
  a32 = axis.y() * axis.z() * one_minus_cosine - axis.x() * sine;
  a33 = sqr(axis.z()) + (1. - sqr(axis.z())) * cosine;
}

/////////////////////////////////////////////////////////////////////////////

// build a scaling matrix
void Matrix3x3::scaling( const Vector& factors )
{
  a11 = factors.x();
  a12 = 0.;
  a13 = 0.;

  a21 = 0.;
  a22 = factors.y();
  a23 = 0.;

  a31 = 0.;
  a32 = 0.;
  a33 = factors.z();
}

/////////////////////////////////////////////////////////////////////////////
