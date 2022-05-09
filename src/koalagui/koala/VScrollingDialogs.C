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
  VScrollingDialogs.C

  Stephane Rehel
  August 24 1996
*/

#include "graphics/mlgl.h"
#include "graphics/MLEvent.h"

#include "VWindow.h"
#include "VVertBar.h"
#include "VOptions.h"
#include "VScrollingCartouche.h"

#include "VScrollingDialogs.h"

#include "kw/KWDialog.h"

// static
int VScrollingDialogs::scrollBarWidth= 0;

/////////////////////////////////////////////////////////////////////////////

// static
int VScrollingDialogs::getScrollBarWidth()
{
  if( vOptions == 0 )
    {
    vOptions= new VOptions;
    vOptions->init();
    }

  return vOptions->scrollBar.width-5;
}

/////////////////////////////////////////////////////////////////////////////

VScrollingDialogs::VScrollingDialogs()
{
  total_height= 0;
  y= 0;
  window= 0;
  scrollBar= 0;

  scrollBarWidth= getScrollBarWidth();

  updatePositionFlag= true;
}

/////////////////////////////////////////////////////////////////////////////

VScrollingDialogs::~VScrollingDialogs()
{
  delete scrollBar; scrollBar= 0;
  delete window; window= 0;

  while( ! dialogs.empty() )
    {
    SDialog* sd= dialogs.getFirst();
    dialogs.removeFirst();
    delete sd;
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean VScrollingDialogs::create( SystemWindow* parent,
                                 int x, int y, int width, int height )
{
  SystemWindow::create( parent, x, y, width, height );

  window= new VWindow(parent);
  window->create( x+width, y, scrollBarWidth, height );

  scrollBar= new VVertBar(window);
  scrollBar->scrollBar_width= scrollBarWidth;
  scrollBar->create( 0, 0, height,
                     1, height );
  scrollBar->postRedraw();

  scrollBar->setWidgetEventHandler(this);
  scrollBar->changeTotalLength( max(0,total_height-getSize().y()) );

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void VScrollingDialogs::map( boolean yes /* = true */ )
{
  SystemWindow::map(yes);
  if( window != 0 )
    window->map(yes);
  if( scrollBar != 0 )
    {
    scrollBar->map(yes);
    if( yes )
      scrollBar->postDrawWidget();
    }
}

/////////////////////////////////////////////////////////////////////////////

void VScrollingDialogs::append( KWDialog* d )
{
  if( d == 0 )
    return;

  append( d->scartouche );
}

/////////////////////////////////////////////////////////////////////////////

void VScrollingDialogs::append( VScrollingCartouche* c )
{
  if( c == 0 )
    return;
  if( c->getKWDialog() == 0 )
    return;

  MLString name= c->getKWDialog()->getName();

  IListIterator<SDialog> li(dialogs);
  while(li)
    {
    SDialog* sd= li.next();
    if( sd->cartouche == c || sd->name == name )
      return;
    }

  SDialog* sd= new SDialog;
  sd->cartouche= c;
  sd->name= name;

  c->scrollingDialogs= this;

  dialogs.append(sd);
  c->setPosition( IPoint(0,total_height) );
  total_height+= c->getSize().y();
  scrollBar->changeTotalLength( max(0,total_height-getSize().y()+1) );

  postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void VScrollingDialogs::remove( const MLString& name )
{
  boolean found= false;
  SDialog* sd= 0;
  IListIterator<SDialog> li(dialogs);
  while( ! li.eol() )
    {
    sd= li.next();
    if( sd->name == name )
      {
      found= true;
      break;
      }
    }
  if( ! found )
    return;

  int height= sd->cartouche->getSize().y();

  while( ! li.eol() )
    {
    SDialog* sdd= li.next();
    IPoint p= sdd->cartouche->getPosition();
    sdd->cartouche->setPosition( IPoint(p.x(), p.y() - height) );
    }

  sd->cartouche->scrollingDialogs= 0;

  dialogs.remove(sd);

  delete sd->cartouche;
  delete sd;
  sd= 0;

  total_height-= height;
  scrollBar->changeTotalLength( max(0,total_height-getSize().y()+1) );

  if( max(0,total_height-getSize().y()+1) == 0 )
    set_y(0);

  postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

KWDialog* VScrollingDialogs::find( const MLString& name )
{
  IListIterator<SDialog> li(dialogs);
  while( ! li.eol() )
    {
    SDialog* sd= li.next();
    if( sd->name == name )
      return sd->cartouche->getKWDialog();
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void VScrollingDialogs::set_y( int new_y )
{
  new_y= max( 0, new_y );
  new_y= min( new_y, total_height-1 );
  if( new_y == y )
    return;

  y= new_y;

  if( updatePositionFlag && scrollBar != 0 )
    scrollBar->changePosition(y+1);

  int p= -y;
  IListIterator<SDialog> li(dialogs);
  while(li)
    {
    SDialog* sd= li.next();
    VScrollingCartouche* sc= sd->cartouche;
    sc->setPosition( IPoint(0,p) );
    p+= sc->getSize().y();
    sc->removeRedraw();
    }

  postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void VScrollingDialogs::delta_y( int delta_y )
{
  set_y( y + delta_y );
}

/////////////////////////////////////////////////////////////////////////////

// handles private widget events from VScrollBar
boolean VScrollingDialogs::handlePrivateWidgetEvent( const VWidgetEvent& event )
{
  if( scrollBar == 0 )
    return true;

  if( event.getWidget() == scrollBar )
    {
    if( event.type() == VWidgetEvent::SETBARPOS )
      {
      scrollBar->handlePrivateWidgetEvent(event);
      updatePositionFlag= false; // prevent from infinite loop
      set_y( scrollBar->getPosition()-1 );
      updatePositionFlag= true;
      return true;
      }
    }

  return scrollBar->handlePrivateWidgetEvent(event);
}

/////////////////////////////////////////////////////////////////////////////

void VScrollingDialogs::drawDialogsImages()
{
  int height= SystemWindow::getSize().y();

  int p= -y;
  IListIterator<SDialog> li(dialogs);
  while(li)
    {
    SDialog* sd= li.next();
    VScrollingCartouche* sc= sd->cartouche;
    int h= sc->getSize().y();
    if( !(p+h-1 < 0 || p >= height) )
      sc->putImage();
    p+= h;
    if( p >= height )
      break;
    }

  if( p < height )
    {
    SystemWindow::currentPixel();
    mlFront(); // assume front drawing
    mlDarkGray(); //glColor3i(0,0,0);

    if( p == -y ) --p;
    mlRect( 0, height-1-p, getSize().x()-1, 0 );
    }
}

/////////////////////////////////////////////////////////////////////////////

void VScrollingDialogs::postDrawScrollBar()
{
  if( scrollBar != 0 )
    scrollBar->postDrawWidget();
}

/////////////////////////////////////////////////////////////////////////////

boolean VScrollingDialogs::handleEvent( const MLEvent& event )
{
  if( event.type == MLEvent::REDRAW )
    {
    drawDialogsImages();
    return true;
    }

  return SystemWindow::handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////
