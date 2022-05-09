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
  MLWindow.h

  Abstract Window (either SoftWindow or SystemWindow)

  Stephane Rehel
  June 28, 1996
*/

#ifndef __MLWindow_h
#define __MLWindow_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __IList_h
#include "tools/IList.h"
#endif

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

class MLEvent;
class MLEventManager;
class MLEventHandler;

class SoftWindow;
class SystemWindow;

/////////////////////////////////////////////////////////////////////////////

class MLWindow
{
  friend class MLEventManager;
  friend class SoftWindow;

protected:
  IList<SoftWindow> softChildren; // only SoftWindow children

private:
  MLEventHandler* _eventHandler;

protected:
  IPoint position;
  IVector size;
  boolean isMapped;
  boolean mousePointerInto;

public:
  MLWindow();

  virtual ~MLWindow();

  SoftWindow* newSoftChild( int x, int y, int width, int height );

  void setEventHandler( MLEventHandler* __eventHandler );

  virtual void redraw()
    {}

  // return true if handled
  virtual boolean handleEvent( const MLEvent& );

  virtual boolean isSystemWindow() const =0;
  SystemWindow* getSystemWindow() const;

  boolean isSoftWindow() const
    {
    return ! isSystemWindow();
    }

  boolean mapped() const
    {
    return isMapped;
    }
  virtual void map( boolean yes = true ) = 0;

  virtual void current() =0; // current for opengl-drawing
  virtual void swapBuffers()
    {}

  virtual void setPosition( const IPoint& ) = 0;
  virtual void setSize( const IVector& ) = 0;

  const IVector& getSize() const
    {
    return size;
    }
  const IPoint& getPosition() const
    {
    return position;
    }
  IPoint getAbsolutePosition() const;
  IPoint reverseY( const IPoint& p ) const
    {
    return IPoint( p.x(), size.y()-1-p.y() );
    }

  int x1() const { return position.x(); }
  int y1() const { return position.y(); }
  int x2() const { return position.x()+size.x()-1; }
  int y2() const { return position.y()+size.y()-1; }

  boolean into( const IPoint& p ) const;

  void pixelMode();
  void clear( float r, float g, float b );

  void currentPixel()
    {
    current();
    pixelMode();
    }

  void postRedraw( boolean ignore_unmapped = false );
  boolean removeRedraw();

  virtual IPoint getAbsPosition() const = 0;

  boolean hasMousePointer() const
    {
    return mousePointerInto;
    }

protected:
  virtual boolean create( int x, int y, int width, int height );

  virtual MLWindow* findCursorWindow( const IPoint& p );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLWindow_h
