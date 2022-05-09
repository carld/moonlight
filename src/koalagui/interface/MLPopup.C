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
  MLPopup.C

  Stephane Rehel
  November 19 1996
*/

#include "MLPopup.h"

#include "tools/IRectangle.h"

#include "graphics/SystemWindow.h"
#include "graphics/MLFont.h"
#include "graphics/MLFonts.h"
#include "graphics/MLPixmap.h"
#include "graphics/MLPixmaps.h"
#include "graphics/KeyCodes.h"
#include "graphics/GraphicsSystem.h"
#include "graphics/MLEvent.h"
#include "graphics/MLEventManager.h"
#include "graphics/mlgl.h"

#include "MLButtonsPanel.h"

/////////////////////////////////////////////////////////////////////////////

// static
int MLPopup::OPEN= 1;
int MLPopup::CLOSE= 2;

static int labelCounter= 10;
static MLFont* font= 0;

static const int margin= 1;
static int messageHeight= 0;

static const int accel_margin= 3;

static unsigned long pressTimeDelay= 200; // milliseconds

static const int max_open_popups= 20;
static MLPopup* openPopups[max_open_popups];
static SystemWindow* windowPopups[max_open_popups];
static int nOpenPopups= -1;

static MLEventGrabber* previousGrabber= 0;
static MLPopup* activePopup= 0;

/////////////////////////////////////////////////////////////////////////////

MLPopup::Item::Item()
{
  message= MLString("");
  label= 0;
  subPopup= 0;
  highlighted= false;
  checkable= false;
  checked= false;
  enabled= true;
}

/////////////////////////////////////////////////////////////////////////////

