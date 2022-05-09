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
   VProgressBar.h

   Stephane Rehel
   August 12 1996
*/

#include <stdio.h>

#include "graphics/MLEvent.h"
#include "graphics/MLFont.h"
#include "graphics/MLFonts.h"

#include "VOptions.h"
#include "VProgressBar.h"

/////////////////////////////////////////////////////////////////////////////

VProgressBar::VProgressBar( VWindow* _parent ): VRangeBar(_parent)
{
  perc= 0;
}

/////////////////////////////////////////////////////////////////////////////

VProgressBar::~VProgressBar()
{}

/////////////////////////////////////////////////////////////////////////////

void VProgressBar::create( int x, int y, int _width, int _height )
{
  VRangeBar::create( x, y, _width, _height,
                     0., 0., 1. );
  perc= 0;
}

/////////////////////////////////////////////////////////////////////////////

void VProgressBar::drawProgressBar()
{
  VRangeBar::drawRangeBar();

  if( ! beginDraw() )
    return;

  MLString s= MLString::printf( "%d%%", perc );

  MLFont* font= widgetBoldFont;
  VWindow::setFont(font);

  gray3();
  int width= font->getStringWidth(s);
  int x= (VWindow::getSize().x() - width) / 2;
  int y= (VWindow::getSize().y() - font->getHeight()) / 2
         + (font->getAscent()-1);
  drawString( x, y, s.get() );

  endDraw();
}

/////////////////////////////////////////////////////////////////////////////

// old_value <= value <= 1
void VProgressBar::changeValue( double _value )
{
  _value= min( 1., max(0.,_value) );

  int new_perc= int( _value*100. + .5 );
  if( new_perc < 0 ) new_perc= 0;
  if( new_perc > 100 ) new_perc= 100;

  if( new_perc != perc )
    {
    perc= new_perc;
    VRangeBar::changeValue(_value);
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean VProgressBar::handleEvent( MLEvent& event )
{
  if( event.type == MLEvent::REDRAW )
    {
    drawProgressBar();
    return true;
    }

  return VWindow::handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////

void VProgressBar::drawWidget()
{
  removeRedraw();
  drawProgressBar();
}

/////////////////////////////////////////////////////////////////////////////

