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
  VTextBox.C

  Stephane Rehel
  July 16 1996
*/

#include "graphics/MLEvent.h"
#include "graphics/MLFont.h"

#include "VOptions.h"
#include "VTextBox.h"

/////////////////////////////////////////////////////////////////////////////

VTextBox::VTextBox( VWindow* parent ): VWidget(parent)
{
  tbFont= vOptions->label.font;
  x0= 0;
  justify= CENTER;
  _message= "";
}

/////////////////////////////////////////////////////////////////////////////

VTextBox::~VTextBox()
{}

/////////////////////////////////////////////////////////////////////////////

void VTextBox::setFixedFont()
{
  tbFont= vOptions->fixedFont;
}

/////////////////////////////////////////////////////////////////////////////

void VTextBox::create( int x, int y, int width,
                       const MLString& _message,
                       VTextBox::Justify _justify /* = CENTER */ )
{
  justify= _justify;

  int height= 21;
  VWindow::create( x, y, width, height );
  setFont(tbFont);

  changeMessage(_message);
}

/////////////////////////////////////////////////////////////////////////////

void VTextBox::changeMessage( const MLString& message )
{
  hcm.set(message,tbFont);

  if( message == _message )
    return;

  _message= message;

  int width= getSize().x();
  hcm.message= VWindow::cutString( hcm.message, width-4 );

//  x0= (width-4-tbFont->getStringWidth(hcm.message)) / 2 + 2;

  switch( justify )
    {
    case LEFT:
      x0= 5;
      break;
    case RIGHT:
      x0= width-4-tbFont->getStringWidth(hcm.message);
      break;
    case CENTER:
    default:
      x0= (width-4-tbFont->getStringWidth(hcm.message)) / 2 + 2;
      break;
    }

  postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void VTextBox::drawWidget()
{
  removeRedraw();
  drawTextBox();
}

/////////////////////////////////////////////////////////////////////////////

char VTextBox::getHotChar() const
{
  return hcm.ch;
}

/////////////////////////////////////////////////////////////////////////////

void VTextBox::drawTextBox()
{
  if( ! beginDraw() )
    return;

  int width= getSize().x();
  int height= getSize().y();

  gray0();
  drawHorizLine( 1, width-2, 1 );
  drawVertLine( 1, 1, height-3 );
  gray3();
  drawHorizLine( 0, width-1, height-1 );
  drawVertLine( width-1, 0, height-1 );
  gray2();
  drawHorizLine( 2, width-2, height-2 );
  drawVertLine( width-2, 2, height-2 );
  gray1();
  drawHorizLine( 0, width-2, 0 );
  drawVertLine( 0, 0, height-2 );
  drawVertLine( width-2, 1, 1 );
  drawVertLine( 1, height-2, height-2 );
  fillRectangle( IPoint(2,2), IPoint(width-3,height-3) );

  if( isAvailable() )
    gray3();
   else
    gray2();
  int y0= (height-4-tbFont->getHeight())/2+2;
  y0+= tbFont->getAscent()-1;

  hcm.print( this, IPoint(x0,y0) );

  endDraw();
}

/////////////////////////////////////////////////////////////////////////////

boolean VTextBox::handleEvent( MLEvent& event )
{
  if( event.type == MLEvent::REDRAW )
    {
    drawTextBox();
    return true;
    }

  return VWindow::handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////
