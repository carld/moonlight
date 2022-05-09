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
  MLStatusLineItem.h

  Stephane Rehel

  Abstract Class.

  July 1 1997
*/

#ifndef __MLStatusLineItem_h
#define __MLStatusLineItem_h

#ifndef __Color_h
#include "tools/Color.h"
#endif

#ifndef __MLEventHandler_h
#include "graphics/MLEventHandler.h"
#endif

#ifndef __MLKeyboardHandler_h
#include "graphics/MLKeyboardHandler.h"
#endif

class SoftWindow;
class MLStatusLine;
class MLModule;

/////////////////////////////////////////////////////////////////////////////

class MLStatusLineItem: public MLEventHandler, public MLKeyboardHandler
{
  friend class MLStatusLine;

public:
  enum Side { LEFT= 1, RIGHT };

protected:
  MLModule* module;

  SoftWindow* window;
  MLStatusLine* line;
  Side side;
  boolean expandable;
  int size_x;
  int x_position;

  boolean pressed;
  boolean highlight_when_pressed;
  boolean highlight_when_cursor_enter;
  boolean cursor_highlighted;

  Color pressed_bg_color;

public:
  MLStatusLineItem( MLModule* _module,
                    MLStatusLine* _line,
                    int _size_x,
                    Side _side,
                    boolean _expandable = false );

  virtual ~MLStatusLineItem();

  void flush();

  void setPressed( boolean _yes );
  boolean getPressed() const
    {
    return pressed;
    }

private:
  // called by MLStatusLine::finalize()
  void create();

  void postRedraw();

  virtual void draw() = 0;

protected:
  virtual void _draw();

  virtual boolean eventHandler( MLWindow*, MLEvent& );

  // sent by MLStatusLine::handleMLKeyboard()
  // return false if the key event must be handle by the interface
  virtual boolean handleMLKeyboard( MLEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLStatusLineItem_h
