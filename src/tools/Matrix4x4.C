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
  Matrix4x4.C

  Homogeneous vectors & matrices classes, from homogen.h

  Stephane Rehel [SCFR::Kepler], Stephane.Rehel@imag.fr

  June 23rd 1995 from mathcls.h

  Revisions:
    + SR, April 22nd, 1996
    + SR, October 15 1996
*/

#include <stdio.h>

#include "Matrix4x4.h"

/////////////////////////////////////////////////////////////////////////////

// static
const double Matrix4x4::_identity[16] = { 1., 0., 0., 0.,
                                          0., 1., 0., 0.,
                                          0., 0., 1., 0.,
                                          0., 0., 0., 1. };

/////////////////////////////////////////////////////////////////////////////

void Matrix4x4::setTranslation( const Vector& v )
{
  aij(0,0)= aij(1,1)= aij(2,2)= 1.;

            aij(0,1)= aij(0,2)=
  aij(1,0)=           aij(1,2)=
  aij(2,0)= aij(2,1)=
  aij(3,0)= aij(3,1)= aij(3,2)= 0.;

  aij(0,3)= v.x();
  aij(1,3)= v.y();
  aij(2,3)= v.z();
  aij(3,3)= 1.;
}

/////////////////////////////////////////////////////////////////////////////

void Matrix4x4::setScaling( const Vector& v )
{
  aij(0,0)= v.x();
  aij(1,1)= v.y();
  aij(2,2)= v.z();
  aij(3,3)= 1.;

            aij(0,1)= aij(0,2)= aij(0,3)=
  aij(1,0)=           aij(1,2)= aij(1,3)=
  aij(2,0)= aij(2,1)=           aij(2,3)=
  aij(3,0)= aij(3,1)= aij(3,2)=            0.;
}

/////////////////////////////////////////////////////////////////////////////

void Matrix4x4::setRotation( int i, const double angle )
{
  switch(i)
    {
    case 0: setRotationX( angle ); return;
    case 1: setRotationY( angle ); return;
    case 2: setRotationZ( angle ); return;
    default: break;
    }
}

/////////////////////////////////////////////////////////////////////////////

void Matrix4x4::setRotationX( const double angle )
{
  aij(0,0)= aij(3,3)= 1.;
            aij(0,1)= aij(0,2)= aij(0,3)=
  aij(1,0)=                     aij(1,3)=
  aij(2,0)=                     aij(2,3)=
  aij(3,0)= aij(3,1)= aij(3,2)=           0.;

  aij(1,1)= aij(2,2)= cos(angle);
  register double sina= sin(angle);
  aij(1,2)= -sina;
  aij(2,1)=  sina;
//  setLines( HVector(    1.,    0.,    0., 0. ),
//            HVector(    0.,  cosa, -sina, 0. ),
//            HVector(    0.,  sina,  cosa, 0. ),
//            HVector(    0.,    0.,    0., 1. ) );
}

/////////////////////////////////////////////////////////////////////////////

void Matrix4x4::setRotationY( const double angle )
{
  aij(1,1)= aij(3,3)= 1.;

            aij(0,1)=           aij(0,3)=
  aij(1,0)=           aij(1,2)= aij(1,3)=
            aij(2,1)=           aij(2,3)=
  aij(3,0)= aij(3,1)= aij(3,2)=           0.;

  aij(0,0)= aij(2,2)= cos(angle);
  register double sina= sin(angle);
  aij(2,0)= -sina;
  aij(0,2)=  sina;
//  setLines( HVector(  cosa,    0.,  sina, 0. ),
//            HVector(    0.,    1.,    0., 0. ),
//            HVector( -sina,    0.,  cosa, 0. ),
//            HVector(    0.,    0.,    0., 1. ) );
}

/////////////////////////////////////////////////////////////////////////////

void Matrix4x4::setRotationZ( const double angle )
{
  aij(2,2)= aij(3,3)= 1.;

                      aij(0,2)= aij(0,3)=
                      aij(1,2)= aij(1,3)=
  aij(2,0)= aij(2,1)=           aij(2,3)=
  aij(3,0)= aij(3,1)= aij(3,2)=           0.;

  aij(0,0)= aij(1,1)= cos(angle);
  register double sina= sin(angle);
  aij(0,1)= -sina;
  aij(1,0)=  sina;
//  setLines( HVector(  cosa, -sina,    0., 0. ),
//            HVector(  sina,  cosa,    0., 0. ),
//            HVector(    0.,    0.,    1., 0. ),
//            HVector(    0.,    0.,    0., 1. ) );
}

