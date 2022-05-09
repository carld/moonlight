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
  XWindowDriver.h

  X System Window

  Stephane Rehel
  June 29, 1996
*/

#ifndef __XWindowDriver_h
#define __XWindowDriver_h

#ifndef __WindowDriver_h
#include "graphics/WindowDriver.h"
#endif

#include <X11/Xlib.h>

#include "GL/glx.h"

class SystemWindow;
class XEventManager;

/////////////////////////////////////////////////////////////////////////////

class XWindowDriver: public WindowDriver
{
  friend class XEventManager;

protected:
  Display* display;
  Window window;
  GLXContext context;
  GC gc;

public:
  XWindowDriver( SystemWindow* _systemWindow );

  virtual ~XWindowDriver();

  // _parent may be NULL
  boolean create( SystemWindow* _parent,
                  int x, int y, int width, int height );

  void map( boolean yes = true );
  void createContext();
  void destroyContext();
  void current(); // current for opengl-drawing
  void swapBuffers();

  void setPosition( const IPoint& );
  void setSize( const IVector& );

  void setDefaultCursor();
  void setWaitCursor();

  Window getWindowID() const
    {
    return window;
    }

  static GLXContext getMainContext();

protected:
  // set cursor defined by WindowDriver::cursor
  void _setCursor();

  void drawXORFrame( int _x1, int _y1, int _x2, int _y2, boolean undo );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __XWindowDriver_h

