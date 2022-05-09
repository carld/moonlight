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
  MLHashTable.h

  Stephane Rehel

  March 21 1998
*/

#ifndef __MLHashTable_h
#define __MLHashTable_h

#include <assert.h>
#include "MLArray.h"

#include "MLDictionary.h"
#include "MLHashTableEntry.h"

template<class KEY,class OBJ> class MLHashTableEnumerator;

/////////////////////////////////////////////////////////////////////////////

template<class KEY,class OBJ>
class MLHashTable: public MLDictionary<KEY,OBJ>
{
  friend class MLHashTableEnumerator<KEY,OBJ>;

protected:
  int capacity;
  float load_factor;

  int nElements;

  MLArray< MLHashTableEntry<KEY,OBJ>* >* table;

public:
  // hash table can store up to int(capacity/load_factor) elements
  MLHashTable( int _capacity = 20, float _load_factor = 0.5 );

  virtual ~MLHashTable()
    {
    empty_it();

    delete table;
    table= 0;
    }

  void empty_it()
    {
    for( int i= 0; i < capacity; ++i )
      {
      MLHashTableEntry<KEY,OBJ>* e= table->get(i+1);
      while( e != 0 )
        {
        MLHashTableEntry<KEY,OBJ>* next=
                                      (MLHashTableEntry<KEY,OBJ>*) e->next;

        delete e;

        e= next;
        }

      (*table)[i+1]= 0;
      }

    nElements= 0;
    }

  int size() const
    {
    return nElements;
    }

  float getLoadFactor() const
    {
    return load_factor;
    }
  int getCapacity() const
    {
    return capacity;
    }

  boolean isEmpty() const
    {
    return size() == 0;
    }

  // synchronized
  boolean contains( const OBJ* o )
    {
    for( int i= 0; i < capacity; ++i )
      {
      MLHashTableEntry<KEY,OBJ>* e= table->get(i+1);
      while( e != 0 )
        {
        if( e->element == o )
          return true;

        e= e->next;
        }
      }

    return false;
    }

protected:
  int getKeyIndex( const KEY* k ) const
    {
    int hash= k->hashCode();
    return (hash & 0x7fffffff) % capacity;
//    return ((hash<0) ? (-hash) : hash) % capacity;
    }

public:
  boolean containsKey( const KEY* k )
    {
    return get(k) != 0;
    }

  OBJ* get( const KEY* k ) const
    {
    assert( table != 0 );

    int i= getKeyIndex(k);

    MLHashTableEntry<KEY,OBJ>* e= table->get(i+1);
    while( e != 0 )
      {
      if( e->key->equals(*k) )
        return e->element;

      e= e->next;
      }

    return 0;
    }

protected:
  void rehash();

public:

  // synchronized
  boolean put( KEY* k, OBJ* o );

  // synchronized
  // return 0 if not found
  // return the key stored in the hash table if found
  KEY* remove( const KEY* k );
};

/////////////////////////////////////////////////////////////////////////////

template<class KEY,class OBJ>
MLHashTable<KEY,OBJ>::MLHashTable( int _capacity /* = 20 */,
                                   float _load_factor /* = 0.5 */ )
{
  capacity= _capacity;
  if( capacity < 1 )
    capacity= 1;

  nElements= 0;

  load_factor= _load_factor;
  if( load_factor < 0.1 )
    load_factor= 0.1;

  table= new MLArray< MLHashTableEntry<KEY,OBJ>* > ( capacity );
  table->fill(0);
}

/////////////////////////////////////////////////////////////////////////////

template<class KEY,class OBJ>
void MLHashTable<KEY,OBJ>::rehash()
{
  int new_capacity= int(float(nElements) * load_factor);
  if( capacity >= new_capacity )
    return;

  MLArray< MLHashTableEntry<KEY,OBJ>* >* new_table=
                 new MLArray< MLHashTableEntry<KEY,OBJ>* > ( new_capacity );
  new_table->fill(0);

  for( int i= 0; i < capacity; ++i )
    {
    MLHashTableEntry<KEY,OBJ>* e= table->get(i+1);
    while( e != 0 )
      {
      MLHashTableEntry<KEY,OBJ>* next= e->next;

      int hash= e->hash_code;
//      int index= ((hash<0) ? (-hash) : hash) % new_capacity;
      int index= (hash & 0x7fffffff) % new_capacity;

      e->next= (*new_table)[index+1];
      (*new_table)[index+1]= e;

      e= next;
      }

    (*table)[i+1]= 0;
    }

  delete table;
  table= new_table;

  capacity= new_capacity;
}

/////////////////////////////////////////////////////////////////////////////

template<class KEY,class OBJ>
boolean MLHashTable<KEY,OBJ>::put( KEY* k, OBJ* o )
{
  int hash= k->hashCode();
//  int i= ((hash<0) ? (-hash) : hash) % capacity;
  int i= (hash & 0x7fffffff) % capacity;

  MLHashTableEntry<KEY,OBJ>* e= table->get(i+1);
  while( e != 0 )
    {
    if( e->key->equals(*k) )
      {
      e->element= o;
      return true;
      }
    e= e->next;
    }

  e= new MLHashTableEntry<KEY,OBJ>;
  e->hash_code= hash;
  e->key= k;
  e->element= o;
  e->next= table->get(i+1);
  (*table)[i+1]= e;
  ++nElements;

  rehash();

  return false;
}

/////////////////////////////////////////////////////////////////////////////

// synchronized
// return 0 if not found
// return the key stored in the hash table if found
template<class KEY,class OBJ>
KEY* MLHashTable<KEY,OBJ>::remove( const KEY* k )
{
  int i= getKeyIndex(k);

  MLHashTableEntry<KEY,OBJ>* prev_e= 0;
  MLHashTableEntry<KEY,OBJ>* e= table->get(i+1);
  while( e != 0 )
    {
    if( e->key->equals(*k) )
      {
      KEY* _k= e->key;

      if( prev_e != 0 )
        prev_e->next= e->next;
       else
        (*table)[i+1]= e->next;

      delete e;
      --nElements;

      return _k;
      }
    prev_e= e;
    e= e->next;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLHashTable_h