MLPopup::MLPopup( MLPopupHandler* _handler )
{
  handler= _handler;

  popupLabel= ++labelCounter;
  max_items= 30;
  items= new Item [ max_items ];
  nItems= 0;
  window= 0;
  buttonsPanel= 0;
  pressTime= 0;

  previouslySelected= -1;

  if( nOpenPopups == -1 )
    {
    nOpenPopups= 0;
    for( int i= 0; i < max_open_popups; ++i )
      {
      openPopups[i]= 0;
      windowPopups[i]= 0;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

MLPopup::~MLPopup()
{
  for( int i= nItems-1; i >= 0; --i )
    {
    delete items[ i ].subPopup;
    items[ i ].subPopup= 0;
    }

  delete [] items;
  items= 0;
  nItems= 0;
}

/////////////////////////////////////////////////////////////////////////////

int MLPopup::add( const MLString& message, MLPopup* sub /* = 0 */ )
{
  if( nItems >= max_items )
    return 0;

  ++labelCounter;

  int a= message.indexOf('\t');

  if( a == -1 )
    {
    items[ nItems ].accel_msg= "";
    items[ nItems ].message= message;
    }
   else
    {
    items[ nItems ].accel_msg= message.substring(a+1,message.length()-1);
    items[ nItems ].message= message.substring(0,a-1);
    }

  items[ nItems ].label= labelCounter;
  items[ nItems ].subPopup= sub;
  items[ nItems ].enabled= true;
  ++nItems;

  return labelCounter;
}

/////////////////////////////////////////////////////////////////////////////

int MLPopup::addCheck( const MLString& message )
{
  int label= add(message,0);

  items[ nItems - 1 ].checkable= true;
  items[ nItems - 1 ].checked= false+1;

  return label;
}

/////////////////////////////////////////////////////////////////////////////

// return 0 if not found
MLPopup::Item* MLPopup::findItem( int label )
{
  int i;

  for( i= 0; i < nItems; ++i )
    {
    Item* it= items + i;
    if( it->label == label )
      return it;
    }

  for( i= 0; i < nItems; ++i )
    {
    Item* it= items + i;
    if( it->subPopup != 0 )
      {
      Item* found= it->subPopup->findItem(label);
      if( found != 0 )
        return found;
      }
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLPopup::checked( int label )
{
  Item* i= findItem(label);
  if( i == 0 )
    return false;

  return i->checked;
}

/////////////////////////////////////////////////////////////////////////////

void MLPopup::check( int label, boolean yes )
{
  Item* i= findItem(label);
  if( i == 0 )
    return;
  if( ! i->checkable )
    return;

  i->checked= yes;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLPopup::isEnabled( int label )
{
  Item* i= findItem(label);
  if( i == 0 )
    return false;

  return i->enabled;
}

/////////////////////////////////////////////////////////////////////////////

// return previous value
boolean MLPopup::enable( int label, boolean yes /* = true */ )
{
  Item* i= findItem(label);
  if( i == 0 )
    return false;

  boolean prev= i->enabled;
  i->enabled= yes;

  return prev;
}

/////////////////////////////////////////////////////////////////////////////

void MLPopup::open( int x, int y, MLButtonsPanel* _buttonsPanel,
                    unsigned long _pressTime )
{
  if( window != 0 )
    return;

  if( nItems <= 0 )
    {
    if( handler != 0 )
      handler->popupCommand( this, popupLabel );
    return;
    }

  buttonsPanel= _buttonsPanel;
  pressTime= _pressTime;

  font= widgetFont;

  create(x,y);

  MLEventManager* eventManager= MLEventManager::getEventManager();

  if( previousGrabber == 0 )
    previousGrabber= eventManager->setEventGrabber(this);
}

/////////////////////////////////////////////////////////////////////////////

MLPopup::Item* MLPopup::getShortcutItem()
{
  MLPopup* p= this;

  for(;;)
    {
    if( p->previouslySelected < 0 || p->previouslySelected >= p->nItems )
      break;

    MLPopup::Item* item= &(p->items[p->previouslySelected]);

    if( ! item->enabled )
      break;

    if( item->subPopup == 0 )
      return item;

    p= item->subPopup;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLPopup::handleAction( Item* item )
{
  if( item == 0 )
    return false;

  if( handler == 0 )
    return false;

  if( ! item->enabled )
    return false;

  handler->popupCommand( this, item->label );

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// protected
void MLPopup::create( int x, int y )
{
  activePopup= this;

  if( window != 0 )
    return; // already open!

  messageHeight= font->getHeight()+6-1; // 20

  if( handler != 0 )
    handler->popupCommand( this, MLPopup::OPEN );

  int width= 0;
  int i;
  hasChecks= false;
  for( i= 0; i < nItems; ++i )
    {
    int w= font->getStringWidth(items[i].message);
    if( items[i].subPopup != 0 )
      w += 3 + MLPixmaps::smallWhiteArrow->getWidth();
    if( items[i].checkable )
      hasChecks= true;
    if( items[i].accel_msg.length() > 0 )
      w += ::accel_margin * font->getCharWidth(' ')
           + font->getStringWidth(items[i].accel_msg);

    width= max( width, w );
    }
  if( hasChecks )
    width += 2 + MLPixmaps::checkedBox->getWidth();

  int height= nItems * messageHeight - 1;

  width+= 2*(margin+5);
  height+= 2*margin;

  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  IVector maxSize= graphicsSystem->getRootWindow()->getSize();
  if( width > maxSize.x() )
    width= maxSize.x();
  if( height > maxSize.y() )
    height= maxSize.y();

  if( x + width - 1 >= maxSize.x() )
    x= maxSize.x() - width;
  if( y + height - 1 >= maxSize.y() )
    y= maxSize.y() - height;
  if( x < 0 ) x= 0;
  if( y < 0 ) y= 0;

  IPoint pos(x,y);

  window= new SystemWindow;
  window->setSaveUnder();

  window->create( graphicsSystem->getRootWindow(),
                  pos.x(), pos.y(), width, height );
  window->map();

  for( i= 0; i < nItems; ++i )
    items[i].highlighted= false;

  if( nOpenPopups < max_open_popups )
    {
    openPopups[ nOpenPopups ]= this;
    windowPopups[ nOpenPopups ]= window;
    ++nOpenPopups;
    }

  draw();

  if( previouslySelected >= 0 && previouslySelected < nItems )
    setActiveItem(previouslySelected,false);
}

/////////////////////////////////////////////////////////////////////////////

// return -1 if none
int MLPopup::getSelected() const
{
  for( int i= 0; i < nItems; ++i )
    if( items[i].highlighted )
      return i;

  return -1;
}

/////////////////////////////////////////////////////////////////////////////

// where is mouse pointer?
void MLPopup::get_popup( int x, int y,
                         MLWindow* window,
                         MLPopup*& popup,
                         int& position,
                         int& item )
{
  popup= 0;
  position= 0;
  item= 0;
  if( window == 0 )
    return;

  IPoint pos= IPoint(x,y) + window->getAbsolutePosition();

  for( int i= nOpenPopups-1; i >= 0; --i )
    {
    SystemWindow* sw= windowPopups[i];
    if( sw == 0 )
      continue;
    IPoint p1= sw->getAbsolutePosition();
    IPoint p2= p1 + sw->getSize() - IVector(1,1);
    if( IRectangle(p1,p2).into(pos) )
      {
      int dy= pos.y() - p1.y();

      popup= openPopups[i];
      if( popup == 0 )
        continue;
      position= i;
      item= (dy-margin) / messageHeight;
      if( item >= 0 && item < popup->nItems )
        return;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLPopup::setActiveItem( int i, boolean openSub )
{
  if( i < 0 || i >= nItems )
    i= -1;
  if( window == 0 )
    return;

  window->currentPixel();
  mlFront();
  for( int j= 0; j < nItems; ++j )
    {
    if( i == j )
      continue;

    if( items[j].highlighted )
      {
      if( items[j].subPopup != 0 )
        {
        items[j].subPopup->close();
        window->currentPixel();
        mlFront();
        }

      items[j].highlighted= false;
      drawItem(j);
      }
    }

  if( i == -1 )
    return;

  if( items[i].highlighted )
    {
    if( items[i].subPopup != 0 )
      {
      boolean opened= (items[i].subPopup->window != 0);
      if( (opened?1:0) == (openSub?1:0) )
        return; // no change to be done to sub popup
      }
     else
      return;
    }

  items[i].highlighted= true;
  drawItem(i);

  if( openSub && items[i].subPopup != 0 && items[i].enabled )
    {
    IPoint apos= window->getAbsolutePosition();
    int x= apos.x() + window->getSize().x() - 1;
    int y= apos.y() + messageHeight * i;
    items[i].subPopup->create(x,y);
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLPopup::draw()
{
  if( window == 0 )
    return;

  window->currentPixel();
  mlFront();

  mlBlack();

  IVector size= window->getSize();
  mlFrame( 0, 0, size.x()-1, size.y()-1 );

  for( int i= 0; i < nItems; ++i )
    drawItem(i);
  glFlush();
}

/////////////////////////////////////////////////////////////////////////////

void MLPopup::drawItem( int i )
{
  if( i < 0 || i >= nItems )
    return;
  if( window == 0 )
    return;

  IVector size= window->getSize();

  int y2= size.y() - 1 - messageHeight * i - 1;
  int y1= y2 - (messageHeight-1);
  ++y1;
  int x1= 1;
  int x2= size.x() - 2;

  Item& item= items [ i ];

  #define horizLine(x1,x2,y) mlLine(x1,y,x2,y)
  #define  vertLine(x,y1,y2) mlLine(x,y1,x,y2)

  if( item.highlighted && item.enabled )
    mlDarkGray()
   else
    mlWhite();
  horizLine( x1, x2, y2 );
  vertLine( x1, y2, y1 );
  mlDarkGray();
  horizLine( x1+1, x1+2, y1 );
  vertLine( x2, y2-1, y1 );
  horizLine( x2-1, x2, y1 );
  mlBlack();
  horizLine( x1+2, x2-2, y1 );
  horizLine( x1, x2, y1-1 );

  if( item.highlighted && item.enabled )
    mlWhite();
   else
    mlLightGray();

  mlRect( x1+1, y1+1, x2-1, y2-1 );

  if( item.checkable )
    {
    MLPixmap* p= item.checked ? MLPixmaps::checkedBox
                              : MLPixmaps::uncheckedBox;
    int h= p->getHeight();

    p->print( x1 + 3, y1 + (messageHeight-h) / 2 + h - 2 );
    }

  if( hasChecks )
    x1 += 2 + MLPixmaps::checkedBox->getWidth();

  if( item.enabled )
    mlBlack();
   else
    mlDarkGray();

  int text_y= y1+(messageHeight-font->getHeight())/2
                 +font->getDescent()+1;

  font->print( x1 + 4, text_y - 1, item.message );

  if( item.subPopup != 0 )
    {
    int w= MLPixmaps::smallWhiteArrow->getWidth();
    MLPixmaps::smallWhiteArrow->print( x2 - 3 - w,
                                       y1 + messageHeight/2 + 1 );
    }

  if( item.accel_msg.length() > 0 )
    {
    int w= font->getStringWidth(item.accel_msg);
    font->print( x2 - w - 3, text_y - 1, item.accel_msg );
    }

  #undef vertLine
  #undef horizLine
}

/////////////////////////////////////////////////////////////////////////////

void MLPopup::close()
{
  if( window == 0 )
    return;

  int i;
  for( i= 0; i < nItems; ++i )
    {
    if( items[i].highlighted )
      {
      if( items[i].subPopup != 0 )
        items[i].subPopup->close();
      items[i].highlighted= false;
      }
    }

  for( i= 0; i < nOpenPopups; ++i )
    if( openPopups[i] == this )
      break;

  if( i < nOpenPopups )
    {
    openPopups[i]= 0;
    windowPopups[i]= 0;
    --nOpenPopups;
    }

  delete window;
  window= 0;

  if( activePopup == this )
    activePopup= 0;

  if( handler != 0 )
    handler->popupCommand( this, MLPopup::CLOSE );
}

/////////////////////////////////////////////////////////////////////////////

void MLPopup::quit( boolean unpress_button_panel /* =true */ )
{
  if( window == 0 )
    return;

  close();

  activePopup= 0;

  MLEventManager* eventManager= MLEventManager::getEventManager();

  eventManager->setEventGrabber(previousGrabber);
  previousGrabber= 0;

  if( unpress_button_panel && buttonsPanel != 0 )
    {
    buttonsPanel->unpress();
    buttonsPanel= 0;
    }
}


/////////////////////////////////////////////////////////////////////////////

boolean MLPopup::keyPressed( MLEvent& event )
{
  Key key= event.key;

  if( key == KeyCodes::ESC )
    {
    quit();
    return true;
    }

  if( key == KeyCodes::ENTER || key == KeyCodes::SPACE  )
    {
    if( activePopup == 0 )
      {
      quit();
      return true;
      }

    MLPopup* popup= activePopup;
    int i= popup->getSelected();
    if( i == -1 )
      return true;

    if( ! popup->items[i].enabled )
      return true;

    if( popup->items[i].subPopup != 0 )
      {
      popup->setActiveItem(i,true);
      activePopup= popup->items[i].subPopup;
      return true;
      }

    for( int j= 0; j < nOpenPopups; ++j )
      {
      if( openPopups[j] == 0 )
        continue;
      openPopups[j]->previouslySelected= openPopups[j]->getSelected();
      }
    if( popup->handler != 0 )
      {
      quit(false); // since this menu has been selected!
      popup->handler->popupCommand( popup, popup->items[i].label );
      }
     else
      quit();
    return true;
    }

  if( activePopup == 0 )
    return true;

  // could return -1
  int index= activePopup->getSelected();

  // check for full disabled items
  boolean all_disabled= true;
  for( int k= 0; k < activePopup->nItems; ++k )
    if( activePopup->items[k].enabled )
      {
      all_disabled= false;
      break;
      }

  if( all_disabled )
    return true;

doit_again:
  if( key == KeyCodes::DOWN || key == KeyCodes::TAB )
    {
    index= (index >= activePopup->nItems-1) ? 0 : (index+1);
    goto update;
    }
  if( key == KeyCodes::END || key == KeyCodes::PAGEDOWN )
    {
    index= activePopup->nItems-1;
    key= KeyCodes::UP; // in case last item is disabled
    goto update;
    }
  if( key == KeyCodes::HOME || key == KeyCodes::PAGEUP )
    {
    index= 0;
    key= KeyCodes::DOWN; // in case first item is disabled
    goto update;
    }
  if( key == KeyCodes::UP || key == KeyCodes::SHIFT_TAB )
    {
    index= (index<=0) ? (activePopup->nItems-1) : (index-1);
    goto update;
    }

  if( key == KeyCodes::RIGHT && index != -1 )
    {
    if( activePopup->items[index].subPopup != 0 &&
        activePopup->items[index].enabled )
      {
      MLPopup* popup= activePopup;
      popup->setActiveItem(index,true);
      activePopup= popup->items[index].subPopup;
      return true;
      }
    return true;
    }
  if( key == KeyCodes::LEFT )
    {
    int i;
    for( i= 0; i < nOpenPopups; ++i )
      if( openPopups[i] == activePopup )
        {
        if( i == 0 )
          return true;
        activePopup->close();
        activePopup= openPopups[i-1];
        return true;
        }
    return true;
    }

  return true;

update:
  if( ! activePopup->items[index].enabled )
    goto doit_again;

  activePopup->setActiveItem(index,false);
  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLPopup::eventGrabber( MLWindow* window, MLEvent& event )
{
  MLPopup* popup= 0;
  int item= 0;
  int position= 0;

  switch( event.type )
    {
    case MLEvent::REDRAW:
      {
      boolean found= false;
      int i;
      for( i= 0; i < nOpenPopups; ++i )
        if( windowPopups[i] == window )
          {
          found= true;
          break;
          }

      if( ! found )
        {
        MLEventManager* eventManager= MLEventManager::getEventManager();

        return eventManager->sendEvent(window,event,false);
        }

      MLPopup* popup= openPopups[i];
      if( popup == 0 )
        return false;

      popup->draw();
      break;
      }

    case MLEvent::MOUSE_RELEASE:
      {
      get_popup( event.position.x(), event.position.y(), event.window,
                 popup, position, item );
      switch( event.button )
        {
        case 3:
          {
          quit();
          break;
          }
        case 1:
          {
          if( popup == 0 )
            {
            unsigned long delta= event.time - pressTime;
            if( delta > ::pressTimeDelay )
              quit();
            break;
            }
          if( popup->items[item].subPopup != 0 )
            break;
          for( int j= 0; j < nOpenPopups; ++j )
            {
            if( openPopups[j] == 0 )
              continue;
            openPopups[j]->previouslySelected= openPopups[j]->getSelected();
            }
          if( handler != 0 && popup->items[item].enabled )
            {
            quit(false); // since this menu has been selected!
            handler->popupCommand( this, popup->items[item].label );
            }
           else
            quit();
          break;
          }
        default:
          break;
        }
      break;
      }

    case MLEvent::MOUSE_MOTION:
      {
      get_popup( event.position.x(), event.position.y(), event.window,
                 popup, position, item );

      if( ! event.left_pressed() )
        break;
      if( popup != 0 )
        {
        popup->setActiveItem(item,true);
        activePopup= popup;
        }
       else
        {
        unsigned long delta= event.time - pressTime;
        if( delta > ::pressTimeDelay )
          {
          if( activePopup != 0 )
            activePopup->setActiveItem(-1,false);
          }
        }

      break;
      }

    case MLEvent::MOUSE_PRESS:
      {
      get_popup( event.position.x(), event.position.y(), event.window,
                 popup, position, item );
      if( event.button != 1 )
        break;
      if( popup == 0 )
        quit();
       else
        {
        popup->setActiveItem(item,true);
        activePopup= popup;
        }
      break;
      }

    case MLEvent::KEY_PRESS:
      return keyPressed(event);

    default:
      break;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
