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
   MLCursor.C

   Stephane Rehel
   July 22 1997

   Format for lines[]:
     ' ' : and=1 xor=0  (unchanged)
     '|' : and=1 xor=1  (reverse screen)
     '-' : and=0 xor=0  (black)
     '+' : and=0 xor=1  (white)
*/

#include "tools/Common.h"

#include "MLCursor.h"

/////////////////////////////////////////////////////////////////////////////

MLCursor::MLCursor()
{
  width= height= 0;
  xHotSpot= yHotSpot= 0;
  ANDPlane= XORPlane= 0;
  line_size= 0;
  background_color= Color8(255,255,255);
  foreground_color= Color8(255,  0,  0); // this is the real color
}

/////////////////////////////////////////////////////////////////////////////

MLCursor::~MLCursor()
{
  destroy();
}

/////////////////////////////////////////////////////////////////////////////

void MLCursor::destroy()
{
  delete ANDPlane; ANDPlane= 0;
  delete XORPlane; XORPlane= 0;
  xHotSpot= yHotSpot= 0;
  width= height= 0;
  line_size= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLCursor::create( int _width, int _height, const char* lines[],
                       int _xHotSpot, int _yHotSpot )
{
  create( _width, _height, lines, _xHotSpot, _yHotSpot,
          Color8(255,255,255),
          Color8(255,  0,  0) );
}

/////////////////////////////////////////////////////////////////////////////

void MLCursor::create( int _width, int _height, const char* lines[],
                       int _xHotSpot, int _yHotSpot,
                       const Color8& _background_color,
                       const Color8& _foreground_color )
{
  destroy();
  if( lines == 0 || _width <= 0 || _height <= 0 )
    return;

  getDefaultSize( &width, &height );

  xHotSpot= max( 0, min( _xHotSpot, width -1 ) );
  yHotSpot= max( 0, min( _yHotSpot, height-1 ) );
  background_color= _background_color;
  foreground_color= _foreground_color;

  line_size= (width + 7) / 8;
  ANDPlane= new unsigned char [ line_size * height ];
  XORPlane= new unsigned char [ line_size * height ];

  int i, j;
  for( i= line_size * height - 1; i >= 0; --i )
    {
    ANDPlane[i]= 0;
    XORPlane[i]= 0;
    }

  for( j= 0; j < _height; ++j )
    for( i= 0; i < _width; ++i )
      {
      unsigned char ch= lines[j][i];
      int and_bit, xor_bit;

      switch(ch)
        {
        case '|': { and_bit= 1; xor_bit= 1; break; }
        case '-': { and_bit= 0; xor_bit= 0; break; }
        case '+': { and_bit= 0; xor_bit= 1; break; }
        case ' ':
        default:  { and_bit= 1; xor_bit= 0; break; }
        }

      put( i, j, ANDPlane, and_bit );
      put( i, j, XORPlane, xor_bit );
      }
}

/////////////////////////////////////////////////////////////////////////////

void MLCursor::put( int i, int j, unsigned char* plane, int yes )
{
  if( plane == 0 || i < 0 || i >= width || j < 0 || j >= height )
    return;

  int i_byte= i / 8;
#ifdef __WINDOWS__
  int i_bit = 7 - (i & 7);
#else
  int i_bit = (i & 7);
#endif

  unsigned char mask= 1 << i_bit;
  unsigned char* byte= plane + j * line_size + i_byte;
  if( yes )
    *byte |= mask;
   else
    *byte &= ~mask;
}

/////////////////////////////////////////////////////////////////////////////

