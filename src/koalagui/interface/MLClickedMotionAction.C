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
  MLClickedMotionAction.C

  User clicks, drag, and unclick.
  Nothing drawn on the screen.

  Stephane Rehel

  December 8 1996
*/

#include "graphics/MLEvent.h"
#include "graphics/MLWindow.h"

#include "MLClickedMotionAction.h"
#include "MLActionHandler.h"

/////////////////////////////////////////////////////////////////////////////

MLClickedMotionAction::MLClickedMotionAction()
{
  button= 0;
  window= 0;
  motionIndex= 0;
  origin= IPoint(0,0);
  prev_size= IVector(0,0);
  size= IVector(0,0);
  speed= Vector2(0,0);
  last_point= IPoint(0,0);
  last_time= 0;
  _moving= false;
}

/////////////////////////////////////////////////////////////////////////////

MLClickedMotionAction::~MLClickedMotionAction()
{
  window= 0; // safer
}

/////////////////////////////////////////////////////////////////////////////

boolean MLClickedMotionAction::handleEvent( MLEvent& event )
{
  if( event.type != MLEvent::MOUSE_PRESS &&
      event.type != MLEvent::MOUSE_MOTION &&
      event.type != MLEvent::MOUSE_RELEASE )
    return false;

  if( event.window == 0 )
    return true;
  if( _moving && event.window != window )
    return true;

  if( event.type == MLEvent::MOUSE_PRESS )
    {
    if( _moving )
      return true;

    button= event.button;
    _moving= true;
    type= START;
    window= event.window;
    motionIndex= 0;
    origin= window->reverseY(event.position);
    prev_size= size= IVector(0,0);
    speed= Vector2(0,0);

    last_time= event.time;
    last_point= origin;

    if( actionHandler != 0 )
      actionHandler->handleAction(this);

    return true;
    }

  if( event.type == MLEvent::MOUSE_MOTION )
    {
    if( ! _moving )
      return true;

    ++motionIndex;
    type= MOVING;
    prev_size= size;
    IPoint position= window->reverseY(event.position);
    size= position - origin;

    unsigned long delta= event.time - last_time; // milliseconds
    if( delta == 0 )
      delta= 1;
    speed= Vector2( DBL(last_point.x()-position.x()),
                    DBL(last_point.y()-position.y()) ) * (1000./DBL(delta));
    last_point= position;
    last_time= event.time;

    if( actionHandler != 0 )
      actionHandler->handleAction(this);

    return true;
    }

  if( event.type == MLEvent::MOUSE_RELEASE )
    {
    if( ! _moving )
      return true;
    if( event.button != button )
      return true;

    type= END;

    if( actionHandler != 0 )
      actionHandler->handleAction(this);

    _moving= false;
    return true;
    }

  return true; // never reached
}

/////////////////////////////////////////////////////////////////////////////

