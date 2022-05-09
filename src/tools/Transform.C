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
  Transform.C

  Stephane Rehel
  October 15 1996
*/

#include "Transform.h"


/////////////////////////////////////////////////////////////////////////////

static Transform* identity= 0;

/////////////////////////////////////////////////////////////////////////////

// static
const Transform& Transform::getIdentity()
{
/*
  static int first= 1;
  if( first )
    {
    first= 0;
    identity.reset(); // careful with C++ initializers on C++ compilers..
    }
*/

  if( ::identity == 0 )
    {
    ::identity= new Transform;
    ::identity->reset();
    }

  return *(::identity);
}

/////////////////////////////////////////////////////////////////////////////

Transform::Transform()
{
  reset();
}

/////////////////////////////////////////////////////////////////////////////

void Transform::reset()
{
  _o2w.setIdentity();
  _w2o.setIdentity();
}

/////////////////////////////////////////////////////////////////////////////

Transform& Transform::translate( const Vector& t )
{
Matrix4x4 tm;
tm.setTranslation(t); _o2w= tm * _o2w;
tm.setTranslation(-t);_w2o= _w2o * tm;
return *this;
/*
  o2w.aij(0,3) += t.x();
  o2w.aij(1,3) += t.y();
  o2w.aij(2,3) += t.z();

  w2o.aij(0,3)-=  w2o.aij(0,0)*t.x()
                + w2o.aij(0,1)*t.y()
                + w2o.aij(0,2)*t.z();
  w2o.aij(1,3)-=  w2o.aij(1,0)*t.x()
                + w2o.aij(1,1)*t.y()
                + w2o.aij(1,2)*t.z();
  w2o.aij(2,3)-=  w2o.aij(2,0)*t.x()
                + w2o.aij(2,1)*t.y()
                + w2o.aij(2,2)*t.z();

  return *this;
*/
}

/////////////////////////////////////////////////////////////////////////////

Transform& Transform::scale( const Vector& s )
{
Matrix4x4 tm;
tm.setScaling(s); _o2w= tm * _o2w;
tm.setScaling(Vector(1./s.x(),1./s.y(),1./s.z())); _w2o= _w2o * tm;
return *this;

/*
  o2w.aij(0,0) *= s.x();
  o2w.aij(0,1) *= s.x();
  o2w.aij(0,2) *= s.x();
  o2w.aij(0,3) *= s.x();

  o2w.aij(1,0) *= s.y();
  o2w.aij(1,1) *= s.y();
  o2w.aij(1,2) *= s.y();
  o2w.aij(1,3) *= s.y();

  o2w.aij(2,0) *= s.z();
  o2w.aij(2,1) *= s.z();
  o2w.aij(2,2) *= s.z();
  o2w.aij(2,3) *= s.z();

  register DBL is;

  is= 1. / s.x();
  w2o.aij(0,0)*= is;
  w2o.aij(1,0)*= is;
  w2o.aij(2,0)*= is;

  is= 1. / s.y();
  w2o.aij(0,1)*= is;
  w2o.aij(1,1)*= is;
  w2o.aij(2,1)*= is;

  is= 1. / s.z();
  w2o.aij(0,2)*= is;
  w2o.aij(1,2)*= is;
  w2o.aij(2,2)*= is;

  return *this;
*/
}

/////////////////////////////////////////////////////////////////////////////