/////////////////////////////////////////////////////////////////////////////

void Matrix4x4::setRotation( const Vector& axis, const double angle )
{
  register double sina= sin(-angle );
  register double cosa= cos( angle );
  register double one_cosa= 1. - cosa;

  aij(0,0)= sqr(axis.x()) + (1. - sqr(axis.x())) * cosa;
  aij(0,1)= axis.x() * axis.y() * one_cosa + axis.z() * sina;
  aij(0,2)= axis.x() * axis.z() * one_cosa - axis.y() * sina;
  aij(0,3)= 0.;

  aij(1,0)= axis.x() * axis.y() * one_cosa - axis.z() * sina;
  aij(1,1)= sqr(axis.y()) + (1. - sqr(axis.y())) * cosa;
  aij(1,2)= axis.y() * axis.z() * one_cosa + axis.x() * sina;
  aij(1,3)= 0.;

  aij(2,0)= axis.x() * axis.z() * one_cosa + axis.y() * sina;
  aij(2,1)= axis.y() * axis.z() * one_cosa - axis.x() * sina;
  aij(2,2)= sqr(axis.z()) + (1. - sqr(axis.z())) * cosa;
  aij(2,3)= 0.;

  aij(3,0)=
  aij(3,1)=
  aij(3,2)= 0.;
  aij(3,3)= 1.;
}

/////////////////////////////////////////////////////////////////////////////

void Matrix4x4::setTransformation( const Vector& scale,
                                   const Vector& rotate,
                                   const Vector& translate )
{
  aij(3,0)= aij(3,1)= aij(3,2)= 0.;
  aij(3,3)= 1.;
  aij(0,3)= translate.x();
  aij(1,3)= translate.y();
  aij(2,3)= translate.z();
  double c1= cos(rotate.x()), s1= sin(rotate.x());
  double c2= cos(rotate.y()), s2= sin(rotate.y());
  double c3= cos(rotate.z()), s3= sin(rotate.z());
  double sx= scale.x(), sy= scale.y(), sz= scale.z();

  aij(0,0)= sx*c2*c3;
  aij(0,1)= sy*c2*s3;
  aij(0,2)=-sz*s2;
  aij(1,0)= sx*(s1*s2*c3-c1*s3);
  aij(1,1)= sy*(s1*s2*s3+c1*c3);
  aij(1,2)= sz*s1*c2;
  aij(2,0)= sx*(c1*s2*c3+s1*s3);
  aij(2,1)= sy*(c1*s2*s3-s1*c3);
  aij(2,2)= sz*c1*c2;
}

/////////////////////////////////////////////////////////////////////////////

void Matrix4x4::getTransformation( Vector& scale,
                                   Vector& rotate,
                                   Vector& translate )
{
  translate[0]= aij(0,3);
  translate[1]= aij(1,3);
  translate[2]= aij(2,3);

  double a1, a2, a3;

  a1= atan2( aij(1,2), aij(2,2) );
  double c1= cos(a1), s1= sin(a1);
  if( fabs(c1) > fabs(s1) )
    a2= atan2( -aij(0,2) * c1, aij(2,2) );
   else
    a2= atan2( -aij(0,2) * s1, aij(1,2) );
  double c2= cos(a2), s2= sin(a2);
  a3= atan2( c1 * aij(0,1), (c2*aij(1,1)-aij(0,1)*s1*s2) );

  double c3= cos(a3), s3= sin(a3);
  double sx= 1., sy= 1., sz= 1.;

  switch( Vector(aij(0,0),aij(1,0),aij(2,0)).getMaxAbsCoord() )
    {
    case 0: sx= aij(0,0) / (c2*c3); break;
    case 1: sx= aij(1,0) / (s1*s2*c3-c1*s3); break;
    case 2: sx= aij(2,0) / (c1*s2*c3+s1*s3); break;
    }
  switch( Vector(aij(0,1),aij(1,1),aij(2,1)).getMaxAbsCoord() )
    {
    case 0: sy= aij(0,1) / (c2*s3); break;
    case 1: sy= aij(1,1) / (s1*s2*s3+c1*c3); break;
    case 2: sy= aij(2,1) / (c1*s2*s3-s1*c3); break;
    }
  switch( Vector(aij(0,2),aij(1,2),aij(2,2)).getMaxAbsCoord() )
    {
    case 0: sz= aij(0,2) / -s2; break;
    case 1: sz= aij(1,2) / (s1*c2); break;
    case 2: sz= aij(2,2) / (c1*c2); break;
    }

  scale= Vector(sx,sy,sz);
  rotate= Vector(a1,a2,a3);
}

