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
  MLHashTableEnumerator.h

  Stephane Rehel
  March 21 1998
*/

#ifndef __MLHashTableEnumerator_h
#define __MLHashTableEnumerator_h

#include "MLEnumeration.h"
#include "MLHashTable.h"
#include "MLHashTableEntry.h"

/////////////////////////////////////////////////////////////////////////////

template<class KEY,class OBJ>
class MLHashTableEnumerator: public MLEnumeration<OBJ>
{
protected:
  MLHashTable<KEY,OBJ>* hashtable;
  int index;
  MLHashTableEntry<KEY,OBJ>* entry;

public:
  MLHashTableEnumerator( MLHashTable<KEY,OBJ>& _hashtable )
    {
    hashtable= &_hashtable;
    index= hashtable->capacity;
    entry= 0;
    }

  MLHashTableEnumerator( MLHashTable<KEY,OBJ>* _hashtable )
    {
    hashtable= _hashtable;
    index= hashtable->capacity;
    entry= 0;
    }

  virtual ~MLHashTableEnumerator()
    {}

  void reset()
    {
    index= hashtable->capacity;
    entry= 0;
    }

  boolean hasMoreElements()
    {
    if( entry != 0 )
      return true;

    while( index-- > 0 )
      {
      entry= hashtable->table->get(index+1);
      if( entry != 0 )
        return true;
      }

    return false;
    }

protected:
  MLHashTableEntry<KEY,OBJ>* next()
    {
    if( entry == 0 )
      {
      while( index-- > 0 )
        {
        entry= (*hashtable->table)[index+1];
        if( entry != 0 )
          break;
        }
      }

    if( entry == 0 )
      return 0;

    MLHashTableEntry<KEY,OBJ>* e= entry;

    entry= entry->next;

    return e;
    }

public:
  OBJ* nextElement()
    {
    MLHashTableEntry<KEY,OBJ>* e= next();
    if( e == 0 )
      return 0;

    return e->element;
    }

  KEY* nextKey()
    {
    MLHashTableEntry<KEY,OBJ>* e= next();
    if( e == 0 )
      return 0;

    return e->key;
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLHashTableEnumerator_h
