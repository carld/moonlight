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
  IList.h

  Indirect Template List Class

  Stephane Rehel [SCFR::Kepler], Stephane.Rehel@imag.fr
  from set.h, March 1995

  Creation: July 14th, 1995
  Revisions:

*/

#ifndef __IList_h
#define __IList_h

#include <assert.h>

#ifndef __boolean_h
#include "boolean.h"
#endif

////////////////////////////////////////////////////////////////////////////
// IList<T> Template Class
//  + doesn't own its elements
//    (call destroyElements to destroy all the elements and the list)
//  + destroy() destroys the list but not the elements
//

template<class T> struct IListElement
{
  T* element;
  IListElement<T>* prev;
  IListElement<T>* next;

  IListElement() { element= 0; prev= next= 0; }
  IListElement( T* _element,
                IListElement<T>* _prev, IListElement<T>* _next ):
    element(_element), prev(_prev), next(_next)
    {}
};


template<class T> class IList
{
private:
  IListElement<T>* first;
  IListElement<T>* last;
  int nElements;

public:
  IList()
    {
    first= last= 0;
    nElements= 0;
    }
  virtual ~IList()
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
  IListElement<T>* getFirstElement() const
    {
    return first;
    }
  // get last IListElement
  IListElement<T>* getLastElement() const
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
  void insert( IListElement<T>* ile, T* t );

  // insert element t just *before* element i, 1 <= i <= nElements
  // Cost: O(nElements)
  void insert( int i, T* t );

  // get element i with 1<=i<=nElements
  // assertion false if invalid index i (never return 0)
  // Cost: O(nElements)
  IListElement<T>* getElement( register int i ) const;

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
  IListElement<T>* findElement( const T* t );

  // return true if t exists in list
  // Cost: O(nElements)
  boolean exist( const T* t )
    {
    return findElement(t) != 0;
    }

  // remove IListElement ile, but dont delete the IListElement::element
  // Cost: O(1)
  void remove( IListElement<T>* ile );

  // remove first element, but dont delete the IListElement::element
  // Cost: O(1)
  void removeFirst()
    {
    remove( getFirstElement() );
    }

  // remove last element, but dont delete the IListElement::element
  // Cost: O(1)
  void removeLast()
    {
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
void IList<T>::destroy()
{
  while( first != 0 )
    remove( first );

  assert( last == 0 );
  assert( nElements == 0 );
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
inline
void IList<T>::cons( T* t )
{
  IListElement<T>* ile= new IListElement<T>( t, 0, first );

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
    assert( nElements != 0 );
    first->prev= ile;
    }

  first= ile;
  ++nElements;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
inline
void IList<T>::append( T* t )
{
  IListElement<T>* ile= new IListElement<T>( t, last, 0 );

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
    assert( nElements != 0 );
    last->next= ile;
    }

  last= ile;
  ++nElements;
}

/////////////////////////////////////////////////////////////////////////////

// insert element t just *before* element ile
// Cost: O(1)
template<class T>
inline
void IList<T>::insert( IListElement<T>* ile, T* t )
{
  assert( ile != 0 );

  if( ile == first ) // first element changes
    {
    cons(t);
    return;
    }

  // last element never changes
  IListElement<T>* newile= new IListElement<T>( t, ile->prev, ile );
  ile->prev= newile;

  assert( newile->prev != 0 );
  newile->prev->next= newile;
  ++nElements;
}


/////////////////////////////////////////////////////////////////////////////

// insert element t just *before* element i, 1 <= i <= nElements
// Cost: O(nElements)
template<class T>
inline
void IList<T>::insert( int i, T* t )
{
  assert( i >= 1 );
  assert( i <= nElements );

  if( i == 1 ) // first element changes
    {
    cons(t);
    return;
    }

  insert( getElement(i), t );
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
inline
IListElement<T>* IList<T>::getElement( register int i ) const
{
  assert( i >= 1 );
  assert( i <= nElements );

  register IListElement<T>* ile= getFirstElement();
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
int IList<T>::getIndex( const T* t ) const
{
  register int i= 1;
  register const IListElement<T>* ile= first;

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
IListElement<T>* IList<T>::findElement( const T* t )
{
  register IListElement<T>* ile= getFirstElement();

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
void IList<T>::remove( IListElement<T>* ile )
{
  assert( nElements > 0 );
  assert( ile != 0 );

  if( ile == first )
    {
    // first element
    assert( first->prev == 0 );
    first= first->next;
    if( first != 0 ) // list is not empty?
      first->prev= 0;
     else
      last= 0; // empty list
    }
   else
    if( ile == last )
      {
      // last element
      assert( last->next == 0 );
      last= last->prev;
      assert( last != 0 );
      last->next= 0;
      }
     else
      {
      // middle element
      ile->next->prev= ile->prev;
      ile->prev->next= ile->next;
      }

  --nElements;
  delete ile;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
inline
boolean IList<T>::remove( const T* t )
{
  IListElement<T>* ile= findElement(t);
  if( ile != 0 )
    remove(ile);
  return ile != 0;
}

/////////////////////////////////////////////////////////////////////////////


/*
// gob 's' (ajout en fin de liste) et detruit 's'
// introduction ici du concept algorithmique de 'gobage' qui
// consiste a faire 'gober' un objet par un autre objet
// 10:20 p.m. March 6th, 1995. /SCFR::Kepler
// (concept fondamental en algorithmique)
template<class T>
inline
void Set<T>::gob( Set<T>* s )
{
  if( s->nElements == 0 )
    {
    assert( s->first == 0 );
    assert( s->last == 0 );
    delete s;
    return;
    }

  if( nElements == 0 )
    {
    assert( first == 0 );
    assert( last == 0 );
    assert( nElements == 0 );

    first= s->first;
    last= s->last;
    nElements= s->nElements;
    }
   else
    {
    last->next= s->first;
    s->first->prev= last;
    last= s->last;
    nElements+= s->nElements;
    }

  s->first= s->last= 0;
  s->nElements= 0;
  delete s;
}
*/


/*
// static
template<class T>
inline
Set<T>* Set<T>::duplicate( const Set<T>* set0 )
{
  Set<T>* set= new Set<T>;

  const SetElement<T>* se0= set0->first;
  while( se0 != 0 )
    {
    set->add( T::duplicate( se0->element ) );
    se0= se0->next;
    }

  return set;
}
*/


////////////////////////////////////////////////////////////////////////////
// IListIterator<T> Class
//
// Use it like this: (list = IList<int>* or IList<int>)
//
//   IListIterator<int> li( list );
//   while( ! li.eol() )
//     {
//     int* element= li.next();
//     // work with element
//     }
// Constant version:
//
//   ConstIListIterator<int> li( list );
//   while( ! li.eol() )
//     {
//     const int* element= li.next();
//     // work with constant element
//     }
//
// Disgusting define ...
//
#define ILISTITERATOR( NAME ) \
template<class T> class NAME         \
{                                    \
private:                             \
  CONST_ IList<T>* list;             \
  CONST_ IListElement<T>* cur;       \
public:                              \
  NAME()                             \
    {                                \
    list= 0;                         \
    cur= 0;                          \
    }                                \
  NAME( CONST_ IList<T>* _list )     \
    {                                \
    init( _list );                   \
    }                                \
  NAME( CONST_ IList<T>& _list )     \
    {                                \
    init( &_list );                  \
    }                                \
  ~NAME()                            \
    {}                               \
                                     \
  void init( CONST_ IList<T>* _list ) \
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
  void goLast()                      \
    {                                \
    if( list == 0 )                  \
      cur= 0;                        \
     else                            \
      cur= list->getLastElement();   \
    }                                \
                                     \
  boolean eol() const                  \
    {                                \
    return cur == 0;                 \
    }                                \
  operator int () const              \
    {                                \
    return cur != 0;                 \
    }                                \
  CONST_ IListElement<T>* nextElement() \
    {                                \
    assert( cur != 0 );              \
    register CONST_ IListElement<T>* c= cur; \
    cur= cur->next;                  \
    return c;                        \
    }                                \
  CONST_ T* next()                   \
    {                                \
    assert( cur != 0 );              \
    register CONST_ T* e= cur->element; \
    cur= cur->next;                  \
    return e;                        \
    }                                \
  CONST_ T* operator ++ (int)        \
    {                                \
    return next();                   \
    }                                \
  CONST_ T* operator -- (int)        \
    {                                \
    assert( cur != 0 );              \
    register CONST_ T* e= cur->element;  \
    cur= cur->prev;                  \
    return e;                        \
    }                                \
  CONST_ IListElement<T>* getElement() const \
    {                                \
    assert( cur != 0 );              \
    return cur;                      \
    }                                \
  CONST_ T* get() const              \
    {                                \
    assert( cur != 0 );              \
    return cur->element;             \
    }                                \
};

#define CONST_
ILISTITERATOR( IListIterator )

#undef CONST_
#define CONST_ const
ILISTITERATOR( ConstIListIterator )

#undef CONST_

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __IList_h

