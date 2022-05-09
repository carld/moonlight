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
  MLCanvasPopup.C

  Created by an engine, associated with a canvas title, drawn and
  clickable on a canvas title

  Stephane Rehel

  November 27 1996
*/

#include "tools/IRectangle.h"

#include "graphics/mlgl.h"
#include "graphics/SoftWindow.h"
#include "graphics/MLEvent.h"
#include "graphics/MLFont.h"
#include "graphics/MLPixmap.h"

#include "MLCanvasPopup.h"
#include "MLPopup.h"
#include "MLCanvas.h"
#include "MLModule.h"

/////////////////////////////////////////////////////////////////////////////

static MLPixmap* down_arrow_pixmap= 0;

/////////////////////////////////////////////////////////////////////////////

MLCanvasPopup::MLCanvasPopup( MLCanvas* _canvas ): MLCanvasAction(_canvas)
{
  popup= 0;
  selected_label= 0;
  title= "";
  text_y= 0;

#define begin_pixmap(name) { char* _##name[]= {
#define end_pixmap(name) };name= new MLPixmap; \
                  (name)->make( (const unsigned char**)_##name, \
                                 sizeof(_##name)/sizeof(char*) ); }

  if( down_arrow_pixmap == 0 )
    {
    begin_pixmap(down_arrow_pixmap)
      "11111",
      "1   1",
      "1   1",
      " 1 1 ",
      " 1 1 ",
      "  1  "
    end_pixmap(down_arrow_pixmap)
    }
#undef end_pixmap
#undef begin_pixmap

  highlight_when_cursor_enter= true;
}

/////////////////////////////////////////////////////////////////////////////

MLCanvasPopup::~MLCanvasPopup()
{
  delete popup;
  popup= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasPopup::create( const MLString& _title )
{
  popup= new MLPopup(this);
  setTitle(_title);
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasPopup::setTitle( const MLString& _title )
{
  if( _title == title || canvas == 0 )
    return;

  title= _title;

  int width1= canvas->titleFont->getStringWidth(title);
  int margin= canvas->titleFont->getCharWidth(' ') / 2;

  MLCanvasAction::width= margin + width1 + margin;

  if( down_arrow_pixmap != 0 )
    MLCanvasAction::width+= down_arrow_pixmap->getWidth();
}

/////////////////////////////////////////////////////////////////////////////


void MLCanvasPopup::createWindow()
{
  if( canvas == 0 )
    return;
  if( canvas->titleWindow == 0 )
    return;

  IVector size= IVector( MLCanvasAction::getWidth(), canvas->titleHeight );

  text_y= (canvas->titleHeight-canvas->titleFont->getHeight()) / 2
         +canvas->titleFont->getDescent() + 1;

  int xx= x;
  if( left == -1 )
    xx -= size.x();

  window= new SoftWindow;
  window->create( canvas->titleWindow, xx, 0, size.x(), size.y() );
  window->map();
  window->setEventHandler(this);
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasPopup::draw()
{
  if( window == 0 )
    return;
  if( ! window->mapped() )
    return;

  window->currentPixel();
  mlFront();

  if( pressed )
    mlWhite();
   else
    mlLightGray();

  mlRect( 0, 0, window->getSize().x()-1, window->getSize().y()-1 );

  if( !pressed && cursor_highlighted )
    glColor3f(.3,.3,.3);
   else
    mlBlack();

  int margin= canvas->titleFont->getCharWidth(' ') / 2;

  canvas->titleFont->print( margin, text_y-1, title );

  if( down_arrow_pixmap != 0 )
    {
    int x2= margin + canvas->titleFont->getStringWidth(title) + 1;
    down_arrow_pixmap->print(x2,down_arrow_pixmap->getHeight());
    }
  glFlush();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLCanvasPopup::eventHandler( MLWindow* w, MLEvent& event )
{
  if( canvas == 0 || popup == 0 )
    return false;

  if( ! canvas->getModule()->inFunction() )
    {
    switch( event.type )
      {
      case MLEvent::MOUSE_PRESS:
        {
        if( event.button != 1 )
          break;

        if( window == 0 )
          return false;

        IPoint pos= window->getAbsolutePosition()
                   +IVector(0,window->getSize().y()+1);

        popup->open( pos.x(), pos.y(), 0, event.time );
        return true;
        }

      default:
        break;
      }
    }

  return MLCanvasAction::eventHandler(w,event);
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasPopup::setChecked( int label, boolean yes )
{
  if( popup == 0 )
    return;

  popup->check(label,yes);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLCanvasPopup::getChecked( int label ) const
{
  if( popup == 0 )
    return false;

  return popup->checked(label);
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasPopup::setAvailable( int label, boolean yes )
{
  if( popup == 0 )
    return;

  popup->enable(label,yes);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLCanvasPopup::getAvailable( int label ) const
{
  if( popup == 0 )
    return false;

  return popup->isEnabled(label);
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasPopup::popupCommand( MLPopup* _popup, int label )
{
  if( _popup != popup || canvas == 0 )
    return; // ugh

  if( label == MLPopup::OPEN )
    {
    pressed= true;
    draw();
    return;
    }

  if( label == MLPopup::CLOSE )
    {
    pressed= false;
    cursor_highlighted= false;
    draw();
    return;
    }

  selected_label= label;
  canvas->handleAction(this);
}

/////////////////////////////////////////////////////////////////////////////

