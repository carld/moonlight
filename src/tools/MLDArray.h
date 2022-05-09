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
  MLDArray.h

  Stephane Rehel
  November 11 1996

  April 20 1998: renamed MLDArray.h into MLDArray.h
*/

#ifndef __MLDArray_h
#define __MLDArray_h

#include <assert.h>
#include <string.h>

/////////////////////////////////////////////////////////////////////////////

template<class T>
class MLDArray
{
private:
  int blockSizeExp; // n
  int blockSize;    // 2**n
  int blockSizeAnd; // 2**n - 1

  T** blocks;
  int nBlocks;

  int maxElements;
  int nElements;

public:
  MLDArray( int _blockSize = 8 );

  virtual ~MLDArray();

  void clear();

  T& operator[] ( register int i )
    {
    assert( i >= 1 );
    assert( i <= nElements );
    --i;
    return blocks[ i >> blockSizeExp ][ i & blockSizeAnd ];
    }

  const T& get( register int i ) const
    {
    assert( i >= 1 );
    assert( i <= nElements );
    --i;
    return blocks[ i >> blockSizeExp ][ i & blockSizeAnd ];
    }

  int length() const
    {
    return nElements;
    }

  // for compatibility
  int getSize() const
    {
    return length();
    }

  T& append();

  void append( const T& t )
    {
    append() = t;
    }

  void remove( int i );

  void realloc( int size );

  int getSizeOf() const;

  MLDArray<T>& operator = ( const MLDArray<T>& a );


  void fill( int beginIndex, int endIndex, const T& t );

private:
  void expand();
};

/////////////////////////////////////////////////////////////////////////////

template<class T>
MLDArray<T>::MLDArray( int _blockSize /* = 8 */ )
{
  if( _blockSize <= 0 )
    _blockSize= 0;

  // get the biggest bse so that 2^bse be lower or equal to _blockSize
  // ex: 7 -> 4
  //     8 -> 8
  //     9 -> 8
  int bse= 0;
  while( _blockSize >= (1<<bse) )
    ++bse;
  --bse;

  blockSizeExp= bse;
  blockSize= 1 << blockSizeExp;
  blockSizeAnd= blockSize - 1;

  nBlocks= blockSize;
  blocks= new T* [ nBlocks ];
  for( int i= 0; i < nBlocks; ++i )
    blocks[i]= 0;

  nElements= 0;
  maxElements= blockSize * nBlocks;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
MLDArray<T>::~MLDArray()
{
  clear();
  delete blocks;
  blocks= 0;
  nBlocks= 0;
  maxElements= 0;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
void MLDArray<T>::clear()
{
  if( nElements == 0 )
    return; // speed up

  for( register int i= 0; i < nBlocks; ++i )
    {
    T* one_block= blocks[i];
    if( one_block != 0 )
      {
      // call destructor for each element

      delete [] one_block;
      blocks[i]= 0;
      }
    }

  nElements= 0;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
T& MLDArray<T>::append()
{
  if( nElements >= maxElements )
    expand();

  register int block_index= nElements >> blockSizeExp;
  if( blocks[block_index] == 0 )
    expand();

  T& e= blocks[ block_index ][ nElements & blockSizeAnd ];
  ++nElements;

  return e;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
void MLDArray<T>::remove( int i )
{
  if( i < 1 || i > nElements )
    return;

  if( i == nElements )
    {
    // speed up for last element
    --nElements;
    return;
    }

  --i;
  int block_index= i >> blockSizeExp;
  int index= i && blockSizeAnd;
  if( index < blockSize-1 )
    memmove( (void*) &blocks[block_index][index],
             (void*) &blocks[block_index][index+1],
             (blockSize-1-index) * sizeof(T) );

  for(;;)
    {
    ++block_index;
    if( block_index >= nBlocks )
      break;
    if( blocks[block_index] == 0 )
      break;
    memcpy( (void*) &blocks[block_index-1][blockSize-1],
            (void*) &blocks[block_index][0],
            sizeof(T) );
    memmove( (void*) &blocks[block_index][0],
             (void*) &blocks[block_index][1],
             (blockSize-1) * sizeof(T) );
    }

  --nElements;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
void MLDArray<T>::realloc( int size )
{
  int delta= size - nElements;

  if( delta > 0 )
    {
    while( delta > 0 )
      {
      append();
      --delta;
      }
    }
   else
    {
    while( delta < 0 )
      {
      remove(length());
      ++delta;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
int MLDArray<T>::getSizeOf() const
{
  int s= sizeof(*this) + nBlocks * sizeof(blocks[0]);

  int n= 0;
  for( int i= 0; i < nBlocks; ++i )
    if( blocks[i] != 0 )
      ++n;
  s+= n * (blockSize * sizeof(blocks[0][0]));

  return s;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
void MLDArray<T>::expand()
{
  if( nElements < maxElements )
    {
    int block_index= nElements >> blockSizeExp;
    if( blocks[block_index] == 0 )
      {
      // Add a new block
      blocks[block_index]= new T [ blockSize ];
      }
    }
   else
    {
    // Expand nBlocks: +25%
    int new_nBlocks= nBlocks + nBlocks / 4;
    T** new_blocks= new T* [ new_nBlocks ];
    if( blocks != 0 )
      memcpy( (void*) new_blocks, (void*) blocks, nBlocks * sizeof(void*) );
    memset( (void*) (new_blocks+nBlocks), 0,
            sizeof(void*) * (new_nBlocks-nBlocks) );

    delete blocks;
    blocks= new_blocks;
    nBlocks= new_nBlocks;

    maxElements= nBlocks * blockSize;
    }
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
MLDArray<T>& MLDArray<T>::operator = ( const MLDArray<T>& a )
{
  if( &a == this )
    return *this;

  clear();

  for( int i= 1; i <= a.length(); ++i )
    append( a.get(i) );

  return *this;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
void MLDArray<T>::fill( int beginIndex, int endIndex, const T& t )
{
  if( beginIndex < 1 )
    beginIndex= 1;

  if( endIndex > n )
    endIndex= n;

  if( beginIndex > endIndex )
    return;

  for( int i= beginIndex; i <= endIndex; ++i )
    (operator[](i))= t;
}

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLDArray_h
