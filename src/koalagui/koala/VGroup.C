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
  VGroup.C

  Stephane Rehel
  July 15 1996
*/

#include "graphics/MLEvent.h"
#include "graphics/MLFont.h"

#include "VGroup.h"

#include "VOptions.h"

#include "VWidgetsSet.h"

/////////////////////////////////////////////////////////////////////////////

VGroup::VGroup( VWindow* parent ): VWidget(parent)
{
  widgetsArea= IRectangle( IPoint(0,0), IVector(0,0) );
  labelFont= vOptions->label.font;
}

/////////////////////////////////////////////////////////////////////////////

VGroup::~VGroup()
{}

/////////////////////////////////////////////////////////////////////////////

void VGroup::create( int x, int y, int width, int height,
                     const MLString& _message )
{
  VWindow::create( x, y, width, height );

  int fx= vOptions->label.font->getWidth();
  int fy= vOptions->label.font->getHeight();
  p1= IPoint( 0, fy/2 );
  p2= IPoint( width-1, height-1 );

  hcm.set(_message,labelFont);

  int z= 1;
  textPos= IPoint( p1.x()+fx*2+z, labelFont->getAscent()-1+z );
  labelPos= IPoint( p1.x()+fx*2, 0 );

  int label_height= labelFont->getHeight()+2*z;

  widgetsArea= IRectangle( IPoint( x+p1.x()+2, y+label_height ),
                           IVector( p2.x()-p1.x()+1-4,
                                    p2.y()-label_height+1-4 ) );
}

/////////////////////////////////////////////////////////////////////////////

void VGroup::changeMessage( const MLString& message )
{
  hcm.set(message,labelFont);
}

/////////////////////////////////////////////////////////////////////////////

void VGroup::drawWidget()
{
  removeRedraw();
  drawGroup();
}

/////////////////////////////////////////////////////////////////////////////

char VGroup::getHotChar() const
{
  return hcm.ch;
}

/////////////////////////////////////////////////////////////////////////////

void VGroup::drawGroup()
{
  int margin= vOptions->label.font->getWidth()/2;
  int x1= labelPos.x();
  int x2= labelPos.x()+(2+labelFont->getStringWidth(hcm.message)) + margin;
  x1-= 1+margin;

  if( x2 > p2.x()-2 )
    x2= p2.x()-2;

  if( hcm.message.length() == 0 ) // just a frame
    x2= x1+1;

  if( ! beginDraw() )
    return;

  gray2();
  fillRectangle( IPoint(x1,textPos.y()-(labelFont->getAscent()-1)),
                 IPoint(p2.x()-2,textPos.y()+labelFont->getDescent()) );

  gray1();
  drawHorizLine( p1.x(), x1, p1.y() );
  drawHorizLine( x2, p2.x(), p1.y() );
  drawHorizLine( p1.x()+2, p2.x()-1, p2.y()-1 );
  drawVertLine( p1.x(), p1.y(), p2.y() );
  drawVertLine( p2.x()-1, p1.y()+1, p2.y()-1 );
  gray3();
  drawHorizLine( p1.x()+1, x1, p1.y()+1 );
  drawHorizLine( x2, p2.x(), p1.y()+1 );
  drawHorizLine( p1.x()+1, p2.x(), p2.y() );
  drawVertLine( p1.x()+1, p1.y()+1, p2.y() );
  drawVertLine( p2.x(), p1.y()+1, p2.y() );

  setFont(labelFont);

  if( ! isAvailable() )
    gray1();
   else
    gray0();

  hcm.print( this, textPos );

  endDraw();
}

/////////////////////////////////////////////////////////////////////////////

boolean VGroup::handleEvent( MLEvent& event )
{
  if( event.type == MLEvent::REDRAW )
    {
    drawGroup();
    return true;
    }

  return VWindow::handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////
