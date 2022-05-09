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
   VChooser.C

   Stephane Rehel
   August 17 1996
*/

#include "tools/Chrono.h"

#include "graphics/GraphicsSystem.h"
#include "graphics/SystemWindow.h"
#include "graphics/MLEvent.h"
#include "graphics/MLEventManager.h"
#include "graphics/MLFont.h"
#include "graphics/KeyCodes.h"
#include "graphics/MLPixmap.h"
#include "graphics/MLPixmaps.h"
#include "graphics/mlgl.h"

#include "kw/kw.h"
#include "VWidgetEvent.h"
#include "VOptions.h"
#include "VChooser.h"

/////////////////////////////////////////////////////////////////////////////

VChooser::VChooser( VWindow* _parent ): VButtonBase(_parent)
{
  buttonFont= 0;
  previousGrabber= 0;
  grabbing= false;
  currentMessage= MLString("");
  drawnMessage= MLString("");
  popup= 0;
  popupSW= 0;
}

/////////////////////////////////////////////////////////////////////////////

VChooser::~VChooser()
{
  end_grabbing();

  while( ! messages.empty() )
    {
    delete messages.getFirst();
    messages.removeFirst();
    }
}

/////////////////////////////////////////////////////////////////////////////

void VChooser::create( int x, int y, int _width, int _height )
{
  buttonFont= vOptions->button.font;

  width= _width;
  height= _height;

  VButtonBase::create( x, y, width, height );

  changeMessage(MLString(""));
}

/////////////////////////////////////////////////////////////////////////////

