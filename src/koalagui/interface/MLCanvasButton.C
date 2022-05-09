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
  MLCanvasButton.C

  Created by an engine, associated with a canvas title, drawn and
  clickable on a canvas title

  Stephane Rehel

  November 30 1996
*/

#include "tools/IRectangle.h"

#include "graphics/mlgl.h"
#include "graphics/SoftWindow.h"
#include "graphics/MLEvent.h"
#include "graphics/MLFont.h"
#include "graphics/MLPixmap.h"

#include "MLCanvasButton.h"
#include "MLCanvas.h"
#include "MLModule.h"

/////////////////////////////////////////////////////////////////////////////

MLCanvasButton::MLCanvasButton( MLCanvas* _canvas ): MLCanvasAction(_canvas)
{
  title= "";
  pixmap= 0;
  highlighted_pixmap= 0;
  text_y= 0;

  highlight_when_cursor_enter= true;
}

/////////////////////////////////////////////////////////////////////////////

MLCanvasButton::~MLCanvasButton()
{
  pixmap= 0;
  highlighted_pixmap= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasButton::create( const MLString& _title )
{
  create( _title, 0, 0 );
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasButton::create( MLPixmap* _pixmap,
                             MLPixmap* _highlighted_pixmap /* = 0 */ )
{
  create( "", _pixmap, _highlighted_pixmap );
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasButton::create( const MLString& _title,
                             MLPixmap* _pixmap,
                             MLPixmap* _highlighted_pixmap )
{
  title= _title;

  set( _pixmap, _highlighted_pixmap );
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasButton::set( MLPixmap* _pixmap,
                          MLPixmap* _highlighted_pixmap /* = 0 */ )
{
  pixmap= _pixmap;
  highlighted_pixmap= _highlighted_pixmap;

  if( pixmap == 0 )
    highlighted_pixmap= 0;
   else
    {
    if( highlighted_pixmap == 0 )
      highlighted_pixmap= pixmap;
    }

  if( canvas != 0 )
    {
    int width1;
    if( pixmap == 0 )
      width1= canvas->titleFont->getStringWidth(title);
     else
      width1= pixmap->getWidth();

    int margin= canvas->titleFont->getCharWidth(' ') / 2;

    MLCanvasAction::width= margin + width1 + margin;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasButton::createWindow()
{
  if( canvas == 0 )
    return;
  if( canvas->titleWindow == 0 )
    return;

  text_y= (canvas->titleHeight-canvas->titleFont->getHeight()) / 2
         +canvas->titleFont->getDescent() + 1;

  IVector size( MLCanvasAction::getWidth(), canvas->titleHeight );

  int xx= x;
  if( left == -1 )
    xx -= size.x();
  window= new SoftWindow;
  window->create( canvas->titleWindow, xx, 0, size.x(), size.y() );
  window->map();
  window->setEventHandler(this);
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasButton::setTitle( const MLString& _title )
{
  if( title == _title )
    return;

  if( pixmap != 0 )
    return;

  title= _title;
  if( window != 0 )
    window->postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasButton::draw()
{
  if( canvas == 0 )
    return;
  if( canvas->mainWindow == 0 || window == 0 )
    return;

  int margin= canvas->titleFont->getCharWidth(' ') / 2;

  window->currentPixel();
  mlFront();

  if( pressed )
    mlWhite();
   else
    mlLightGray();

  mlRect( 0, 0, window->getSize().x()-1, window->getSize().y()-1 );

  if( pixmap == 0 )
    {
    if( !pressed && cursor_highlighted )
      glColor3f(.3,.3,.3);
     else
      mlBlack();

    canvas->titleFont->print( margin, text_y-1, title );
    }
   else
    {
    MLPixmap* p= pressed ? highlighted_pixmap : pixmap;

    int y= (canvas->titleHeight - p->getHeight()) / 2;
    y= window->getSize().y() - 1 - y;

    p->print( margin, y );
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLCanvasButton::eventHandler( MLWindow* w, MLEvent& event )
{
  if( canvas == 0 || window == 0 )
    return false;

  boolean into= window->into(event.position);

  if( ! canvas->getModule()->inFunction() )
    {
    switch( event.type )
      {
      case MLEvent::MOUSE_PRESS:
        {
        if( event.button != 1 )
          break;

        pressed= true;
        window->postRedraw();
        return true;
        }

      case MLEvent::MOUSE_MOTION:
        {
        if( ! event.left_pressed() )
          break;

        boolean _pressed= (boolean) into;
        if( _pressed != pressed )
          {
          pressed= _pressed;
          window->postRedraw();
          }
        return true;
        }

      case MLEvent::MOUSE_RELEASE:
        {
        if( event.button != 1 )
          break;

        pressed= false;
        window->postRedraw();
        if( into ) // do action...
          canvas->handleAction(this);

        return true;
        }

      default:
        break;
      }
    }

  return MLCanvasAction::eventHandler(w,event);
}

/////////////////////////////////////////////////////////////////////////////
