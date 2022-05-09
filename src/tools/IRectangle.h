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
   IRectangle.h

   Creation: SR, April 23rd, 1996, from VDisplay.h
   Revisions:
*/

#ifndef __IRectangle_h
#define __IRectangle_h

#ifndef __IVector_h
#include "IVector.h"
#endif

/////////////////////////////////////////////////////////////////////////////

struct IRectangle
{
  IPoint origin;
  IVector size;

  IRectangle()
    {}
  IRectangle( const IPoint& _origin, const IVector& _size ):
    origin( _origin ), size( _size )
    {}
  IRectangle( const IPoint& p1, const IPoint& p2 ):
    origin( p1 ),
    size( p2.x()-p1.x()+1, p2.y()-p1.y()+1 )
    {}

  IPoint p1() const { return origin; }
  IPoint p2() const { return origin+size-IVector(1,1); }
  const IVector& getSize() const { return size; }
  int x() const { return origin.x(); }
  int y() const { return origin.y(); }
  int x1() const { return origin.x(); }
  int y1() const { return origin.y(); }
  int x2() const { return origin.x()+size.x()-1; }
  int y2() const { return origin.y()+size.y()-1; }
  int width() const { return size.x(); }
  int height() const { return size.y(); }

  boolean into( const IPoint& p ) const
    {
    return p.x() >= x1() && p.y() >= y1() &&
           p.x() <= x2() && p.y() <= y2();
    }
  IRectangle& operator = ( const IRectangle& r )
    {
    origin= r.origin;
    size= r.size;
    return *this;
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __IRectangle_h
