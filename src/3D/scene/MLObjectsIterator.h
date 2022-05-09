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
  MLObjectsIterator.h

  Stephane Rehel

  January 12 1997
*/

#ifndef __MLObjectsIterator_h
#define __MLObjectsIterator_h

#ifndef __IArray_h
#include "tools/IArray.h"
#endif

#ifndef __MLObject_h
#include "MLObject.h"
#endif

class MLScene;
class MLObject;

/////////////////////////////////////////////////////////////////////////////

class MLObjectsIterator
{
protected:
  MLScene* scene;
  IArray* objects;
  int maxObjects;
  int i;

public:
  MLObjectsIterator( MLScene* _scene );

  virtual ~MLObjectsIterator() {}

  void reset();

  boolean eol() const
    {
    return i > maxObjects;
    }

  MLObject* next()
    {
    if( i > maxObjects )
      return 0;

    MLObject* o= (MLObject*) (*objects)[i];
    ++i;
    follow();
    return o;
    }

  virtual MLObject* follow()
    {
    register MLObject* o= 0;
    for(;;)
      {
      if( i > maxObjects )
        return 0;

      o= (MLObject*) (*objects)[i];
      if( o != 0 )
        {
        if( ! o->detached.flag )
          break;
        }
      ++i;
      }

    return o;
    }

  MLObject* operator ++ (int)
    {
    return next();
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLObjectsIterator_h
