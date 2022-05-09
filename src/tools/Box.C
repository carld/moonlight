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
   Box.C

   Creation: SR, August 24th, 1995
   Revisions:
     + SR January 21 1997
*/

#include "Box.h"

/////////////////////////////////////////////////////////////////////////////

// direction is normalized
boolean Box::intersect_from_exterior( const Point& origin,
                                    const Vector& direction,
                                    double min_distance,
                                    double max_distance ) const
{
  Vector size= maxP - minP;

  Vector O= origin - minP;

  double eps= 1e-10;

#define TEST(Z,X,Y,label) \
  if( direction.Z() < -eps )                                \
    {                                                      \
    register double t= -(O.Z()-size.Z()) / direction.Z();  \
    if( t < min_distance || t > max_distance )             \
      goto label;                                          \
    register double I= O.X() + t * direction.X();          \
    if( I >= 0. && I <= size.X() )                         \
      {                                                    \
      I= O.Y() + t * direction.Y();                        \
      if( I >= 0. && I <= size.Y() )                       \
        return true;                                      \
      }                                                    \
    }                                                      \
   else                                                    \
    {                                                      \
    if( direction.Z() < eps )                             \
      goto label;                                          \
    register double t= -O.Z() / direction.Z();             \
    if( t < min_distance || t > max_distance )             \
      goto label;                                          \
    register double I= O.X() + t * direction.X();          \
    if( I >= 0. && I <= size.X() )                         \
      {                                                    \
      I= O.Y() + t * direction.Y();                        \
      if( I >= 0. && I <= size.Y() )                       \
        return true;                                      \
      }                                                    \
    }

  TEST(z,x,y,next_y)

next_y:
  TEST(y,z,x,next_x)

next_x:
  TEST(x,y,z,next_z)

next_z:
  return false;

#undef TEST
}

/////////////////////////////////////////////////////////////////////////////

// direction is normalized
boolean Box::intersect_from_interior( const Point& origin,
                                    const Vector& direction,
                                    double min_distance,
                                    double max_distance ) const
{
  Vector size= maxP - minP;

  Vector O= origin - minP;

  double eps= 1e-10;

#define TEST(Z,X,Y,label) \
  if( direction.Z() > eps )                                \
    {                                                      \
    register double t= -(O.Z()-size.Z()) / direction.Z();  \
    if( t < min_distance || t > max_distance )             \
      goto label;                                          \
    register double I= O.X() + t * direction.X();          \
    if( I >= 0. && I <= size.X() )                         \
      {                                                    \
      I= O.Y() + t * direction.Y();                        \
      if( I >= 0. && I <= size.Y() )                       \
        return true;                                      \
      }                                                    \
    }                                                      \
   else                                                    \
    {                                                      \
    if( direction.Z() > -eps )                             \
      goto label;                                          \
    register double t= -O.Z() / direction.Z();             \
    if( t < min_distance || t > max_distance )             \
      goto label;                                          \
    register double I= O.X() + t * direction.X();          \
    if( I >= 0. && I <= size.X() )                         \
      {                                                    \
      I= O.Y() + t * direction.Y();                        \
      if( I >= 0. && I <= size.Y() )                       \
        return true;                                      \
      }                                                    \
    }

  TEST(z,x,y,next_y)

next_y:
  TEST(y,z,x,next_x)

next_x:
  TEST(x,y,z,next_z)

next_z:
  return false;

#undef TEST
}

/////////////////////////////////////////////////////////////////////////////

boolean Box::segment_intersect( const Point& p1, const Point& p2 )
{
  Vector size= maxP - minP;

  Vector O= p1 - minP;
  Vector direction= p2 - p1;

  const double eps= 1e-10;

#define TEST(Z,X,Y) \
  if( direction.Z() < -eps )                               \
    {                                                      \
    register double t= -(O.Z()-size.Z()) / direction.Z();  \
    if( t >= 0. && t <= 1. )                               \
      {                                                    \
      register double I= O.X() + t * direction.X();        \
      if( I >= 0. && I <= size.X() )                       \
        {                                                  \
        I= O.Y() + t * direction.Y();                      \
        if( I >= 0. && I <= size.Y() )                     \
          return true;                                    \
        }                                                  \
      }                                                    \
    }                                                      \
  if( direction.Z() > eps )                                \
    {                                                      \
    register double t= -O.Z() / direction.Z();             \
    if( t >= 0. && t <= 1. )                               \
      {                                                    \
      register double I= O.X() + t * direction.X();        \
      if( I >= 0. && I <= size.X() )                       \
        {                                                  \
        I= O.Y() + t * direction.Y();                      \
        if( I >= 0. && I <= size.Y() )                     \
          return true;                                    \
        }                                                  \
      }                                                    \
    }

  TEST(z,x,y)

  TEST(y,z,x)

  TEST(x,y,z)

  return false;

#undef TEST
}

/////////////////////////////////////////////////////////////////////////////

boolean Box::inHalfPlane( const Point& point,
                        const Vector& normal // not normalized
                      ) const
{
  Vector mp= minP - point;
  Vector Mp= maxP - point;
  register double mx= normal.x() * mp.x();
  register double my= normal.y() * mp.y();
  register double mz= normal.z() * mp.z();
  register double Mx= normal.x() * Mp.x();
  register double My= normal.y() * Mp.y();
  register double Mz= normal.z() * Mp.z();

  return (mx + my + mz >= 0.) ||
         (Mx + my + mz >= 0.) ||
         (Mx + My + mz >= 0.) ||
         (mx + My + mz >= 0.) ||
         (mx + my + Mz >= 0.) ||
         (Mx + my + Mz >= 0.) ||
         (Mx + My + Mz >= 0.) ||
         (mx + My + Mz >= 0.);
}

/////////////////////////////////////////////////////////////////////////////
