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
  MLVerticesIterator.h

  Stephane Rehel

  March 13 1998
*/

#ifndef __MLVerticesIterator_h
#define __MLVerticesIterator_h

#ifndef __MLVertex_h
#include "tools/3D/MLVertex.h"
#endif

#ifndef __MLVertexArray_h
#include "MLVertexArray.h"
#endif

class MLMesh;
class MeshRep;

/////////////////////////////////////////////////////////////////////////////

class MLVerticesIterator
{
protected:
  int index;
  int max_index;
  MLVertexArray* vertices;

public:
  MLVerticesIterator( MLMesh* mesh );
  MLVerticesIterator( MeshRep* meshrep );
  MLVerticesIterator( MLVertexArray& _vertices );

  virtual ~MLVerticesIterator()
    {}

  void reset();

  boolean eol() const
    {
    return index > max_index;
    }

  MLVertex* next()
    {
    if( index > max_index )
      return 0;

    register MLVertex* v= &((*vertices)[index]);

    ++index;
    follow();

    return v;
    }

  MLVertex* operator ++ (int)
    {
    return next();
    }

protected:
  void follow()
    {
    while( index <= max_index )
      {
      register MLVertex* v= &((*vertices)[index]);

      if( v->allocated() )
        break;

      ++index;
      }
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLVerticesIterator_h
