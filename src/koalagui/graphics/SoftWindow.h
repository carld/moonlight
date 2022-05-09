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
  SoftWindow.h

  Stephane Rehel
  June 30, 1996
*/

#ifndef __SoftWindow_h
#define __SoftWindow_h

#ifndef __MLWindow_h
#include "MLWindow.h"
#endif

class MLEventManager;
class SystemWindow;

/////////////////////////////////////////////////////////////////////////////

class SoftWindow: public MLWindow
{
  friend class MLEventManager;
  friend class MLWindow;

protected:
  SystemWindow* systemParent;
  IPoint absPosition;

  MLWindow* parent; // either SystemWindow or SoftWindow

public:
  SoftWindow();

  virtual ~SoftWindow();

  boolean isSystemWindow() const { return false; }

  // _parent != 0
  virtual boolean create( MLWindow* _parent,
                        int x, int y, int width, int height );

  virtual void map( boolean yes = true );
  void current(); // current for opengl-drawing

  void setPosition( const IPoint& );
  void setSize( const IVector& );

private:
  void adjustAbsPosition( const IVector& delta );

public:
  SystemWindow* getSystemParent() const
    {
    return systemParent;
    }

  virtual IPoint getAbsPosition() const
    {
    return absPosition;
    }

  void scissor();
  void unscissor();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __SoftWindow_h
