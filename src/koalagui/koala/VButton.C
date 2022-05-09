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
   VButton.C

   Creation: SR, July 26th, 1995
   Revisions:
     + SR July 5 1996

*/

#include "tools/Chrono.h"

#include "graphics/MLEvent.h"
#include "graphics/MLFont.h"
#include "graphics/KeyCodes.h"
#include "graphics/MLPixmap.h"
#include "graphics/MLPixmaps.h"

#include "kw/kw.h"
#include "VWidgetEvent.h"
#include "VOptions.h"
#include "VButton.h"

/////////////////////////////////////////////////////////////////////////////

VButton::VButton( VWindow* _parent ): VButtonBase(_parent)
{
  buttonFont= 0;
  hasEnterPixmap= false;
  message= "";
}

/////////////////////////////////////////////////////////////////////////////

void VButton::create( int x, int y, const MLString& _message,
                      int iwidth /* = 0 */ )
{
  buttonFont= vOptions->button.font;

  width= height= 10; // dummy values for initMessage()
  initMessage(_message);

  if( iwidth == 0 )
    width= buttonFont->getStringWidth(hcm.message)
          +2*buttonFont->getWidth();
   else
    width= buttonFont->getWidth() * iwidth;

  height= buttonFont->getHeight() * 2*24/28;

  VButtonBase::create( x, y, width, height );
}

/////////////////////////////////////////////////////////////////////////////

void VButton::create( int x, int y, int _width, int _height,
                      const MLString& _message )
{
  buttonFont= vOptions->button.font;

  width= _width;
  height= _height;
  initMessage(_message);

  VButtonBase::create( x, y, width, height );
}

/////////////////////////////////////////////////////////////////////////////

void VButton::changeMessage( const MLString& newMessage )
{
  if( message == newMessage )
    return;
  initMessage( newMessage );
  postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void VButton::setEnterPixmap( boolean yes )
{
  hasEnterPixmap= yes;
}

/////////////////////////////////////////////////////////////////////////////

char VButton::getHotChar() const
{
  return hcm.ch;
}

/////////////////////////////////////////////////////////////////////////////

void VButton::initMessage( const MLString& _message )
{
  message= _message;
  hcm.set( _message, buttonFont );
}

/////////////////////////////////////////////////////////////////////////////

void VButton::drawMessage( const IVector& displ /* = IVector(0,0) */ )
{
  setFont(buttonFont);

  int _width= width-4;
  if( hasEnterPixmap )
    _width-= MLPixmaps::enterArrow->getWidth()+3;

  IPoint textpos( 2+(_width - buttonFont->getStringWidth(hcm.message.get()))/2,
                  (height + buttonFont->getHeight())/2
                          - buttonFont->getDescent()-1 );
  textpos+= displ;

  hcm.print( this, textpos );

  if( hasEnterPixmap )
    drawPixmap( MLPixmaps::enterArrow,
                width-2-3-MLPixmaps::enterArrow->getWidth()+displ.x(),
                textpos.y() - buttonFont->getAscent()/2
                            +6 - MLPixmaps::enterArrow->getHeight() );
}

/////////////////////////////////////////////////////////////////////////////

void VButton::drawButton()
{
  if( ! beginDraw() )
    return;

  int _width= isCurrent() ? (width-1) : width;
  int _height= isCurrent() ? (height-1) : height;
  int d= isCurrent() ? 1 : 0 ;

  switch( state )
    {
    case RELEASED:
      drawUnpressedRectangle( d, d, _width, _height );
      gray2();
      fillRectangle( IPoint(d+1,d+1), IPoint(_width-3,_height-3) );
      break;
    case PRESSED:
      if( isCurrent() )
        {
        gray0();
        drawRectangle( IPoint(1,1), IPoint(width-2,height-2) );
        gray2();
        fillRectangle( IPoint(2,2), IPoint(width-3,height-3) );
        }
       else
        {
        drawPressedRectangle( _width, _height );
        gray2();
        fillRectangle( IPoint(d+2,d+2), IPoint(_width-2,_height-2) );
        }
      break;
    }

  if( ! isAvailable() )
    gray1();
   else
    gray0();

  switch( state )
    {
    case RELEASED:
      drawMessage();
      break;
    case PRESSED:
      drawMessage( IVector(1,1) );
      break;
    }

  if( isCurrent() )
    {
    gray0();
    drawRectangle( IPoint(0,0), IPoint(width-1,height-1) );
    }

  endDraw();
}

/////////////////////////////////////////////////////////////////////////////

boolean VButton::handleEvent( MLEvent& event )
{
  if( isAvailable() && event.type == MLEvent::MOUSE_PRESS )
    setCurrent(true);

  return VButtonBase::handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////

boolean VButton::handleKey( Key key )
{
  boolean pressMe= false;
  if( getHotChar() != '\0' )
    if( key == Key(getHotChar()).alt() )
      pressMe= true;
  if( key == KeyCodes::SPACE || key == KeyCodes::ENTER )
    pressMe= true;

  if( pressMe )
    {
    press();
    drawWidget();
    VWindow::flush();
    callPrivateWidgetEventHandler( VWidgetEvent::BeginPress(this) );
    Chrono::delay( vOptions->button.pressingDuration ); // sleeping

    boolean ok= (state==PRESSED);
    if( ok )
      {
      unpress();
      drawWidget();
      }
    callPrivateWidgetEventHandler( VWidgetEvent::EndPress(this) );
    if( ok )
      kwCall(KWEvent::SELECT);

    return true;
    }

  return false;
}


/////////////////////////////////////////////////////////////////////////////
