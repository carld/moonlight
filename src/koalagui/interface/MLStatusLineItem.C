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
  MLStatusLineItem.C

  Stephane Rehel

  Abstract class.

  July 1 1997
*/

#include "MLStatusLineItem.h"
#include "MLStatusLine.h"

#include "graphics/mlgl.h"
#include "graphics/SoftWindow.h"
#include "graphics/MLEvent.h"

#include "MLModule.h"

/////////////////////////////////////////////////////////////////////////////

MLStatusLineItem::MLStatusLineItem( MLModule* _module,
                                    MLStatusLine* _line,
                                    int _size_x,
                                    MLStatusLineItem::Side _side,
                                    boolean _expandable /* = false */ )
{
  module= _module;
  line= _line;
  size_x= _size_x;
  side= _side;
  expandable= _expandable;
  x_position= 0;

  window= 0;

  pressed= true;
  highlight_when_pressed= false;
  highlight_when_cursor_enter= false;
  cursor_highlighted= false;

  pressed_bg_color= Color(1,1,1) * (118./255.);

  line->addItem(this);
}

/////////////////////////////////////////////////////////////////////////////

// called by MLStatusLine::finalize()
void MLStatusLineItem::create()
{
  if( window != 0 )
    return;

  window= new SoftWindow;
  window->create( line->getWindow(),
                  x_position, 0, size_x, line->getHeight() );
  window->setEventHandler(this);
}

/////////////////////////////////////////////////////////////////////////////

MLStatusLineItem::~MLStatusLineItem()
{
  delete window;
  window= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineItem::flush()
{
  if( window == 0 )
    return;

  if( window->removeRedraw() )
    {
    draw();
    glFlush();
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineItem::setPressed( boolean _yes )
{
  if( (pressed?1:0) == (_yes?1:0) )
    return;
  pressed= _yes;
  postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineItem::postRedraw()
{
  if( window == 0 )
    return;
  if( ! window->mapped() )
    return;
  window->postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineItem::_draw()
{
  if( window == 0 )
    return;

  const IVector& size= window->getSize();

  if( pressed && highlight_when_pressed )
   {
//   mlMiddleGray();
   mlColor3f( pressed_bg_color.r(),
              pressed_bg_color.g(),
              pressed_bg_color.b() );
   }
  else
   {
   mlDarkGray();
   }

  mlRect( 1, 1, size.x()-2, size.y()-2 );

  if( pressed )
    { mlBlack(); }
   else
    { mlLightGray(); }
  mlLine( 0, size.y()-1, size.x()-1, size.y()-1 );
  mlLine( 0, 1, 0, size.y()-1 );
  if( pressed )
    { mlLightGray(); }
   else
    { mlBlack(); }
  mlLine( 0, 0, size.x()-1, 0 );
  mlLine( size.x()-1, 0, size.x()-1, size.y()-1 );
}

/////////////////////////////////////////////////////////////////////////////

boolean MLStatusLineItem::eventHandler( MLWindow* w, MLEvent& e )
{
  if( w != window || window == 0 )
    return false;

  if( e.type == MLEvent::REDRAW )
    {
    draw();
    return true;
    }

  if( highlight_when_cursor_enter &&
      (e.type == MLEvent::MOUSE_ENTER || e.type == MLEvent::MOUSE_LEAVE) )
    {
    boolean enter= (e.type == MLEvent::MOUSE_ENTER);
    if( module != 0 && module->inFunction() )
      enter= false;

    cursor_highlighted= enter;
    window->postRedraw();
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

// sent by MLStatusBars::handleMLKeyboard()
// return false if the key event must be handle by the interface
boolean MLStatusLineItem::handleMLKeyboard( MLEvent& )
{
  return false;
}

/////////////////////////////////////////////////////////////////////////////