/////////////////////////////////////////////////////////////////////////////

Matrix4x4 Matrix4x4::operator-() const
{
  return Matrix4x4( -a[ 0], -a[ 4], -a[ 8], -a[12],
                    -a[ 1], -a[ 5], -a[ 9], -a[13],
                    -a[ 2], -a[ 6], -a[10], -a[14],
                    -a[ 3], -a[ 7], -a[11], -a[15] );
}

/////////////////////////////////////////////////////////////////////////////

Matrix4x4 Matrix4x4::operator+( const Matrix4x4& m ) const
{
  return Matrix4x4(
         a[ 0]+m.a[ 0], a[ 4]+m.a[ 4], a[ 8]+m.a[ 8], a[12]+m.a[12],
         a[ 1]+m.a[ 1], a[ 5]+m.a[ 5], a[ 9]+m.a[ 9], a[13]+m.a[13],
         a[ 2]+m.a[ 2], a[ 6]+m.a[ 6], a[10]+m.a[10], a[14]+m.a[14],
         a[ 3]+m.a[ 3], a[ 7]+m.a[ 7], a[11]+m.a[11], a[15]+m.a[15] );
}

/////////////////////////////////////////////////////////////////////////////

Matrix4x4 Matrix4x4::operator-( const Matrix4x4& m ) const
{
  return Matrix4x4(
         a[ 0]-m.a[ 0], a[ 4]-m.a[ 4], a[ 8]-m.a[ 8], a[12]-m.a[12],
         a[ 1]-m.a[ 1], a[ 5]-m.a[ 5], a[ 9]-m.a[ 9], a[13]-m.a[13],
         a[ 2]-m.a[ 2], a[ 6]-m.a[ 6], a[10]-m.a[10], a[14]-m.a[14],
         a[ 3]-m.a[ 3], a[ 7]-m.a[ 7], a[11]-m.a[11], a[15]-m.a[15] );
}

/////////////////////////////////////////////////////////////////////////////

Matrix4x4 Matrix4x4::operator*( const Matrix4x4& m ) const
{
  return Matrix4x4( _aij(0,0) * m._aij(0,0)
                   +_aij(0,1) * m._aij(1,0)
                   +_aij(0,2) * m._aij(2,0)
                   +_aij(0,3) * m._aij(3,0),
                    _aij(0,0) * m._aij(0,1)
                   +_aij(0,1) * m._aij(1,1)
                   +_aij(0,2) * m._aij(2,1)
                   +_aij(0,3) * m._aij(3,1),
                    _aij(0,0) * m._aij(0,2)
                   +_aij(0,1) * m._aij(1,2)
                   +_aij(0,2) * m._aij(2,2)
                   +_aij(0,3) * m._aij(3,2),
                    _aij(0,0) * m._aij(0,3)
                   +_aij(0,1) * m._aij(1,3)
                   +_aij(0,2) * m._aij(2,3)
                   +_aij(0,3) * m._aij(3,3), // line
                    _aij(1,0) * m._aij(0,0)
                   +_aij(1,1) * m._aij(1,0)
                   +_aij(1,2) * m._aij(2,0)
                   +_aij(1,3) * m._aij(3,0),
                    _aij(1,0) * m._aij(0,1)
                   +_aij(1,1) * m._aij(1,1)
                   +_aij(1,2) * m._aij(2,1)
                   +_aij(1,3) * m._aij(3,1),
                    _aij(1,0) * m._aij(0,2)
                   +_aij(1,1) * m._aij(1,2)
                   +_aij(1,2) * m._aij(2,2)
                   +_aij(1,3) * m._aij(3,2),
                    _aij(1,0) * m._aij(0,3)
                   +_aij(1,1) * m._aij(1,3)
                   +_aij(1,2) * m._aij(2,3)
                   +_aij(1,3) * m._aij(3,3), // line
                    _aij(2,0) * m._aij(0,0)
                   +_aij(2,1) * m._aij(1,0)
                   +_aij(2,2) * m._aij(2,0)
                   +_aij(2,3) * m._aij(3,0),
                    _aij(2,0) * m._aij(0,1)
                   +_aij(2,1) * m._aij(1,1)
                   +_aij(2,2) * m._aij(2,1)
                   +_aij(2,3) * m._aij(3,1),
                    _aij(2,0) * m._aij(0,2)
                   +_aij(2,1) * m._aij(1,2)
                   +_aij(2,2) * m._aij(2,2)
                   +_aij(2,3) * m._aij(3,2),
                    _aij(2,0) * m._aij(0,3)
                   +_aij(2,1) * m._aij(1,3)
                   +_aij(2,2) * m._aij(2,3)
                   +_aij(2,3) * m._aij(3,3), // line
                    _aij(3,0) * m._aij(0,0)
                   +_aij(3,1) * m._aij(1,0)
                   +_aij(3,2) * m._aij(2,0)
                   +_aij(3,3) * m._aij(3,0),
                    _aij(3,0) * m._aij(0,1)
                   +_aij(3,1) * m._aij(1,1)
                   +_aij(3,2) * m._aij(2,1)
                   +_aij(3,3) * m._aij(3,1),
                    _aij(3,0) * m._aij(0,2)
                   +_aij(3,1) * m._aij(1,2)
                   +_aij(3,2) * m._aij(2,2)
                   +_aij(3,3) * m._aij(3,2),
                    _aij(3,0) * m._aij(0,3)
                   +_aij(3,1) * m._aij(1,3)
                   +_aij(3,2) * m._aij(2,3)
                   +_aij(3,3) * m._aij(3,3)  // line
                  );
}

