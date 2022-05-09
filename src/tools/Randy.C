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
   Randy.C
   Small but fine random generator

   Stephane Rehel
   April 14th, 1996
*/

#include <stdlib.h>

#include "Randy.h"

/////////////////////////////////////////////////////////////////////////////

Randy::Randy( unsigned int _init_rnd /* = 1 */ )
{
  init( _init_rnd );
}

/////////////////////////////////////////////////////////////////////////////

void Randy::init( unsigned int _init_rnd /* = 1 */,
                  unsigned int _factor /* = (unsigned int) 0x015A4E35 */ )
{
  init_rnd= _init_rnd;
  factor= _factor;
  rnd= _init_rnd;
}

/////////////////////////////////////////////////////////////////////////////

void Randy::reset()
{
  rnd= init_rnd;
}

/////////////////////////////////////////////////////////////////////////////

int Randy::get()
{
  rnd *= factor;
  ++rnd;
  rnd= (rnd << 16) | (rnd >>16);
  return abs( int(rnd) );
}

/////////////////////////////////////////////////////////////////////////////

int Randy::get( int min_range, int max_range )
{
  return min_range + get() % (max_range-min_range+1);
}

/////////////////////////////////////////////////////////////////////////////

void Randy::get_times( int i )
{
  while( i-- > 0 )
    get();
}

/////////////////////////////////////////////////////////////////////////////
