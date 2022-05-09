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
  MLMotionAction.C

  User moves the mouse, while clicking a button. This action may be
  entered while pressing a button.
  Nothing drawn.

  Stephane Rehel

  December 8 1996
*/

#include "graphics/MLEvent.h"
#include "graphics/MLWindow.h"

//#include "scene/MLCamera.h"

#include "MLMotionAction.h"
#include "MLActionHandler.h"

/////////////////////////////////////////////////////////////////////////////

MLMotionAction::MLMotionAction()
{
  button= 0;
  window= 0;
  motionIndex= 0;
  delta_size= IVector(0,0);
  speed= Vector2(0,0);
  last_time= 0;
  _moving= false;
}

/////////////////////////////////////////////////////////////////////////////

MLMotionAction::~MLMotionAction()
{
  window= 0; // safer
}

/////////////////////////////////////////////////////////////////////////////

static int which_button( MLEvent& e )
{
  if( e.left_pressed  () ) return 1;
  if( e.middle_pressed() ) return 2;
  if( e.right_pressed () ) return 3;
  return 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLMotionAction::handleEvent( MLEvent& event )
{
  if( event.type != MLEvent::MOUSE_PRESS &&
      event.type != MLEvent::MOUSE_MOTION &&
      event.type != MLEvent::MOUSE_RELEASE )
    return false;

  if( event.window == 0 )
    return true;
  if( _moving && event.window != window )
    return true;

  // well, if mouse is already pressed while entering,
  // we loose ths first motion, but it's not important
  if( (!_moving && event.type==MLEvent::MOUSE_MOTION) ||
      event.type == MLEvent::MOUSE_PRESS )
    {
    if( event.type == MLEvent::MOUSE_PRESS && _moving )
      return false;

    motionIndex= 0;

    if( event.type == MLEvent::MOUSE_PRESS )
      button= event.button;
     else
      button= which_button(event);

    _moving= true;
    type= START;
    window= event.window;
    motionIndex= 0;
    delta_size= IVector(0,0);
    speed= Vector2(0,0);
    position1= event.position - window->getSize()/2;
    position2= position1;

    last_time= event.time;

    if( actionHandler != 0 )
      actionHandler->handleAction(this);

    return true;
    }

  if( event.type == MLEvent::MOUSE_MOTION )
    {
    ++motionIndex;
    type= MOVING;
    delta_size= IVector( event.size.x(), -event.size.y() );

    unsigned long delta= event.time - last_time; // milliseconds
    if( delta == 0 )
      delta= 1;
    position1= position2;
    position2= event.position - window->getSize()/2;
    speed= Vector2( DBL(delta_size.x()),
                    DBL(delta_size.y()) ) * (1000./DBL(delta));
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

