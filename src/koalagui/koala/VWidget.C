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
   VWidget.C

   Creation: SR, July 28th, 1995
   Revisions:
     + SR July 5 1996
     + SR August 9 1996
*/

#include "graphics/MLFont.h"

#include "VOptions.h"
#include "VWidget.h"
#include "VWidgetEvent.h"
#include "VDialog.h"
#include "VDialogManager.h"

#include "kw/kw.h"
#include "kw/KWDialog.h"

/////////////////////////////////////////////////////////////////////////////

VWidget::VWidget( VWindow* parent ): VWindow(parent)
{
  widgetEventHandler= 0;
  available= true;
  current= false;
  dialog= 0;
  staticWidget= true;
  linkWidget= 0;

  id= 0;
  card= 0;
  kwDialog= 0;
}

/////////////////////////////////////////////////////////////////////////////

VWidget::~VWidget()
{
  if( dialog != 0 )
    dialog->removeWidget(this);
  if( kwDialog != 0 )
    kwDialog->removeWidget(this);

  widgetEventHandler= 0;
}

/////////////////////////////////////////////////////////////////////////////

void VWidget::setWidgetEventHandler( VWidgetEventHandler* weh )
{
  widgetEventHandler= weh;
}

/////////////////////////////////////////////////////////////////////////////

void VWidget::setAvailable( boolean yes /* = true */ )
{
  if( (available?1:0) == (yes?1:0) )
    return;

  available= yes;

  if( linkWidget != 0 )
    if( linkWidget->type() != GROUP )
      linkWidget->setAvailable(yes);

  int n;
  VWidget** widgets= getWidgetsList(&n);
  if( n > 1 && widgets != 0 )
    {
    for( int i= 0; i < n; ++i )
      {
      VWidget* w= widgets[i];
      if( w == this || w == 0 )
        continue;
      w->setAvailable(yes);
      }
    }

  postDrawWidget();
}

/////////////////////////////////////////////////////////////////////////////

VWidget** VWidget::getWidgetsList( int* nWidgets )
{
  static VWidget* widget;

  *nWidgets= 1;
  widget= this;

  return &widget;
}

/////////////////////////////////////////////////////////////////////////////

