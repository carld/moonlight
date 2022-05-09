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
  MLClickedMotionAction.h

  User clicks, drag, and unclick.
  Nothing drawn on the screen.

  Stephane Rehel

  December 8 1996
*/

#ifndef __MLClickedMotionAction_h
#define __MLClickedMotionAction_h

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

#ifndef __Vector2_h
#include "tools/Vector2.h"
#endif

#ifndef __MLAction_h
#include "MLAction.h"
#endif

class MLWindow;

/////////////////////////////////////////////////////////////////////////////

class MLClickedMotionAction: public MLAction
{
public:
  enum TYPE
    {
    START= 1,
    MOVING,
    END
    };

  // message sent:
  TYPE type;
  int button; // 1, 2, 3
  MLWindow* window;
  int motionIndex; // == 1 for first motion
  IPoint origin; // y is up
  IVector prev_size;  // y is up
  IVector size;  // y is up
  Vector2 speed; // in pixels per second

protected:
  boolean _moving;
  unsigned long last_time;
  IPoint last_point;

public:
  MLClickedMotionAction();
  virtual ~MLClickedMotionAction();

  boolean moving() const
    {
    return _moving;
    }

  virtual boolean handleEvent( MLEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLClickedMotionAction_h
