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
  DList.h

  Direct Template List Class

  Stephane Rehel [SCFR::Kepler], Stephane.Rehel@imag.fr

  Creation: August 23rd, 1995
  Revisions:

*/

#ifndef __DList_h
#define __DList_h

#ifndef __boolean_H
#include "boolean.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// A DListElement<T,L> template is a list element. Element of type T
// inherits of DListElement<T,L>, and can be part of the list L
// L is a dummy type: if class 'foo' is to be element of two different
// lists in the same time, then you write:
//
//   class PINK;
//   class ORANGE; // never really defined
//
//   class Foo: public DListElement<Foo,PINK>,
//              public DListElement<Foo,ORANGE>
//    {
//    // ...
//    };
//
//   DList<Foo,PINK> pinkList;
//   DList<Foo,ORANGE> orangeList;
//   Foo* foo= // ...
//
//   pinkList.cons(foo); // add foo to the pink list
//   orangeList.append(foo); // append foo to the orange list
//
//   pinkList.remove(foo); // remove foo from the pinkList
//
//
// DList<T,L> elements have to inherit DListElement<T,L> to become part
// of any DList<T,L>
//
/////////////////////////////////////////////////////////////////////////////

template<class T,class L> class DList;
template<class T,class L> class DListIterator;
template<class T,class L> class ConstDListIterator;


template<class T,class L> class DListElement
{
  friend class DList<T,L>;
  friend class DListIterator<T,L>;
  friend class ConstDListIterator<T,L>;

private:
  DList<T,L>* list;
  DListElement<T,L>* prev;
  DListElement<T,L>* next;

protected:

  DListElement()
    {
    list= 0;
    prev= next= 0;
    }

  virtual ~DListElement()
    {
    if( list != 0 )
      list->remove( (T*) this );
    }

  boolean lonely() const
    {
    return list == 0;
    }

  DList<T,L>* getDList() const
    {
    assert( list != 0 );
    return list;
    }

  void* getOwner() const
    {
    return getDList()->getOwner();
    }
};

/////////////////////////////////////////////////////////////////////////////

template<class T,class L> class DList
{
private:
  DListElement<T,L>* first;
  DListElement<T,L>* last;
  int nElements;
  void* owner;

public:
  DList( void* _owner = 0 )
    {
    first= last= 0;
    nElements= 0;
    owner= _owner;
    }

  // destructor detach each element but dont delete them
  virtual ~DList();

  void* getOwner() const
    {
    return owner;
    }

  // delete each element and empty the list
  void destroyElements();

  int getNElements() const
    {
    return nElements;
    }
  boolean empty() const
    {
    return nElements == 0;
    }

  // get first DListElement<T,L>
  DListElement<T,L>* getFirstElement() const
    {
    return first;
    }
  // get last DListElement<T,L>
  DListElement<T,L>* getLastElement() const
    {
    return last;
    }

  // get first T element
  // assertion false if empty list
  T* getFirst() const
    {
    assert( first != 0 );
    return (T*) first;
    }

  // get last T element
  // assertion false if empty list
  T* getLast() const
    {
    assert( last != 0 );
    return (T*) last;
    }

  // add an element at the beginning
  // Cost: O(1)
  void cons( T* t );

  // add an element at the end
  // Cost: O(1)
  void append( T* t );

  // insert element t just *before* element i, 1 <= i <= nElements
  // Cost: O(1)
  void insert( int i, T* t );

  // get element i with 1<=i<=nElements
  // assertion fails if invalid index i (never return 0)
  // Cost: O(nElements)
  DListElement<T,L>* getElement( register int i ) const;

  // get element i with 1<=i<=nElements
  // assertion fails if invalid index i
  // Cost: O(nElements)
  T* get( int i ) const
    {
    return (T*) getElement(i);
    }

  // return true if t is in list
  // Return false if not found; no error if lonely element
  // Cost: O(1)
  boolean exist( const T* t ) const;

  // remove element t, but dont delete it
  // no error if lonely element
  // return true if found and removed
  // Assertion failed if element doesn't delong to this list!
  // Cost: O(1)
  boolean remove( T* t );

  // remove first element, but dont delete it
  // Assertion failed if empty list
  // Cost: O(1)
  boolean removeFirst()
    {
    assert( first != 0 );
    return remove( (T*) first );
    }

  // remove last element, but dont delete it
  // Assertion failed if empty list
  // Cost: O(1)
  boolean removeLast()
    {
    assert( last != 0 );
    return remove( (T*) last );
    }
};


/////////////////////////////////////////////////////////////////////////////

// destructor detach each element but dont delete them
template<class T,class L>
inline
DList<T,L>::~DList()
{
  while( first != 0 )
    {
    DListElement<T,L>* next= first->next;
    first->prev= 0;
    first->next= 0;
    first= next;
    }
  last= 0;
  nElements= 0;
}

/////////////////////////////////////////////////////////////////////////////

// delete each element and empty the list
template<class T,class L>
inline
void DList<T,L>::destroyElements()
{
  while( first != 0 )
    delete first;

  assert( last == 0 );
  assert( nElements == 0 );
}

/////////////////////////////////////////////////////////////////////////////

template<class T,class L>
inline
void DList<T,L>::cons( T* t )
{
  assert( t != 0 );

  DListElement<T,L>* e= (DListElement<T,L>*) t;
  assert( e->lonely() );
  assert( e->prev == 0 );
  assert( e->next == 0 );

  e->list= this;
  if( first == 0 )
    {
    // it's the first element
    assert( last == 0 );
    assert( nElements == 0 );
    last= e;
    }
   else
    {
    assert( last != 0 );
    assert( nElements != 0 );
    assert( first->prev == 0 );
    e->next= first;
    first->prev= e;
    }

  first= e;
  ++nElements;
}

