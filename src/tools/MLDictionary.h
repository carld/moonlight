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
  MLDictionary.h

  Stephane Rehel
  March 21 1998
*/

#ifndef __MLDictionary_h
#define __MLDictionary_h

#include "boolean.h"

/////////////////////////////////////////////////////////////////////////////

template<class KEY,class OBJ>
class MLDictionary
{
public:
  MLDictionary()
    {}

  virtual ~MLDictionary()
    {}

  virtual int size() const = 0;

  boolean isEmpty() const
    {
    return size() == 0;
    }

//  virtual MLEnumeration<KEY>* keys() = 0;
//  virtual MLEnumeration<OBJ>* elements() = 0;

  virtual OBJ* get( const KEY* k ) const = 0;

  // return true if found
  virtual boolean put( KEY* k, OBJ* o ) = 0;

  // return 0 if not found
  // return the key stored in the hash table if found
  virtual KEY* remove( const KEY* k ) = 0;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLDictionary_h
