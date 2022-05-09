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
  VScrollingCartouche.C

  Stephane Rehel
  August 24 1996
*/

#include <iostream>

#include "graphics/MLEvent.h"
#include "graphics/MLFont.h"
#include "graphics/MLFonts.h"
#include "graphics/MLPixmap.h"
#include "graphics/MLPixmaps.h"
#include "graphics/MLEventManager.h"
#include "graphics/SoftWindow.h"

#include "VButtonBase.h"
#include "VScrollingCartouche.h"
#include "VScrollingDialogs.h"
#include "VDialog.h"
#include "kw/kw.h"
#include "kw/KWDialog.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////

class VSKillButton: public VButtonBase
{
public:
  VSKillButton( VWindow* _parent ): VButtonBase(_parent) {}
  virtual ~VSKillButton() {}
  void create( int x, int y );
  VWidget::TYPE type() const { return VWidget::FIRST; }
  virtual void drawButton();
};

void VSKillButton::create( int x, int y )
{
  VButtonBase::create( x, y,
                       MLPixmaps::cross->getWidth()+5,
                       MLPixmaps::cross->getWidth()+5 );
}

void VSKillButton::drawButton()
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

VScrollingCartouche::VScrollingCartouche( SystemWindow* _parent,
                                          KWDialog* _kwDialog /* = 0 */ ):
  VWindow( (MLWindow*)_parent )
{
  parent= _parent;
  kwDialog= _kwDialog;

  frame= true; killIcon= false;
  title= MLString("");
  killButton= 0;
  titleFont= widgetFont;
  titleHeight= titleFont->getHeight()+5;

  dialog= 0;
}

/////////////////////////////////////////////////////////////////////////////

VScrollingCartouche::~VScrollingCartouche()
{
  delete killButton;
  killButton= 0;

  delete dialog;
  dialog= 0;
}

/////////////////////////////////////////////////////////////////////////////

IRectangle VScrollingCartouche::getClientArea() const
{
  if( frame )
    {
    IVector size= VWindow::getSize();
    return IRectangle( IPoint(3,3+titleHeight+1),
                       IPoint(size.x()-4,size.y()-4) );
    }
   else
    return VWindow::getRectangle();
}

/////////////////////////////////////////////////////////////////////////////

// static
IVector VScrollingCartouche::probeCartoucheSize( const IVector& clientSize,
                                                 boolean frame )
{
  if( frame )
    {
    int titleHeight= widgetFont->getHeight()+5;
    return clientSize + IVector(3+3,3+3+titleHeight+1);
    }
   else
    return clientSize;
}

/////////////////////////////////////////////////////////////////////////////

void VScrollingCartouche::create( int x, int y, int width, int height,
                                  boolean _frame, boolean _killIcon,
                                  const MLString& _title )
{
  frame= _frame;
  killIcon= (frame) ? _killIcon : false;
  title= _title;

  IVector windowSize= probeCartoucheSize( IVector(width,height), frame );

  VWindow::create( x, y, windowSize.x(), windowSize.y() );
  VWindow::createWindowImage();

  if( killIcon )
    {
    killButton= new VSKillButton(this);
    killButton->create( windowSize.x()-1-3-2-14, 3+2 );
    killButton->removeRedraw();
    killButton->setWidgetEventHandler(this);
    }

  IRectangle client= getClientArea();

  dialog= new VDialog(this);
//  dialog->scartouche= this;
  dialog->create( client.p1().x(), client.p1().y(),
                  client.width(), client.height() );
}

/////////////////////////////////////////////////////////////////////////////

void VScrollingCartouche::drawCartouche()
{
  if( !frame )
    return;
#ifdef DEBUG
  cerr << "drawing cartouche" << endl;
#endif
  if( ! beginDraw() )
    return;

  int width= VWindow::getSize().x();
  int height= VWindow::getSize().y();
  gray2();
  drawRectangle( IPoint(2,2), IPoint(width-3,height-3) );
  drawHorizLine( 0, width-2, 0 );

gray0();
drawVertLine( 0, 0, height-2 );
gray3();
drawHorizLine( 2, width-3, 3+titleHeight );

int yy=0;
#ifdef __WINDOWS__
++yy;
#endif

  gray3();
  drawHorizLine( 1, width-3+yy, 1 );
  drawVertLine( 1, 1, height-3+yy );
  gray1();
  drawVertLine( width-2, 1, height-2 );
  drawHorizLine( 1, width-2, height-2 );

  gray0();
  drawHorizLine( 0, width-1, height-1 );
  drawVertLine( width-1, 0, height-1 );
  gray1();
  fillRectangle( IPoint(3,3), IPoint(width-4,titleHeight-1+3) );
  gray3();

  setFont( widgetBoldFont );
  int titleWidth= width-6-3-3;
  if( killButton != 0 )
    titleWidth-= killButton->getSize().x() - 3;
  MLString t= cutString( title, titleWidth );
  drawString( IPoint( 7, 3+2+titleFont->getAscent()-1 ), t );

  endDraw();

  if( killButton != 0 )
    killButton->drawWidget();
}

/////////////////////////////////////////////////////////////////////////////

void VScrollingCartouche::drawImage()
{
  if( ! VWindow::mapped() )
    return;

  VWindow::systemWindow->current();
  VWindow::back();
  drawCartouche();
  dialog->drawWidget();
//  VWindow::swapBuffers();
  VWindow::front();

// single buffered
//  current();
//  drawCartouche();
//  dialog->drawWidget();
}

/////////////////////////////////////////////////////////////////////////////

boolean VScrollingCartouche::handleEvent( MLEvent& event )
{
  if( event.type == MLEvent::REDRAW )
    {
    drawImage();
    return true;
    }

  if( scrollingDialogs != 0 )
    {
    static IPoint absPosition(0,0);

    if( event.type == MLEvent::MOUSE_PRESS )
      {
      absPosition= VWindow::getAbsolutePosition() + event.position;
      return true;
      }

    if( event.type == MLEvent::MOUSE_MOTION &&
        (event.left_pressed() ||
         event.middle_pressed() ||
         event.right_pressed()) )
      {
      IPoint ap= VWindow::getAbsolutePosition() + event.position;
      int dy= ap.y() - absPosition.y();
      scrollingDialogs->delta_y( -dy );
      absPosition= ap;
      return true;
      }

    return true;
    }

  return VWindow::handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////

boolean VScrollingCartouche::handlePrivateWidgetEvent(
                                                 const VWidgetEvent& event )
{
  if( killButton == 0 )
    return false;

  if( event.getWidget() == killButton &&
      event.type() == VWidgetEvent::ENDPRESS && kwDialog != 0 )
    {
    KWEvent event;
    event.type= KWEvent::KILL_DIALOG;
    event.widget= KWidget(0);
    event.key= Key::NULL_KEY;
    event.dialog= kwDialog;
    kwDialog->kwDialogCallback(event);
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////
