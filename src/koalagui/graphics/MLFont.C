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
  MLFont.C

  Stephane Rehel
  June 27, 1996
*/

#include <string.h>
#include "GL/opengl.h"

#include "MLFont.h"

/////////////////////////////////////////////////////////////////////////////

MLFont::MLFont()
{
  data= 0;
  bitmap_data= 0;
  count= first= last= 0;
  height= 0;

  real_name= 0;
  name= MLString("");
  list_base= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLFont::~MLFont()
{
  if( list_base != 0 )
    {
    glDeleteLists( list_base, (GLuint) last+1 );
    list_base= 0;
    }

  data= 0;
  bitmap_data= 0;
  count= first= last= 0;
  height= 0;

  real_name= 0;
  name= MLString("");

}

/////////////////////////////////////////////////////////////////////////////

void MLFont::build()
{
  if( data == 0 || bitmap_data == 0 )
    return;
  if( list_base != 0 )
    return; // already done

  list_base= (int) glGenLists( (GLuint) last+1 );
  if( list_base == 0 )
    return;

  GLint swapbytes, lsbfirst, rowlength;
  GLint skiprows, skippixels, alignment;

  // Save the current packing mode for bitmaps.
  glGetIntegerv( GL_UNPACK_SWAP_BYTES, &swapbytes );
  glGetIntegerv( GL_UNPACK_LSB_FIRST, &lsbfirst );
  glGetIntegerv( GL_UNPACK_ROW_LENGTH, &rowlength );
  glGetIntegerv( GL_UNPACK_SKIP_ROWS, &skiprows );
  glGetIntegerv( GL_UNPACK_SKIP_PIXELS, &skippixels );
  glGetIntegerv( GL_UNPACK_ALIGNMENT, &alignment );

  // Enforce a standard packing mode
  glPixelStorei( GL_UNPACK_SWAP_BYTES, GL_FALSE );
  glPixelStorei( GL_UNPACK_LSB_FIRST, GL_FALSE );
  glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
  glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
  glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

  for( int i= 0; i < count; i++ )
    {
    MLFontData& font= data[i];
    int list= list_base + i + first;

    glNewList( (GLuint) list, GL_COMPILE );

    if( font.length > 0 )
      glBitmap( (GLsizei) font.width,
                (GLsizei) font.height,
                (GLfloat) font.x0,
                (GLfloat) font.y0,
                (GLfloat) font.xi,
                (GLfloat) font.yi,
                (const GLubyte*) (bitmap_data + font.data_pos) );
     else
      glBitmap( (GLsizei) 0,
                (GLsizei) 0,
                (GLfloat) 0.,
                (GLfloat) 0.,
                (GLfloat) font.xi,
                (GLfloat) font.yi,
                (const GLubyte*) 0 );
    glEndList();
    }

  // Restore saved packing modes.
  glPixelStorei( GL_UNPACK_SWAP_BYTES, swapbytes );
  glPixelStorei( GL_UNPACK_LSB_FIRST, lsbfirst );
  glPixelStorei( GL_UNPACK_ROW_LENGTH, rowlength );
  glPixelStorei( GL_UNPACK_SKIP_ROWS, skiprows );
  glPixelStorei( GL_UNPACK_SKIP_PIXELS, skippixels );
  glPixelStorei( GL_UNPACK_ALIGNMENT, alignment );
}

/////////////////////////////////////////////////////////////////////////////

int MLFont::getCharWidth( unsigned char ch ) const
{
  register int i= int(ch) - first;
  if( i < 0 || i >= count )
    return 0;

  return (int) data[i].xi;
}

/////////////////////////////////////////////////////////////////////////////

int MLFont::getStringWidth( const char* string ) const
{
  if( string == 0 )
    return 0;

  register int w= 0;
  while( *string != '\0' )
    w+= getCharWidth( *(string++) );

  return w;
}

/////////////////////////////////////////////////////////////////////////////

int MLFont::getStringWidth( const char* string, int length ) const
{
  if( string == 0 )
    return 0;

  register int w= 0;
  while( *string != '\0' )
    {
    if( length-- <= 0 )
      break;
    w+= getCharWidth( *(string++) );
    }

  return w;
}

/////////////////////////////////////////////////////////////////////////////

void MLFont::print( const char* string, int length )
{
  if( string == 0 || length <= 0 )
    return;
  glPushAttrib( GL_LIST_BIT );
  glListBase( (GLuint) list_base );
  glCallLists( (GLsizei) length, GL_UNSIGNED_BYTE, (const GLubyte*) string );
  glPopAttrib();
}

/////////////////////////////////////////////////////////////////////////////

void MLFont::print( const char* string )
{
  if( string == 0 )
    return;
  print( string, strlen(string) );
}

/////////////////////////////////////////////////////////////////////////////

void MLFont::print( const MLString& string )
{
  print( string.get(), string.length() );
}

/////////////////////////////////////////////////////////////////////////////

void MLFont::print( int x, int y, const char* string )
{
  glRasterPos2i(x,y);
  print(string);
}

/////////////////////////////////////////////////////////////////////////////

void MLFont::print( int x, int y, const char* string, int length )
{
  glRasterPos2i(x,y);
  print(string,length);
}

/////////////////////////////////////////////////////////////////////////////

void MLFont::print( int x, int y, const MLString& string )
{
  glRasterPos2i(x,y);
  print(string);
}

/////////////////////////////////////////////////////////////////////////////
