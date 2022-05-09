// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//                 2002       Patrick Mauritz
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
  XWindowDriver.C

  X System Window

  Stephane Rehel
  June 29, 1996
*/

#include <iostream>

#include "tools/SIList.h"

#include "SystemWindow.h"

#include "XWindowDriver.h"
#include "XGraphicsSystem.h"
#include "XMLCursor.h"

#include "GL/opengl.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////

static XGraphicsSystem* xGraphicsSystem= 0;
static SIList<GLXContext> shareListContext;
static GLXContext mainContext= GLXContext(0);

/////////////////////////////////////////////////////////////////////////////

XWindowDriver::XWindowDriver( SystemWindow* _systemWindow ):
  WindowDriver(_systemWindow)
{
  if( xGraphicsSystem == 0 )
    {
    GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

    assert( graphicsSystem != 0 );
    xGraphicsSystem= (XGraphicsSystem*) graphicsSystem;
    }

  display= xGraphicsSystem->getDisplay();
  window= Window(0);
  context= GLXContext(0);
  gc= GC(0);
}

/////////////////////////////////////////////////////////////////////////////

XWindowDriver::~XWindowDriver()
{
  destroyContext();

  if( display != 0 && gc != GC(0) )
    {
    XFreeGC( display, gc );
    gc= GC(0);
    }

  if( window != Window(0) && display != 0 )
    {
    XSync(display,False);
    XDestroyWindow( display, window );
    XSync(display,False);

    window= Window(0);
    }

  display= 0;
}

/////////////////////////////////////////////////////////////////////////////

// _parent may be NULL
boolean XWindowDriver::create( SystemWindow* _parent,
                               int x, int y, int width, int height )