/////////////////////////////////////////////////////////////////////////////

// static
// assume last line of a and b is: (0 0 0 1)
// i.e. a(3,0..3)= a(3,0..3)= [0 0 0 1]
Matrix4x4 Matrix4x4::mult_3x4( const Matrix4x4& a, const Matrix4x4& b )
{
  return Matrix4x4( a(0,0) * b(0,0)
                   +a(0,1) * b(1,0)
                   +a(0,2) * b(2,0),
                    a(0,0) * b(0,1)
                   +a(0,1) * b(1,1)
                   +a(0,2) * b(2,1),
                    a(0,0) * b(0,2)
                   +a(0,1) * b(1,2)
                   +a(0,2) * b(2,2),
                    a(0,0) * b(0,3)
                   +a(0,1) * b(1,3)
                   +a(0,2) * b(2,3)
                   +a(0,3) , // line
                    a(1,0) * b(0,0)
                   +a(1,1) * b(1,0)
                   +a(1,2) * b(2,0),
                   +a(1,0) * b(0,1)
                   +a(1,1) * b(1,1)
                   +a(1,2) * b(2,1),
                    a(1,0) * b(0,2)
                   +a(1,1) * b(1,2)
                   +a(1,2) * b(2,2),
                    a(1,0) * b(0,3)
                   +a(1,1) * b(1,3)
                   +a(1,2) * b(2,3)
                   +a(1,3) , // line
                    a(2,0) * b(0,0)
                   +a(2,1) * b(1,0)
                   +a(2,2) * b(2,0),
                    a(2,0) * b(0,1)
                   +a(2,1) * b(1,1)
                   +a(2,2) * b(2,1),
                    a(2,0) * b(0,2)
                   +a(2,1) * b(1,2)
                   +a(2,2) * b(2,2),
                    a(2,0) * b(0,3)
                   +a(2,1) * b(1,3)
                   +a(2,2) * b(2,3)
                   +a(2,3) , // line
                    0., 0., 0., 1.  // line
                  );
}

/////////////////////////////////////////////////////////////////////////////

Matrix4x4& Matrix4x4::operator+=( const Matrix4x4& m )
{
  a[ 0]+= m.a[ 0];
  a[ 1]+= m.a[ 1];
  a[ 2]+= m.a[ 2];
  a[ 3]+= m.a[ 3];
  a[ 4]+= m.a[ 4];
  a[ 5]+= m.a[ 5];
  a[ 6]+= m.a[ 6];
  a[ 7]+= m.a[ 7];
  a[ 8]+= m.a[ 8];
  a[ 9]+= m.a[ 9];
  a[10]+= m.a[10];
  a[11]+= m.a[11];
  a[12]+= m.a[12];
  a[13]+= m.a[13];
  a[14]+= m.a[14];
  a[15]+= m.a[15];

  return *this;
}