void VWidget::postDrawWidget()
{
  postRedraw();

  int n;
  VWidget** widgets= getWidgetsList(&n);
  if( n > 1 && widgets != 0 )
    {
    for( int i= 0; i < n; ++i )
      {
      VWidget* w= widgets[i];
      if( w == this || w == 0 )
        continue;
      w->postDrawWidget();
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

void VWidget::setCurrent( boolean yes )
{
  if( dialog == 0 || staticWidget )
    return;

  if( (isCurrent()?1:0) == (yes?1:0) )
    return;

  current= yes;
  if( yes )
    dialog->setCurrentWidget(this);
}

/////////////////////////////////////////////////////////////////////////////

boolean VWidget::isCurrent() const
{
  if( dialog == 0 )
    return current;

  if( ! VDialogManager::hasFocus(dialog) )
    return false;

  return current;
}

/////////////////////////////////////////////////////////////////////////////

char VWidget::getHotChar() const
{
  return '\0';
}

/////////////////////////////////////////////////////////////////////////////

void VWidget::setLinkWidget( VWidget* _linkWidget )
{
  if( _linkWidget == this )
     _linkWidget= 0;

  linkWidget= _linkWidget;
  if( linkWidget == 0 )
    return;
  linkWidget->staticWidget= true;
}

/////////////////////////////////////////////////////////////////////////////

IRectangle VWidget::getWidgetRectangle() const
{
  return VWindow::getRectangle();
}

/////////////////////////////////////////////////////////////////////////////

void VWidget::setWidgetPosition( const IPoint& pos )
{
  VWindow::setPosition(pos);
}

/////////////////////////////////////////////////////////////////////////////

boolean VWidget::handleKey( Key /*key*/ )
{
  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean VWidget::kwCall( KWEvent& event )
{
  if( kwDialog == 0 )
    return false; // not handled

  if( kwDialog->validateCancel &&
      type() == BUTTON && event.type == KWEvent::SELECT )
    {
    if( kwDialog->cancelID == id )
      {
      kwDialog->quit(KWDialog::CANCEL);
      return true;
      }
    if( kwDialog->validateID == id )
      {
      kwDialog->validateDialog();
      return true;
      }
    }

  event.dialog= kwDialog;
  event.widget= id;
  return kwDialog->kwDialogCallback(event);
}

/////////////////////////////////////////////////////////////////////////////

boolean VWidget::kwCall( int t ) // KWEvent::Type t )
{
  KWEvent event;
  event.type= KWEvent::Type(t);
  return kwCall(event);
}

/////////////////////////////////////////////////////////////////////////////

void VWidget::map( boolean yes /* = true */ )
{
  if( (yes?1:0) == (VWindow::isMapped?1:0) )
    return;

  VWindow::map(yes);

  int n;
  VWidget** widgets= getWidgetsList(&n);
  if( n > 1 && widgets != 0 )
    {
    for( int i= 0; i < n; ++i )
      {
      VWidget* w= widgets[i];
      if( w == this || w == 0 )
        continue;
      w->map(yes);
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean VWidget::handleWidgetEvent( const VWidgetEvent& /*event*/ )
{
  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean VWidget::callPrivateWidgetEventHandler( const VWidgetEvent& event )
{
  if( widgetEventHandler == 0 )
    return false;
   else
    {
    // we call the widget event handler

/*
    // prevent it from beeing called again and again
    VWidgetEventHandler* saveWidgetEventHandler= widgetEventHandler;
    widgetEventHandler= 0;
    boolean outcome= saveWidgetEventHandler->handlePrivateWidgetEvent(event);
    widgetEventHandler= saveWidgetEventHandler;
    return outcome;
*/
    return widgetEventHandler->handlePrivateWidgetEvent(event);
    }
}

/////////////////////////////////////////////////////////////////////////////

VWidget::HotCharMessage::HotCharMessage( const MLString& msg, MLFont* font )
{
  set(msg,font);
}

/////////////////////////////////////////////////////////////////////////////

VWidget::HotCharMessage::HotCharMessage()
{
  set( MLString(""), 0 );
}

/////////////////////////////////////////////////////////////////////////////

void VWidget::HotCharMessage::set( const MLString& msg, MLFont* font )
{
  if( msg.length() == 0 )
    {
    message= MLString("");
    x1= x2= 0;
    ch= '\0';
    return;
    }
  if( font == 0 )
    {
    message= msg;
    x1= x2= 0;
    ch= '\0';
    return;
    }

  message= msg;
  int j= 0;

  int i= 0;
  int x= 0;
  x1= x2= 0;
  ch= '\0';
  while( i < msg.length() )
    {
    char c= msg.charAt(i);
    if( c == vOptions->button.hotCharMark )
      {
      ch= msg.charAt(i+1);
      message.remove(j);
      i+= 2;
      x1= x;
      }
     else
      {
      ++i; ++j;
      x+= font->getCharWidth(c);
      }
    }

  if( ch != '\0' )
    x2= x1 + font->getCharWidth(ch) - 1;

  if( ch >= 'a' && ch <= 'z' )
    ch-= 'a' - 'A';

  width= font->getStringWidth(message);
}

/////////////////////////////////////////////////////////////////////////////

void VWidget::HotCharMessage::print( VWindow* window, const IPoint& pos )
{
  if( window == 0 )
    return;

  if( message.length() == 0 )
    return;

  window->drawString( pos, message );

  if( ch != '\0' )
    window->drawHorizLine( pos.x()+x1, pos.x()+x2, pos.y()+2 );
}

/////////////////////////////////////////////////////////////////////////////

