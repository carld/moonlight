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
  MLTagVertexArray.h

  Stephane Rehel

  October 28 1997
*/

#ifndef __MLTagVertexArray_h
#define __MLTagVertexArray_h

#include <assert.h>

#ifndef __IntegerArray_h
#include "tools/IntegerArray.h"
#endif

class MLVertex;
class MLMesh;
class MLNURBS_ControlPoint;

/////////////////////////////////////////////////////////////////////////////

class MLTagVertexArray: public IntegerArray
{
protected:
  MLScene* scene;

public:
  MLTagVertexArray( MLScene* _scene );

  virtual ~MLTagVertexArray()
    {}

  MLTagVertexArray& operator = ( const MLTagVertexArray& tva );

  // i >= 0
  MLMesh* getMesh( int i );

  // i >= 0
  MLVertex* getVertex( int i );

  int getNVertices() const
    {
    assert( (getSize() % 2) == 0 );
    return getSize() / 2;
    }

  // In case of NURBS control points

  // i >= 0
  boolean isControlPoint( int i );
  MLNURBS_ControlPoint* getControlPoint( int i );
  int getNControlPoints() const
    {
    return getNVertices();
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLTagVertexArray_h
