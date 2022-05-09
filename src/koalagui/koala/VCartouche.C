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
  VCartouche.C

  Stephane Rehel
  August 6 1996
*/

#include "graphics/MLEvent.h"
#include "graphics/MLFont.h"
#include "graphics/MLFonts.h"
#include "graphics/MLPixmap.h"
#include "graphics/MLPixmaps.h"
#include "graphics/MLEventManager.h"

#include "VButtonBase.h"
#include "VCartouche.h"
#include "VDialog.h"
#include "kw/KWDialog.h"

/////////////////////////////////////////////////////////////////////////////

class VKillButton: public VButtonBase
{
public:
  VKillButton( VWindow* _parent ): VButtonBase(_parent) {}
  virtual ~VKillButton() {}
  void create( int x, int y );
  VWidget::TYPE type() const { return VWidget::FIRST; }
  virtual void drawButton();
};

void VKillButton::create( int x, int y )
{
  VButtonBase::create( x, y,
                       MLPixmaps::cross->getWidth()+5,
                       MLPixmaps::cross->getWidth()+5 );
}

void VKillButton::drawButton()
{
  if( ! beginDraw() )
    return;

  VButtonBase::_drawButtonBase();
  int d= 0;
  if( state == PRESSED )
    ++d;
  drawPixmap( MLPixmaps::cross, 2+d, 2+d );

  endDraw();
}

/////////////////////////////////////////////////////////////////////////////

VCartouche::VCartouche( SystemWindow* _parent, KWDialog* _kwDialog /* = 0 */ )
{
  parent= _parent;
  kwDialog= _kwDialog;

  window= 0;

  frame= true; killIcon= false;
  title= MLString("");
  killButton= 0;
  titleHeight= widgetFont->getHeight()+5;

  dialog= 0;
}

/////////////////////////////////////////////////////////////////////////////

VCartouche::~VCartouche()
{
  delete killButton;
  killButton= 0;

  delete dialog;
  dialog= 0;

  delete window;
  window= 0;
}

/////////////////////////////////////////////////////////////////////////////

IRectangle VCartouche::getClientArea() const
{
  if( window == 0 )
    return IRectangle( IPoint(0,0), IVector(1,1) );

  if( frame )
    {
    IVector size= window->getSize();
    return IRectangle( IPoint(3,3+titleHeight+1),
                       IPoint(size.x()-4,size.y()-4) );
    }
   else
    return window->getRectangle();
}

/////////////////////////////////////////////////////////////////////////////

IVector VCartouche::probeCartoucheSize( const IVector& clientSize,
                                        boolean frame ) const
{
  if( frame )
    return clientSize + IVector(3+3,3+3+titleHeight+1);
   else
    return clientSize;
}

/////////////////////////////////////////////////////////////////////////////

void VCartouche::create( int x, int y, int width, int height,
                         boolean _frame, boolean _killIcon,
                         const MLString& _title )
{
  frame= _frame;
  killIcon= (frame) ? _killIcon : false;
  title= _title;

  IVector windowSize= probeCartoucheSize( IVector(width,height), frame );

  // for convenience under windoze
  SystemWindow::setSaveUnder();
  SystemWindow::create( parent, x, y, windowSize.x(), windowSize.y() );

  window= new VWindow(this);
  window->create( 0, 0, windowSize.x(), windowSize.y() );
  window->setEventHandler(this);

  if( killIcon )
    {
    killButton= new VKillButton(window);
    killButton->create( windowSize.x()-1-3-2-14, 3+2 );
    killButton->removeRedraw();
    killButton->setWidgetEventHandler(this);
    }

  IRectangle client= getClientArea();

  dialog= new VDialog(window);
  dialog->cartouche= this;
  dialog->create( client.p1().x(), client.p1().y(),
                  client.width(), client.height() );

  MLEventManager* eventManager= MLEventManager::getEventManager();

  eventManager->removeRedraw(this);
}

/////////////////////////////////////////////////////////////////////////////

void VCartouche::drawCartouche()
{
  if( !frame )
    return;

  if( ! window->beginDraw() )
    return;

  int width= window->getSize().x();
  int height= window->getSize().y();
  window->gray2();
  window->drawRectangle( IPoint(2,2), IPoint(width-3,height-3) );
  window->drawHorizLine( 2, width-3, 3+titleHeight );
window->gray0();
window->drawHorizLine( 0, width-2, 0 );
window->drawVertLine( 0, 0, height-2 );
  window->gray3();

  window->drawHorizLine( 1, width-2, 1 );
  window->drawVertLine( 1, 1, height-2 );
  window->gray1();
  window->drawVertLine( width-2, 1, height-2 );
  window->drawHorizLine( 1, width-2, height-2 );
  window->gray0();
  window->drawHorizLine( 0, width-1, height-1 );
  window->drawVertLine( width-1, 0, height-1 );
  window->gray1();
  window->fillRectangle( IPoint(3,3), IPoint(width-4,titleHeight-1+3) );
window->gray3();
window->drawHorizLine( 2, width-3, 3+titleHeight );

  window->gray3();

  window->setFont( widgetBoldFont );
  int titleWidth= width-6-3-3;
  if( killButton != 0 )
    titleWidth-= killButton->getSize().x() - 3;
  MLString t= window->cutString( title, titleWidth );
  window->drawString( IPoint( 7, 3+2+widgetFont->getAscent()-1 ), t );

  window->endDraw();

  if( killButton != 0 )
    killButton->drawWidget();
}

/////////////////////////////////////////////////////////////////////////////

// double buffered
void VCartouche::drawCartoucheAndDialog()
{
  if( ! SystemWindow::mapped() )
    return;

//  eventManager->removeRedraw(this);
  current();
  window->back();
  drawCartouche();
  dialog->drawWidget();
  window->swapBuffers();
  window->front();

// single buffered
//  current();
//  drawCartouche();
//  dialog->drawWidget();
}

/////////////////////////////////////////////////////////////////////////////

boolean VCartouche::handleEvent( const MLEvent& event )
{
  if( event.type == MLEvent::REDRAW )
    {
    drawCartoucheAndDialog();
    return true;
    }

  return SystemWindow::handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////

boolean VCartouche::VWindowEvent( VWindow*, MLEvent& event )
{
  return window->handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////

// newSize = new dialogSize
void VCartouche::setDialogSize( const IVector& newSize )
{
  if( dialog == 0 )
    return;

  IVector old_cSize= SystemWindow::getSize();
  IVector cSize= probeCartoucheSize(newSize,frame);

  SystemWindow::setSize(cSize);
  window->setSize(cSize);
  dialog->setSize(newSize);

  if( killButton != 0 )
    killButton->setPosition( killButton->getPosition()
                             + IVector(cSize.x()-old_cSize.x(),0) );
}

/////////////////////////////////////////////////////////////////////////////

boolean VCartouche::handlePrivateWidgetEvent( const VWidgetEvent& event )
{
  if( killButton == 0 )
    return false;

  if( event.getWidget() == killButton &&
      event.type() == VWidgetEvent::ENDPRESS && kwDialog != 0 )
    kwDialog->quit(KWDialog::CANCEL);

  return false;
}

/////////////////////////////////////////////////////////////////////////////
