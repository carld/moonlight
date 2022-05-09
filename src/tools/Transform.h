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
  Transform.h

  Transform object coordinates to world coordinates

  Stephane Rehel
  October 15 1996
*/

#ifndef __Transform_h
#define __Transform_h

#ifndef __Matrix4x4_h
#include "Matrix4x4.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class Transform
{
protected:
  Matrix4x4 _o2w, // object to world
            _w2o; // world to object

public:
  Transform();

  Transform( const Transform& t )
    {
    _o2w= t._o2w;
    _w2o= t._w2o;
    }

  ~Transform()
    {}

  void reset();

  static const Transform& getIdentity();

  Transform& translate( const Vector& t );
  Transform& scale( const Vector& s );
  Transform& rotateX( DBL angle ); // angle in radians
  Transform& rotateY( DBL angle ); // angle in radians
  Transform& rotateZ( DBL angle ); // angle in radians

  // axis normalized
  // angle in radians
  Transform& rotate( const Vector& axis, DBL angle );

  const DBL* dump_o2w() const
    {
    return _o2w.dump();
    }

  Transform& operator = ( const Transform& t )
    {
    _o2w= t._o2w;
    _w2o= t._w2o;

    return *this;
    }

  Transform& concat( const Transform& next )
    {
//    o2w &= previous.o2w; // i.e.: o2w= previous.o2w * o2w;
///    o2w = previous.o2w * o2w;
///    w2o *= previous.o2w;
    _o2w *= next._o2w;
    _w2o &= next._w2o; // i.e.: w2o= next.o2w * w2o;

    return *this;
    }

  const Matrix4x4& get_o2w() const { return _o2w; }
  const Matrix4x4& get_w2o() const { return _w2o; }

  Point o2w( const Point& p ) const
    {
    return _o2w.apply(p);
    }
  Point w2o( const Point& p ) const
    {
    return _w2o.apply(p);
    }
  Vector o2w( const Vector& v ) const
    {
    return _o2w.apply(v);
    }
  Vector w2o( const Vector& v ) const
    {
    return _w2o.apply(v);
    }

  Vector normal_o2w( const Vector& v ) const
    {
    return Vector( _w2o(0,0)*v.x() + _w2o(1,0)*v.y() + _w2o(2,0)*v.z(),
                   _w2o(0,1)*v.x() + _w2o(1,1)*v.y() + _w2o(2,1)*v.z(),
                   _w2o(0,2)*v.x() + _w2o(1,2)*v.y() + _w2o(2,2)*v.z() );
    }
  Vector normal_w2o( const Vector& v ) const
    {
    return Vector( _o2w(0,0)*v.x() + _o2w(1,0)*v.y() + _o2w(2,0)*v.z(),
                   _o2w(0,1)*v.x() + _o2w(1,1)*v.y() + _o2w(2,1)*v.z(),
                   _o2w(0,2)*v.x() + _o2w(1,2)*v.y() + _o2w(2,2)*v.z() );
    }

  void set( const Matrix4x4& __o2w,
            const Matrix4x4& __w2o )
    {
    _o2w= __o2w;
    _w2o= __w2o;
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef  __Transform_h
