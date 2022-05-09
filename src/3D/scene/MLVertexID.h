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
  MLVertexID.h

  Stephane Rehel

  October 28 1997
*/

#ifndef __MLVertexID_h
#define __MLVertexID_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

class MLScene;
class MLMesh;
class MLVertex;

/////////////////////////////////////////////////////////////////////////////

class MLVertexID
{
public:
  int object;
  int vertex;

public:
  MLVertexID()
    {
    object= vertex= -1;
    }

  MLVertexID( int _object, int _vertex )
    {
    object= _object;
    vertex= _vertex;
    }

  MLVertexID( const MLVertexID& vid )
    {
    operator = ( vid );
    }

  MLVertexID& operator = ( const MLVertexID& vid )
    {
    object= vid.object;
    vertex= vid.vertex;

    return *this;
    }

  boolean operator == ( const MLVertexID& vid ) const
    {
    return vertex == vid.vertex && object == vid.object;
    }
  boolean operator != ( const MLVertexID& vid ) const
    {
    return ! operator == ( vid );
    }

  MLMesh* getMesh( MLScene* scene );
  MLVertex* getVertex( MLScene* scene );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLVertexID_h
