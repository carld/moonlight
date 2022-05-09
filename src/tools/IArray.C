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
  IArray.C

  Expandable Indirect Array. Well, like a list.

  Stephane Rehel
  October 8 1996
*/

#include <string.h>

#include "IArray.h"

/////////////////////////////////////////////////////////////////////////////

// For 32-bits systems:
// Default initial allocated memory: 32 bytes, 64 bytes for 1 element
//
// Allocated Memory: (in bytes)
// blockSizeExp  Elements:  0      1
//   0 (1)                 4      4
//   1 (2)                 8      16
//   2 (4)                 16     32
//   3 (8)                 32     64
//   4 (16)                64     128
//   8 (256)               1024   2048
//
IArray::IArray( int _blockSize /* = 8 */ )
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
  blocks= new void** [ nBlocks ];
  for( int i= 0; i < nBlocks; ++i )
    blocks[i]= 0;

  nElements= 0;
  maxElements= blockSize * nBlocks;
}

/////////////////////////////////////////////////////////////////////////////

IArray::~IArray()
{
  clear();
  delete blocks;
  blocks= 0;
  nBlocks= 0;
  maxElements= 0;
}

/////////////////////////////////////////////////////////////////////////////

void IArray::clear()
{
  for( register int i= 0; i < nBlocks; ++i )
    {
    delete blocks[i];
    blocks[i]= 0;
    }

  nElements= 0;
}

/////////////////////////////////////////////////////////////////////////////

void IArray::expand()
{
  if( nElements < maxElements )
    {
    int block_index= nElements >> blockSizeExp;
    if( blocks[block_index] == 0 )
      {
      // Add a new block
      blocks[block_index]= new void* [ blockSize ];
      memset( (void*) blocks[block_index], 0, blockSize * sizeof(void*) );
      }
    }
   else
    {
    // Expand nBlocks: +25%
    int new_nBlocks= nBlocks + nBlocks / 4;
    void*** new_blocks= new void** [ new_nBlocks ];
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

// return nElements
int IArray::append( void* t )
{
  if( nElements >= maxElements )
    expand();

  register int block_index= nElements >> blockSizeExp;
  if( blocks[block_index] == 0 )
    expand();

  blocks[ block_index ][ nElements & blockSizeAnd ]= t;
  ++nElements;

  return nElements;
}

/////////////////////////////////////////////////////////////////////////////

void IArray::remove( int i )
{
  if( i < 1 || i > nElements )
    return;
  --i;
  int block_index= i >> blockSizeExp;
  int index= i & blockSizeAnd;
  if( index < blockSize-1 )
    memmove( (void*) &blocks[block_index][index],
             (void*) &blocks[block_index][index+1],
             (blockSize-1-index)*sizeof(void*) );
  blocks[block_index][blockSize-1]= 0;

  for(;;)
    {
    ++block_index;
    if( block_index >= nBlocks )
      break;
    if( blocks[block_index] == 0 )
      break;
    blocks[block_index-1][blockSize-1]= blocks[block_index][0];
    memmove( (void*) &blocks[block_index][0],
             (void*) &blocks[block_index][1],
             (blockSize-1) * sizeof(void*) );
    blocks[block_index][blockSize-1]= 0;
    }

  --nElements;
}

/////////////////////////////////////////////////////////////////////////////

int IArray::find( const void* t ) const
{
  typedef const void* CVOIDP;
  int block_index= 0;
  register int j= 0;
  register CVOIDP* b= (const void**) blocks[0];
  register int i= 1;

  for(;;)
    {
    if( *b == t )
      return i;
    ++i;
    if( i == nElements )
      break;

    ++j;
    ++b;

    if( j == blockSize )
      {
      ++block_index;
      j= 0;
      b= (const void**) blocks[block_index];
      }
    }

  return 0; // not found
}

/////////////////////////////////////////////////////////////////////////////

int IArray::getSizeOf() const
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
