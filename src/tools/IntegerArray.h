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
  IntegerArray.h

  Stephane Rehel
  August 2 1997
*/

#ifndef __IntegerArray_h
#define __IntegerArray_h

#include <assert.h>
#include <string.h>

/////////////////////////////////////////////////////////////////////////////

class IntegerArray
{
protected:
  int n;
  int* array;

public:
  IntegerArray()
    {
    array= 0;
    n= 0;
    }
  IntegerArray( const IntegerArray& ia )
    {
    array= 0;
    n= 0;
    operator = (ia);
    }

  virtual ~IntegerArray()
    {
    clear();
    }

  void clear()
    {
    delete array;
    array= 0;
    n= 0;
    }

  void alloc( int _n );

//  void realloc( int _n );

// This "inline" is due to a bug of pgcc-2.90.23 980102 (egcs-1.0.1)
// To be followed!
inline void IntegerArray::realloc( int _n )
{
  if( _n == n )
    return;

  if( _n < 0 )
    _n= 0;

  if( _n == 0 )
    {
    clear();
    return;
    }

  int* new_array= new int [ _n ];
  memcpy( (void*) new_array, (const void*) array,
          sizeof(int) * ((n<_n) ? n : _n) );

  delete array;
  array= new_array;
  n= _n;
}

  int getSize() const
    {
    return n;
    }

  // 0 <= i < n
  int get( int i ) const
    {
    assert( i >= 0 );
    assert( i <  n );
    return array[i];
    }

  // 0 <= i < n
  int& operator [] ( int i )
    {
    assert( i >= 0 );
    assert( i <  n );
    return array[i];
    }

  void fill( int i );

  void truncate( int new_size );

  IntegerArray& operator = ( const IntegerArray& ia );

  friend void swap( IntegerArray& a1, IntegerArray& a2 );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __IntegerArray_h
