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
  Quaternion.C

  Stephane Rehel

  August 21 1997
*/

#include "Quaternion.h"

#include "Matrix4x4.h"

/////////////////////////////////////////////////////////////////////////////

void Quaternion::getRotationMatrix( Matrix4x4& m ) const
{
  double X= v.x();
  double Y= v.y();
  double Z= v.z();

  double k= 2. / (X*X + Y*Y + Z*Z + s*s);

  double xk= X * k;
  double yk= Y * k;
  double zk= Z * k;
  double wx= s * xk;
  double wy= s * yk;
  double wz= s * zk;
  double xx= X * xk;
  double xy= X * yk;
  double xz= X * zk;
  double yy= Y * yk;
  double yz= Y * zk;
  double zz= Z * zk;

  m.aij(0,0)= 1. - (yy + zz);
  m.aij(0,1)= xy - wz;
  m.aij(0,2)= xz + wy;
  m.aij(0,3)= 0.;

  m.aij(1,0)= xy + wz;
  m.aij(1,1)= 1. - (xx + zz);
  m.aij(1,2)= yz - wx;
  m.aij(1,3)= 0.;

  m.aij(2,0)= xz - wy;
  m.aij(2,1)= yz + wx;
  m.aij(2,2)= 1. - (xx + yy);
  m.aij(2,3)= 0.;

  m.aij(3,0)= 0.;
  m.aij(3,1)= 0.;
  m.aij(3,2)= 0.;
  m.aij(3,3)= 1.;
}

/////////////////////////////////////////////////////////////////////////////

void Quaternion::makeFromRotationMatrix( const Matrix4x4& m )
{
  double trace= m(0,0) + m(1,1) + m(2,2);
  if( trace > 0. )
    {
    double k= sqrt(trace + 1.);

    double W= k * 0.5;
    k= 0.5 / k;

    double X= (m(2,1) - m(1,2)) * k;
    double Y= (m(0,2) - m(2,0)) * k;
    double Z= (m(1,0) - m(0,1)) * k;

    s= W;
    v= Vector(X,Y,Z);
    }
   else
    {
    static int nxt[3] = { 1, 2, 0 };
    int i= 0;
    if( m(1,1) > m(0,0) )
      i= 1;
    if( m(2,2) > m(i,i) )
      i= 2;
    int j= nxt[i];
    int k= nxt[j];

    double t= sqrt( (m(i,i) - (m(j,j)+m(k,k))) + 1. );

    v[i]= t * 0.5;
    t= 0.5 / t;

    s= (m(k,j) - m(j,k)) * k;
    v[j]= (m(j,i) + m(i,j)) * k;
    v[k]= (m(k,i) + m(i,k)) * k;
    }
}

/////////////////////////////////////////////////////////////////////////////

// static
// Spherical interpolation between p and q, parameter t
Quaternion Quaternion::slerp( const Quaternion& p,
                              const Quaternion& q,
                              double t )
{
  const double epsilon = 1e-6;

  double cosom=  p.v.x() * q.v.x()
               + p.v.y() * q.v.y()
               + p.v.z() * q.v.z()
               + p.s     * q.s ;

  if( (1. + cosom) > epsilon )
    {
    double sclp, sclq;

    if( (1. - cosom) > epsilon )
      {
      double omega= acos(cosom);
      double sinom= sin(omega);
      sclp= sin( (1. - t) * omega ) / sinom;
      sclq= sin( t * omega ) / sinom;
      }
     else
      {
      sclp= 1. - t;
      sclq= t;
      }
    return Quaternion( sclp * p.s + sclq * q.s,
                       sclp * p.v + sclq * q.v );
    }

  double qts= p.v.z();
  Vector qtv= Vector( -p.v.y(), p.v.x(), -p.s );
  double sclp= sin( (1. - t) * M_PI * 0.5 );
  double sclq= sin( t * M_PI * 0.5 );
  return Quaternion( sclp * p.s + sclq * qts,
                     sclp * p.v + sclq * qtv );

}

/////////////////////////////////////////////////////////////////////////////
