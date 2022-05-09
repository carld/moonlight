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
  WGraphicsSystem.h

  Windows 95/NT Graphics System Class

  Stephane Rehel
  July 6 1997
*/

#ifndef __WGraphicsSystem_h
#define __WGraphicsSystem_h

#ifndef __GraphicsSystem_h
#include "GraphicsSystem.h"
#endif

#include "mlwindows.h"

class MLCursor;
class SystemWindow;

/////////////////////////////////////////////////////////////////////////////

class WGraphicsSystem: public GraphicsSystem
{
  friend class WWindowDriver;
  friend class WMLGLPixmap; // calls initDIBOpenGL()
public:
  WGraphicsSystem();
  virtual ~WGraphicsSystem();

  boolean init( MLConfig* config, MLCommandLine& options );

protected:
  void _iconify( boolean yes );

public:
  const MLString& getName();

  IVector getScreenSize() const;

  // in millimeters
  IVector getScreenDimemsion() const;

  HINSTANCE getInstance() const;

  int getDepth() const;

  void autoRepeat( boolean yes = true );
  void emitBeep();

  void setApplicationName( const char* name );

  // parent can be NULL for root window
  HWND createWindow( HWND parent,
                     int x, int y, int width, int height,
                     boolean saveUnder );

  HWND getRootWindowID() const;

  HPALETTE getColormap() const;

  WindowDriver* newWindowDriver( SystemWindow* sw ) const;
  MLGLPixmap* newMLGLPixmap() const;
  MLCursor* newMLCursor() const;

  // where is the cursor now?
  // returned in MLEvent::window, fills: MLEvent::position, MLEvent::state
  MLWindow* getCursorWindow( MLEvent* event );

private:
  boolean initOpenGL( HDC hDC );
  void setupPalette( HDC hDC );

  boolean initDIBOpenGL( HDC hDC );
  void setupDIBPalette( HDC hDC );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __WGraphicsSystem_h

