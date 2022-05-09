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
  MLCanvasAction.C

  Created by an engine, associated with a canvas title, drawn and
  clickable on a canvas title

  Stephane Rehel

  November 27 1996
*/

#include "graphics/SoftWindow.h"
#include "graphics/MLEvent.h"

#include "MLCanvasAction.h"
#include "MLCanvas.h"
#include "MLModule.h"

/////////////////////////////////////////////////////////////////////////////

MLCanvasAction::MLCanvasAction( MLCanvas* _canvas )
{
  canvas= _canvas;
  left= 1;
  window= 0;
  x= 0;
  width= 0;

  pressed= false;
  highlight_when_cursor_enter= false;
  cursor_highlighted= false;
}

/////////////////////////////////////////////////////////////////////////////

MLCanvasAction::~MLCanvasAction()
{
  destroyWindow();
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasAction::map( boolean yes /* = true */ )
{
  if( window != 0 )
    window->map(yes);
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasAction::destroyWindow()
{
  delete window;
  window= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLCanvasAction::eventHandler( MLWindow* w, MLEvent& e )
{
  if( w != window )
    return false;

  if( e.type == MLEvent::REDRAW )
    {
    draw();
    return true;
    }

  if( e.type == MLEvent::MOUSE_ENTER ||
      e.type == MLEvent::MOUSE_LEAVE )
    {
    boolean enter= (e.type == MLEvent::MOUSE_ENTER);

    if( canvas != 0 &&
        canvas->getModule() != 0 &&
        canvas->getModule()->inFunction() )
      enter= false;

    if( highlight_when_cursor_enter )
      {
      cursor_highlighted= enter;
      if( ! pressed )
        window->postRedraw();
      }
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////
