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
  SIList.h

  Simple Indirect Template List Class

  Stephane Rehel [SCFR::Kepler], Stephane.Rehel@imag.fr
  from set.h, March 1995

  Creation: July 14th, 1995
  Revisions:

*/

#ifndef __SIList_h
#define __SIList_h

#include <assert.h>

#ifndef __boolean_h
#include "boolean.h"
#endif

////////////////////////////////////////////////////////////////////////////
// SIList<T> Template Class
//  + doesn't own its elements
//    (call destroyElements to destroy all the elements and the list)
//  + destroy() destroys the list but not the elements
//  + previous element is unknown
//  + to function SIList<>::destroyElements()
//

template<class T> struct SIListElement
{
  T* element;
  SIListElement<T>* next;

  SIListElement() { element= 0; next=  0; }
  SIListElement( T* _element, SIListElement<T>* _next ):
    element(_element), next(_next)
    {}
};


template<class T> class SIList
{
private:
  SIListElement<T>* first;
  SIListElement<T>* last;
  int nElements;

public:
  SIList()
    {
    first= last= 0;
    nElements= 0;
    }
  virtual ~SIList()
    {
    destroy();
    }

  // destroy list but not the elements
  void destroy();

  int getNElements() const
    {
    return nElements;
    }
  boolean empty() const
    {
    return nElements == 0;
    }

  // get first IListElement
  SIListElement<T>* getFirstElement() const
    {
    return first;
    }
  // get last IListElement
  SIListElement<T>* getLastElement() const
    {
    return last;
    }

  // get first T element
  // assertion false if empty list
  T* getFirst() const
    {
    assert( first != 0 );
    return first->element;
    }

  // get last T element
  // assertion false if empty list
  T* getLast() const
    {
    assert( last != 0 );
    return last->element;
    }

  // add an element at the beginning
  // Cost: O(1)
  void cons( T* t );

  // add an element at the end
  // Cost: O(1)
  void append( T* t );

  // insert element t just *before* element ile
  // Cost: O(1)
//  void insert( SIListElement<T>* ile, T* t );

  // insert element t just *before* element i, 1 <= i <= nElements
  // Cost: O(nElements)
//  void insert( int i, T* t );

  // get element i with 1<=i<=nElements
  // assertion false if invalid index i (never return 0)
  // Cost: O(nElements)
  SIListElement<T>* getElement( register int i ) const;

  // get element i with 1<=i<=nElements
  // assertion false if invalid index i
  // Cost: O(nElements)
  T* get( int i ) const
    {
    return getElement(i)->element;
    }

  // get index i of element t
  // return 1 <= i <= nElements if t found
  // return 0 if t not found
  int getIndex( const T* t ) const;

  // find element t and return the IListElement
  // Return 0 if not found
  // Cost: O(nElements)
  SIListElement<T>* findElement( const T* t );

  // return true if t exists in list
  // Cost: O(nElements)
  boolean exist( const T* t )
    {
    return findElement(t) != 0;
    }

  // remove IListElement ile, but dont delete the IListElement::element
  // Cost: O(nElements)
  void remove( SIListElement<T>* ile );

  // remove first element, but dont delete the IListElement::element
  // Cost: O(1)
  void removeFirst()
    {
    assert( ! empty() );
    remove( getFirstElement() );
    }

  // remove last element, but dont delete the IListElement::element
  // Cost: O(1)
  void removeLast()
    {
    assert( ! empty() );
    remove( getLastElement() );
    }

  // remove element t, but dont delete t
  // no error if t isn't found in list
  // return true if found and destroyed
  // Cost: O(nElements)
  boolean remove( const T* t );

//  void append( IList<T>* list );
};


/////////////////////////////////////////////////////////////////////////////

