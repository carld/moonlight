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
  BoundingCone.C

  Stephane Rehel
  Februray 17 1997
*/

#include "BoundingCone.h"
#include "BoundingSphere.h"

/////////////////////////////////////////////////////////////////////////////

BoundingCone::BoundingCone( const Point& _origin,
                            const Vector& _axis, // not normalized
                            const Point& one_point )
{
  Vector d= one_point - _origin;
  double norm= d.norm();

  if( norm < 1e-20 )
    {
    set( _origin, _axis, -1. );
    return;
    }

  double axis_norm= _axis.norm();
  if( axis_norm < 1e-20 )
    {
    set( _origin, Vector(0,0,1), -1. );
    return;
    }

  set( _origin, _axis, (_axis|d) / (norm * axis_norm) );
}

/////////////////////////////////////////////////////////////////////////////

BoundingCone::BoundingCone( const Point& _origin,
                            const BoundingSphere& bs )
{
  Vector axis= bs.getCenter() - _origin;
  double axis_norm2= axis.norm2();
  double axis_norm= sqrt(axis_norm2);

  if( axis_norm < 1e-20 )
    {
    set( _origin, Vector(0,0,1), -1. );
    return;
    }

  double cos_a= axis_norm / sqrt( axis_norm2 + bs.getRadius2() );
  set( _origin, axis, cos_a );
}

/////////////////////////////////////////////////////////////////////////////

void BoundingCone::set( const Point& _origin,
                        const Vector& _axis, // not normalized
                        double _cos_alpha )
{
  double axis_norm= _axis.norm();
  if( axis_norm < 1e-20 )
    {
    axis= Vector(0,0,1);
    _cos_alpha= -1.;
    }
   else
    axis= _axis / axis_norm;

  origin= _origin;
  cos_alpha= _cos_alpha;
  if( cos_alpha > 1. ) cos_alpha= 1.; else
  if( cos_alpha <-1. ) cos_alpha=-1.;
}

/////////////////////////////////////////////////////////////////////////////

boolean BoundingCone::intersectSphere( const BoundingSphere& bsph ) const
{
/*
c'est faux
  Vector OC= bsph.getCenter() - origin;
  register double d2= OC.norm2();
  register double radius= bsph.getRadius();
  register double r2= bsph.getRadius2();
  if( d2 <= r2 )
    return true;
  register double a= (OC | axis);

  return a*sqrt(d2-r2) + radius*sqrt(d2-a*a) >= d2*cos_alpha;
*/
  return same_origin_intersect( BoundingCone(origin,bsph) );
}

/////////////////////////////////////////////////////////////////////////////

// Test if *this and c intersect
// *this and c are to have the same origin
boolean BoundingCone::same_origin_intersect( const BoundingCone& c ) const
{
  double gamma= acos( axis | c.axis );
  double alpha= acos(cos_alpha);
  double beta = acos(c.cos_alpha);

  return gamma <= alpha + beta;
}

/////////////////////////////////////////////////////////////////////////////
