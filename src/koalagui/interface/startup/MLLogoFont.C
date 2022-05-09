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
  MLLogoFont.C

  Stephane Rehel
  March 25 1997
*/

#include "MLLogoFont.h"
#include "graphics/MLPicture.h"

/////////////////////////////////////////////////////////////////////////////

MLLogoFont::MLLogoFont( int font_height, int font_descent,
                        const unsigned char* font_data,
                        const int* font_char_addr )
{
  height= font_height;
  descent= font_descent;
  data= font_data;
  char_addr= font_char_addr;
}

/////////////////////////////////////////////////////////////////////////////

MLLogoFont::~MLLogoFont()
{}

/////////////////////////////////////////////////////////////////////////////

int MLLogoFont::getCharWidth( int ch ) const
{
  if( ch < 32 || ch > 127 ) ch= 32;

  const unsigned char* p= data + char_addr[ch-32];

  return *(p++);
}

/////////////////////////////////////////////////////////////////////////////

int MLLogoFont::getStringWidth( const char* string ) const
{
  if( string == 0 )
    return 0;

  int w= 0;
  while( *string != '\0' )
    w+= getCharWidth( *(string++) );

  return w;
}

/////////////////////////////////////////////////////////////////////////////

// XFont-like coordinates
void MLLogoFont::drawChar( int i, int j, MLPicture* picture, int ch )
{
  if( picture == 0 )
    return;

  if( ch < 32 || ch > 127 ) ch= 32;

  j-= descent;
  const unsigned char* p= data + char_addr[ch-32];
  int width= *(p++);
  ++p;
  int ki_begin= 0;
  int ki_end= width-1;
  if( i < 0 )
    ki_begin= -i;
  if( i+width >= picture->getWidth() )
    ki_end= picture->getWidth() - i;

  if( ki_end < ki_begin )
    return;

  register int pixel_weight= picture->hasAlpha() ? 4 : 3;
  for( int kj= height-1; kj >= 0; --kj, p+= width )
    {
    int y= j+kj;
    if( y < 0 || y >= picture->getHeight() )
      continue;
    register unsigned char* pixels= picture->getPixel(i,j+kj);
    for( register int ki= ki_begin; ki <= ki_end; ++ki,
                                                  pixels+= pixel_weight )
      {
      register unsigned char b= p[ki];
      pixels[0]+= b;
      pixels[1]+= b;
      pixels[2]+= b;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLLogoFont::opaqueChar( int i, int j, MLPicture* picture, int ch )
{
  if( picture == 0 )
    return;
  int width= getCharWidth(ch);
  j-= descent;
  picture->fill( i, j, i+width-1, j+height-1, 0, 0, 0 );
}

/////////////////////////////////////////////////////////////////////////////

void MLLogoFont::drawString( int i, int j, MLPicture* picture,
                             const char* string )
{
  while( *string != '\0' )
    {
    drawChar( i, j, picture, *string );
    i+= getCharWidth(*string);
    ++string;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLLogoFont::opaqueString( int i, int j, MLPicture* picture,
                               const char* string )
{
  while( *string != '\0' )
    {
    opaqueChar( i, j, picture, *string );
    i+= getCharWidth(*string);
    ++string;
    }
}

/////////////////////////////////////////////////////////////////////////////

