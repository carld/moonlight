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
  MLXORRectAction.C

  User defines a xor'ed area (click, drag, unclick)

  Stephane Rehel

  December 8 1996
*/

#include "MLXORRectAction.h"
#include "MLClickedMotionAction.h"

#include "graphics/SystemWindow.h"

/////////////////////////////////////////////////////////////////////////////

MLXORRectAction::MLXORRectAction( SystemWindow* _window )
{
  window= _window;
  motion= new MLClickedMotionAction;
  motion->setActionHandler(this);
}

/////////////////////////////////////////////////////////////////////////////

MLXORRectAction::~MLXORRectAction()
{
  if( moving() && window != 0 )
    {
    IPoint _p1= window->reverseY(p1);
    IPoint _p2= window->reverseY(p2);
    window->drawXORFrame( _p1.x(), _p1.y(), _p2.x(), _p2.y(),true );
    }

  delete motion;
  motion= 0;
  window= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLXORRectAction::moving() const
{
  return (motion==0) ? false : motion->moving();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLXORRectAction::handleEvent( MLEvent& event )
{
  return (motion==0) ? false : motion->handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLXORRectAction::handleAction( MLAction* action )
{
  if( action != motion || motion == 0 )
    return false;
  if( window != motion->window )
    return false;

  p1= motion->origin;
  p2= motion->origin + motion->size;

  if( motion->type == MLClickedMotionAction::MOVING )
    {
    if( motion->motionIndex > 1 )
      {
      IPoint pp1= motion->origin;
      IPoint pp2= motion->origin + motion->prev_size;
      pp1= window->reverseY(pp1);
      pp2= window->reverseY(pp2);
      window->drawXORFrame( pp1.x(), pp1.y(), pp2.x(), pp2.y(),true );
      }
    IPoint _p1= window->reverseY(p1);
    IPoint _p2= window->reverseY(p2);
    window->drawXORFrame( _p1.x(), _p1.y(), _p2.x(), _p2.y(),false );
    return true;
    }

  if( motion->type == MLClickedMotionAction::END )
    {
    IPoint _p1= window->reverseY(p1);
    IPoint _p2= window->reverseY(p2);
    window->drawXORFrame( _p1.x(), _p1.y(), _p2.x(), _p2.y(),true );

    button= motion->button;

    if( actionHandler != 0 )
      actionHandler->handleAction(this);

    return true;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
