// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//                 2002       Patrick Mauritz
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
  MLButtonsPanel.C

  Stephane Rehel
  November 21 1996
*/

#include "MLButtonsPanel.h"

#include "graphics/SoftWindow.h"
#include "graphics/SystemWindow.h"
#include "graphics/MLEvent.h"
#include "graphics/GraphicsSystem.h"
#include "graphics/mlgl.h"
#include "graphics/MLFont.h"
#include "graphics/MLFonts.h"
#include "graphics/MLPixmap.h"
#include "graphics/MLPixmaps.h"

#include "MLPopup.h"
#include "MLModule.h"

/////////////////////////////////////////////////////////////////////////////

static const int nButtons= 20;
static int horizMargin= 2;
static const int rightMargin= 4;
static int internalMargin= 3;

/////////////////////////////////////////////////////////////////////////////

MLButtonsPanel::MLButtonsPanel( MLModule* _module )
{
  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  if( graphicsSystem != 0 )
    {
    if( graphicsSystem->getScreenSize().x() < 900 )
      {
      horizMargin= 1;
      internalMargin= 2;
      }
    }

  module= _module;

  windows= 0;
  buttons= 0;
  titles= 0;
  activated= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLButtonsPanel::~MLButtonsPanel()
{
  delete buttons;
  buttons= 0;

  for( int i= 1; i <= nButtons; ++i )
    {
    delete windows[i];
    windows[i]= 0;
    }

  delete windows;
  windows= 0;

  if( titles != 0 )
    {
    delete [] titles;
    titles= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLButtonsPanel::create( const IPoint& position, const IVector& size )
{
  if( windows != 0 )
    return; // crazy?

  windows= new SoftWindow* [ nButtons + 1 ];
  buttons= new MLPopup* [ nButtons + 1 ];
  titles= new MLString [ nButtons + 1 ];
  int i;

  for( i= 0; i <= nButtons; ++i )
    {
    windows[i]= 0;
    buttons[i]= 0;
    titles[i]= MLString("");
    }

  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  for( i= 1; i <= nButtons; ++i )
    {
    int x1= position.x();
    int x2= x1 + size.x() - 1;
    int y1= position.y() + (i-1) * size.y() / nButtons;
    int y2= position.y() +  i    * size.y() / nButtons - 1;

    windows[i]= new SoftWindow;
    windows[i]->create( graphicsSystem->getRootWindow(),
                        x1, y1, x2-x1+1, y2-y1+1 );
    windows[i]->setEventHandler(this);
    windows[i]->map(false);
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLButtonsPanel::map( boolean yes /* = true */ )
{
  if( windows == 0 )
    return;

  for( int i= 1; i <= nButtons; ++i )
    if( windows[i] != 0 )
      windows[i]->map(yes);
}

/////////////////////////////////////////////////////////////////////////////

void MLButtonsPanel::postRedraw()
{
  if( windows == 0 )
    return;

  for( int i= 1; i <= nButtons; ++i )
    if( windows[i] != 0 )
      {
      if( windows[i]->mapped() )
        windows[i]->postRedraw();
      }
}

/////////////////////////////////////////////////////////////////////////////

void MLButtonsPanel::setButton( int i, const MLString& title,
                                MLPopup* popup )
{
  if( i < 1 || i > nButtons )
    return;

  if( titles != 0 )
    titles[i]= title;
  if( buttons != 0 )
    buttons[i]= popup;
}

/////////////////////////////////////////////////////////////////////////////

template<class T> void swap( T& t1, T& t2 )
{ T t= t1; t1= t2; t2= t; }

void MLButtonsPanel::drawButton( int i,
                                 boolean pressed,
                                 boolean hightlighted /* = false */ )
{
  if( i < 1 || i > nButtons )
    return;
  if( windows == 0 )
    return;

  SoftWindow* window= windows[i];
  if( window == 0 )
    return;

  if( ! window->mapped() )
    return;

  IVector size= window->getSize();

  IPoint p1= IPoint(0,horizMargin);
  IPoint p2= IPoint(size.x()-1-rightMargin,size.y()-1-horizMargin);

  window->currentPixel();
  mlFront();
  mlBlack();
  mlRect( 0, 0, size.x()-1, p1.y()-1 );
  mlRect( 0, p2.y()+1, size.x()-1, size.y()-1 );
  mlRect( p2.x()+1, 0, size.x()-1, size.y()-1 );

  IPoint b1= p1 + IVector(internalMargin,internalMargin);
  IPoint b2= p2 - IVector(internalMargin,internalMargin);
  if( internalMargin > 0 )
    {
    mlColor( module->getColor() );
    mlRect( p1.x(), p1.y(), b1.x()-1, p2.y() );
    mlRect( b2.x()+1, p1.y(), p2.x(), p2.y() );
    mlRect( p1.x(), p1.y(), p2.x(), b1.y()-1 );
    mlRect( p1.x(), b2.y()+1, p2.x(), p2.y() );
    }

  mlBlack();
  mlFrame( b1.x(), b1.y(), b2.x(), b2.y() );

  int yy= 0;
  #ifdef __WINDOWS__
  ++yy;
  #endif

  if( pressed )
    { mlDarkGray(); }
   else
    { mlMiddleGray(); }
  mlLine( b1.x()+1, b2.y()-1, b2.x()-1+yy, b2.y()-1 );
  mlLine( b1.x()+1, b2.y()-1, b1.x()+1, b1.y()+1 );
  mlDarkGray();
  mlLine( b1.x()+2, b1.y()+1, b1.x()+2, b1.y()+1 );
  mlLine( b2.x()-2, b1.y()+1, b2.x()-1, b1.y()+1 );
  mlLine( b2.x()-1, b2.y()-2, b2.x()-1, b1.y()+1 );
  mlBlack();
  mlLine( b1.x()+3-yy*2, b1.y()+1, b2.x()-3+yy*3, b1.y()+1 );

/*
  if( pressed )
    mlWhite(); // { mlLightGray(); }
   else
    { mlMiddleGray(); }
  mlRect( b1.x()+2, b1.y()+2, b2.x()-2, b2.y()-2 );
*/

  boolean has_sub_popups= false;
  if( buttons[i] != 0 )
    if( buttons[i]->getNItems() > 0 )
      has_sub_popups= true;;

  if( pressed )
    {
    mlWhite();
    mlRect( b1.x()+2, b1.y()+2, b2.x()-2, b2.y()-2 );
    }
   else
    {
    int x1= b1.x()+2;
    int y1= b1.y()+2;
    int x2= b2.x()-1;
    int y2= b2.y()-1;
#ifdef __WINDOWS__
--x1;
#endif
    Color c1= Color(0.35,0.35,0.35);
    Color c2= Color(0.80,0.80,0.80);
    if( hightlighted && buttons[i] != 0 )
      swap( c1, c2 );
    Color c3= (c1+c2) / 2.;
    glShadeModel(GL_SMOOTH);
    glBegin(GL_QUADS);
      mlColor(c3);
      glVertex2i( x1, y1 );
      mlColor(c1);
      glVertex2i( x2, y1 );
      mlColor(c3);
      glVertex2i( x2, y2 );
      mlColor(c2);
      glVertex2i( x1, y2 );
    glEnd();
    glShadeModel(GL_FLAT);
    }

  MLString title("");
  if( titles != 0 )
    title= titles[i];

  MLFont* font= widgetFont;
  int title_x= b1.x()
               + ((b2.x()-b1.x()+1) - font->getStringWidth(title)) / 2;
  int title_y= b1.y() + ((b2.y()-b1.y()+1) - font->getHeight()) / 2
                      + font->getDescent() + 1;

//title_x= b2.x()-15-font->getStringWidth(title);

  if( has_sub_popups )
    {
    MLPixmap* arrow= MLPixmaps::rightArrow;
    int x= b2.x() - arrow->getWidth() * 3/2-2;
    int y= title_y + arrow->getHeight()-2;

    arrow->print( x, y-1 );
    }

  mlBlack();
  font->print( title_x, title_y-1, title );

  if( pressed )
    activated= i;

  glFlush();
}

/////////////////////////////////////////////////////////////////////////////

void MLButtonsPanel::draw()
{
  if( windows == 0 )
    return;

  for( int i= 1; i <= nButtons; ++i )
    drawButton(i,false);
}

/////////////////////////////////////////////////////////////////////////////

void MLButtonsPanel::unpress()
{
  if( activated >= 1 && activated <= nButtons )
    {
    drawButton(activated,false);
    activated= 0;
    glFlush();
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLButtonsPanel::eventHandler( MLWindow* w, MLEvent& event )
{
  if( w == 0 )
    return true;
  if( windows == 0 )
    return true;

  int i;
  for( i= 1; i <= nButtons; ++i )
    if( windows[i] == w )
      break;
  if( i > nButtons )
    return false; // unknown window!

  static boolean middle_action= false;
  static int pressed_button= 0;
  static boolean into= false;

  switch( event.type )
    {
    case MLEvent::MOUSE_ENTER:
    case MLEvent::MOUSE_LEAVE:
      {
      boolean enter= (event.type == MLEvent::MOUSE_ENTER);
      if( module->inFunction() )
        enter= false;
      drawButton(i,false,enter);
      break;
      }
    case MLEvent::REDRAW:
      {
      drawButton(i,false);
      break;
      }

    case MLEvent::MOUSE_PRESS:
      {
      if( module->inFunction() )
        break;

      if( event.button == 1 && !middle_action )
        {
        if( buttons[i] == 0 )
          return true;

        drawButton(i,true);
        glFlush();
        IPoint pos= windows[i]->getAbsolutePosition();
        IVector size= windows[i]->getSize();
        buttons[i]->open( pos.x() + size.x() - rightMargin
                                             - internalMargin,
                          pos.y() + horizMargin + internalMargin,
                          this,
                          event.time );
        glFlush();
        return true;
        }

      if( event.button == 2 &&
          !event.left_pressed() &&
          !event.right_pressed() )
        {
        middle_action= true;
        into= false;
        if( buttons[i] == 0 )
          pressed_button= 0;
         else
          {
          MLPopup::Item* item= buttons[i]->getShortcutItem();
          if( item == 0 )
            pressed_button= 0;
           else
            {
            drawButton(i,true);
            glFlush();
            pressed_button= i;
            into= true;
            }
          }
        return true;
        }

      break;
      }

    case MLEvent::MOUSE_MOTION:
      {
      if( event.middle_pressed() && middle_action && pressed_button != 0 )
        {
        boolean into2= false;
        if( pressed_button != 0 &&
            buttons[pressed_button] != 0 &&
            windows[pressed_button] != 0 )
          {
          if( windows[pressed_button]->into(event.position) )
            into2= true;
          }

        if( (into?1:0) == (into2?1:0) )
          return true;

        into= into2;
        drawButton(pressed_button,into);
        return true;
        }

      middle_action= false;
      return true;
      }

    case MLEvent::MOUSE_RELEASE:
      {
      if( event.button == 2 && middle_action )
        {
        middle_action= false;
        into= false;
        if( pressed_button == i && buttons[i] != 0 )
          {
          MLPopup::Item* item= buttons[i]->getShortcutItem();
          if( item != 0 )
            buttons[i]->handleAction(item);

          drawButton(i,false);
          glFlush();
          }
        pressed_button= 0;
        return true;
        }

      middle_action= false;
      break;
      }

    default:
      return w->handleEvent(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

