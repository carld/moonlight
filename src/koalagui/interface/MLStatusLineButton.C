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
  MLStatusLineButton.C

  Stephane Rehel
  August 1 1997
*/

#include "graphics/MLEvent.h"
#include "graphics/MLWindow.h"
#include "graphics/SoftWindow.h"

#include "MLActionHandler.h"

#include "MLStatusLineButton.h"

/////////////////////////////////////////////////////////////////////////////

MLStatusLineButton::MLStatusLineButton( MLModule* module,
                                        MLStatusLine* _line, int n_chars,
                                        MLStatusLineItem::Side _side,
                                        boolean _expandable /* = false */ ):
  MLStatusLineText( module, _line, n_chars, _side, _expandable )
{
  MLStatusLineItem::highlight_when_pressed= true;
  MLStatusLineItem::highlight_when_cursor_enter= true;

  MLStatusLineText::text_offset_if_pressed= true;
  one_time_press= false;
}

/////////////////////////////////////////////////////////////////////////////

MLStatusLineButton::~MLStatusLineButton()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLStatusLineButton::eventHandler( MLWindow* w, MLEvent& event )
{
  if( w == 0 || w != window )
    return MLStatusLineText::eventHandler(w,event);

  static int init_pressed= 0;
  boolean into= window->into(event.position);

  switch( event.type )
    {
    case MLEvent::MOUSE_PRESS:
      {
      if( event.button != 1 )
        break;

      init_pressed= pressed;
      if( init_pressed && one_time_press )
        break;

      MLStatusLineItem::setPressed( !init_pressed );
      return true;
      }

    case MLEvent::MOUSE_MOTION:
      {
      if( ! event.left_pressed() )
        break;
      if( init_pressed && one_time_press )
        break;

      MLStatusLineItem::setPressed( init_pressed ? !into : into );
      return true;
      }

    case MLEvent::MOUSE_RELEASE:
      {
      if( event.button != 1 )
        break;
      if( init_pressed && one_time_press )
        break;

//      setPressed(init_pressed);

      if( into && MLAction::actionHandler != 0 )
        MLAction::actionHandler->handleAction(this);

      return true;
      }

    default:
      break;
    }

  return MLStatusLineText::eventHandler(w,event);
}

/////////////////////////////////////////////////////////////////////////////
