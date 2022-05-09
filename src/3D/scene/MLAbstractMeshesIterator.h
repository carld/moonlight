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
  MLAbstractMeshesIterator.h

  Stephane Rehel

  April 26 1997
*/

#ifndef __MLAbstractMeshesIterator_h
#define __MLAbstractMeshesIterator_h

#ifndef __IArray_h
#include "tools/IArray.h"
#endif

#ifndef __MLObject_h
#include "MLObject.h"
#endif

class MLScene;
class MLAbstractMesh;

/////////////////////////////////////////////////////////////////////////////

class MLAbstractMeshesIterator
{
protected:
  MLScene* scene;
  IArray* objects;
  int nObjects;
  int i, j;

public:
  MLAbstractMeshesIterator( MLScene* _scene );

  virtual ~MLAbstractMeshesIterator()
    {}

  void reset();

  boolean eol() const
    {
    return j >= nObjects;
    }

  MLAbstractMesh* next()
    {
    if( j >= nObjects ) return 0;
    MLAbstractMesh* o= (MLAbstractMesh*) (*objects)[i];
    ++i;
    ++j;
    follow();
    return o;
    }

  virtual boolean follow()
    {
    if( j >= nObjects ) return false;

    register MLAbstractMesh* o= 0;
    for(;;)
      {
      o= (MLAbstractMesh*) (*objects)[i];
      if( o != 0 )
        break;
      ++i;
      }

    return true;
    }

  MLAbstractMesh* operator ++ (int)
    {
    return next();
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLAbstractMeshesIterator_h