/////////////////////////////////////////////////////////////////////////////

Matrix4x4& Matrix4x4::operator-=( const Matrix4x4& m )
{
  a[ 0]-= m.a[ 0];
  a[ 1]-= m.a[ 1];
  a[ 2]-= m.a[ 2];
  a[ 3]-= m.a[ 3];
  a[ 4]-= m.a[ 4];
  a[ 5]-= m.a[ 5];
  a[ 6]-= m.a[ 6];
  a[ 7]-= m.a[ 7];
  a[ 8]-= m.a[ 8];
  a[ 9]-= m.a[ 9];
  a[10]-= m.a[10];
  a[11]-= m.a[11];
  a[12]-= m.a[12];
  a[13]-= m.a[13];
  a[14]-= m.a[14];
  a[15]-= m.a[15];

  return *this;
}

/////////////////////////////////////////////////////////////////////////////

Matrix4x4 Matrix4x4::operator*( const double d ) const
{
  return Matrix4x4( a[ 0]*d, a[ 4]*d, a[ 8]*d, a[12]*d,
                    a[ 1]*d, a[ 5]*d, a[ 9]*d, a[13]*d,
                    a[ 2]*d, a[ 6]*d, a[10]*d, a[14]*d,
                    a[ 3]*d, a[ 7]*d, a[11]*d, a[15]*d );
}

/////////////////////////////////////////////////////////////////////////////

Matrix4x4 Matrix4x4::operator/( double d ) const
{
  d= 1. / d;
  return Matrix4x4( a[ 0]*d, a[ 4]*d, a[ 8]*d, a[12]*d,
                    a[ 1]*d, a[ 5]*d, a[ 9]*d, a[13]*d,
                    a[ 2]*d, a[ 6]*d, a[10]*d, a[14]*d,
                    a[ 3]*d, a[ 7]*d, a[11]*d, a[15]*d );
}

/////////////////////////////////////////////////////////////////////////////

Matrix4x4& Matrix4x4::operator*=( const double d )
{
  a[ 0]*= d;
  a[ 1]*= d;
  a[ 2]*= d;
  a[ 3]*= d;
  a[ 4]*= d;
  a[ 5]*= d;
  a[ 6]*= d;
  a[ 7]*= d;
  a[ 8]*= d;
  a[ 9]*= d;
  a[10]*= d;
  a[11]*= d;
  a[12]*= d;
  a[13]*= d;
  a[14]*= d;
  a[15]*= d;

  return *this;
}

/////////////////////////////////////////////////////////////////////////////

Matrix4x4& Matrix4x4::operator/=( double d )
{
  d= 1. / d;

  a[ 0]*= d;
  a[ 1]*= d;
  a[ 2]*= d;
  a[ 3]*= d;
  a[ 4]*= d;
  a[ 5]*= d;
  a[ 6]*= d;
  a[ 7]*= d;
  a[ 8]*= d;
  a[ 9]*= d;
  a[10]*= d;
  a[11]*= d;
  a[12]*= d;
  a[13]*= d;
  a[14]*= d;
  a[15]*= d;

  return *this;
}

/////////////////////////////////////////////////////////////////////////////

