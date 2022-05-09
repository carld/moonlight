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
   VScrollBar.C

   Creation: SR, July 28th, 1995
   Revisions:
     + SR, July 29th 1995: Global revision...:-)
     + SR July 5 1996
*/

#include "graphics/MLEvent.h"

#include "VOptions.h"

#include "VScrollBar.h"
#include "VArrow.h"

/////////////////////////////////////////////////////////////////////////////

VScrollBar::VScrollBar( VWindow* _parent, const IVector& _X ):
  VBar( _parent, _X )
{
  upArrow= 0;
  downArrow= 0;
}

/////////////////////////////////////////////////////////////////////////////

VScrollBar::~VScrollBar()
{
  VWindow::parent->deleteChild(upArrow); upArrow= 0;
  VWindow::parent->deleteChild(downArrow); downArrow= 0;
}

/////////////////////////////////////////////////////////////////////////////

void VScrollBar::create( int x, int y, int _height,
                         int totalLength, int visualLength )
{
  int width= vOptions->scrollBar.width;
  int height= _height;

  int arrowsHeight= vOptions->scrollBar.arrowsHeight;
  int barHeight= height - 2*(1+arrowsHeight);

  IPoint up_pos= IPoint(x,y);
  IPoint bar_pos= up_pos + Y*(arrowsHeight+1);
  IPoint down_pos= up_pos + Y*(height-arrowsHeight);

  IPoint p1, p2, mp, Mp;
  IVector diag;

  upArrow= new VArrow(VWindow::getParent());
  p1= up_pos;
  p2= up_pos + X*(width-1) + Y*(arrowsHeight-1);
  mp= min( p1, p2 );
  Mp= max( p1, p2 );
  diag= Mp - mp;
  upArrow->create( mp.x(), mp.y(), diag.x() + 1, diag.y() + 1,
                   vertical() ? VArrow::UP : VArrow::LEFT );

  downArrow= new VArrow(VWindow::getParent());
  p1= down_pos;
  p2= down_pos + X*(width-1) + Y*(arrowsHeight-1);
  mp= min( p1, p2 );
  Mp= max( p1, p2 );
  diag= Mp - mp;
  downArrow->create( mp.x(), mp.y(), diag.x() + 1, diag.y() + 1,
                     vertical() ? VArrow::DOWN : VArrow::RIGHT );

  VBar::create( bar_pos.x(), bar_pos.y(), barHeight,
                totalLength, visualLength );

  VBar::setWidgetEventHandler(this);
  upArrow->setWidgetEventHandler(this);
  downArrow->setWidgetEventHandler(this);
}

/////////////////////////////////////////////////////////////////////////////

IRectangle VScrollBar::getWidgetRectangle() const
{
  IPoint p1= upArrow->getPosition();
  IPoint p2= downArrow->getPosition()+downArrow->getSize();

  return IRectangle( p1, p2-p1 );
}

/////////////////////////////////////////////////////////////////////////////

void VScrollBar::setWidgetPosition( const IPoint& pos )
{
  IVector d= pos - getWidgetRectangle().p1();
  VBar::setWidgetPosition( VWindow::getPosition() + d );
  upArrow->setWidgetPosition( upArrow->getWidgetRectangle().p1() + d );
  downArrow->setWidgetPosition( downArrow->getWidgetRectangle().p1() + d );
}

/////////////////////////////////////////////////////////////////////////////

void VScrollBar::drawWidget()
{
  removeRedraw();
  VBar::drawWidget();

  if( upArrow != 0 )
    upArrow->drawWidget();

  if( downArrow != 0 )
    downArrow->drawWidget();
}

/////////////////////////////////////////////////////////////////////////////

VWidget** VScrollBar::getWidgetsList( int* nWidgets )
{
  static VWidget* w[4];

  w[0]= this;
  w[1]= upArrow;
  w[2]= downArrow;
  w[3]= (VWidget*) bar.button;
  *nWidgets= 4;

  return w;
}

/////////////////////////////////////////////////////////////////////////////

boolean VScrollBar::handlePrivateWidgetEvent( const VWidgetEvent& event )
{
  if( event.getWidget()->type() != VWidget::ARROW )
    return VBar::handlePrivateWidgetEvent(event);

  VArrow* a= (VArrow*) event.getWidget();

  if( event.type() == VWidgetEvent::BEGINPRESS ||
      event.type() == VWidgetEvent::REPEATPRESS )
    {
    if( a == upArrow )
      VBar::up();
     else
      VBar::down();
    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////
