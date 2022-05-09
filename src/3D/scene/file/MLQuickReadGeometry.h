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
  MLQuickReadGeometry.h

  Stephane Rehel
  April 21 1997
*/

#ifndef __MLQuickReadGeometry_h
#define __MLQuickReadGeometry_h

#ifndef __Box_h
#include "tools/Box.h"
#endif

class Point;
class MLString;
class MLRFileBlock;

/////////////////////////////////////////////////////////////////////////////

class MLQuickReadGeometry
{
public:
  int nVertices;
  Point* vertices;

  struct Edge
    {
    int v1, v2;
    Edge() { v1= v2= 0; }
    };

  int nEdges;
  Edge* edges;

  Box bbox;

  int nSVertices;
  int nPolygons;

public:
  MLQuickReadGeometry();
  virtual ~MLQuickReadGeometry();

  void alloc( int _nVertices, int _nEdges );

  static MLQuickReadGeometry* read( const MLString& filename );

  static MLQuickReadGeometry* read_1_0( MLRFileBlock& block );


};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLQuickReadGeometry_h
