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
   VRadioButton.C

   Creation: SR, August 9th, 1995
   Revisions:
     + SR July 5 1996
*/

#include "tools/Chrono.h"
#include "graphics/KeyCodes.h"
#include "graphics/MLEvent.h"

#include "kw/kw.h"
#include "VOptions.h"

#include "VRadioButton.h"

#include "VCircleButton.h"


/////////////////////////////////////////////////////////////////////////////

VRadioButton::VRadioButton( VWindow* _parent ): VLabel(_parent)
{
  box= 0;
  checkedFlag= false;
}

/////////////////////////////////////////////////////////////////////////////

VRadioButton::~VRadioButton()
{
  VWindow::parent->deleteChild(box);
  box= 0;
}

/////////////////////////////////////////////////////////////////////////////

void VRadioButton::create( int x, int y, int width, int height,
                           const MLString& _message,
                           VLabel::Justify justify /* = VLabel::LEFT */,
                           boolean _checked /* = false */ )
{
  box= new VCircleButton( VWindow::getParent() );
  box->create( x, y+1 );
  box->setWidgetEventHandler(this);

  int margin= box->getSize().x()+2;

  if( width < margin )
    width= 0;
   else
    width-= margin;
  VLabel::create( x+margin, y, width, height, _message, justify );
  VLabel::setWidgetEventHandler(this);

  box->setPosition( IPoint( x,
                        y+VLabel::getSize().y()/2-box->getSize().y()/2-1 ));

  checkedFlag= false;
  if( _checked )
    check();
}

/////////////////////////////////////////////////////////////////////////////

void VRadioButton::check( boolean yes /* = true */ )
{
  if( box == 0 )
    return;

  if( (yes?1:0) == (checkedFlag?1:0) )
    return;

  if( yes )
    box->press();
   else
    box->unpress();

  checkedFlag= yes;
}

/////////////////////////////////////////////////////////////////////////////

void VRadioButton::uncheck()
{
  check(false);
}

/////////////////////////////////////////////////////////////////////////////

void VRadioButton::drawWidget()
{
  removeRedraw();

  VLabel::drawLabel();
  if( box != 0 )
    box->drawWidget();

  if( isCurrent() )
    VLabel::drawDotRectangle();
}

/////////////////////////////////////////////////////////////////////////////

VWidget** VRadioButton::getWidgetsList( int* nWidgets )
{
  static VWidget* w[2];

  w[0]= this;
  w[1]= box;
  *nWidgets= 2;

  return w;
}

/////////////////////////////////////////////////////////////////////////////

IRectangle VRadioButton::getWidgetRectangle() const
{
  if( box == 0 )
    return VWindow::getRectangle();

  IRectangle r1= box->getWidgetRectangle();
  IRectangle r2= VWindow::getRectangle();

  return IRectangle( min(r1.p1(),r2.p1()), max(r1.p2(),r2.p2()) );
}

/////////////////////////////////////////////////////////////////////////////

void VRadioButton::setWidgetPosition( const IPoint& pos )
{
  if( box == 0 )
    return;

  IPoint cur= getWidgetRectangle().p1();
  IVector d= pos - cur;
  VLabel::setWidgetPosition( VLabel::getPosition() + d );
  box->setWidgetPosition( box->getPosition() + d );
}

/////////////////////////////////////////////////////////////////////////////


boolean VRadioButton::handlePrivateWidgetEvent( const VWidgetEvent& event )
{
  if( box == 0 )
    return false;

  if( ! isAvailable() )
    return false;

  if( event.getWidget() == box || event.getWidget() == this )
    {
    if( event.type() == VWidgetEvent::MLEVENT )
      {
      MLEvent* e= event.getMLEvent();
      switch( e->type )
        {
        case MLEvent::MOUSE_PRESS:
          {
          if( ! isCurrent() )
            {
            setCurrent(true);
            drawWidget();
            }
          check( !checkedFlag );
          box->drawWidget();
          box->flush();
          Chrono::delay( vOptions->button.pressingDuration ); // sleeping
          callPrivateWidgetEventHandler( VWidgetEvent::PressRadio(this) );
          kwCall(KWEvent::SELECT);
          return true;
          }

        case MLEvent::MOUSE_RELEASE:
          if( event.getWidget() == box )
            return true;
          break;

        default:
          break;
        }
      }
    return false;
    }

  return false; // never reached
}

/////////////////////////////////////////////////////////////////////////////

boolean VRadioButton::handleEvent( MLEvent& event )
{
  return VLabel::handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////

boolean VRadioButton::handleKey( Key key )
{
  boolean pressMe= false;

  if( getHotChar() != '\0' )
    if( key == Key(getHotChar()).alt() )
      pressMe= true;
  if( key == KeyCodes::SPACE )
    pressMe= true;

  if( pressMe )
    {
    check( !checkedFlag );
    box->drawWidget();
    kwCall(KWEvent::SELECT);
    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////
