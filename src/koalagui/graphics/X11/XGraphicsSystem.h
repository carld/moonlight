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
  XGraphicsSystem.h

  X Graphics System Class

  Stephane Rehel
  June 28, 1996
*/

#ifndef __XGraphicsSystem_h
#define __XGraphicsSystem_h

#ifndef __GraphicsSystem_h
#include "GraphicsSystem.h"
#endif

#include <X11/Xlib.h>
#include <X11/Xutil.h>

class WindowDriver;
class SystemWindow;
class MLGLPixmap;
class MLCursor;

/////////////////////////////////////////////////////////////////////////////

class XGraphicsSystem: public GraphicsSystem
{
public:
  XGraphicsSystem();
  virtual ~XGraphicsSystem();

  boolean init( MLConfig* config, MLCommandLine& options );

protected:
  void _iconify( boolean yes );

public:
  const MLString& getName();

  IVector getScreenSize() const;

  // in millimeters
  IVector getScreenDimemsion() const;

  int getDepth() const;

  void autoRepeat( boolean yes = true );
  void emitBeep();

  void setApplicationName( const char* name );

  Display* getDisplay() const;

  XVisualInfo* getVisualInfo() const;

  // parent can be NULL for root window
  Window createWindow( Window parent,
                       int x, int y, int width, int height,
                       boolean saveUnder );

  Window getRootWindowID() const; // root window if the application

  Window getXRootWindowID() const; // root window of X-Window system

  Colormap getColormap() const;

  WindowDriver* newWindowDriver( SystemWindow* sw ) const;
  MLGLPixmap* newMLGLPixmap() const;
  MLCursor* newMLCursor() const;

  // where is the cursor now?
  // returned in MLEvent::window, fills: MLEvent::position, MLEvent::state
  virtual MLWindow* getCursorWindow( MLEvent* event );

  // for the XEventManager::unblockWaiting() function
//  Display* getDisplay2() const;
//  Atom getUnblockProtocol() const;

private:
  boolean initOpenGL();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __GraphicsSystem_h

