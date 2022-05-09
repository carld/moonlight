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
  MLXORRectAction.h

  User defines a xor'ed area (click, drag, unclick)

  Stephane Rehel

  December 8 1996
*/

#ifndef __MLXORRectAction_h
#define __MLXORRectAction_h

#ifndef __MLAction_h
#include "MLAction.h"
#endif

#ifndef __MLActionHandler_h
#include "MLActionHandler.h"
#endif

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

class MLClickedMotionAction; // actuuuce
class SystemWindow;

/////////////////////////////////////////////////////////////////////////////

class MLXORRectAction: public MLAction, public MLActionHandler
{
public:
  // message sent:
  int button; // 1, 2, 3
  IPoint p1, p2; // careful: they could be reversed

protected:
  MLClickedMotionAction* motion;
  SystemWindow* window;

public:
  MLXORRectAction( SystemWindow* _window );
  virtual ~MLXORRectAction();

  boolean moving() const;

  virtual boolean handleEvent( MLEvent& event );

public:
  virtual boolean handleAction( MLAction* action );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLXORRectAction_h
