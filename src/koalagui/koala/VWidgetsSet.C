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
  VWidgetsSet.C

  Stephane Rehel
  July 15 1996
*/

#include "graphics/MLEvent.h"

#include "VWidgetsSet.h"

/////////////////////////////////////////////////////////////////////////////

VWidgetsSet::VWidgetsSet( VWindow* parent ): VWidget(parent)
{}

/////////////////////////////////////////////////////////////////////////////

VWidgetsSet::~VWidgetsSet()
{
  while( ! widgets.empty() )
    {
    VWidget* w= widgets.getFirst();
    widgets.removeFirst();
    delete w;
    }

  if( VWindow::systemWindow != 0 )
    {
    // clear widgets area
    if( beginDraw() )
      {
      VWindow::fillWindow( getGray2() );
      endDraw();
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

void VWidgetsSet::create( int x, int y, int width, int height )
{
  VWindow::create( x, y, width, height );
}

/////////////////////////////////////////////////////////////////////////////

void VWidgetsSet::addWidget( VWidget* widget,
                             boolean staticWidget /* = false */ )
{
  if( widget == 0 )
    return;

  widgets.append(widget);
  widget->staticWidget= staticWidget;
}

/////////////////////////////////////////////////////////////////////////////

void VWidgetsSet::drawWidget()
{
  removeRedraw();

  if( beginDraw() )
    {
    VWindow::fillWindow( getGray2() );
//  VWindow::fillWindow( Color8(255,0,0) );
    endDraw();
    }

  SIListIterator<VWidget> li(widgets);
  while( ! li.eol() )
    li.next()->drawWidget();
}

/////////////////////////////////////////////////////////////////////////////

void VWidgetsSet::postDrawWidget()
{
/*
  removeRedraw();

  if( beginDraw() )
    {
    VWindow::fillWindow( getGray2() );
    endDraw();
    }
*/

  VWidget::postDrawWidget();
}

/////////////////////////////////////////////////////////////////////////////

VWidget** VWidgetsSet::getWidgetsList( int* nWidgets )
{
  static int _n= 0;
  static VWidget** w= 0;

  int n= widgets.getNElements()+1;
  if( w == 0 || _n < n )
    {
    delete w;
    w= new VWidget* [n];
    _n= n;
    }

  w[0]= this;

  int i= 1;
  SIListIterator<VWidget> li(widgets);
  while( ! li.eol() )
    w[i++]= li.next();

  *nWidgets= n;

  return w;
}

/////////////////////////////////////////////////////////////////////////////

boolean VWidgetsSet::handleEvent( MLEvent& event )
{
  if( event.type == MLEvent::REDRAW )
    {
    drawWidget();
    return true;
    }

  return VWindow::handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////
