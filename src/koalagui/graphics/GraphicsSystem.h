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
  GraphicsSystem.h

  Abstract graphics system Class

  Stephane Rehel
  June 28, 1996
*/

#ifndef __GraphicsSystem_h
#define __GraphicsSystem_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLCommandLine;
class MLWindow;
class SystemWindow;
class WindowDriver;
class MLGLPixmap;
class MLCursor;
class Color;
class Color8;
class MLGammaCorrection;
class MLConfig;
class MLEvent;

extern unsigned char gray_level_0,
                     gray_level_1,
                     middle_gray,
                     gray_level_2,
                     gray_level_3;

/////////////////////////////////////////////////////////////////////////////

class GraphicsSystem
{
private:
  static GraphicsSystem* graphicsSystem; // THE graphics system

protected:
  MLString rc_group; // in resource file

  SystemWindow* rootWindow;
  boolean reallyFullScreen; // set by init() in XGraphicsSystem/WGraphicsSystem

  MLGammaCorrection* gammaCorrection;

  boolean _doRedrawEvents;
  boolean _supportSaveUnder;

  boolean _isIconic;

public:
  GraphicsSystem();
  virtual ~GraphicsSystem();

  // should create the rootWindow
  virtual boolean init( MLConfig* config, MLCommandLine& options );

  boolean isReallyFullScreen() const
    {
    return reallyFullScreen;
    }

  MLGammaCorrection* getGammaCorrection() const
    {
    return gammaCorrection;
    }
  boolean hasGammaCorrection() const;

  static GraphicsSystem* getGraphicsSystem()
    {
    return graphicsSystem;
    }

  boolean doRedrawEvents() const
    {
    return _doRedrawEvents;
    }
  boolean supportSaveUnder() const
    {
    return _supportSaveUnder;
    }

  boolean isIconic() const
    {
    return _isIconic;
    }
  void iconify( boolean yes );

protected:
  virtual void _iconify( boolean yes ) = 0;

public:
  virtual const MLString& getName() =0;

  virtual IVector getScreenSize() const =0;

  // in millimeters
  virtual IVector getScreenDimemsion() const =0;

  virtual int getDepth() const =0;

  virtual void autoRepeat( boolean yes = true ) =0;
  virtual void emitBeep() =0;

  SystemWindow* newFullScreenWindow();

  SystemWindow* getRootWindow() const
    {
    return rootWindow;
    }

  boolean initDefaultLists();

  virtual void setApplicationName( const char* name );

  virtual WindowDriver* newWindowDriver( SystemWindow* sw ) const = 0;
  virtual MLGLPixmap* newMLGLPixmap() const = 0;
  virtual MLCursor* newMLCursor() const = 0;

  void refreshScreen();
  void invalidateRect( SystemWindow* sw );

  // where is the cursor now?
  // returned in MLEvent::window, fills: MLEvent::position, MLEvent::state
  virtual MLWindow* getCursorWindow( MLEvent* event ) = 0;

private:
  void _invalidateRect( const IPoint& p1, const IPoint& p2,
                        SystemWindow* sw );

protected:
  MLWindow* findCursorWindow( const IPoint& p );

public:
  void pushWaitCursor();
  void popWaitCursor();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __GraphicsSystem_h

