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
  MLCurveVertex.h

  Stephane Rehel

  January 23 1998
*/

#ifndef __MLCurveVertex_h
#define __MLCurveVertex_h

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

/////////////////////////////////////////////////////////////////////////////

struct MLCurveVertex
{
  Point point;
  double parameter;

  typedef unsigned int Flag;
  Flag flags;

  enum FLAG
    {
    ALLOCATED    = (1<<0),
    SELECTED     = (1<<1),
    TMP_FLAG     = (1<<2)
    };

  MLCurveVertex()
    {
    point= Point(0,0,0);
    parameter= 0.;
    flags= Flag(ALLOCATED);
    }

  ~MLCurveVertex()
    {
    flags &= ~Flag(ALLOCATED);
    }

  int getSizeOf() const;

  void set( double _parameter, const Point& _point );

  int allocated() const
    {
    return (flags & Flag(ALLOCATED)) != Flag(0);
    }

  int selected() const
    {
    return (flags & Flag(SELECTED)) != Flag(0);
    }

  MLCurveVertex& operator = ( const MLCurveVertex& v );

  void unallocate();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLCurveVertex_h
