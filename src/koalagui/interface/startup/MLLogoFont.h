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
  MLLogoFont.h

  Stephane Rehel
  March 25 1997
*/

#ifndef __MLLogoFont_h
#define __MLLogoFont_h

class MLPicture;

/////////////////////////////////////////////////////////////////////////////

class MLLogoFont
{
protected:
  int height, descent;
  const unsigned char* data;
  const int* char_addr;

public:
  MLLogoFont( int font_height, int font_descent,
              const unsigned char* font_data,
              const int* font_char_addr );

  virtual ~MLLogoFont();

  int getHeight() const
    {
    return height;
    }

  int getDescent() const
    {
    return descent;
    }

  int getCharWidth( int ch ) const;
  int getStringWidth( const char* string ) const;

  // XFont-like coordinates
  void drawChar( int i, int j, MLPicture* picture, int ch );
  void opaqueChar( int i, int j, MLPicture* picture, int ch );

  void drawString( int i, int j, MLPicture* picture, const char* string );
  void opaqueString( int i, int j, MLPicture* picture, const char* string );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLLogoFont_h

