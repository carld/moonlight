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
  IArray.h

  Expandable Indirect Array. Well, like a list.

  Stephane Rehel
  October 8 1996
*/

#ifndef __IArray_h
#define __IArray_h

#include <assert.h>

#ifndef __boolean_H
#include "tools/boolean.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class IArray
{
private:
  int blockSizeExp; // n
  int blockSize;    // 2**n
  int blockSizeAnd; // 2**n - 1

  void*** blocks;
  int nBlocks;

  int maxElements;
  int nElements;

public:
  IArray( int _blockSize = 8 );
  ~IArray();

         void       clear      ();
  inline void*&     operator[] ( register int i );
  inline void*      get        ( register int i ) const;
  inline int        getSize    () const;

         int        append     ( void* t );
         void       remove     ( int i );
         int        find       ( const void* t ) const;

         int        getSizeOf  () const;

private:
         void       expand     ();
};

/////////////////////////////////////////////////////////////////////////////

inline int IArray::getSize() const
{
  return nElements;
}

/////////////////////////////////////////////////////////////////////////////

inline void*& IArray::operator [] ( register int i )
{
  assert( i >= 1 );
  assert( i <= nElements );
  --i;
  return blocks[ i >> blockSizeExp ][ i & blockSizeAnd ];
}

/////////////////////////////////////////////////////////////////////////////

inline void* IArray::get( int i ) const
{
  assert( i >= 1 );
  assert( i <= nElements );
  --i;
  return blocks[ i >> blockSizeExp ][ i & blockSizeAnd ];
}

/////////////////////////////////////////////////////////////////////////////

#endif // __IArray_h

