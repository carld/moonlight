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
   MLCursor.h

   Stephane Rehel
   July 22 1997
*/

#ifndef __MLCursor_h
#define __MLCursor_h

#ifndef __Color8_h
#include "tools/Color8.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLCursor
{
protected:
  int width, height;
  int xHotSpot, yHotSpot;
  int line_size;
  Color8 background_color;
  Color8 foreground_color;

  unsigned char* ANDPlane;
  unsigned char* XORPlane;

public:
  MLCursor();
  virtual ~MLCursor();

protected:
  void destroy();

public:
  void create( int _width, int _height, const char* lines[],
               int _xHotSpot, int _yHotSpot );

  void create( int _width, int _height, const char* lines[],
               int _xHotSpot, int _yHotSpot,
               const Color8& _background_color,
               const Color8& _foreground_color );

  virtual void build() = 0;

  virtual void getDefaultSize( int* width, int* height ) const = 0;

protected:
  void put( int i, int j, unsigned char* plane, int yes );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLCursor_h

