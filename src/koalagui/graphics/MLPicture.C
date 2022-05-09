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
  MLPicture.C

  Stephane Rehel
  August 16 1996
*/

#include "GL/opengl.h"

#include "MLPicture.h"

#include "GraphicsSystem.h"

/////////////////////////////////////////////////////////////////////////////

MLPicture::MLPicture()
{
  refCount= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLPicture::~MLPicture()
{}

/////////////////////////////////////////////////////////////////////////////

// static
void MLPicture::draw( MLImage* image, int x, int y )
{
  if( image == 0 )
    return;

  unsigned char* data= image->getData();
  int width= image->getWidth();
  int height= image->getHeight();

  if( data == 0 )
    return;

  glRasterPos2i(x,y);

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

  glPushAttrib(GL_ENABLE_BIT);
  glPushAttrib(GL_PIXEL_MODE_BIT);
  glPixelZoom(1.,1.);
  glPixelTransferf( GL_RED_SCALE,   1. );
  glPixelTransferf( GL_GREEN_SCALE, 1. );
  glPixelTransferf( GL_BLUE_SCALE,  1. );
  glPixelTransferf( GL_ALPHA_SCALE, 1. );
  glPixelTransferf( GL_RED_BIAS,    0. );
  glPixelTransferf( GL_GREEN_BIAS,  0. );
  glPixelTransferf( GL_BLUE_BIAS,   0. );
  glPixelTransferf( GL_ALPHA_BIAS,  0. );

  if( image->hasAlpha() )
    {
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc( GL_GEQUAL, 0.5 );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
    glDrawPixels( width, height, GL_RGBA, GL_UNSIGNED_BYTE, (void*) data );
    }
   else
    {
    glDisable(GL_ALPHA_TEST);
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glDrawPixels( width, height, GL_RGB, GL_UNSIGNED_BYTE, (void*) data );
    }

  glPopAttrib();
  glPopAttrib();

  // Restore saved packing modes.
  glPixelStorei( GL_UNPACK_SWAP_BYTES, swapbytes );
  glPixelStorei( GL_UNPACK_LSB_FIRST, lsbfirst );
  glPixelStorei( GL_UNPACK_ROW_LENGTH, rowlength );
  glPixelStorei( GL_UNPACK_SKIP_ROWS, skiprows );
  glPixelStorei( GL_UNPACK_SKIP_PIXELS, skippixels );
  glPixelStorei( GL_UNPACK_ALIGNMENT, alignment );
}

/////////////////////////////////////////////////////////////////////////////

void MLPicture::draw( int x, int y )
{
  MLPicture::draw( this, x, y );
}

/////////////////////////////////////////////////////////////////////////////

void MLPicture::draw( int x, int y,
                      int x1, int y1, int x2, int y2 )
{
  if( data == 0 )
    return;

  x1= max( 0, x1 );
  y1= max( 0, y1 );
  x1= min( width-1, x1 );
  y1= min( height-1, y1 );
  if( x1 > x2 || y1 > y2 )
    return;

  int block_width=  x2 - x1 + 1;
  int block_height= y2 - y1 + 1;

  int block_row_length= width;
  int block_skip_pixels= x1;
  int block_skip_rows  = y1;

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
  glPixelStorei( GL_UNPACK_ROW_LENGTH, block_row_length );
  glPixelStorei( GL_UNPACK_SKIP_ROWS,  block_skip_rows );
  glPixelStorei( GL_UNPACK_SKIP_PIXELS, block_skip_pixels );

  glPushAttrib(GL_ENABLE_BIT);
  glPushAttrib(GL_PIXEL_MODE_BIT);

  glPixelZoom(1.,1.);
  glPixelTransferf( GL_RED_SCALE,   1. );
  glPixelTransferf( GL_GREEN_SCALE, 1. );
  glPixelTransferf( GL_BLUE_SCALE,  1. );
  glPixelTransferf( GL_RED_BIAS,    0. );
  glPixelTransferf( GL_GREEN_BIAS,  0. );
  glPixelTransferf( GL_BLUE_BIAS,   0. );

  glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );

  glRasterPos2i(x+x1,y+y1);

  if( with_alpha )
    {
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc( GL_GEQUAL, 0.5 );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
    glDrawPixels( block_width, block_height,
                  GL_RGBA, GL_UNSIGNED_BYTE, (void*) data );
    }
   else
    {
    glDisable(GL_ALPHA_TEST);
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glDrawPixels( block_width, block_height,
                  GL_RGB, GL_UNSIGNED_BYTE, (void*) data );
    }

  glPopAttrib();
  glPopAttrib();

  // Restore saved packing modes.
  glPixelStorei( GL_UNPACK_SWAP_BYTES, swapbytes );
  glPixelStorei( GL_UNPACK_LSB_FIRST, lsbfirst );
  glPixelStorei( GL_UNPACK_ROW_LENGTH, rowlength );
  glPixelStorei( GL_UNPACK_SKIP_ROWS, skiprows );
  glPixelStorei( GL_UNPACK_SKIP_PIXELS, skippixels );
  glPixelStorei( GL_UNPACK_ALIGNMENT, alignment );
}

/////////////////////////////////////////////////////////////////////////////

// get gamma correction from GraphicsSystem
void MLPicture::gammaCorrect()
{
  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  if( graphicsSystem == 0 )
    return;

  MLGammaCorrection* gc= graphicsSystem->getGammaCorrection();
  if( gc == 0 )
    return;

  MLImage::gammaCorrect(*gc);
}

/////////////////////////////////////////////////////////////////////////////

