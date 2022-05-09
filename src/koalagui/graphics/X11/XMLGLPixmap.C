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
  XMLGLPixmap.C

  Stephane Rehel
  July 22 1996
*/

#include <iostream>

#include "GL/opengl.h"

#include "MLWindow.h"
#include "SystemWindow.h"
#include "SoftWindow.h"
#include "GraphicsSystem.h"

#include "XGraphicsSystem.h"
#include "XWindowDriver.h"
#include "XMLGLPixmap.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////

static XGraphicsSystem* xgs= 0;

/////////////////////////////////////////////////////////////////////////////

XMLGLPixmap::XMLGLPixmap()
{
  if( xgs == 0 )
    {
    GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();
    xgs= (XGraphicsSystem*) graphicsSystem;
    }

  pm= Pixmap(0);
  glxpm= GLXPixmap(0);
}

/////////////////////////////////////////////////////////////////////////////

XMLGLPixmap::~XMLGLPixmap()
{
  if( glxpm != GLXPixmap(0) )
    {
    glXDestroyGLXPixmap( xgs->getDisplay(), glxpm );
    glxpm= GLXPixmap(0);
    }

  if( pm != Pixmap(0) )
    {
    XFreePixmap(xgs->getDisplay(),pm);
    pm= Pixmap(0);
    }
}

/////////////////////////////////////////////////////////////////////////////

void XMLGLPixmap::create( int width, int height )
{
  width= max( 1, width );
  height= max( 1, height );
  MLGLPixmap::size= IVector(width,height);

  Display* display= xgs->getDisplay();

  XWindowDriver* wd= (XWindowDriver*) xgs->getRootWindow()->getDriver();
  Window systemRoot= wd->getWindowID();
  pm= XCreatePixmap( display,
                     systemRoot,
                     width, height,
                     xgs->getVisualInfo()->depth );
  if( pm == Pixmap(None) )
    return;

  glxpm= glXCreateGLXPixmap( display,
                             xgs->getVisualInfo(),
                             pm );
}

/////////////////////////////////////////////////////////////////////////////

void XMLGLPixmap::current()
{
  if( glxpm == GLXPixmap(0) )
    return;

#ifdef DEBUG
  cerr << "XMLGLPixmap::current() calling glXMakeCurrent()" << endl;
#endif
  glXMakeCurrent( xgs->getDisplay(), glxpm, XWindowDriver::getMainContext() );

  glViewport( 0, 0, size.x(), size.y() );
  glDrawBuffer(GL_FRONT);
}

/////////////////////////////////////////////////////////////////////////////

void XMLGLPixmap::print( MLWindow* dest,
                      int src_x, int src_y,
                      int width, int height,
                      int dest_x, int dest_y )
{
  if( dest == 0 || glxpm == GLXPixmap(0) )
    return;

  SystemWindow* s_dest;
  if( dest->isSystemWindow() )
    s_dest= (SystemWindow*) dest;
   else
    {
    s_dest= ((SoftWindow*) dest)->getSystemParent();
    const IPoint& absPosition= ((SoftWindow*) dest)->getAbsPosition();
    dest_x+= absPosition.x();
    dest_y+= absPosition.y();
    }

  Window w_dest= ((XWindowDriver*)(s_dest->getDriver()))->getWindowID();

  Display* display= xgs->getDisplay();
  GC gc= XCreateGC( display, w_dest, 0, 0 );
  if( gc == GC(None) )
    return;

  XCopyArea( display,
             pm, w_dest,  // src, dest
             gc,
             src_x, src_y,
             width, height,
             dest_x, dest_y );

  XFreeGC( display, gc );
}

/////////////////////////////////////////////////////////////////////////////

void XMLGLPixmap::print( MLGLPixmap* _dest,
                      int src_x, int src_y,
                      int width, int height,
                      int dest_x, int dest_y )
{
  if( _dest == 0 || glxpm == GLXPixmap(0) )
    return;

  XMLGLPixmap* dest= (XMLGLPixmap*) _dest;

  if( dest->glxpm == GLXPixmap(0) )
    return;

  Display* display= xgs->getDisplay();
  GC gc= XCreateGC( display, dest->pm, 0, 0 );
  if( gc == GC(None) )
    return;

  XCopyArea( display,
             pm, dest->pm,  // src, dest
             gc,
             src_x, src_y,
             width, height,
             dest_x, dest_y );

  XFreeGC( display, gc );
}

/////////////////////////////////////////////////////////////////////////////