{
  assert( xGraphicsSystem != 0 );

  if( window != Window(0) )
    return false;

  if( systemWindow == 0 )
    return false;

  Window parentID= Window(0);
  if( _parent != 0 )
    parentID= ((XWindowDriver*) _parent->getDriver())->window;

  window= xGraphicsSystem->createWindow( parentID,
                                         x, y, width, height,
                                         hasSaveUnder() );
  if( window == Window(0) )
    return false;

  gc= XCreateGC( display, window, 0, 0 );

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void XWindowDriver::createContext()
{
  if( display == 0 || window == Window(0) )
    return;

  if( context != GLXContext(0) )
    return;

  context= glXCreateContext( display,
                             xGraphicsSystem->getVisualInfo(),
                             0, // shareList
			    // never direct - direct rendering doesn't 
			    // like the offscreen buffers we use
			    // thus GL_FALSE
                             GL_FALSE 
                           );

  if( context == GLXContext(0) )
    return;

  if( ::mainContext == GLXContext(0) )
    ::mainContext= context;
}

/////////////////////////////////////////////////////////////////////////////

void XWindowDriver::destroyContext()
{
  if( context == GLXContext(0) || display == 0 )
    return;

/*
  if( glXGetCurrentContext() == context &&
      glXGetCurrentDrawable() == window )
    glXMakeCurrent( display, Drawable(0), GLXContext(0) );
*/

  glXDestroyContext( display, context );

  SIListIterator<GLXContext> li(shareListContext);
  while( ! li.eol() )
    {
    GLXContext* c= li.next();
    if( *c == context )
      {
      shareListContext.remove(c);
      delete c;
      break;
      }
    }

  context= GLXContext(0);
}

/////////////////////////////////////////////////////////////////////////////

void XWindowDriver::current() // current for opengl-drawing
{
  GLXContext ctx= (context==GLXContext(0)) ? ::mainContext : context;

#ifdef DEBUG
  cerr << "XWindowDriver::current() calling glXMakeCurrent()" << endl;
#endif
  glXMakeCurrent( display, window, ctx );
  glDrawBuffer(GL_FRONT);

//if( xGraphicsSystem->getDepth() > 8 )
//  glDisable(GL_DITHER);
}

/////////////////////////////////////////////////////////////////////////////

void XWindowDriver::swapBuffers()
{
  glXSwapBuffers( display, window );
}

/////////////////////////////////////////////////////////////////////////////

void XWindowDriver::map( boolean yes /* = true */ )
{
  if( display == 0 || window == Window(0) )
    return;

  if( yes )
    {
    XMapWindow( display, window );
    if( xGraphicsSystem->getRootWindow()->getDriver() == this )
      XSetInputFocus( display, window, RevertToPointerRoot, CurrentTime );
    }
   else
    XUnmapWindow( display, window );
//  XSync( display, False );
}

/////////////////////////////////////////////////////////////////////////////

/*
void XWindowDriver::setDefaultCursor()
{
  Cursor cursor= xGraphicsSystem->getDefaultCursor();
  if( cursor == Cursor(0) )
    return;

  XSetWindowAttributes w_attr;
  w_attr.cursor= cursor;
  XChangeWindowAttributes( display, window, CWCursor, &w_attr );
}

/////////////////////////////////////////////////////////////////////////////

void XWindowDriver::setWaitCursor()
{
  Cursor cursor= xGraphicsSystem->getWaitCursor();
  if( cursor == Cursor(0) )
    return;

  XSetWindowAttributes w_attr;
  w_attr.cursor= cursor;
  XChangeWindowAttributes( display, window, CWCursor, &w_attr );
}
*/

/////////////////////////////////////////////////////////////////////////////

void XWindowDriver::setPosition( const IPoint& newPosition )
{
  if( display == 0 || window == Window(0) )
    return;

  XMoveWindow( display, window, newPosition.x(), newPosition.y() );
}

/////////////////////////////////////////////////////////////////////////////

void XWindowDriver::setSize( const IVector& newSize )
{
  if( display == 0 || window == Window(0) )
    return;

  XResizeWindow( display, window, newSize.x(), newSize.y() );
}

/////////////////////////////////////////////////////////////////////////////

// static
GLXContext XWindowDriver::getMainContext()
{
  return ::mainContext;
}

/////////////////////////////////////////////////////////////////////////////

// set cursor defined by WindowDriver::cursor
void XWindowDriver::_setCursor()
{
  if( cursor == 0 )
    return;

  XMLCursor* xcursor= (XMLCursor*) cursor;
  Cursor c= xcursor->get();

  if( c == Cursor(0) )
    return;

  if( display == 0 || window == Window(0) )
    return;

  XDefineCursor( display, window, c );
}

/////////////////////////////////////////////////////////////////////////////

void XWindowDriver::drawXORFrame( int _x1, int _y1, int _x2, int _y2,
                                  boolean undo )
{
  if( display == 0 || window == Window(0) || gc == GC(0) )
    return;

  XSetFunction( display, gc, GXxor );

  static int first= 1;
  static unsigned long xor1Pixel= 0;
  static unsigned long xor2Pixel= 0;

  if( first )
    {
    first= 0;
    XColor color;

    color.flags= DoRed | DoGreen | DoBlue;
    Colormap colormap= xGraphicsSystem->getColormap();

    switch( xGraphicsSystem->getDepth() )
      {
      case 8:
        {
        xor1Pixel= 112; // mesa private colormap for gray 0x7f7f * 3
        color.red= color.green= color.blue= 0xffff;
        if( XAllocColor( display, colormap, &color ) )
          xor2Pixel= color.pixel;
         else
          xor2Pixel= 255;
        break;
        }
      case 16:
        {
        color.red= color.green= color.blue= 0x8617; //65535 / 2;
        if( XAllocColor( display, colormap, &color ) )
          xor1Pixel= color.pixel;
         else
          xor1Pixel= 255;
        xor2Pixel= WhitePixel(display,DefaultScreen(display));
        break;
        }
      case 24:
      default:
        {
        color.red= color.green= color.blue= 0x8080;
        if( XAllocColor( display, colormap, &color ) )
          xor1Pixel= color.pixel;
         else
          xor1Pixel= 255;
        xor2Pixel= WhitePixel(display,DefaultScreen(display));
        break;
//    color.red= 0x8618;
//    color.green= 0x8208;
//    color.blue= 0x8618;
        }
      }
    }

  int x1= min( _x1, _x2 );
  int x2= max( _x1, _x2 );
  int y1= min( _y1, _y2 );
  int y2= max( _y1, _y2 );

  XSetForeground( display, gc, undo ? xor2Pixel : xor1Pixel );
  XDrawRectangle( display, window, gc, x1, y1, x2-x1+1, y2-y1+1 );

  XSetForeground( display, gc, undo ? xor1Pixel : xor2Pixel );
  XDrawRectangle( display, window, gc, x1, y1, x2-x1+1, y2-y1+1 );
}

/////////////////////////////////////////////////////////////////////////////