Matrix4x4 Matrix4x4::computeInverse() const
{
  double t4, t6, t8, t10, t12, t14;
  double t20, t23, t24, t43, t46, t47, t51, t54, t57, t58, t59, t60;
  register double t17;

  Matrix4x4 inv;

#define A(r,c) inv.a[ 4*c + r - 5 ]
#define B(r,c) a[ 4*c + r - 5 ]

  if( B(4,1) != 0.0 || B(4,2) != 0.0 ||
      B(4,3) != 0.0 || B(4,4) != 1.0 )
    return computeInverse_pivot(); // use the slow classical algo

  t4= B(1,1)*B(2,2);
  t6= B(1,1)*B(3,2);
  t8= B(2,1)*B(1,2);
  t10= B(2,1)*B(3,2);
  t12= B(3,1)*B(1,2);
  t14= B(3,1)*B(2,2);
  t17= (t8-t4)*B(3,3) + (t6-t12)*B(2,3) +(t14-t10)*B(1,3);

  if( t17 == 0.0F )
    goto singular;

  t17= 1. / t17;
  t20= B(3,2)*B(1,3);
  t23= B(1,2)*B(2,3);
  t24= B(2,2)*B(1,3);
  t43= B(3,1)*B(1,3);
  t46= B(1,1)*B(2,3);
  t47= B(2,1)*B(1,3);
  t51= B(1,1)*B(2,4);
  t54= B(2,1)*B(1,4);
  t57= B(3,1)*B(1,4);
  t58= B(3,2)*B(2,3);
  t59= B(2,2)*B(3,3);
  t60= B(1,2)*B(3,3);

  A(1,1)= (t58-t59) * t17;
  A(1,2)= (t60-t20) * t17;
  A(1,3)= (t24-t23) * t17;
  A(1,4)= ( (t23-t24)*B(3,4) + (t20-t60)*B(2,4) + (t59-t58)*B(1,4) )
          * t17;

  A(2,1)= ( B(2,1)*B(3,3) - B(3,1)*B(2,3) ) * t17;
  A(2,2)= ( t43 - B(1,1)*B(3,3) ) * t17;
  A(2,3)= (t46-t47) * t17;
  A(2,4)= ( (t47-t46)*B(3,4) + (t51-t54)*B(3,3) - t43*B(2,4) + t57*B(2,3) )
          * t17;

  A(3,1)= (t14-t10) * t17;
  A(3,2)= (t6-t12) * t17;
  A(3,3)= (t8-t4) * t17;
  A(3,4)= ( (t4-t8)*B(3,4) + (t54-t51)*B(3,2) + t12*B(2,4) - t57*B(2,2) )
          * t17;

  A(4,1)=
  A(4,2)=
  A(4,3)= 0.;
  A(4,4)= 1.;
  return inv;

singular:
#ifndef NDEBUG
  fprintf(stderr,"Matrix4x4: Got a singular trf matrix!\n");
#endif
  inv.setIdentity();
  return inv;

#undef A
#undef B
}

/////////////////////////////////////////////////////////////////////////////

// private:
// From Mesa, src/matrix.c:
//   >This function was contributed by Thomas Malik (malik@rhrk.uni-kl.de).
//   >Thanks Thomas!
Matrix4x4 Matrix4x4::computeInverse_pivot() const
{
  Matrix4x4 inv;

#define A(r,c) inv.a[ 4*(c) + (r) ]
#define B(r,c) tmp.a[ 4*(c) + (r) ]

  Matrix4x4 tmp= *this;

  inv.setIdentity();

  int i, j, k, ind;
  for( i= 0; i != 4; ++i )
    {
    double val= B(i,i); // find pivot
    ind= i;
    for( j= i+1; j != 4; ++j )
      {
      if( fabs(B(j,i)) > fabs(val) )
        {
        ind= j;
        val= B(j,i);
        }
      }

    if( ind != i )
      {
      // swap columns
      for( j= 0; j != 4; ++j )
        {
        register double val2= A(i,j);
        A(i,j)= A(ind,j);
        A(ind,j)= val2;
        val2= B(i,j);
        B(i,j)= B(ind,j);
        B(ind,j)= val2;
        }
      }

    if( val == 0. )
      {
      // The matrix is singular
#ifndef NDEBUG
      fprintf(stderr,"Matrix4x4: Got a singular matrix! (pivoting)\n");
#endif
      inv.setIdentity();
      return inv;
      }

    for( j= 0; j != 4; ++j )
      {
      B(i,j) /= val;
      A(i,j) /= val;
      }

    for( j= 0; j != 4; ++j)
      {
      // eliminate column
      if( j == i )
        continue;
      register double val= B(j,i);
      for( k= 0; k != 4; ++k )
        {
        B(j,k) -= B(i,k) * val;
        A(j,k) -= A(i,k) * val;
        }
      }
    }

#undef A
#undef B

  return inv;
}

/////////////////////////////////////////////////////////////////////////////

double Matrix4x4::mabs() const
{
  double ab= 0.;

  for( int i= 0; i < 16; ++i )
    ab+= fabs(a[i]);

  return ab;
}

/////////////////////////////////////////////////////////////////////////////

