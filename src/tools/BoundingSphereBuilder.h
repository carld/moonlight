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
  BoundingSphereBuilder.h

  Stephane Rehel

  September 12 1997
*/

#ifndef __BoundingSphereBuilder_h
#define __BoundingSphereBuilder_h

#ifndef __BoundingSphere_h
#include "BoundingSphere.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class BoundingSphereBuilder
{
protected:
  Point center;
  double radius, radius2;
  int nPoints;

  const Point* xmin;
  const Point* xmax;
  const Point* ymin;
  const Point* ymax;
  const Point* zmin;
  const Point* zmax;

public:
  BoundingSphereBuilder();
  virtual ~BoundingSphereBuilder();

  BoundingSphere get() const;

  void init();

  void pass1( const Point& p );
  void end_pass1();;

  void pass2( const Point& p );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __BoundingSphereBuilder_h
