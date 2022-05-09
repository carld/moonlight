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
  VLine.C

  Stephane Rehel
  July 15 1996
*/

#include "graphics/MLEvent.h"

#include "VLine.h"

/////////////////////////////////////////////////////////////////////////////

VLine::VLine( VWindow* parent, const IVector& _Y ): VWidget(parent)
{
  Y= _Y;
}

/////////////////////////////////////////////////////////////////////////////

VLine::~VLine()
{}

/////////////////////////////////////////////////////////////////////////////

void VLine::create( const IPoint& _p1, const IPoint& _p2 )
{
  p1= _p1;
  p2= _p2;

  int x1= min(p1.x(),p2.x());
  int x2= max(p1.x(),p2.x());
  int y1= min(p1.y(),p2.y());
  int y2= max(p1.y(),p2.y());

  int width= x2 - x1 + 1;
  int height= y2 - y1 + 1;

  if( width < 2 )
    {
    x1-= 0;
    x2+= 4;
    width= x2 - x1 + 1;
    }

  if( height < 2 )
    {
    y1-= 0;
    y2+= 4;
    height= y2 - y1 + 1;
    }

  p1-= IVector(x1,y1);
  p2-= IVector(x1,y1);

  VWindow::create( x1, y1, width, height );
}

/////////////////////////////////////////////////////////////////////////////

boolean VLine::horizontal() const
{
  return Y.y() != 0;
}

/////////////////////////////////////////////////////////////////////////////

void VLine::drawWidget()
{
  removeRedraw();
  drawTheLine();
}

/////////////////////////////////////////////////////////////////////////////

void VLine::drawTheLine()
{
  if( ! beginDraw() )
    return;

  gray1();
  drawLine( p1, p2 );
  gray3();
  drawLine( p1+Y, p2+Y );

  endDraw();
}

/////////////////////////////////////////////////////////////////////////////

boolean VLine::handleEvent( MLEvent& event )
{
  if( event.type == MLEvent::REDRAW )
    {
    drawTheLine();
    return true;
    }

  return VWindow::handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////
