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
  IntegerArray.C

  Stephane Rehel
  August 2 1997
*/

#include <string.h>

#include "IntegerArray.h"

/////////////////////////////////////////////////////////////////////////////

void IntegerArray::alloc( int _n )
{
  clear();

  if( _n < 1 )
    return;

  n= _n;

  array= new int [ n ];
}

/////////////////////////////////////////////////////////////////////////////

/*
void IntegerArray::realloc( int _n )
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
*/

/////////////////////////////////////////////////////////////////////////////

void IntegerArray::fill( int i )
{
  if( array == 0 )
    return;

  register int* a= array;
  for( register int j= n; j; --j )
    *(a++)= i;
}

/////////////////////////////////////////////////////////////////////////////

IntegerArray& IntegerArray::operator = ( const IntegerArray& ia )
{
  if( ia.getSize() == 0 )
    {
    clear();
    return *this;
    }

  alloc(ia.getSize());

  memcpy( (void*) array, (void*) ia.array, sizeof(int)*getSize() );

  return *this;
}

/////////////////////////////////////////////////////////////////////////////

void IntegerArray::truncate( int new_size )
{
  if( new_size <= 0 )
    {
    clear();
    return;
    }

  if( new_size < n )
    n= new_size;
}

/////////////////////////////////////////////////////////////////////////////

void swap( IntegerArray& a1, IntegerArray& a2 )
{
  int n= a1.n;
  int* array= a1.array;

  a1.n= a2.n;
  a1.array= a2.array;

  a2.n= n;
  a2.array= array;
}

/////////////////////////////////////////////////////////////////////////////
