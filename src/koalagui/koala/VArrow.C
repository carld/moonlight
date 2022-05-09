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
   VArrow.C

   Creation: SR, July 28th, 1995
   Revisions:
     + SR July 5 1996
*/

#include "tools/Chrono.h"

#include "graphics/MLEvent.h"
#include "graphics/MLPixmaps.h"

#include "VWidgetEvent.h"
#include "VOptions.h"
#include "VArrow.h"


/////////////////////////////////////////////////////////////////////////////

VArrow::VArrow( VWindow* _parent ): VButtonBase(_parent)
{
  pixmap= 0;
  pixmapNA= 0;
}

/////////////////////////////////////////////////////////////////////////////

VArrow::~VArrow()
{}

/////////////////////////////////////////////////////////////////////////////

void VArrow::create( int x, int y, int _width, int _height,
                     int direction )
{
  switch( direction )
    {
    case UP: pixmap= MLPixmaps::upArrow;
             pixmapNA= MLPixmaps::upArrowGray; break;
    case DOWN: pixmap= MLPixmaps::downArrow;
               pixmapNA= MLPixmaps::downArrowGray; break;
    case RIGHT: pixmap= MLPixmaps::rightArrow;
                pixmapNA= MLPixmaps::rightArrowGray; break;
    case LEFT: pixmap= MLPixmaps::leftArrow;
               pixmapNA= MLPixmaps::leftArrowGray; break;

    default: pixmap= MLPixmaps::rightArrow;
             pixmapNA= MLPixmaps::rightArrowGray; break;
    }

  VButtonBase::create( x, y, _width, _height );
}

/////////////////////////////////////////////////////////////////////////////

void VArrow::drawButton()
{
  if( ! beginDraw() )
    return;

  VButtonBase::_drawButtonBase();

  int d= (state==RELEASED) ? 0 : 1;

  if( isAvailable() )
    drawPixmap( pixmap, 3+d, 3+d );
   else
    drawPixmap( pixmapNA, 3+d, 3+d );

  endDraw();
}

/////////////////////////////////////////////////////////////////////////////

boolean VArrow::handleEvent( MLEvent& event )
{
  if( event.type == MLEvent::REDRAW )
    {
    drawButton();
    return true;
    }

  if( ! isAvailable() )
    return VButtonBase::handleEvent(event);

  static boolean buttonPressed= false;

  if( callPrivateWidgetEventHandler( VWidgetEvent::VMLEvent(this,event) ) )
    return true;

  switch( event.type )
    {
    case MLEvent::MOUSE_PRESS:
      {
      buttonPressed= true;
      state= PRESSED;
      drawWidget();
      VWindow::flush();

      callPrivateWidgetEventHandler( VWidgetEvent::BeginPress(this) );

      Chrono::delay( vOptions->button.pressingDuration );

/*
      if( ! event.left_pressed() )
        break;

      // button 1 is still pressed
      Chrono::delay( vOptions->button.firstPressingDuration
                     -vOptions->button.pressingDuration );

      // while button 1 pressed
      while( getVDisplay()->getButtonsState().geti(0) )
        {
        callPrivateWidgetEventHandler( VWidgetEvent::RepeatPress(this) );
        getVDisplay()->delay( vOptions->button.repeatPressingDuration );
        }
*/

      break;
      }

    case MLEvent::MOUSE_RELEASE:
      {
      buttonPressed= false;
      state= RELEASED;
      drawWidget();

      callPrivateWidgetEventHandler( VWidgetEvent::EndPress(this) );
      break;
      }

    default:
      return VButtonBase::handleEvent(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void VArrow::drawWidget()
{
  removeRedraw();
  drawButton();
}

/////////////////////////////////////////////////////////////////////////////
