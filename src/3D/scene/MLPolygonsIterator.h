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
  MLPolygonsIterator.h

  Stephane Rehel

  August 12 1997
*/

#ifndef __MLPolygonsIterator_h
#define __MLPolygonsIterator_h

#ifndef __MLPolygon_h
#include "tools/3D/MLPolygon.h"
#endif

#ifndef __MLPolygonArray_h
#include "MLPolygonArray.h"
#endif

class MLMesh;
class MeshRep;

/////////////////////////////////////////////////////////////////////////////

class MLPolygonsIterator
{
protected:
  int index;
  int max_index;
  MLPolygonArray* polygons;

public:
  MLPolygonsIterator( MLMesh* mesh );
  MLPolygonsIterator( MeshRep* meshrep );
  MLPolygonsIterator( MLPolygonArray& _polygons );

  virtual ~MLPolygonsIterator()
    {}

  void reset();

  boolean eol() const
    {
    return index > max_index;
    }

  MLPolygon* next()
    {
    if( index > max_index )
      return 0;

    register MLPolygon* p= &((*polygons)[index]);

    ++index;
    follow();

    return p;
    }

  MLPolygon* operator ++ (int)
    {
    return next();
    }

protected:
  void follow()
    {
    while( index <= max_index )
      {
      register MLPolygon* p= &((*polygons)[index]);

      if( p->allocated() )
        break;

      ++index;
      }
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLPolygonsIterator_h
