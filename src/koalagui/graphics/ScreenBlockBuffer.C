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
  ScreenBlockBuffer.C

  Stephane Rehel
  July 20 1997
*/
#include <stdio.h>
#include "ScreenBlockBuffer.h"

#include "GraphicsSystem.h"
#include "SystemWindow.h"
#include "mlgl.h"

/////////////////////////////////////////////////////////////////////////////

ScreenBlockBuffer::ScreenBlockBuffer()
{
  x1= y1= x2= y2= 0;
  data= 0;
}

/////////////////////////////////////////////////////////////////////////////

ScreenBlockBuffer::~ScreenBlockBuffer()
{
  destroy();
}

/////////////////////////////////////////////////////////////////////////////

void ScreenBlockBuffer::destroy()
{
  delete data;
  data= 0;
  x1= y1= x2= y2= 0;
}

/////////////////////////////////////////////////////////////////////////////

void ScreenBlockBuffer::get( SystemWindow* sw )
{
  destroy();

  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  if( sw == 0 || graphicsSystem == 0 )
    return;

  IPoint p1= sw->getAbsolutePosition();
  IVector size= sw->getSize();

  p1[1]= graphicsSystem->getScreenSize().y() - 1 - (p1[1] + size.y() - 1);

  get( p1.x(), p1.y(), size.x(), size.y() );
}

/////////////////////////////////////////////////////////////////////////////

// (x,y) from left-bottom
void ScreenBlockBuffer::get( int x, int y, int width, int height )
{
  destroy();

  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  if( graphicsSystem == 0 )
    return;

  IVector screenSize= graphicsSystem->getScreenSize();

  x1= x;
  y1= y;
  x2= x + width - 1;
  y2= y + height - 1;
  x1= max( 0, min( screenSize.x()-1, x1 ) );
  y1= max( 0, min( screenSize.y()-1, y1 ) );
  x2= max( 0, min( screenSize.x()-1, x2 ) );
  y2= max( 0, min( screenSize.y()-1, y2 ) );
  width = x2 - x1 + 1;
  height= y2 - y1 + 1;
  if( width <= 0 || height <= 0 )
    return;

  SystemWindow* rootWindow= graphicsSystem->getRootWindow();
  if( rootWindow == 0 )
    return;

  rootWindow->current();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho( 0., GLfloat(rootWindow->getSize().x()),
           0., GLfloat(rootWindow->getSize().y()), -1., 1. );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

/*
#ifdef __WINDOWS__
  glTranslatef( 0.5, 0.375, 0. );
#else
  glTranslatef( 0.375, 0.375, 0. );
#endif

  glShadeModel(GL_FLAT);
//  glShadeModel(GL_SMOOTH);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
*/

  data= new unsigned char [ width * height * 4 ];

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
  glPixelTransferf( GL_RED_BIAS,    0. );
  glPixelTransferf( GL_GREEN_BIAS,  0. );
  glPixelTransferf( GL_BLUE_BIAS,   0. );

  glDisable(GL_ALPHA_TEST);
  glDisable(GL_DITHER);

  glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );

  glReadBuffer(GL_FRONT);

  glFlush();
  glReadPixels( x1, y1, width, height,
                GL_RGBA, GL_UNSIGNED_BYTE,
                (void*) data );

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

void ScreenBlockBuffer::restore()
{
  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  if( data == 0 || graphicsSystem == 0 )
    return;

  SystemWindow* rootWindow= graphicsSystem->getRootWindow();
  if( rootWindow == 0 )
    return;

  putRec(rootWindow);
}

/////////////////////////////////////////////////////////////////////////////

// return true if block is completely inside this window
boolean ScreenBlockBuffer::putRec( SystemWindow* sw )
{
  if( sw == 0 )
    return false;

  IListIterator<SystemWindow> li(sw->systemWindows);
  while( ! li.eol() )
    {
    SystemWindow* child= li++;
    if( child == 0 )
      continue;

    if( putRec(child) )
      return true;
    }

  return put(sw);
}

/////////////////////////////////////////////////////////////////////////////

// return true if block is completely inside this window
boolean ScreenBlockBuffer::put( SystemWindow* sw )
{
  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  if( sw == 0 || data == 0 || graphicsSystem == 0 )
    return false;
  if( ! sw->mapped() )
    return false;

  IVector windowSize= sw->getSize();
  IPoint p1= sw->getAbsolutePosition();
  IVector screenSize= graphicsSystem->getScreenSize();
  p1[1]= screenSize.y() - 1 - (p1[1] + windowSize.y() - 1);
  IPoint p2= p1 + windowSize + IVector(-1,-1);

  if( p1.x() >= screenSize.x() || p2.x() < 0 ||
      p1.y() >= screenSize.y() || p2.y() < 0 )
    return false; // window is ouside the screen

  if( x2 < p1.x() || x1 > p2.x() || y2 < p1.y() || y1 > p2.y() )
    return false; // image is outside the window

  boolean inside= ( x1 >= p1.x() && x2 <= p2.x() &&
                  y1 >= p1.y() && y2 <= p2.y() );

  int block_x= x1;
  int block_y= y1;
  int block_width=  x2 - x1 + 1;
  int block_height= y2 - y1 + 1;
  int block_skip_pixels= p1.x() - x1;
  int block_skip_rows  = p1.y() - y1;

  if( block_skip_pixels <= 0 )
    block_skip_pixels= 0;
   else
    {
    block_x= p1.x();
    block_width -= block_skip_pixels;
    }

  if( block_skip_rows <= 0 )
    block_skip_rows= 0;
   else
    {
    block_y= p1.y();
    block_height -= block_skip_rows;
    }

  if( x2 > p2.x() )
    block_width -= x2 - p2.x();
  if( y2 > p2.y() )
    block_height-= y2 - p2.y();

  int block_row_length= x2 - x1 + 1;
  if( block_skip_pixels == 0 && block_skip_rows == 0 )
    block_row_length= 0;

  block_x -= p1.x();
  block_y -= p1.y();

#ifdef __WINDOWS__
  sw->current();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho( 0., GLfloat(sw->getSize().x()),
           0., GLfloat(sw->getSize().y()), -1., 1. );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
/*
#ifdef __WINDOWS__
  glTranslatef( 0.5, 0.375, 0. );
#else
  glTranslatef( 0.375, 0.375, 0. );
#endif

  glShadeModel(GL_FLAT);
//  glShadeModel(GL_SMOOTH);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
*/
#else
  sw->currentPixel();
#endif

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

  glDisable(GL_ALPHA_TEST);
  glDisable(GL_DITHER);

  glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );

  glDrawBuffer(GL_BACK);

  glRasterPos2i( block_x, block_y );
  glDrawPixels( block_width, block_height,
                GL_RGBA, GL_UNSIGNED_BYTE,
                (void*) data );
  sw->swapBuffers();
  glFlush();

  glPopAttrib();
  glPopAttrib();

  // Restore saved packing modes.
  glPixelStorei( GL_UNPACK_SWAP_BYTES, swapbytes );
  glPixelStorei( GL_UNPACK_LSB_FIRST, lsbfirst );
  glPixelStorei( GL_UNPACK_ROW_LENGTH, rowlength );
  glPixelStorei( GL_UNPACK_SKIP_ROWS, skiprows );
  glPixelStorei( GL_UNPACK_SKIP_PIXELS, skippixels );
  glPixelStorei( GL_UNPACK_ALIGNMENT, alignment );

  return inside;
}

/////////////////////////////////////////////////////////////////////////////

