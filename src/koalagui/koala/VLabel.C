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
   VLabel.C

   Creation: SR, July 27th, 1995
   Revisions:
     + SR July 5 1996
*/

#include "graphics/MLFont.h"
#include "graphics/MLEvent.h"

#include "VWidgetEvent.h"
#include "VOptions.h"
#include "VLabel.h"

/////////////////////////////////////////////////////////////////////////////

VLabel::VLabel( VWindow* _parent ): VWidget(_parent)
{
  justify= LEFT;
  justMargin= 0;
  labelFont= vOptions->label.font;
  margin= 1;
  message= "";
}

/////////////////////////////////////////////////////////////////////////////

VLabel::~VLabel()
{}

/////////////////////////////////////////////////////////////////////////////

void VLabel::setFixedFont()
{
  labelFont= vOptions->fixedFont;
}

/////////////////////////////////////////////////////////////////////////////

void VLabel::setBigFont()
{
  labelFont= vOptions->bigFont;
}

/////////////////////////////////////////////////////////////////////////////

void VLabel::setMargin( int _justMargin )
{
  justMargin= _justMargin;
}

/////////////////////////////////////////////////////////////////////////////

void VLabel::create( int x, int y, int _width, int _height,
                     const MLString& _message,
                     VLabel::Justify _justify /* = LEFT */ )
{
  if( _justify == CENTER )
    justMargin= 0;

  justify= _justify;
  hcm.set( _message, labelFont );

  int width= _width;
  int height= _height;

  if( width == 0 )
    width= ((hcm.width==0) ? 0 : hcm.width) + 2*margin + justMargin;
  if( height == 0 )
    height= labelFont->getHeight() + 2*margin;

  VWindow::create( x, y, width, height );
  setFont(labelFont);

  changeMessage(_message);
}

/////////////////////////////////////////////////////////////////////////////

void VLabel::changeMessage( const MLString& _message )
{
  if( message == _message )
    return;
  message= _message;
  hcm.set( _message, labelFont );

  int w= getSize().x() - 2*margin - justMargin;
  if( hcm.width > w )
    {
    if( hcm.x2 >= w )
      hcm.set("",labelFont);
    hcm.message= VWindow::cutString(labelFont,hcm.message,w);
    }

  int y= (getSize().y()-labelFont->getHeight())/2 + labelFont->getAscent()-1;
  switch( justify )
    {
    case CENTER: textpos= IPoint( (getSize().x()-hcm.width)/2, y); break;
    case RIGHT: textpos= IPoint( getSize().x()-margin-hcm.width-justMargin, y); break;
    case LEFT:
    default:
      textpos= IPoint( margin+justMargin, y ); break;
    }

  postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void VLabel::drawLabel()
{
  if( ! beginDraw() )
    return;

  fillWindow( getGray2() );

  if( isAvailable() )
    gray0();
   else
    gray1();

  hcm.print( this, textpos );

  endDraw();
}

/////////////////////////////////////////////////////////////////////////////

boolean VLabel::handleEvent( MLEvent& event )
{
  if( event.type == MLEvent::REDRAW )
    {
    drawWidget();
    return true;
    }

  if( callPrivateWidgetEventHandler( VWidgetEvent::VMLEvent(this,event) ) )
    return true;

  return VWindow::handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////

void VLabel::drawWidget()
{
  removeRedraw();
  drawLabel();
}

/////////////////////////////////////////////////////////////////////////////

void VLabel::drawDotRectangle()
{
  if( ! beginDraw() )
    return;

  gray0();
  int x= textpos.x();
  int y= textpos.y() - (labelFont->getAscent()-1);
  IPoint p1= IPoint( x-margin, y-margin );
  IPoint p2= IPoint( x+hcm.width-1+margin,
                     y+labelFont->getHeight()-1+margin );
  VWindow::drawDotRectangle( p1, p2 );
  endDraw();
}

/////////////////////////////////////////////////////////////////////////////

char VLabel::getHotChar() const
{
  return hcm.ch;
}

/////////////////////////////////////////////////////////////////////////////
