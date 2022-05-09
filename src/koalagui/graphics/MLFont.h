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
  MLFont.h

  Stephane Rehel
  June 27, 1996
*/

#ifndef __MLFont_h
#define __MLFont_h

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLFont
{
public:
  struct MLFontData
    {
    int width, height;
    float x0, y0, xi, yi;
    int length;
    int data_pos;
    };

  unsigned int list_base;

  int height, ascent, descent;
  int first, last;
  int count;
  MLFontData* data;
  unsigned char* bitmap_data;

  const char* real_name;
  MLString name;

public:
  MLFont();
  virtual ~MLFont();

  void build();

  int getHeight() const
    {
    return height;
    }
  int getAscent() const
    {
    return ascent;
    }
  int getDescent() const
    {
    return descent;
    }
  int getCharWidth( unsigned char ch ) const;
  int getStringWidth( const char* string ) const;
  int getStringWidth( const char* string, int length ) const;

  int getStringWidth( const MLString& string ) const
    {
    return getStringWidth( string.get() );
    }

  int getWidth() const
    {
    return getCharWidth('_');
    }

  void print( const char* string );
  void print( const char* string, int length );
  void print( const MLString& string );

  void print( int x, int y, const char* string );
  void print( int x, int y, const char* string, int length );
  void print( int x, int y, const MLString& string );

};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLFont_h
