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
  XMLCursor.C

  Stephane Rehel
  July 29 1997
*/

#include "XMLCursor.h"
#include "XGraphicsSystem.h"

/////////////////////////////////////////////////////////////////////////////

XMLCursor::XMLCursor()
{
  mask= shape= Pixmap(0);
  x_back_color.pixel= 0;
  x_fore_color.pixel= 0;
  cursor= Cursor(0);
}

/////////////////////////////////////////////////////////////////////////////

XMLCursor::~XMLCursor()
{
  destroy();
}

/////////////////////////////////////////////////////////////////////////////

void XMLCursor::destroy()
{
  if( cursor != Cursor(0) )
    {
//    DestroyCursor(cursor);
    cursor= Cursor(0);
    }

  x_back_color.pixel= 0;
  x_fore_color.pixel= 0;

  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();
  if( graphicsSystem != 0 )
    {
    XGraphicsSystem* xgs= (XGraphicsSystem*) graphicsSystem;
    if( xgs != 0 )
      {
      if( mask != Pixmap(0) )
        XFreePixmap( xgs->getDisplay(), mask );
      if( shape != Pixmap(0) )
        XFreePixmap( xgs->getDisplay(), shape );
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

void XMLCursor::build()
{
  destroy();
  if( ANDPlane == 0 || XORPlane == 0 )
    return;

  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();
  if( graphicsSystem == 0 )
    return;

  XGraphicsSystem* xgs= (XGraphicsSystem*) graphicsSystem;
  if( xgs == 0 )
    return;
  if( xgs->getDisplay() == 0 ) //|| xgs->getRootWindowID() == Window(0) )
    return;

  typedef unsigned short ColorComponent;

  x_fore_color.flags= DoRed | DoGreen | DoBlue;
  x_fore_color.red=   ColorComponent(foreground_color.r())
                     *ColorComponent(256);
  x_fore_color.green= ColorComponent(foreground_color.g())
                     *ColorComponent(256);
  x_fore_color.blue=  ColorComponent(foreground_color.b())
                     *ColorComponent(256);
  x_fore_color.pixel= 0;

  x_back_color.flags= DoRed | DoGreen | DoBlue;
  x_back_color.red=   ColorComponent(background_color.r())
                     *ColorComponent(256);
  x_back_color.green= ColorComponent(background_color.g())
                     *ColorComponent(256);
  x_back_color.blue=  ColorComponent(background_color.b())
                     *ColorComponent(256);
  x_back_color.pixel= 0;

  mask= XCreatePixmapFromBitmapData( xgs->getDisplay(),
                                     xgs->getXRootWindowID(),
//                                     xgs->getRootWindowID(),
                                     (char*) ANDPlane,
                                     width, height,
                                     0, 1, // fg, bg
                                     1 // depth
                                   );
  if( mask == Pixmap(0) )
    return;

  shape=XCreatePixmapFromBitmapData( xgs->getDisplay(),
                                     xgs->getXRootWindowID(),
//                                     xgs->getRootWindowID(),
                                     (char*) XORPlane,
                                     width, height,
                                     1, 0, // fg, bg
                                     1 // depth
                                   );
  if( shape == Pixmap(0) )
    return;

  cursor= XCreatePixmapCursor( xgs->getDisplay(),
                               shape,
                               mask,
                               &x_fore_color,
                               &x_back_color,
                               xHotSpot,
                               yHotSpot );
}

/////////////////////////////////////////////////////////////////////////////

void XMLCursor::getDefaultSize( int* width, int* height ) const
{
  if( width != 0 )
    *width = 16;
  if( height != 0 )
    *height= 16;
}

/////////////////////////////////////////////////////////////////////////////