Transform& Transform::rotateX( DBL angle ) // angle in radians
{
Matrix4x4 tm;
tm.setRotationX(angle); _o2w= tm * _o2w;
tm.setRotationX(-angle);_w2o= _w2o * tm;
return *this;
/*
  register DBL cosa= cos(angle);
  register DBL sina= sin(angle);

  register DBL d1, d2;

  d1= o2w(1,0);
  d2= o2w(2,0);
  o2w.aij(1,0)= cosa*d1 - sina*d2;
  o2w.aij(2,0)= sina*d1 + cosa*d2;
  d1= o2w(1,1);
  d2= o2w(2,1);
  o2w.aij(1,1)= cosa*d1 - sina*d2;
  o2w.aij(2,1)= sina*d1 + cosa*d2;
  d1= o2w(1,2);
  d2= o2w(2,2);
  o2w.aij(1,2)= cosa*d1 - sina*d2;
  o2w.aij(2,2)= sina*d1 + cosa*d2;
  d1= o2w(1,3);
  d2= o2w(2,3);
  o2w.aij(1,3)= cosa*d1 - sina*d2;
  o2w.aij(2,3)= sina*d1 + cosa*d2;

  d1= w2o(0,1);
  d2= w2o(0,2);
  w2o.aij(0,1)= cosa*d1 - sina*d2;
  w2o.aij(0,2)= sina*d1 + cosa*d2;
  d1= w2o(1,1);
  d2= w2o(1,2);
  w2o.aij(1,1)= cosa*d1 - sina*d2;
  w2o.aij(1,2)= sina*d1 + cosa*d2;
  d1= w2o(2,1);
  d2= w2o(2,2);
  w2o.aij(2,1)= cosa*d1 - sina*d2;
  w2o.aij(2,2)= sina*d1 + cosa*d2;
  d1= w2o(3,1);
  d2= w2o(3,2);
  w2o.aij(3,1)= cosa*d1 - sina*d2;
  w2o.aij(3,2)= sina*d1 + cosa*d2;

  return *this;
*/
}

/////////////////////////////////////////////////////////////////////////////

Transform& Transform::rotateY( DBL angle ) // angle in radians
{
Matrix4x4 tm;
tm.setRotationY(angle); _o2w= tm * _o2w;
tm.setRotationY(-angle);_w2o= _w2o * tm;
return *this;
/*
  register DBL cosa= cos(angle);
  register DBL sina= sin(angle);

  register DBL d1, d2;

  d1= o2w(2,0);
  d2= o2w(0,0);
  o2w.aij(2,0)= cosa*d1 - sina*d2;
  o2w.aij(0,0)= sina*d1 + cosa*d2;
  d1= o2w(2,1);
  d2= o2w(0,1);
  o2w.aij(2,1)= cosa*d1 - sina*d2;
  o2w.aij(0,1)= sina*d1 + cosa*d2;
  d1= o2w(2,2);
  d2= o2w(0,2);
  o2w.aij(2,2)= cosa*d1 - sina*d2;
  o2w.aij(0,2)= sina*d1 + cosa*d2;
  d1= o2w(2,3);
  d2= o2w(0,3);
  o2w.aij(2,3)= cosa*d1 - sina*d2;
  o2w.aij(0,3)= sina*d1 + cosa*d2;

  d1= w2o(0,2);
  d2= w2o(0,0);
  w2o.aij(0,2)= cosa*d1 - sina*d2;
  w2o.aij(0,0)= sina*d1 + cosa*d2;
  d1= w2o(1,2);
  d2= w2o(1,0);
  w2o.aij(1,2)= cosa*d1 - sina*d2;
  w2o.aij(1,0)= sina*d1 + cosa*d2;
  d1= w2o(2,2);
  d2= w2o(2,0);
  w2o.aij(2,2)= cosa*d1 - sina*d2;
  w2o.aij(2,0)= sina*d1 + cosa*d2;
  d1= w2o(3,2);
  d2= w2o(3,0);
  w2o.aij(3,2)= cosa*d1 - sina*d2;
  w2o.aij(3,0)= sina*d1 + cosa*d2;

  return *this;
*/
}

/////////////////////////////////////////////////////////////////////////////