/////////////////////////////////////////////////////////////////////////////

template<class T,class L>
inline
void DList<T,L>::append( T* t )
{
  assert( t != 0 );

  DListElement<T,L>* e= (DListElement<T,L>*) t;
  assert( e->lonely() );
  assert( e->prev == 0 );
  assert( e->next == 0 );

  e->list= this;
  if( last == 0 )
    {
    // it's the first element
    assert( first == 0 );
    assert( nElements == 0 );
    first= e;
    }
   else
    {
    assert( first != 0 );
    assert( nElements != 0 );
    assert( last->next == 0 );
    e->prev= last;
    last->next= e;
    }

  last= e;
  ++nElements;
}

/////////////////////////////////////////////////////////////////////////////

// insert element t just *before* element i, 1 <= i <= nElements
// Cost: O(nElements)
template<class T,class L>
inline
void DList<T,L>::insert( int i, T* t )
{
  assert( t != 0 );
  assert( i >= 1 );
  assert( i <= nElements );

  if( i == 1 ) // first element changes
    {
    cons(t);
    return;
    }

  // last element never changes

  DListElement<T,L>* dle= getElement(i);
  DListElement<T,L>* e= (DListElement<T,L>*) t;
  assert( e->lonely() );
  assert( e->prev == 0 );
  assert( e->next == 0 );

  e->list= this;
  e->prev= dle->prev;
  e->next= dle;

  dle->prev= e;
  assert( e->prev != 0 );
  e->prev->next= e;

  ++nElements;
}


/////////////////////////////////////////////////////////////////////////////

template<class T,class L>
inline
DListElement<T,L>* DList<T,L>::getElement( register int i ) const
{
  assert( i >= 1 );
  assert( i <= nElements );

  register DListElement<T,L>* e= getFirstElement();
  assert( e != 0 );

  while( --i )
    {
    e= e->next;
    assert( e != 0 );
    }

  return e;
}

/////////////////////////////////////////////////////////////////////////////

template<class T,class L>
inline
boolean DList<T,L>::exist( const T* t ) const
{
  assert( t != 0 );

  DListElement<T,L>* e= (DListElement<T,L>*) t;
  if( e->lonely() )
    return false;

  register DListElement<T,L>* dle= getFirstElement();

  while( dle != 0 && dle != e )
    dle= dle->next;

  return dle != 0;
}

/////////////////////////////////////////////////////////////////////////////

template<class T,class L>
inline
boolean DList<T,L>::remove( T* t )
{
  assert( t != 0 );

  DListElement<T,L>* e= (DListElement<T,L>*) t;
  if( e->lonely() )
    return false;

  assert( e->list == this );

  if( e == first )
    {
    // this is the first element
    assert( first->prev == 0 );
    first= first->next;
    if( first != 0 ) // list is not empty?
      first->prev= 0;
     else
      last= 0; // empty list
    }
   else
    {
    assert( e->prev != 0 || e->next != 0 );
    if( e == last )
      {
      // this is the last element
      assert( last->next == 0 );
      last= last->prev;
      // there was more than 1 element in the list, since here: e!=first
      assert( last != 0 );
      last->next= 0;
      }
     else
      {
      // middle element
      e->next->prev= e->prev;
      e->prev->next= e->next;
      }
    }

  e->prev= e->next= 0;
  e->list= 0;
  --nElements;

  return true;
}


////////////////////////////////////////////////////////////////////////////
// DListIterator<T> class
//
// Use it like this: (list may be DList<int>* or DList<int>)
//
//   DListIterator<int,L> li(list);
//   while( ! li.eol() )
//     {
//     int* element= li.next();
//     // work with element
//     }
// Constant version:
//
//   ConstDListIterator<int,L> li(list);
//   while( ! li.eol() )
//     {
//     const int* element= li.next();
//     // work with constant element
//     }
//
// And now, for all of you children, here are some disgusting defines...
//
#define DLISTITERATOR( NAME ) \
template<class T,class L> class NAME \
{                                    \
private:                             \
  CONST_ DList<T,L>* list;           \
  CONST_ DListElement<T,L>* cur;     \
public:                              \
  NAME()                             \
    {                                \
    list= 0;                         \
    cur= 0;                          \
    }                                \
  NAME( CONST_ DList<T,L>* _list )   \
    {                                \
    init(_list);                     \
    }                                \
  NAME( CONST_ DList<T,L>& _list )   \
    {                                \
    init(&_list);                    \
    }                                \
  ~NAME()                            \
    {}                               \
                                     \
  void init( CONST_ DList<T,L>* _list ) \
    {                                \
    list= _list;                     \
    reset();                         \
    }                                \
                                     \
  void reset()                       \
    {                                \
    if( list == 0 )                  \
      cur= 0;                        \
     else                            \
      cur= list->getFirstElement();  \
    }                                \
                                     \
  boolean eol() const                  \
    {                                \
    return cur == 0;                 \
    }                                \
  CONST_ DListElement<T,L>* nextElement() \
    {                                \
    assert( cur != 0 );              \
    register CONST_ DListElement<T,L>* c= cur; \
    cur= cur->next;                  \
    return c;                        \
    }                                \
  CONST_ T* next()                   \
    {                                \
    return (T*) nextElement();       \
    }                                \
};

#define CONST_
DLISTITERATOR( DListIterator )

#undef CONST_
#define CONST_ const
DLISTITERATOR( ConstDListIterator )

#undef CONST_

#undef DLISTITERATOR

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __DList_h

