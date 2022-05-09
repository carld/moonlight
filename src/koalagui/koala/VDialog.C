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
  VDialog.C

  VDialog : same as VWidgetsSet but with keyboard handling

  Stephane Rehel
  July 24 1996
*/

#include "tools/Common.h"

#include "graphics/MLEvent.h"
#include "graphics/MLEventManager.h"
#include "graphics/SoftWindow.h"
#include "graphics/KeyCodes.h"

#include "VCartouche.h"
#include "VDialog.h"
#include "VCard.h"
#include "VDialogManager.h"

#include "kw/KWDialog.h"


// private event!
#define DRAW_CARD (MLEvent::LAST+1)

/////////////////////////////////////////////////////////////////////////////

VDialog::VDialog( VWindow* parent ): VWidgetsSet(parent)
{
  cartouche= 0;
  kwDialog= 0;
  active= 0;
  cards= 0;
  nCards= 0;
  activeCard= 0;
  cardsRectangle= IRectangle( IPoint(5,5), IPoint(50,50) );
}

/////////////////////////////////////////////////////////////////////////////

VDialog::~VDialog()
{
  VDialogManager::removeDialog(this);

  delete cards; cards= 0;
  nCards= 0;
}

/////////////////////////////////////////////////////////////////////////////

void VDialog::create( int x, int y, int width, int height )
{
  VWidgetsSet::create( x, y, width, height );
  VDialogManager::addDialog(this);
}

/////////////////////////////////////////////////////////////////////////////

void VDialog::focus()
{
  VDialogManager::focus(this);

  MLEventManager* eventManager= MLEventManager::getEventManager();

  eventManager->setKeyboardFocus(this);
}

/////////////////////////////////////////////////////////////////////////////

void VDialog::getFocus()
{
  if( ! VDialogManager::hasFocus(this) )
    return;

  if( active != 0 )
    active->postDrawWidget(); // redraw it as focused
}

/////////////////////////////////////////////////////////////////////////////

void VDialog::ungetFocus()
{
  if( active != 0 )
    active->postDrawWidget(); // redraw it as unfocused
}

/////////////////////////////////////////////////////////////////////////////

void VDialog::addWidget( VWidget* widget,
                         boolean staticWidget, /* = false */
                         VCard* card /* = 0 */ )
{
  if( widget == 0 )
    return;
  if( widget == this ) // arf :-))))) ooooh funny thing
    return;

  VWidgetsSet::addWidget(widget,staticWidget);

  widget->dialog= this;
  if( widget !=  card ) // arf :-)
    widget->card= card;
}

/////////////////////////////////////////////////////////////////////////////

void VDialog::addCard( VCard* card )
{
  if( card ==  0 )
    return;

  // we put it just at the right of the previous one
  int x2= cardsRectangle.p1().x()+1;
  SIListIterator<VWidget> li(widgets);
  while( ! li.eol() )
    {
    VWidget* w= li.next();
    if( w->type() != VWidget::CARD ) continue;
    VCard* c= (VCard*) w;
    int _x2= c->getPosition().x() + c->getSize().x();
    if( _x2 > x2 ) x2= _x2;
    }

  card->setWidgetPosition( IPoint(x2,cardsRectangle.p1().y()) );

  if( activeCard == 0 )
    activeCard= card;

  addWidget( card, false, 0 );
}

/////////////////////////////////////////////////////////////////////////////