void VChooser::changeMessage( const MLString& message )
{
  if( message == currentMessage )
    return;

  if( message.length() == 0 || messages.getNElements() == 0 )
    {
    currentMessage= MLString("");
    drawnMessage= MLString("");
    postRedraw();
    return;
    }

  if( getMessagePosition(message) == 0 )
    return; // no effect

  currentMessage= message;
  drawnMessage= VWindow::cutString( buttonFont, message,
                                    getSize().x()-2*2-2*3-1
                                    -MLPixmaps::littleButton->getWidth() );
  postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

// may return 0
int VChooser::getInteger()
{
  return getMessagePosition(currentMessage);
}

/////////////////////////////////////////////////////////////////////////////

// 1 <= i <= nElements
boolean VChooser::setMessage( int i, const MLString& text )
{
  if( i < 1 || i > messages.getNElements() )
    return false;
  if( text.length() == 0 )
    return false;

  IListElement<MLString>* e= messages.getElement(i);
  if( text == *e->element )
    return true;

  if( getMessagePosition(text) != 0 )
    return false; // already in list

  boolean current= (*e->element == currentMessage);
  delete e->element;
  e->element= new MLString(text);

  if( current )
    changeMessage(text);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean VChooser::appendMessage( const MLString& text )
{
  if( text.length() == 0 )
    return false;

  if( getMessagePosition(text) != 0 )
    return false;

  messages.append(new MLString(text));

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// 1 <= i <= nElements+1
boolean VChooser::insertMessage( int i, const MLString& text )
{
  if( text.length() == 0 )
    return false;

  if( i == messages.getNElements()+1 )
    return appendMessage(text);

  if( i < 1 || i > messages.getNElements()+1 )
    return false;

  int _i= getMessagePosition(text);
  if( _i != 0 )
    return  (_i==i);

  messages.insert( i, new MLString(text) );

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// 1 <= i <= nElements
boolean VChooser::deleteMessage( int i )
{
  if( i < 1 || i > messages.getNElements()+1 )
    return false;

  IListElement<MLString>* e= messages.getElement(i);
  if( currentMessage == *e->element )
    changeMessage(MLString(""));

  messages.remove(e);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void VChooser::setInteger( int i )
{
  if( i < 1 || i > messages.getNElements() )
    return;
  changeMessage( *messages.get(i) );
}

/////////////////////////////////////////////////////////////////////////////

int VChooser::getLength() const
{
  return messages.getNElements();
}

/////////////////////////////////////////////////////////////////////////////

// 1 <= i <= nElements
const MLString& VChooser::getMessage( int i )
{
  if( i < 1 || i > messages.getNElements() )
    return MLString::empty();

  return *messages.get(i);
}

/////////////////////////////////////////////////////////////////////////////

// return 0 if not in list
int VChooser::getMessagePosition( const MLString& message )
{
  int index= 1;

  IListIterator<MLString> li(messages);
  while( ! li.eol() )
    {
    const MLString& msg= *li.next();
    if( msg == message )
      return index;
    ++index;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void VChooser::drawMessage( const IVector& displ /* = IVector(0,0) */ )
{
  setFont(buttonFont);

  int _width= width-4;
  _width-= MLPixmaps::littleButton->getWidth()+3;

  IPoint textpos( 6,
                  (height + buttonFont->getHeight())/2
                          - buttonFont->getDescent()-1 );
  textpos+= displ;

  drawString( textpos, drawnMessage );

  MLPixmap* pixmap= (state == RELEASED) ? MLPixmaps::littleButton
                                        : MLPixmaps::littlePressedButton;
  drawPixmap( pixmap,
              width-3-pixmap->getWidth(),
              textpos.y() - buttonFont->getAscent()/2 -displ.y()
                          +7 - pixmap->getHeight() );
}

/////////////////////////////////////////////////////////////////////////////

void VChooser::drawButton()
{
  if( ! beginDraw() )
    return;

  int _width= isCurrent() ? (width-1) : width;
  int _height= isCurrent() ? (height-1) : height;
  int d= isCurrent() ? 1 : 0 ;

  switch( state )
    {
    case RELEASED:
      drawUnpressedRectangle( d, d, _width, _height );
      gray2();
      fillRectangle( IPoint(d+1,d+1), IPoint(_width-3,_height-3) );
      break;
    case PRESSED:
      if( isCurrent() )
        {
        gray0();
        drawRectangle( IPoint(1,1), IPoint(width-2,height-2) );
        gray2();
        fillRectangle( IPoint(2,2), IPoint(width-3,height-3) );
        }
       else
        {
        drawPressedRectangle( _width, _height );
        gray2();
        fillRectangle( IPoint(d+2,d+2), IPoint(_width-2,_height-2) );
        }
      break;
    }

  if( ! isAvailable() )
    gray1();
   else
    gray0();

  switch( state )
    {
    case RELEASED:
      drawMessage();
      break;
    case PRESSED:
      drawMessage( IVector(1,1)*0 );
      break;
    }

  if( isCurrent() )
    {
    gray0();
    drawRectangle( IPoint(0,0), IPoint(width-1,height-1) );
    }

  endDraw();
}

/////////////////////////////////////////////////////////////////////////////

void VChooser::drawChooserPopup()
{
  if( popup == 0 )
    return;

  if( ! popup->beginDraw() )
    return;

  mlFront();

  for( int i= 1; i <= messages.getNElements(); ++i )
    drawPopupMessage( i, false );

  const IVector size= popup->getSize();
  popup->gray0();
  popup->drawRectangle( IPoint(0,0), IPoint(size.x()-1,size.y()-1) );

  popup->endDraw();
}

/////////////////////////////////////////////////////////////////////////////

boolean VChooser::handleEvent( MLEvent& event )
{
  if( isAvailable() && event.type == MLEvent::MOUSE_PRESS )
    setCurrent(true);

  if( event.type == MLEvent::REDRAW )
    {
    drawButton();
    return true;
    }

  if( ! isAvailable() )
    return VWindow::handleEvent(event);

  switch( event.type )
    {
    case MLEvent::MOUSE_PRESS:
      {
      press();
      drawWidget();
      VWindow::flush();
      Chrono::delay( vOptions->button.pressingDuration ); // sleeping
      pressTime= event.time;
      go_grabbing();
      break;
      }

    case MLEvent::MOUSE_RELEASE:
      {
      if( state == PRESSED )
        {
        unpress();
        drawWidget();
        }
      break;
      }

    default:
      return VWindow::handleEvent(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean VChooser::handleKey( Key key )
{
  if( key == KeyCodes::SPACE || key == KeyCodes::ENTER )
    {
    press();
    drawWidget();
    VWindow::flush();
    Chrono::delay( vOptions->button.pressingDuration ); // sleeping
    pressTime= 0;
    go_grabbing();
    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

static const int margin= 1;

int VChooser::getMouseIndex( const IPoint& mousePos )
{
  if( popupSW == 0 )
    return 0;
  if( mousePos.x() < popupSW->x1() || mousePos.x() > popupSW->x2() )
    return 0;
  int i= 1 + (mousePos.y()-popupSW->getPosition().y()-margin)
         / messageHeight;
  if( i < 1 || i > messages.getNElements() )
    return 0;

  return i;
}

/////////////////////////////////////////////////////////////////////////////

void VChooser::drawPopupMessage( int i, boolean highlight )
{
  if( popup == 0 )
    return;
  if( i < 1 || i > messages.getNElements() )
    return;

  int width= popup->getSize().x();
  int y= margin + messageHeight * (i-1);

  if( highlight )
    popup->gray1();
   else
    popup->gray3();

  popup->drawHorizLine( margin, width-1-margin, y );
  popup->drawVertLine( margin, y, y + messageHeight-2 );
  popup->gray0();
  popup->drawHorizLine( margin+2, width-1-margin-2, y+messageHeight-2 );
  popup->drawHorizLine( margin, width-1-margin, y+messageHeight-1 );
  popup->gray1();
  popup->drawHorizLine( margin+1, margin+1, y+messageHeight-2 );
  popup->drawHorizLine( width-1-margin-1, width-1-margin-1, y+messageHeight-2 );
  popup->drawVertLine( width-1-margin, y+1, y+messageHeight-2 );
  if( highlight )
    popup->gray3();
   else
    popup->gray2();
  popup->fillRectangle( IPoint(margin+1,y+1),
                        IPoint(width-1-margin-1,y+messageHeight-3) );
  popup->gray0();
  popup->drawString( IPoint(margin+5,
                            y+(messageHeight-buttonFont->getHeight())/2
                             +buttonFont->getAscent()-2),
                     *messages.get(i) );
}

/////////////////////////////////////////////////////////////////////////////

void VChooser::go_grabbing()
{
  if( grabbing )
    return;

  if( messages.empty() )
    {
    unpress();
    drawWidget();
    return;
    }

  messageHeight= buttonFont->getHeight()+6; // 20
  int width= 0;
  IListIterator<MLString> li(messages);
  while( ! li.eol() )
    width= max( width, buttonFont->getStringWidth(*li.next()) );

  int height= messages.getNElements() * messageHeight - 1;

  width+= 2*(margin+5);
  height+= 2*margin;

  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  IVector maxSize= graphicsSystem->getRootWindow()->getSize();
  if( width > maxSize.x() )
    width= maxSize.x();
  if( height > maxSize.y() )
    height= maxSize.y();

  popupSW= new SystemWindow;
  popupSW->setSaveUnder();

  IPoint pos= VWindow::getAbsolutePosition() - IVector(0,1);
  int currentPos= getInteger();
  if( currentPos != 0 )
    pos[1]-= (currentPos-1)*messageHeight;

  if( pos.y() < 0 || pos.y()+height >= maxSize.y() )
    {
    pos[1]= (pos.y()<0) ? 0 : (maxSize.y()-height);
    pos[0]+= getSize().x();
    if( pos.x()+width >= maxSize.x() )
      {
      pos[0] -= getSize().x()+width;
      if( pos.x() < 0 )
        pos[0]= 0;
      }
    }

  popupSW->create( graphicsSystem->getRootWindow(),
                   pos.x(), pos.y(), width, height );
  popupSW->map();

  popup= new VWindow(popupSW);
  popup->create( 0, 0, width, height );

  popup->setFont(buttonFont);

  drawChooserPopup();

  mouseIndex= 0;
  changeMouseIndex(getInteger());
  entered= false;

  grabbing= true;

  MLEventManager* eventManager= MLEventManager::getEventManager();

  previousGrabber= eventManager->setEventGrabber(this);
}

/////////////////////////////////////////////////////////////////////////////

void VChooser::end_grabbing()
{
  if( !grabbing )
    return;

  delete popup;
  popup= 0;

  delete popupSW;
  popupSW= 0;

  grabbing= false;

  MLEventManager* eventManager= MLEventManager::getEventManager();
  eventManager->setEventGrabber(previousGrabber);

  previousGrabber= 0;

  if( mouseIndex >= 1 && mouseIndex <= messages.getNElements() )
    {
    const MLString& newMessage= *messages.get(mouseIndex);
    if( newMessage != currentMessage )
      {
      changeMessage(newMessage);
      kwCall(KWEvent::SELECT);
      }
    }
  mouseIndex= 0;

  unpress();
}

/////////////////////////////////////////////////////////////////////////////

void VChooser::changeMouseIndex( int newIndex )
{
  if( newIndex < 1 || newIndex > messages.getNElements() )
    newIndex= 0;
  if( newIndex == mouseIndex )
    return;
  if( newIndex == 0 && !entered )
    return;

  popup->beginDraw(); // returns true
  drawPopupMessage( mouseIndex, false );
  drawPopupMessage( newIndex, true );
  popup->endDraw();
  mouseIndex= newIndex;
}

/////////////////////////////////////////////////////////////////////////////

boolean VChooser::eventGrabber( MLWindow* window, MLEvent& event )
{
  if( popup == 0 || event.window == 0 )
    return false;

  IPoint pos= event.window->getAbsolutePosition()
             +IVector(event.position.x(),event.position.y());
  int newIndex= getMouseIndex(pos);
  boolean into= pos.x() >= popupSW->x1() &&
              pos.x() <= popupSW->x2() &&
              pos.y() >= popupSW->y1() &&
              pos.y() <= popupSW->y2();

  switch( event.type )
    {
    case MLEvent::REDRAW:
      {
      if( window == popupSW )
        drawChooserPopup();
       else
        {
        MLEventManager* eventManager= MLEventManager::getEventManager();
        eventManager->sendEvent(window,event,false);
        }
      break;
      }

    case MLEvent::MOUSE_PRESS:
      {
      if( event.button != 1 )
        break;
      if( into )
        entered= true;
      if( ! into )
        {
        changeMouseIndex(0);
        end_grabbing();
        break;
        }
      changeMouseIndex(newIndex);
      break;
      }

    case MLEvent::MOUSE_RELEASE:
      {
      if( event.button != 1 )
        break;
      if( into )
        entered= true;
      unsigned long delta= event.time - pressTime;
      if( double(delta)/1000. > vOptions->button.pressingDuration*4. )
        end_grabbing();
      break;
      }

    case MLEvent::MOUSE_MOTION:
      {
      if( ! event.left_pressed() )
        break;
      if( into )
        entered= true;
      changeMouseIndex(newIndex);
      break;
      }

    case MLEvent::KEY_PRESS:
      {
      Key key= event.key;
      if( key == KeyCodes::DOWN || key == KeyCodes::RIGHT )
        {
        int mi= mouseIndex+1;
        if( mi > messages.getNElements() )
          mi= 1;
        changeMouseIndex(mi);
        break;
        }
      if( key == KeyCodes::UP || key == KeyCodes::LEFT )
        {
        int mi= mouseIndex-1;
        if( mi < 1 )
          mi= messages.getNElements();
        changeMouseIndex(mi);
        break;
        }
      if( key == KeyCodes::HOME || key == KeyCodes::PAGEUP )
        {
        changeMouseIndex(1);
        break;
        }
      if( key == KeyCodes::END || key == KeyCodes::PAGEDOWN )
        {
        changeMouseIndex(messages.getNElements());
        break;
        }
      if( key == KeyCodes::ESC )
        {
        changeMouseIndex(0);
        end_grabbing();
        break;
        }
      if( key == KeyCodes::ENTER || key == KeyCodes::SPACE )
        {
        end_grabbing();
        break;
        }
      break;
      }

    default:
      break;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

