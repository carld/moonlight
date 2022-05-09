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
  MLMotionAction.h

  User moves the mouse, while clicking a button. This action may be
  entered while pressing a button.
  Nothing drawn.

  Stephane Rehel

  December 8 1996
*/

#ifndef __MLMotionAction_h
#define __MLMotionAction_h

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

#ifndef __Vector2_h
#include "tools/Vector2.h"
#endif

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

#ifndef __MLAction_h
#include "MLAction.h"
#endif

class MLWindow;
//class MLCamera;

/////////////////////////////////////////////////////////////////////////////

class MLMotionAction: public MLAction
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
  IVector delta_size;  // y is up
  IPoint position1;
  IPoint position2;
  Vector2 speed; // in pixels per second

protected:
  boolean _moving;
  unsigned long last_time;

public:
  MLMotionAction();
  virtual ~MLMotionAction();

  boolean moving() const
    {
    return _moving;
    }

  virtual boolean handleEvent( MLEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLMotionAction_h
