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
   VButtonBase.C

   Abstract Class

   Creation: SR, July 28th, 1995
   Revisions:
     + SR July 5 1996

*/

#include "tools/Chrono.h"
#include "graphics/MLEvent.h"

#include "VButtonBase.h"

#include "kw/kw.h"
#include "VWidgetEvent.h"
#include "VOptions.h"


/////////////////////////////////////////////////////////////////////////////

VButtonBase::VButtonBase( VWindow* _parent ): VWidget(_parent)
{
  state= RELEASED;
}

/////////////////////////////////////////////////////////////////////////////

VButtonBase::~VButtonBase()
{}

/////////////////////////////////////////////////////////////////////////////

void VButtonBase::create( int x, int y, int _width, int _height )
{
  width= _width;
  height= _height;

  VWindow::create( x, y, width, height );

  state= RELEASED;
}

/////////////////////////////////////////////////////////////////////////////

void VButtonBase::create( const IPoint& p1, const IPoint& p2 )
{
  // this will call VButtonBase::create(int,int,int,int) ...
  VWindow::create( p1, p2 );
}

/////////////////////////////////////////////////////////////////////////////

void VButtonBase::setSize( const IVector& newSize )
{
  width= newSize.x();
  height= newSize.y();
  VWindow::setSize(newSize);
}

/////////////////////////////////////////////////////////////////////////////

void VButtonBase::press()
{
  state= PRESSED;
  postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void VButtonBase::unpress()
{
  state= RELEASED;
  postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

boolean VButtonBase::handleEvent( MLEvent& event )
{
  if( event.type == MLEvent::REDRAW )
    {
    drawButton();
    return true;
    }

  if( ! isAvailable() )
    return VWindow::handleEvent(event);

  if( callPrivateWidgetEventHandler( VWidgetEvent::VMLEvent(this,event) ) )
    return true;

  switch( event.type )
    {
    case MLEvent::MOUSE_PRESS:
      {
      press();
      drawWidget();
      VWindow::flush();
      callPrivateWidgetEventHandler( VWidgetEvent::BeginPress(this) );

      Chrono::delay( vOptions->button.pressingDuration ); // sleeping
      break;
      }

    case MLEvent::MOUSE_RELEASE:
      {
      boolean ok= (state==PRESSED);
      if( ok )
        {
        unpress();
        drawWidget();
        callPrivateWidgetEventHandler( VWidgetEvent::EndPress(this) );

        kwCall(KWEvent::SELECT);
        }

      break;
      }

    case MLEvent::MOUSE_MOTION:
      {
      boolean into= VWindow::cursorInto(event);

      if( into && state==RELEASED )
        {
        press();
        drawWidget();
        }
       else
        {
        if( !into && state==PRESSED )
          {
          unpress();
          drawWidget();
          }
        }
      break;
      }

    default:
      return VWindow::handleEvent(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// protected
void VButtonBase::_drawButtonBase()
{
  switch( state )
    {
    case RELEASED:
      drawUnpressedRectangle( width, height );

      gray2();
      fillRectangle( IPoint(1,1), IPoint(width-3,height-3) );

      if( ! isAvailable() )
        {
        gray1();
        drawVertLine( width-1, 0, height-1 );
        drawHorizLine( 0, width-1, height-1 );
        }
      break;
    case PRESSED:
      drawPressedRectangle( width, height );

      gray2();
      fillRectangle( IPoint(2,2), IPoint(width-2,height-2) );
      break;
    }
}

/////////////////////////////////////////////////////////////////////////////

void VButtonBase::drawWidget()
{
  removeRedraw();
  drawButton();
}

/////////////////////////////////////////////////////////////////////////////

