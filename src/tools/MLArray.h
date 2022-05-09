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
   MLArray.h

   Stephane Rehel
   March 24 1998
*/

#ifndef __MLArray_h
#define __MLArray_h

#include <assert.h>

#include "boolean.h"

/////////////////////////////////////////////////////////////////////////////

template<class T>
class MLArray
{
protected:
  int max_n; // real allocated size
  T* array;
  int n;

public:
  MLArray()
    {
    array= 0;
    n= max_n= 0;
    }

  MLArray( int _n )
    {
    array= 0;
    n= max_n= 0;
    realloc(_n);
    }

  MLArray( const MLArray<T>& a )
    {
    array= 0;
    n= max_n= 0;
    operator = ( a );
    }

  virtual ~MLArray()
    {
    clear();
    }

  void clear()
    {
    if( array == 0 )
      return;

    delete [] array;
    array= 0;
    n= max_n= 0;
    }

  // return true if changed
  boolean realloc( int _n );

  int length() const
    {
    return n;
    }

  // 1 <= i <= n
  const T& get( int i ) const
    {
    assert( i >= 1 );
    assert( i <=  n );
    assert( array != 0 );

    return array[i-1];
    }

  // 1 <= i <= n
  T& operator [] ( int i )
    {
    assert( i >= 1 );
    assert( i <= n );
    assert( array != 0 );

    return array[i-1];
    }

  void fill( int beginIndex, int endIndex, const T& t );

  void fill( const T& t )
    {
    fill( 1, n, t );
    }

  void append( const T& t )
    {
    realloc( n + 1 );
    array[ n - 1 ] = t;
    }

  T& append()
    {
    realloc( n + 1 );
    return array[ n - 1 ];
    }

  // 1 <= i <= n
  void remove( int i );

  MLArray<T>& operator = ( const MLArray<T>& a );

  void set( const T* a, int _length );
};

/////////////////////////////////////////////////////////////////////////////

template<class T>
boolean MLArray<T>::realloc( int _n )
{
  if( _n < 0 )
    _n= 0;

  if( n == _n )
    return false;

  if( _n == 0 )
    {
    clear();
    return true;
    }

  if( _n > max_n )
    {
    if( max_n == 0 )
      max_n= 10;

    if( _n > max_n )
      {
      max_n += max_n / 2 ;
      if( _n > max_n )
        max_n= _n;
      }

    T* new_array= new T [ max_n ];

    if( _n < n )
      {
      for( int i= 0; i < _n; ++i )
        new_array[i]= array[i];
      }
     else
      {
      for( int i= 0; i < n; ++i )
        new_array[i]= array[i];
      }

    delete [] array;
    array= new_array;
    }

  n= _n;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
void MLArray<T>::fill( int beginIndex, int endIndex, const T& t )
{
  if( array == 0 )
    return;

  if( beginIndex < 1 )
    beginIndex= 1;

  if( endIndex > n )
    endIndex= n;

  if( beginIndex > endIndex )
    return;

  register T* array_i= array + beginIndex - 1;
  for( register int i= endIndex-beginIndex+1; i; --i, ++array_i )
    *array_i= t;
}

/////////////////////////////////////////////////////////////////////////////

// 1 <= i <= n
template<class T>
void MLArray<T>::remove( int i )
{
  if( i < 1 || i > n )
    return;

  for( int j= i; j < n; ++j )
    array[j-1]= array[j];

  --n;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
MLArray<T>& MLArray<T>::operator = ( const MLArray<T>& a )
{
  if( &a == this )
    return *this;

  set( a.array, a.n );

  return *this;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
void MLArray<T>::set( const T* a, int _length )
{
  if( a == 0 || _length <= 0 )
    {
    clear();
    return;
    }

  if( _length <= max_n )
    {
    for( register int i= 0; i < _length; ++i )
      array[i]= a[i];
    n= _length;
    }
   else
    {
    T* new_array= new T [ _length ];
    for( register int i= 0; i < _length; ++i )
      new_array[i]= a[i];

    delete [] array;
    array= new_array;
    n= max_n= _length;
    }
}

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLArray_h
