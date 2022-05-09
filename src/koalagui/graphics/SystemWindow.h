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
  SystemWindow.h

  System Window Class

  Stephane Rehel
  June 28, 1996
*/

#ifndef __SystemWindow_h
#define __SystemWindow_h

#ifndef __IList_h
#include "tools/IList.h"
#endif

#ifndef __MLWindow_h
#include "MLWindow.h"
#endif

class SoftWindow;
class MLEventManager;
class WindowDriver;
class ScreenBlockBuffer;
class MLCursor;

/////////////////////////////////////////////////////////////////////////////

class SystemWindow: public MLWindow
{
  friend class SoftWindow;
  friend class MLEventManager;
  friend class ScreenBlockBuffer; // access system children
  friend class GraphicsSystem; // access system children
  friend class WindowDriver; // access saveUnder flag

protected:
  WindowDriver* driver;
  SystemWindow* parent;
  IList<SystemWindow> systemWindows;
  boolean firstRedraw;

  boolean saveUnder;
  ScreenBlockBuffer* saveUnderBuffer;

public:
  SystemWindow();
  virtual ~SystemWindow();

  boolean isSystemWindow() const { return true; }

  WindowDriver* getDriver() const
    {
    return driver;
    }
  SystemWindow* getParent() const
    {
    return parent;
    }

  void setSaveUnder();

  // _parent may be NULL
  virtual boolean create( SystemWindow* _parent,
                          int x, int y, int width, int height );

  SystemWindow* newSystemChild( int x, int y, int width, int height );

  virtual void map( boolean yes = true );

  void createContext();
  void destroyContext();
  void current();
  void swapBuffers();

  void setPosition( const IPoint& );
  void setSize( const IVector& );

  void setCursor( MLCursor* cursor, boolean children_too = false );
  void setDefaultCursor();

  void pushWaitCursor();
  void popWaitCursor();

  void drawXORFrame( int x1, int y1, int x2, int y2, boolean undo );

  virtual IPoint getAbsPosition() const;

protected:
  virtual MLWindow* findCursorWindow( const IPoint& p );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __SystemWindow_h