template<class T>
inline
void SIList<T>::destroy()
{
  while( first != 0 )
    remove(first);

  assert( last == 0 );
  assert( nElements == 0 );
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
inline
void SIList<T>::cons( T* t )
{
  SIListElement<T>* ile= new SIListElement<T>( t, first );

  if( first == 0 )
    {
    // it's the first element
    assert( last == 0 );
    assert( nElements == 0 );
    last= ile;
    }
   else
    {
    assert( last != 0 );
    assert( nElements > 0 );
    }

  first= ile;
  ++nElements;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
inline
void SIList<T>::append( T* t )
{
  SIListElement<T>* ile= new SIListElement<T>( t, 0 );

  if( last == 0 )
    {
    // it's the first element
    assert( first == 0 );
    assert( nElements == 0 );
    first= ile;
    }
   else
    {
    assert( first != 0 );
    assert( nElements > 0 );
    last->next= ile;
    }

  last= ile;
  ++nElements;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
inline
SIListElement<T>* SIList<T>::getElement( register int i ) const
{
  assert( i >= 1 );
  assert( i <= nElements );

  register SIListElement<T>* ile= getFirstElement();
  assert( ile != 0 );

  while( --i )
    {
    ile= ile->next;
    assert( ile != 0 );
    }

  return ile;
}

/////////////////////////////////////////////////////////////////////////////

// get index i of element t
// return 1 <= i <= nElements if t found
// return 0 if t not found
template<class T>
inline
int SIList<T>::getIndex( const T* t ) const
{
  register int i= 1;
  register const SIListElement<T>* ile= first;

  while( ile != 0 )
    {
    if( ile->element == t )
      return i;
    ile= ile->next;
    ++i;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
inline
SIListElement<T>* SIList<T>::findElement( const T* t )
{
  register SIListElement<T>* ile= getFirstElement();

  while( ile != 0 )
    {
    if( ile->element == t )
      return ile;
    ile= ile->next;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
inline
void SIList<T>::remove( SIListElement<T>* ile )
{
  remove(ile->element);
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
inline
boolean SIList<T>::remove( const T* t )
{
  if( first == 0 )
    return false;

  assert( last != 0 );
  assert( nElements > 0 );

  if( first->element == t )
    {
    // we need to remove the first element
    SIListElement<T>* next= first->next;
    if( last == first ) // there was only one element
      {
      assert( nElements == 1 );
      assert( next == 0 );
      last= 0;
      }
    delete first;
    first= next;

    assert( nElements >= 1 );
    --nElements;

    return true;
    }

  SIListElement<T>* prev= first;
  register SIListElement<T>* ile= first->next;
  while( ile != 0 )
    {
    if( ile->element == t )
      {
      SIListElement<T>* next= ile->next;
      if( last == ile ) // this the last element
        {
        assert( next == 0 );
        last= prev;
        }
      delete ile;
      assert( prev != 0 );
      assert( prev->next != 0 );
      prev->next= next;

      assert( nElements > 1 );
      --nElements;
      return true;
      }
    prev= ile;
    ile= ile->next;
    }

  return false;
}

////////////////////////////////////////////////////////////////////////////
// SIListIterator<T> Class
//
// Use it like this: (list = SIList<int>* or SIList<int>)
//
//   SIListIterator<int> sli( list );
//   while( ! sli.eol() )
//     {
//     int* element= sli.next();
//     // work with element
//     }
// Constant version:
//
//   ConstSIListIterator<int> sli( list );
//   while( ! sli.eol() )
//     {
//     const int* element= sli.next();
//     // work with constant element
//     }
//
// Disgusting define ...
//
#define SILISTITERATOR( NAME ) \
template<class T> class NAME                  \
{                                             \
private:                                      \
  CONST_ SIList<T>* list;                     \
  CONST_ SIListElement<T>* cur;               \
public:                                       \
  NAME()                                      \
    {                                         \
    list= 0;                                  \
    cur= 0;                                   \
    }                                         \
  NAME( CONST_ SIList<T>* _list )             \
    {                                         \
    init( _list );                            \
    }                                         \
  NAME( CONST_ SIList<T>& _list )             \
    {                                         \
    init( &_list );                           \
    }                                         \
  ~NAME()                                     \
    {}                                        \
                                              \
  void init( CONST_ SIList<T>* _list )        \
    {                                         \
    list= _list;                              \
    reset();                                  \
    }                                         \
                                              \
  void reset()                                \
    {                                         \
    if( list == 0 )                           \
      cur= 0;                                 \
     else                                     \
      cur= list->getFirstElement();           \
    }                                         \
                                              \
  boolean eol() const                           \
    {                                         \
    return cur == 0;                          \
    }                                         \
  operator int () const                       \
    {                                         \
    return cur != 0;                          \
    }                                         \
  CONST_ SIListElement<T>* nextElement()      \
    {                                         \
    assert( cur != 0 );                       \
    register CONST_ SIListElement<T>* c= cur; \
    cur= cur->next;                           \
    return c;                                 \
    }                                         \
  CONST_ T* next()                            \
    {                                         \
    assert( cur != 0 );                       \
    register CONST_ T* e= cur->element;       \
    cur= cur->next;                           \
    return e;                                 \
    }                                         \
  CONST_ T* operator ++ (int)                 \
    {                                         \
    return next();                            \
    }                                         \
  CONST_ SIListElement<T>* getElement() const \
    {                                         \
    assert( cur != 0 );                       \
    return cur;                               \
    }                                         \
  CONST_ T* get() const                       \
    {                                         \
    assert( cur != 0 );                       \
    return cur->element;                      \
    }                                         \
};

#define CONST_
SILISTITERATOR( SIListIterator )

#undef CONST_
#define CONST_ const
SILISTITERATOR( ConstSIListIterator )

#undef CONST_

#define SIList_destroyElements(TYPE,list) \
  {                                       \
  SIListIterator<TYPE> li(list);          \
  while( ! li.eol() )                     \
    delete li.next();                     \
  (list).destroy();                       \
  }

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __SIList_h
