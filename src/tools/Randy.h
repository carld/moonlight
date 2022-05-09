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
   Randy.h
   Small but fine random generator

   Stephane Rehel
   April 14th, 1996
*/

#ifndef __Randy_h
#define __Randy_h

/////////////////////////////////////////////////////////////////////////////

class Randy
{
private:
  unsigned int rnd;
  unsigned int factor;

  unsigned int init_rnd;

public:
  Randy( unsigned int _init_rnd = 1 );

  virtual ~Randy()
    {}

  void init( unsigned int init_rnd = 1,
             unsigned int _factor= (unsigned int) 0x015A4E35 );
  void reset();
  int get();
  int get( int min_range, int max_range );

  // get n random numbers
  void get_times( int i );

};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __Randy_h

