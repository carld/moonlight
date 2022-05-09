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
  MLCurveVertex.C

  Stephane Rehel

  January 23 1998
*/

#include "MLCurveVertex.h"

/////////////////////////////////////////////////////////////////////////////

int MLCurveVertex::getSizeOf() const
{
  return sizeof(*this);
}

/////////////////////////////////////////////////////////////////////////////

void MLCurveVertex::set( double _parameter, const Point& _point )
{
  parameter= _parameter;
  point= _point;
}

/////////////////////////////////////////////////////////////////////////////

MLCurveVertex& MLCurveVertex::operator = ( const MLCurveVertex& v )
{
  point= v.point;
  parameter= v.parameter;

  flags= v.flags;

  return *this;
}

/////////////////////////////////////////////////////////////////////////////

void MLCurveVertex::unallocate()
{
  flags &= ~Flag(ALLOCATED);
}

/////////////////////////////////////////////////////////////////////////////
