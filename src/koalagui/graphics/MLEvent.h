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
  MLEvent.h

  Stephane Rehel
  June 28, 1996
*/

#ifndef __MLEvent_h
#define __MLEvent_h

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

#ifndef __Key_h
#include "Key.h"
#endif

class MLWindow;

/////////////////////////////////////////////////////////////////////////////

class MLEvent
{
public:
  enum Type
    {
    FIRST= 0,
    MOUSE_PRESS,
    MOUSE_RELEASE,
    MOUSE_MOTION,
    MOUSE_ENTER,
    MOUSE_LEAVE,
    KEY_PRESS,
    KEY_RELEASE,
    REDRAW,
    CREATE,
    CLOSE,
    LAST
    };

  enum State
    {
    NULL_STATE=    0,
    LEFT_BUTTON=   1,
    MIDDLE_BUTTON= 2,
    RIGHT_BUTTON=  4,
    SHIFT=         8,
    CONTROL=       16,
    ALT=           32
    };

  MLWindow* window;
  unsigned long time; // milliseconds
  boolean privateEvent; // false if sent by XWindow

  Type type;
  IPoint position;
  IVector size;
  int button;
  Key key;

  int state;

  boolean left_pressed() const { return (state & int(LEFT_BUTTON)) != 0; }
  boolean middle_pressed() const { return (state & int(MIDDLE_BUTTON)) != 0; }
  boolean right_pressed() const { return (state & int(RIGHT_BUTTON)) != 0; }

  boolean shift_pressed() const { return (state & int(SHIFT)) != 0; }
  boolean control_pressed() const { return (state & int(CONTROL)) != 0; }
  boolean alt_pressed() const { return (state & int(ALT)) != 0; }

  void removeTyped( Type eventType );
  void removeTyped( MLWindow* window, Type eventType );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLEvent_h
