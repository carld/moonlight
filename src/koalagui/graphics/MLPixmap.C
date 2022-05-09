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
  MLPixmap.C

  Stephane Rehel
  July 9 1996
*/

#include <assert.h>
#include <string.h>

#include "GL/opengl.h"

#include "tools/boolean.h"

#include "GraphicsSystem.h"
#include "MLPixmap.h"


/////////////////////////////////////////////////////////////////////////////

static boolean buildBitmap( const unsigned char* strings[],
                          int width, int height,
                          unsigned char*& bitmap, char ch );

/////////////////////////////////////////////////////////////////////////////

MLPixmap::MLPixmap()
{
  lists= 0;
  width= height= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLPixmap::~MLPixmap()
{
  if( lists != 0 )
    {
    glDeleteLists( GLuint(lists), 4 );
    lists= 0;
    }

  width= height= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLPixmap::make( const unsigned char* strings[], int _height )
{
  if( lists != 0 )
    return;
  if( _height <= 0 || strings == 0 )
    return;

  width= strlen((const char*)strings[0]);
  height= _height;

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

  lists= (int) glGenLists( GLuint(4) );
  if( lists == 0 )
    return;

//  int colors[4]= {   0,  85, 170, 255 };
  char chars[4]= { '0', '1', '2', '3' };
  unsigned char* gray= 0;

  ok_list= 0;

  for( int i= 0; i < 4; ++i )
    {
    glNewList( (GLuint) (lists+i), GL_COMPILE );

    if( buildBitmap( strings, width, height, gray, chars[i] ) )
      {
//      glColor3ub( colors[i], colors[i], colors[i] );
      glBitmap( (GLsizei) width,
                (GLsizei) height,
                (GLfloat) 0,
                (GLfloat) height-1,
                (GLfloat) 0,
                (GLfloat) 0,
                (const GLubyte*) gray );
      ok_list |= (1 << i);
      }
    delete gray;

    glEndList();
    }

  // Restore saved packing modes
  glPixelStorei( GL_UNPACK_SWAP_BYTES, swapbytes );
  glPixelStorei( GL_UNPACK_LSB_FIRST, lsbfirst );
  glPixelStorei( GL_UNPACK_ROW_LENGTH, rowlength );
  glPixelStorei( GL_UNPACK_SKIP_ROWS, skiprows );
  glPixelStorei( GL_UNPACK_SKIP_PIXELS, skippixels );
  glPixelStorei( GL_UNPACK_ALIGNMENT, alignment );
}

/////////////////////////////////////////////////////////////////////////////

static boolean buildBitmap( const unsigned char* strings[],
                          int width, int height,
                          unsigned char*& bitmap, char ch )
{
  int iwidth= width / 8;
  if( (width & 7) != 0 )
    ++iwidth;

  int size= iwidth * height;
  bitmap= new unsigned char [ size ];

  for( int k= 0; k < size; ++k )
    bitmap[k]= 0;

  boolean found= false;

  for( int j= 0; j < height; ++j )
    {
    const unsigned char* line= strings[height-1-j];
    for( int i= 0; i < width; ++i )
      if( line[i] == ch )
        {
        int x= i / 8;
        int offset= 7 - (i & 7);
        bitmap[j*iwidth+x] |= (unsigned char)1 << offset;
        found= true;
        }
    }

  return found;
}

/////////////////////////////////////////////////////////////////////////////

void MLPixmap::print( int x, int y )
{
  if( lists == 0 )
    return;

  if( (ok_list & 1) )
    {
    glColor3ub( gray_level_0, gray_level_0, gray_level_0 );
    glRasterPos2i(x,y);
    glCallList(lists);
    }

  if( (ok_list & 2) )
    {
    glColor3ub( gray_level_1, gray_level_1, gray_level_1 );
    glRasterPos2i(x,y);
    glCallList(lists+1);
    }

  if( (ok_list & 4) )
    {
    glColor3ub( gray_level_2, gray_level_2, gray_level_2 );
    glRasterPos2i(x,y);
    glCallList(lists+2);
    }

  if( (ok_list & 8) )
    {
    glColor3ub( gray_level_3, gray_level_3, gray_level_3 );
    glRasterPos2i(x,y);
    glCallList(lists+3);
    }
}

/////////////////////////////////////////////////////////////////////////////
