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
  VCard.C

  Stephane Rehel
  August 19 1996
*/

#include "graphics/MLEvent.h"
#include "graphics/MLFont.h"
#include "graphics/KeyCodes.h"

#include "kw/kw.h"
#include "VOptions.h"
#include "VCard.h"
#include "VDialog.h"

static int margin= 1;

/////////////////////////////////////////////////////////////////////////////

VCard::VCard( VDialog* _dialog ): VWidget( _dialog )
{
  dialog= _dialog;
  cardFont= vOptions->label.font;
  message= "";
}

/////////////////////////////////////////////////////////////////////////////

VCard::~VCard()
{}

/////////////////////////////////////////////////////////////////////////////

void VCard::create( int x, int y, int width, int height,
                    const MLString& _message )
{
  hcm.set( _message, cardFont );

  if( height == 0 )
    height= 21;

  if( width == 0 )
    width= 2*(2 + margin + cardFont->getCharWidth(' ')) + hcm.width;
  VWindow::create( x, y, width, height );
  setFont(cardFont);

  changeMessage(_message);
}

/////////////////////////////////////////////////////////////////////////////

void VCard::changeMessage( const MLString& _message )
{
  hcm.set( _message, cardFont );
  message= hcm.message;

  int w= getSize().x() - 2*2 - 2*margin;
  if( hcm.width > w )
    {
    if( hcm.x2 >= w )
      hcm.set("",cardFont);
    hcm.message= VWindow::cutString(cardFont,hcm.message,w);
    }

  int y= (getSize().y()-cardFont->getHeight())/2 + cardFont->getAscent()-1;
  textpos= IPoint( (getSize().x()-2*2-hcm.width)/2+1, y );

  postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void VCard::drawCard()
{
  if( ! beginDraw() )
    return;

  int width= getSize().x();
  int height= getSize().y();

  gray2();
  fillRectangle( IPoint(0,0), IPoint(width-2,height-2) );
  gray3();
  drawHorizLine( IPoint(2,0), width-4 );
  drawHorizLine( IPoint(1,1), 2 ); // last 2 = mesa bug cf.(*)
  drawVertLine( IPoint(0,2), height-2 );
  gray0();
  drawVertLine( IPoint(width-2,1), 2 ); // last 2 = mesa bug
  drawVertLine( IPoint(width-1,2), height-2 );
  gray1();
  drawVertLine( IPoint(width-2,2), height-2 );
  gray2();
  drawHorizLine( IPoint(2,1), 2 ); // (*)

  if( dialog->activeCard == this )
    {
    gray2();
    drawHorizLine( IPoint(1,height-1), width-3 );
    }
   else
    {
    gray3();
    drawHorizLine( IPoint(0,height-1), width );
    }

  if( isAvailable() )
    gray0();
   else
    gray1();

  hcm.print( this, textpos );

  if( isCurrent() )
    {
    gray0();
    int x= textpos.x();
    int y= textpos.y() - (cardFont->getAscent()-1);
    IPoint p1= IPoint( x-margin-1, y-margin );
    IPoint p2= IPoint( x+hcm.width-1+margin+1,
                       y+cardFont->getHeight()-1+margin );
    VWindow::drawDotRectangle( p1, p2 );
    }

  endDraw();
}

/////////////////////////////////////////////////////////////////////////////

char VCard::getHotChar() const
{
  return hcm.ch;
}

/////////////////////////////////////////////////////////////////////////////

void VCard::drawWidget()
{
  removeRedraw();
  drawCard();
}

/////////////////////////////////////////////////////////////////////////////

boolean VCard::handleKey( Key key )
{
  boolean ok= false;

  if( getHotChar() != '\0' )
    if( key == Key(getHotChar()).alt() )
      ok= true;

  if( key == KeyCodes::SPACE )
    ok= true;

  if( ok )
    {
    dialog->setActiveCard(this);
    kwCall(KWEvent::SELECT);
    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean VCard::handleEvent( MLEvent& event )
{
  if( event.type == MLEvent::REDRAW )
    {
    drawCard();
    return true;
    }

  if( ! isAvailable() )
    return VWindow::handleEvent(event);

  switch( event.type )
    {
    case MLEvent::MOUSE_PRESS:
      {
      if( ! isCurrent() )
        postRedraw();
      setCurrent(true);
      dialog->setActiveCard(this);
      kwCall(KWEvent::SELECT);
      break;
      }

    default:
      return VWindow::handleEvent(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