Transform& Transform::rotateZ( DBL angle ) // angle in radians
{
Matrix4x4 tm;
tm.setRotationZ(angle); _o2w= tm * _o2w;
tm.setRotationZ(-angle);_w2o= _w2o * tm;
return *this;
/*
  register DBL cosa= cos(angle);
  register DBL sina= sin(angle);

  register DBL d1, d2;

  d1= o2w(0,0);
  d2= o2w(1,0);
  o2w.aij(0,0)= cosa*d1 - sina*d2;
  o2w.aij(1,0)= sina*d1 + cosa*d2;
  d1= o2w(0,1);
  d2= o2w(1,1);
  o2w.aij(0,1)= cosa*d1 - sina*d2;
  o2w.aij(1,1)= sina*d1 + cosa*d2;
  d1= o2w(0,2);
  d2= o2w(1,2);
  o2w.aij(0,2)= cosa*d1 - sina*d2;
  o2w.aij(1,2)= sina*d1 + cosa*d2;
  d1= o2w(0,3);
  d2= o2w(1,3);
  o2w.aij(0,3)= cosa*d1 - sina*d2;
  o2w.aij(1,3)= sina*d1 + cosa*d2;

  d1= w2o(0,0);
  d2= w2o(0,1);
  w2o.aij(0,0)= cosa*d1 - sina*d2;
  w2o.aij(0,1)= sina*d1 + cosa*d2;
  d1= w2o(1,0);
  d2= w2o(1,1);
  w2o.aij(1,0)= cosa*d1 - sina*d2;
  w2o.aij(1,1)= sina*d1 + cosa*d2;
  d1= w2o(2,0);
  d2= w2o(2,1);
  w2o.aij(2,0)= cosa*d1 - sina*d2;
  w2o.aij(2,1)= sina*d1 + cosa*d2;
  d1= w2o(3,0);
  d2= w2o(3,1);
  w2o.aij(3,0)= cosa*d1 - sina*d2;
  w2o.aij(3,1)= sina*d1 + cosa*d2;

  return *this;
*/
}

/////////////////////////////////////////////////////////////////////////////

// angle in radians
// axis normalized
Transform& Transform::rotate( const Vector& axis, DBL angle )
{
  DBL xx= axis.x() * axis.x();
  DBL xy= axis.x() * axis.y();
  DBL xz= axis.x() * axis.z();
  DBL yy= axis.y() * axis.y();
  DBL yz= axis.y() * axis.z();
  DBL zz= axis.z() * axis.z();

  DBL sina= sin(angle);
  register DBL xs= axis.x() * sina;
  register DBL ys= axis.y() * sina;
  register DBL zs= axis.z() * sina;

  register DBL cosa= cos(angle);
  register DBL one_cosa= 1. - cosa;

  Matrix4x4 m;

  m.aij(0,0)= xx * one_cosa + cosa;
  m.aij(0,1)= xy * one_cosa - zs;
  m.aij(0,2)= xz * one_cosa + ys;

  m.aij(1,0)= xy * one_cosa + zs;
  m.aij(1,1)= yy * one_cosa + cosa;
  m.aij(1,2)= yz * one_cosa - xs;

  m.aij(2,0)= xz * one_cosa - ys;
  m.aij(2,1)= yz * one_cosa + xs;
  m.aij(2,2)= zz * one_cosa + cosa;

  m.aij(0,3)=
  m.aij(1,3)=
  m.aij(2,3)=
  m.aij(3,0)=
  m.aij(3,1)=
  m.aij(3,2)= 0.;
  m.aij(3,3)= 1.;

  _o2w= Matrix4x4::mult_3x4(m,_o2w);

  // please dont loose precision...
  m.aij(0,1)= xy * one_cosa + zs;
  m.aij(0,2)= xz * one_cosa - ys;
  m.aij(1,0)= xy * one_cosa - zs;
  m.aij(1,2)= yz * one_cosa + xs;
  m.aij(2,0)= xz * one_cosa + ys;
  m.aij(2,1)= yz * one_cosa - xs;

  _w2o= Matrix4x4::mult_3x4(_w2o,m);

  return *this;
}

/////////////////////////////////////////////////////////////////////////////
