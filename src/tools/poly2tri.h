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
  poly2tri.h

  Creation: SR, February 3rd, 1996
  Revisions:
*/

#ifndef __poly2tri_h
#define __poly2tri_h

#ifndef __Vector_h
#include "Vector.h"
#endif

/////////////////////////////////////////////////////////////////////////////

// Removes a triangle from the specified polygon.
// The size of the polygon is reduced
boolean remove_triangle( Point* poly, int& polysize,
                       Point& v1, Point& v2, Point& v3 );

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __poly2tri_h

