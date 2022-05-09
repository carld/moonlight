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
  MLChildrenIterator.h

  Stephane Rehel

  December 8 1996
*/

#ifndef __MLChildrenIterator_h
#define __MLChildrenIterator_h

#ifndef __MLObject_h
#include "MLObject.h"
#endif

class MLScene;

/////////////////////////////////////////////////////////////////////////////

class MLChildrenIterator
{
protected:
  MLObject* object;
  int n;
  int n_max;
  MLObject** ptr;

public:
  MLChildrenIterator( MLObject* _object )
    {
    object= _object;
    reset();
    }
  MLChildrenIterator( MLScene* scene );

  ~MLChildrenIterator() {}

  void reset()
    {
    n= 0;
    if( object == 0 )
      {
      ptr= 0;
      n_max= 0;
      }
     else
      {
      ptr= object->children + 1;
      n_max= object->nChildren;
      }
    follow();
    }

  boolean eol() const
    {
    return n >= n_max;
    }
  MLObject* next()
    {
    if( n >= n_max ) return 0;
    MLObject* o= *ptr;
    ++n; ++ptr;
    follow();
    return o;
    }
  void follow()
    {
    if( n >= n_max ) return;
    while( *ptr == 0 ) ++ptr;
    }
  MLObject* operator ++ (int)
    {
    return next();
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLChildrenIterator_h
