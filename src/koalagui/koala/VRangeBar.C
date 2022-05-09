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
   VRangeBar.C

   Creation: SR, April 9th, 1996, at home
   Revisions:
     + SR July 5 1996
*/

#include "graphics/MLEvent.h"

#include "VWidgetEvent.h"
#include "VOptions.h"

#include "VRangeBar.h"

/////////////////////////////////////////////////////////////////////////////

VRangeBar::VRangeBar( VWindow* _parent ): VWidget(_parent)
{
  activateWidget= 0;
  value= 0.;
}

/////////////////////////////////////////////////////////////////////////////

VRangeBar::VRangeBar( VWindow* _parent, VWidget* _activateWidget ):
  VWidget(_parent)
{
  activateWidget= _activateWidget;
  value= 0.;
}

/////////////////////////////////////////////////////////////////////////////

VRangeBar::~VRangeBar()
{}

/////////////////////////////////////////////////////////////////////////////

void VRangeBar::create( int x, int y, int _width, int _height,
                        double _value,
                        double _min_value, double _max_value )
{
  width= _width;
  height= _height;

  if( height <= 0 )
    height= 21;

  min_value= min( _min_value, _max_value );
  max_value= max( _min_value, _max_value );
  value= min( max_value, max( min_value, _value ) );

  int bg_z= vOptions->scrollBar.bg_z;

  top= IPoint( bg_z, bg_z );
  bottom= IPoint(0,0) + IPoint( width-1-bg_z, height-1-bg_z );

  VWindow::create( x, y, width, height );

  changeValue(value);
}

/////////////////////////////////////////////////////////////////////////////

void VRangeBar::setRanges( double _min_value, double _max_value )
{
  min_value= min( _min_value, _max_value );
  max_value= max( _min_value, _max_value );

  value= min( max_value, max( min_value, value ) );
}

/////////////////////////////////////////////////////////////////////////////

double VRangeBar::getMinValue() const
{
  return min_value;
}

/////////////////////////////////////////////////////////////////////////////

double VRangeBar::getMaxValue() const
{
  return max_value;
}

/////////////////////////////////////////////////////////////////////////////

void VRangeBar::drawRangeBar()
{
  if( ! beginDraw() )
    return;

  gray1();
  drawHorizLine( 0, width-1, 0 );
  drawHorizLine( 1, width-2, 1 );
  drawVertLine( 0, 0, height-2 );
  drawVertLine( 1, 1, height-3 );
  gray2();
  drawHorizLine( 1, width-2, height-2 );
  drawVertLine( width-2, 2, height-2 );
  gray3();
  drawHorizLine( 0, width-1, height-1 );
  drawVertLine( width-1, 1, height-1 );

  int x1= 2;
  int x2= 2+lx-1;
  int x3= x2+1;
  int x4= width-3;
  if( x1 <= x2 )
    {
    gray2();
    fillRectangle( IPoint(x1,2), IPoint(x2,height-3) );
    }
  if( x3 <= x4 )
    {
    gray2();
    fillGrayRectangle( IPoint(x3,1), IPoint(x4+1,height-3) );
    gray1();
    fillGrayRectangle2( IPoint(x3,1), IPoint(x4+1,height-3) );
    }

  endDraw();
}

/////////////////////////////////////////////////////////////////////////////

// private
int VRangeBar::value_to_lx( double value ) const
{
  double r= min( max_value, max( min_value, value ) );
  double range= max_value - min_value;

  const int max_lx= width - 2 * vOptions->scrollBar.bg_z - 1;

  if( range < 1e-8 )
    return max_lx;

  int x= int(double(max_lx) * r / range);
  x= min( max_lx, x );
  x= max( 0, x );
  return x;
}

/////////////////////////////////////////////////////////////////////////////

// private
double VRangeBar::lx_to_value( int lx ) const
{
  int max_lx= width - 2 * vOptions->scrollBar.bg_z - 1;
  if( max_lx <= 0 )
    return max_value;

  if( lx <= 0. )
    return min_value;
  if( lx >= max_lx )
    return max_value;

  double range= max_value - min_value;
  if( range < 1e-8 )
    return max_value;

  double v= double(lx) / double(max_lx) * range + min_value;
  v= min( max_value, v );
  v= max( min_value, v );

  return v;
}

/////////////////////////////////////////////////////////////////////////////

// value can be outside range!!
void VRangeBar::changeValue( double _value )
{
  value= _value;
  int new_lx= value_to_lx(value);
  if( new_lx != lx )
    {
    lx= new_lx;
    postDrawWidget();
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean VRangeBar::handleEvent( MLEvent& event )
{
  if( event.type == MLEvent::REDRAW )
    {
    drawRangeBar();
    return true;
    }

  if( ! isAvailable() )
    return true;

  switch( event.type )
    {
    case MLEvent::MOUSE_PRESS:
      {
      if( activateWidget != 0 )
        if( ! activateWidget->isCurrent() )
          activateWidget->setCurrent(true);

      changeValue( lx_to_value( event.position.x() ) );
      drawRangeBar();
      callPrivateWidgetEventHandler(
                              VWidgetEvent::SetRangeBarValue(this,value) );
      }
      break;

    case MLEvent::MOUSE_MOTION:
      {
      changeValue( lx_to_value(event.position.x()) );
      postRedraw();
      callPrivateWidgetEventHandler(
                              VWidgetEvent::SetRangeBarValue(this,value) );
      }
      break;

    default:
      return VWindow::handleEvent(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void VRangeBar::drawWidget()
{
  removeRedraw();
  drawRangeBar();
}

/////////////////////////////////////////////////////////////////////////////

