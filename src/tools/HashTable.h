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
   HashTable.h

   Template Hash Table

   Stephane Rehel [SCFR::Kepler] Stephane.Rehel@imag.fr
   August 95
*/


#ifndef __HashTable_h
#define __HashTable_h

#ifndef __IList_h
#include "IList.h"
#endif

// abstract class, just add:
//   HashCode getHashCode( const T* t ) const;
//   boolean equal( const T* t1, const T* t2 ) const;

typedef unsigned int HashCode;

/////////////////////////////////////////////////////////////////////////////

template<class T> class HashTableIterator;
template<class T> class ConstHashTableIterator;

template<class T> class HashTable
{
  friend class HashTableIterator<T>;
  friend class ConstHashTableIterator<T>;

protected:
  IList<T>* table;
  HashCode size;
  int nElements;

public:
  HashTable()
    {
    table= 0;
    nElements= 0;
    size= 0;
    }
  HashTable( int _size )
    {
    table= 0;
    nElements= 0;
    size= 0;
    create(_size);
    }

  virtual ~HashTable()
    {
    destroy();
    }

  void destroy();

  void create( int _size = 256 );

  HashCode getSize() const
    {
    assert( size != 0 );
    return size;
    }

  int getNElements() const
    {
    return nElements;
    }

  // return 0 if not present
  T* find( const T* t, HashCode hc );

  // return 0 if not present
  T* find( const T* t )
    {
    return find( t, getHashCode(t) );
    }

  // doesn't check if already present in hash table
  void addNew( T* t, HashCode hc );

  // doesn't check if already present in hash table
  void addNew( T* t )
    {
    addNew( t, getHashCode(t) );
    }

  // return 0 if not present and added
  // return element if already in hash table
  T* addUnique( T* t, HashCode hc )
    {
    T* t0= find( t, hc );
    if( t0 != 0 )
      return t0;
    addNew( t, hc );
    return 0;
    }

  // return 0 if not present and added
  // return element if already in hash table
  T* addUnique( T* t )
    {
    return addUnique( t, getHashCode(t) );
    }

  // no error if not found
  // doesn't delete t
  void remove( const T* t, HashCode hc );

  // no error if not found
  // doesn't delete t
  void remove( const T* t )
    {
    remove( t, getHashCode(t) );
    }

  virtual HashCode getHashCode( const T* t ) const =0;
  virtual boolean equal( const T* t1, const T* t2 ) const =0;
};


/////////////////////////////////////////////////////////////////////////////

template<class T>
inline
void HashTable<T>::destroy()
{
  if( table == 0 )
    return;

  for( HashCode i= 0; i< size; ++i )
    table[i].destroy();

  delete [] table;
  table= 0;

  nElements= 0;
  size= 0;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
inline
void HashTable<T>::create( int _size /* = 256 */ )
{
  destroy();

  assert( _size > 0 );
  size= HashCode(_size);

  table= new IList<T> [ size ];
  assert( table != 0 );

  nElements= 0;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
inline
void HashTable<T>::addNew( T* t, HashCode hc )
{
  assert( t != 0 );
  assert( table != 0 );
  assert( hc < size );

  table[hc].cons(t);

  ++nElements;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
inline
T* HashTable<T>::find( const T* t, HashCode hc )
{
  assert( t != 0 );
  assert( table != 0 );
  assert( hc < size );

  IListIterator<T> li( table[hc] );
  while( ! li.eol() )
    {
    T* t0= li.next();
    if( equal( t, t0 ) )
      return t0;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
inline
void HashTable<T>::remove( const T* t, HashCode hc )
{
  assert( t != 0 );
  assert( table != 0 );
  assert( hc < size );

  IListIterator<T> li( table[hc] );
  while( ! li.eol() )
    {
    IListElement<T>* element= li.nextElement();
    if( equal( t, element->element ) )
      {
      table[hc].remove(element);
      break;
      }
    }

  --nElements;
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////HashTableIterator/
//
//  Disgusting define ...
//
#define HASHTABLEITERATOR( NAME, ILISTITERATOR )  \
template<class T> class NAME                             \
{                                                        \
private:                                                 \
  HashCode curHashCode;                                  \
  ILISTITERATOR<T> li;                                   \
  CONST_ HashTable<T>* ht;                               \
public:                                                  \
  NAME()                                                 \
    {                                                    \
    ht= 0;                                               \
    }                                                    \
  NAME( CONST_ HashTable<T>* _ht )                       \
    {                                                    \
    reset(_ht);                                          \
    }                                                    \
  NAME( CONST_ HashTable<T>& _ht )                       \
    {                                                    \
    reset(&_ht);                                         \
    }                                                    \
  virtual ~NAME()                                        \
    {}                                                   \
                                                         \
  void reset( CONST_ HashTable<T>* _ht )                 \
    {                                                    \
    ht= _ht;                                             \
    reset();                                             \
    }                                                    \
                                                         \
  void reset();                                          \
                                                         \
  boolean eol() const                                      \
    {                                                    \
    if( ht == 0 ) return true;                          \
    return curHashCode >= ht->getSize();                 \
    }                                                    \
  CONST_ IListElement<T>* nextElement();                 \
  CONST_ T* next()                                       \
    {                                                    \
    return nextElement()->element;                       \
    }                                                    \
};                                                       \
                                                         \
                                                         \
template<class T>                                        \
inline                                                   \
void NAME<T>::reset()                                    \
{                                                        \
  if( ht == 0 )                                          \
    return;                                              \
                                                         \
  assert( ht->table != 0 );                              \
  curHashCode= 0;                                        \
                                                         \
  while( curHashCode < ht->getSize() )                   \
    {                                                    \
    if( ! ht->table[curHashCode].empty() )               \
      {                                                  \
      li.init( ht->table + curHashCode );                \
      break;                                             \
      }                                                  \
    ++curHashCode;                                       \
    }                                                    \
}                                                        \
                                                         \
                                                         \
template<class T>                                        \
inline                                                   \
CONST_ IListElement<T>* NAME<T>::nextElement()           \
{                                                        \
  assert( ht != 0 );                                     \
  assert( ! eol() );                                     \
                                                         \
  CONST_ IListElement<T>* ile= li.nextElement();         \
                                                         \
  if( li.eol() )                                         \
    {                                                    \
    ++curHashCode;                                       \
    while( curHashCode < ht->getSize() )                 \
      {                                                  \
      if( ! ht->table[curHashCode].empty() )             \
        {                                                \
        li.init( ht->table + curHashCode );              \
        break;                                           \
        }                                                \
      ++curHashCode;                                     \
      }                                                  \
    }                                                    \
                                                         \
  return ile;                                            \
}


#define CONST_
HASHTABLEITERATOR( HashTableIterator, IListIterator )

#undef CONST_
#define CONST_ const
HASHTABLEITERATOR( ConstHashTableIterator, ConstIListIterator )

#undef CONST_

#undef HASHTABLEITERATOR

#endif // ifndef __HashTable_h