// remove widget but dont delete it
// return true if removed
boolean VDialog::removeWidget( VWidget* widget )
{
  VWidgetsSet::widgets.remove(widget);
  if( ! dialogWidgets.remove(widget) )
    return false;

  if( active == widget )
    active= 0;

  if( widget->type() == VWidget::CARD )
    {
    if( nCards == 1 )
      {
      nCards= 0;
      delete cards; cards= 0;
      }
     else
      {
      int i;
      for( i= 0; i < nCards; ++i )
        if( cards[i] == widget )
          break;
      if( i < nCards-1 )
        {
        while( i+1 < nCards )
          {
          cards[i]= cards[i+1];
          ++i;
          }
        }
      cards[nCards-1]= 0;
      --nCards;
      }

    if( widget == activeCard )
      activeCard= 0;
    }

  SIListIterator<VWidget> li(dialogWidgets);
  while( ! li.eol() )
    {
    VWidget* w= li.next();
    if( w->linkWidget == widget )
      w->linkWidget= 0;
    if( w->card == widget )
      w->card= 0;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void VDialog::makeIndex()
{
  dialogWidgets.destroy();

  // index cards first
  SIListIterator<VWidget> li(VWidgetsSet::getWidgets());
  while( ! li.eol() )
    {
    VWidget* w= li.next();
    if( w->type() == VWidget::CARD )
      _makeIndex(w);
    }
  li.reset();
  while( ! li.eol() )
    {
    VWidget* w= li.next();
    if( w->type() != VWidget::CARD )
      _makeIndex(w);
    }

  delete cards; cards= 0;
  nCards= 0;

  SIListIterator<VWidget> dw(dialogWidgets);
  while( ! dw.eol() )
    if( dw.next()->type() == VWidget::CARD )
      ++nCards;

  if( nCards > 0 )
    {
    cards= new VCard* [ nCards ];
    int i= 0;
    dw.reset();
    while( ! dw.eol() )
      {
      VWidget* w= dw.next();
      if( w->type() == VWidget::CARD )
        cards[i++]= (VCard*) w;
      }

    // sort by x
    for( i= 0; i < nCards-1; ++i )
      {
      int x1= cards[i]->getPosition().x();
      int k= -1;
      for( int j= i+1; j < nCards; ++j )
        {
        int x2= cards[j]->getPosition().x();
        if( x2 < x1 )
          {
          k= j;
          x1= x2;
          }
        }
      if( k != -1 )
        swap( (void*&)cards[i], (void*&)cards[k] );
      }

    if( activeCard == 0 )
      activeCard= cards[0];

    dw.reset();
    while( ! dw.eol() )
      {
      VWidget* w= dw.next();
      boolean yes= (w->type() == VWidget::CARD) ||
                 (w->card == 0) ||
                 (w->card == activeCard);
      w->map(yes);
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

// private & recursive
void VDialog::_makeIndex( VWidget* w )
{
  if( w == 0 )
    return;

  w->dialog= this;
  dialogWidgets.append(w);

  VWidgetsSet* set= 0;
  if( w->type() == VWidget::WIDGETS_SET )
    set= (VWidgetsSet*) w;

  if( set == 0 )
    return;

  SIListIterator<VWidget> li(set->getWidgets());
  while( ! li.eol() )
    _makeIndex( li.next() );
}

/////////////////////////////////////////////////////////////////////////////

VWidget* VDialog::getNextActive()
{
  if( dialogWidgets.getNElements() == 0 )
    return 0;

  int i= 0;
  SIListIterator<VWidget> li(dialogWidgets);
  while( ! li.eol() )
    if( li.next() == active )
      break;
     else
      ++i;

  if( active == 0 || li.eol() )
    i= -1; // not found, try first widget

  VWidget* w= 0;
  for( int j= 1; j <= dialogWidgets.getNElements(); ++j )
    {
    int k= (i+j) % dialogWidgets.getNElements();

    w= dialogWidgets.get(k+1);
    if( !w->staticWidget && w->dialog == this &&
        w->isAvailable() &&
        w->mapped() )
      break;
    w= 0;
    }

  return w;
}

/////////////////////////////////////////////////////////////////////////////

void VDialog::_drawCardsRectangle()
{
  if( cards == 0 || activeCard == 0 )
    return;

  IPoint p1= cardsRectangle.p1() + IVector(0,cards[0]->getSize().y()-1);
  IPoint p2= cardsRectangle.p2();

  gray2();
  fillRectangle( p1 + IVector(1,0),
                 cardsRectangle.p2() - IVector(2,2) );

  int x1= p1.x();
  int x2= activeCard->getPosition().x();
  int x3= x2+activeCard->getSize().x();
  int x4= p2.x() -1;
  gray3();
  if( x1 <= x2 ) drawHorizLine( x1, x2, p1.y() );
  if( x3 <= x4 ) drawHorizLine( x3, x4, p1.y() );
  drawVertLine( p1.x(), p1.y(), p2.y()-1 );
  gray1();
  drawHorizLine( p1.x()+1, p2.x()-1, p2.y()-1 );
  drawVertLine( p2.x()-1, p1.y()+1, p2.y()-1 );
  gray0();
  drawVertLine( p2.x(), p1.y(), p2.y() );
  drawHorizLine( p1.x(), p2.x(), p2.y() );
}

/////////////////////////////////////////////////////////////////////////////

void VDialog::drawWidget()
{
  removeRedraw();

  if( ! beginDraw() )
    return;

  VWindow::fillWindow( getGray2() );

  _drawCardsRectangle();

  endDraw();

  SIListIterator<VWidget> li( VWidgetsSet::getWidgets() );
  while( ! li.eol() )
    {
    VWidget* w= li.next();
    if( w->card == 0 ||
        w->card == activeCard )
      w->drawWidget();
    }
}

/////////////////////////////////////////////////////////////////////////////

void VDialog::setCardsRectangle( const IRectangle& r )
{
  cardsRectangle= r;
  postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void VDialog::setActiveCard( VCard* card )
{
  if( card == activeCard )
    return;
  if( ! dialogWidgets.exist(card) )
    return;

  SIListIterator<VWidget> li(dialogWidgets);
  while( ! li.eol() )
    {
    VWidget* w= li.next();
    if( w->card == 0 ) continue;
    if( w->card == activeCard )
      {
      w->map(false);
      continue;
      }
    if( w->card == card )
      {
      w->map(true);
//      w->postRedraw();
      continue;
      }
    }

  if( cartouche == 0 )
    {
    // single buffered
    VWindow::removeEvent(DRAW_CARD);
    VWindow::postPrivateEvent(DRAW_CARD);

    if( activeCard != 0 )
      activeCard->postRedraw();
    card->postRedraw();
    }
   else
    cartouche->postRedraw(); // double buffered

  activeCard= card;
}

/////////////////////////////////////////////////////////////////////////////

void VDialog::setCurrentWidget( VWidget* widget )
{
  boolean hasFocus= VDialogManager::hasFocus(this);

  if( widget == active && hasFocus )
    return;

  if( ! hasFocus )
    VDialogManager::focus(this);

  if( active != 0 && active != widget )
    {
    active->setCurrent(false);
    active->drawWidget();
    }

  active= widget;
}

/////////////////////////////////////////////////////////////////////////////

boolean VDialog::handleMLKeyboard( MLEvent& event )
{
  if( event.type == MLEvent::KEY_RELEASE )
    return true;

  if( event.type != MLEvent::KEY_PRESS )
    return false;

  Key key= event.key;

  if( key == KeyCodes::TAB )
    {
handle_tab:
    VWidget* next= getNextActive();
    if( next != 0 )
      {
      next->setCurrent(true);
      next->drawWidget();
      }
    return true;
    }

  if( key == KeyCodes::SHIFT_TAB )
    {
    if( active == 0 )
      goto handle_tab;

    // disgusting algo! { O(n^3) :-))) }
    VWidget* old_active= active;
    VWidget* previous= 0;
    for( int i= dialogWidgets.getNElements(); i; --i )
      {
      previous= active;
      active= getNextActive();
      if( active == old_active )
        {
        if( previous != 0 )
          {
          previous->setCurrent(true);
          previous->drawWidget();
          }
        break;
        }
      }

    return true;
    }

  if( key.hasAlt() && !key.hasControl() )
    {
    int base= key.getBase();
    if( base > int(' ') && base <= 126 )
      {
      SIListIterator<VWidget> li(dialogWidgets);
      while( !li.eol() )
        {
        VWidget* w= li.next();
        char hotChar= w->getHotChar();

        if( w->staticWidget || !w->isAvailable() || !w->mapped() )
          continue;

        int ok= 0;
        if( Key(char(base)).upperCase() == Key(hotChar) )
          ok= 1;
         else
          {
          VWidget* linkWidget= w->getLinkWidget();
          if( linkWidget != 0 )
            if( linkWidget->isAvailable() && linkWidget->mapped() &&
                Key(char(base)).upperCase() == Key(linkWidget->getHotChar()) )
              ok= 2;
          }
        if( ok != 0 )
          {
          boolean p= w->isCurrent();
          w->setCurrent(true);
          boolean c= w->isCurrent();
          if( !p && c )
            w->drawWidget();

          if( ok == 2 )
            key= KeyCodes::NULL_KEY; // just activate (link widget)
          break;
          }
        }
      }
    }

  if( kwDialog != 0 )
    {
    if( kwDialog->handleValidateCancelKey(key) )
      return true;
    }

  if( active == 0 )
    return true;

  if( active->isAvailable() )
    {
    KWEvent e;
    e.key= key;
    e.type= KWEvent::KEY;
    if( active->kwCall(e) )
      return true;
    }

/*
  if( kwDialog != 0 )
    {
    if( kwDialog->handleValidateCancelKey(key) )
      return true;
    }
*/

  if( ! active->isAvailable() )
    return true;

  if( active->handleKey(key) )
    {
    KWEvent e;
    e.key= key;
    e.type= KWEvent::KEY_PROCESSED;

    active->kwCall(e);

    return true;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean VDialog::handleEvent( MLEvent& event )
{
  if( event.type == DRAW_CARD )
    {
    if( activeCard == 0 )
      return true;
    if( ! beginDraw() )
      return true;
    _drawCardsRectangle();
    endDraw();

    SIListIterator<VWidget> li( VWidgetsSet::getWidgets() );
    while( ! li.eol() )
      {
      VWidget* w= li.next();
      if( w->card == activeCard )
        w->postDrawWidget();
      }

    return true;
    }

  if( event.type == MLEvent::MOUSE_PRESS )
    {
    VDialogManager::focus(this);
    setCurrentWidget(0);
    }

  return VWidgetsSet::handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////

boolean VDialog::eventGrabber( MLWindow* window, MLEvent& event )
{
  if( event.type == MLEvent::KEY_PRESS ||
      event.type == MLEvent::KEY_RELEASE )
    return handleMLKeyboard(event);

  MLEventManager* eventManager= MLEventManager::getEventManager();

  if( event.type == MLEvent::REDRAW )
    return eventManager->sendEvent(window,event,false);

  // test if this event is to be discard
  MLWindow* w= window;
  while( w != cartouche )
    {
    if( w == 0 )
      return false; // not for us
    if( w->isSystemWindow() )
      w= ((SystemWindow*)w)->getParent();
     else
      w= ((SoftWindow*)w)->getSystemParent();
    }

  // ok, dispatch it
  return eventManager->sendEvent(window,event,false);
}

/////////////////////////////////////////////////////////////////////////////
