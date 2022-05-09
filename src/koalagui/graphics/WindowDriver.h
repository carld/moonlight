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
  WindowDriver.h

  Abstract Window Driver Class

  Stephane Rehel
  July 4, 1996
*/

#ifndef __WindowDriver_h
#define __WindowDriver_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

class SystemWindow;
class MLEvent;
class MLCursor;

/////////////////////////////////////////////////////////////////////////////

class WindowDriver
{
  friend class SystemWindow;

protected:
  SystemWindow* systemWindow; // associated system window
  MLCursor* cursor;
  int pushedWaitCursor;
  MLCursor* saveCursor;

public:
  WindowDriver( SystemWindow* _systemWindow );

  virtual ~WindowDriver();

  boolean hasSaveUnder() const;

  // _parent may be NULL
  virtual boolean create( SystemWindow* _parent,
                          int x, int y, int width, int height ) = 0;

  virtual void map( boolean yes = true ) = 0;
  virtual void createContext() = 0;
  virtual void destroyContext() = 0;
  virtual void current() = 0; // current for opengl-drawing
  virtual void swapBuffers() = 0;

  virtual void setPosition( const IPoint& ) = 0;
  virtual void setSize( const IVector& ) = 0;

  void setCursor( MLCursor* _cursor );

protected:
  // set cursor defined by WindowDriver::cursor
  virtual void _setCursor() = 0;

public:
  void pushWaitCursor();
  void popWaitCursor();

  virtual void drawXORFrame( int _x1, int _y1, int _x2, int _y2,
                             boolean undo ) = 0;

};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __WindowDriver_h
