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
   CurveRep.h

   Stephane Rehel

   January 21 1998
*/

#ifndef __CurveRep_h
#define __CurveRep_h

#ifndef __Box_h
#include "tools/Box.h"
#endif

class MLCurveVertex;

/////////////////////////////////////////////////////////////////////////////

class CurveRep
{
public:
  int nVertices;
  MLCurveVertex* vertices; // of MLCurveVertex

  boolean closed;

public:
  CurveRep();

  virtual ~CurveRep();

  int getSizeOf() const;

  CurveRep* duplicate();

  void clear();

  void alloc( int _nVertices );

  int getParameterIndex( double t ) const;
  Point interpolate( double parameter ) const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __CurveRep_h
