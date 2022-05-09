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
   VValueBar.C

   Stephane Rehel
   August 13 1996
*/

#include <math.h>
#include <stdio.h>

#include "graphics/MLEvent.h"
#include "graphics/KeyCodes.h"

#include "kw/kw.h"
#include "VValueBar.h"
#include "VRangeBar.h"

/////////////////////////////////////////////////////////////////////////////

VValueBar::VValueBar( VWindow* _parent ):
  VValueEdit(_parent)
{
  bar= 0;
}

/////////////////////////////////////////////////////////////////////////////

VValueBar::~VValueBar()
{
  if( bar!= 0 )
    {
    VWindow::parent->deleteChild( (VWindow*) bar );
    bar= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

void VValueBar::create( int x, int y, int _totalWidth,
                        int _editWidth,
                        double _barMin,
                        double _barMax,
                        double _editMinValue,
                        double _editMaxValue,
                        int    _editNDigits,
                        double _editValue )
{
  int margin= 5;

  IPoint editPos= IPoint(x,y) + IVector(_totalWidth-_editWidth,0);

  VValueEdit::create( editPos.x(), editPos.y(), _editWidth,
                      _editMaxValue,
                      _editMinValue,
                      _editNDigits,
                      _editValue );

  int bar_height= VValueEdit::getSize().y();
  int bar_y= y;
  int bar_width= _totalWidth - _editWidth - margin;

  bar= new VRangeBar(VWindow::getParent(),this);
  bar->create( x, bar_y, bar_width, bar_height,
               VValueEdit::getValue(),
               _barMin, _barMax );

  bar->setWidgetEventHandler(this);
  VValueEdit::setWidgetEventHandler(this);

  handlePrivateWidgetEvent( VWidgetEvent::EnterKey(this,KeyCodes::ENTER) );
}

/////////////////////////////////////////////////////////////////////////////

void VValueBar::setRanges( double _barMin,
                           double _barMax,
                           double _editMinValue,
                           double _editMaxValue,
                           int    _editNDigits )
{
  VValueEdit::setRanges( _editMinValue, _editMaxValue, _editNDigits );

  if( bar != 0 )
    bar->setRanges( _barMin, _barMax );
}

/////////////////////////////////////////////////////////////////////////////

boolean VValueBar::handleWidgetEvent( const VWidgetEvent& event )
{
  return handlePrivateWidgetEvent( event );
}

/////////////////////////////////////////////////////////////////////////////

boolean VValueBar::handlePrivateWidgetEvent( const VWidgetEvent& event )
{
  if( event.getWidget() == bar &&
      event.type()==VWidgetEvent::SETRANGEBARVALUE )
    {
    double value= event.getValue();

    VValueEdit::changeValue(value);
    kwCall(KWEvent::VALUE);
    return false;
    }

  if( event.getWidget() == this &&
      event.type() == VWidgetEvent::ENTEREDIT )
    {
    VValueEdit::parseValue();
    double value= VValueEdit::getValue();

    if( bar != 0 )
      bar->changeValue(value);

    VValueEdit::enableSelection(true);
    kwCall(KWEvent::VALUE);
    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

void VValueBar::changeValue( double _value )
{
  VValueEdit::changeValue(_value);

  double value= VValueEdit::getValue();

  if( bar != 0 )
    bar->changeValue(value);
}

/////////////////////////////////////////////////////////////////////////////

IRectangle VValueBar::getWidgetRectangle() const
{
  IPoint p1= bar->getPosition();
  IPoint p2= VValueEdit::getPosition() + VValueEdit::getSize();

  return IRectangle( p1, p2 - p1 );
}

/////////////////////////////////////////////////////////////////////////////

void VValueBar::setWidgetPosition( const IPoint& pos )
{
  if( bar == 0 )
    return;

  IVector d= pos - bar->getPosition();

  VValueEdit::setWidgetPosition( VValueEdit::getPosition()+d );
  bar->setWidgetPosition( pos );
}

/////////////////////////////////////////////////////////////////////////////

void VValueBar::drawWidget()
{
  removeRedraw();
  VValueEdit::drawWidget();
  if( bar != 0 )
    bar->drawWidget();
}

/////////////////////////////////////////////////////////////////////////////

VWidget** VValueBar::getWidgetsList( int* nWidgets )
{
  static VWidget* w[2];

  w[0]= this;
  w[1]= bar;
  *nWidgets= 2;

  return w;
}

/////////////////////////////////////////////////////////////////////////////

boolean VValueBar::handleKey( Key key )
{
  boolean control= key.hasControl();
  key= key.uncontrol();

  if( key == KeyCodes::UP || key == KeyCodes::DOWN ||
      key == KeyCodes::PAGEUP || key == KeyCodes::PAGEDOWN )
    {
    VValueEdit::parseValue();
    double value= VValueEdit::getValue();
    double delta= pow( 10., double(-VValueEdit::nDigits) );
    if( control || key == KeyCodes::PAGEUP || key == KeyCodes::PAGEDOWN )
      delta *= 10.;
    value= (key==KeyCodes::UP || key==KeyCodes::PAGEUP)
           ? (value+delta)
           : (value-delta);

    VValueEdit::changeValue(value);
    if( bar != 0 )
      bar->changeValue( VValueEdit::getValue() );

    if( value != VValueEdit::getValue() )
      kwCall(KWEvent::VALUE);

    return true;
    }

  boolean outcome= VValueEdit::handleKey(key);

  if( outcome )
    {
    const MLString& edit= VValueEdit::getEdit();
    double d= 0.;
    if( bar != 0 && sscanf( edit.get(), "%lf", &d ) == 1 )
      {
      if( d < bar->getMinValue() )
        d= bar->getMinValue();

      if( d > bar->getMaxValue() )
        d= bar->getMaxValue();

      bar->changeValue(d);
      }
    }

  return outcome;
}

/////////////////////////////////////////////////////////////////////////////
