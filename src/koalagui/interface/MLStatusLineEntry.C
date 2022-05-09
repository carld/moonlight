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
  MLStatusLineEntry.C

  Stephane Rehel

  May 24 1998
*/

#include "tools/MLString.h"
#include "tools/Color.h"

#include "graphics/mlgl.h"
#include "graphics/GraphicsSystem.h"
#include "graphics/SoftWindow.h"
#include "graphics/KeyCodes.h"
#include "graphics/MLEvent.h"
#include "graphics/MLFont.h"

#include "MLModule.h"

#include "MLStatusLine.h"
#include "MLStatusLineEntry.h"
#include "MLStatusLineEntryCallback.h"

/////////////////////////////////////////////////////////////////////////////

MLStatusLineEntry::MLStatusLineEntry( MLModule* module,
                                      MLStatusLine* _line, int n_chars,
                                      MLStatusLineItem::Side _side,
                                      boolean _expandable /* = false */ ):
  MLStatusLineItem( module,
                    _line,
                    1 + MLStatusLine::getWidthFromChars(n_chars) + 2,
                    _side, _expandable )
{
  callback= 0;

  msg_offset= 0;
  cursor_position= 0;
  highlighted= false;
  entering= false;

  pressed= true;
  active_bg_color= Color(1,1,1) * (118./255.);
  unactive_bg_color= Color(1,1,1) / 3.;

  pressed_bg_color= unactive_bg_color;

  highlight_when_pressed= true;

  msg= "";

  history_size= 5;
  history_position= 0;
  history= new MLString* [ history_size + 1 ];
  for( int i= 0; i <= history_size; ++i )
    history[i]= 0;
  history[0]= new MLString("");

  last_log= "";
}

/////////////////////////////////////////////////////////////////////////////

MLStatusLineEntry::~MLStatusLineEntry()
{
  if( history != 0 )
    {
    for( int i= 0; i <= history_size; ++i )
      {
      delete history[i];
      history[i]= 0;
      }
    delete history;
    history= 0;
    }

  history_position= 0;
  history_size= 0;
}

/////////////////////////////////////////////////////////////////////////////

const MLString& MLStatusLineEntry::getLastLog() const
{
  return last_log;
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineEntry::setLastLog( const MLString& _last_log )
{
  last_log= _last_log;
  if( window != 0 )
    window->postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineEntry::setCallback( MLStatusLineEntryCallback* _callback )
{
  callback= _callback;
}

/////////////////////////////////////////////////////////////////////////////

int MLStatusLineEntry::getCharsWidth() const
{
  if( window == 0 )
    return 0;

  MLFont* font= MLStatusLine::getFont();
  if( font == 0 )
    return 0;

  return window->getSize().x() / font->getCharWidth(' ');
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineEntry::_draw()
{
  if( window == 0 )
    return;

  MLFont* font= MLStatusLine::getFont();
  if( font == 0 )
    return;

  IPoint position= MLStatusLine::getTextPosition();

  Color color= Color(1,1,1);
  mlColor(color);

  int charsWidth= getCharsWidth();

  const MLString* the_msg= 0;
  int mo= 0, // msg_offset
      cp= 0; // cursor_position
  if( !highlighted || !entering )
    {
    the_msg= &getLastLog();
    mo= 0;
    cp= the_msg->length();
    if( cp >= charsWidth )
      cp= charsWidth-1;
    }
   else
    {
    the_msg= &msg;
    mo= msg_offset;
    cp= cursor_position;
    }

  MLString m= the_msg->substring( mo, the_msg->length()-1 );
  if( m.length() > charsWidth )
    m= m.substring( 0, charsWidth-1 );

  font->print( position.x(), position.y(), m );

  if( highlighted )
    {
    // draw the cursor
    if( cp >= mo && cp < mo + charsWidth )
      {
      int fontWidth= font->getCharWidth(' ');
      int screen_position= cp - mo;
      int x= screen_position * fontWidth;
      int x1= position.x() + x;
      int y1= position.y() - font->getDescent() - 1;
      int x2= x1 + fontWidth - 1;
      int y2= y1 + font->getHeight() - 1;

      mlRect( x1, y1, x2, y2 );
      if( screen_position >= 0 &&
          screen_position < charsWidth &&
          cp < the_msg->length() )
        {
        mlColor(active_bg_color);
        font->print( position.x() + screen_position*fontWidth,
                     position.y(),
                     the_msg->get()+cp, 1 );
        }
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineEntry::draw()
{
  if( window == 0 )
    return;
  if( ! window->mapped() || module->isIconic() )
    return;

  window->currentPixel();
  mlFront();

  MLStatusLineItem::_draw();
  _draw();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLStatusLineEntry::eventHandler( MLWindow* w , MLEvent& e )
{
  if( w != window )
    return MLStatusLineItem::eventHandler(w,e);

  if( module != 0 )
    if( module->inFunction() )
      return MLStatusLineItem::eventHandler(w,e);

  if( e.type == MLEvent::MOUSE_ENTER )
    {
    pressed_bg_color= active_bg_color;
    highlighted= true;
    entering= false;
    window->postRedraw();
    return true;
    }

  if( e.type == MLEvent::MOUSE_LEAVE )
    {
    pressed_bg_color= unactive_bg_color;
    highlighted= false;
    entering= false;
    window->postRedraw();
    return true;
    }

  if( e.type == MLEvent::MOUSE_PRESS )
    {
    if( ! entering )
      {
      entering= true;
      window->postRedraw();
      }
     else
      {
      MLFont* font= MLStatusLine::getFont();
      if( font == 0 )
        return true;
      int fontWidth= font->getCharWidth(' ');

      IPoint position= MLStatusLine::getTextPosition();
      int p= (e.position.x() - position.x()) / fontWidth;
      if( p >= 0 )
        {
        p+= msg_offset;
        if( p > msg.length() )
          p= msg.length();

        if( p >= 0 && cursor_position != p )
          {
          cursor_position= p;
          window->postRedraw();
          }
        }
      }
    return true;
    }

  return MLStatusLineItem::eventHandler(w,e);
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineEntry::go_right()
{
  if( cursor_position >= msg.length() )
    return;
  int charsWidth= getCharsWidth();
  if( cursor_position+1 >= charsWidth )
    ++msg_offset;
  ++cursor_position;
  window->postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineEntry::go_left()
{
  if( cursor_position <= 0 )
    return;
  if( cursor_position == msg_offset && msg_offset > 0 )
    --msg_offset;
  --cursor_position;
  window->postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineEntry::go_end()
{
  int charsWidth= getCharsWidth();
  int cp= msg.length();
  int mo= 0;
  if( cp >= charsWidth )
    mo= cp - charsWidth + 1;
  if( cursor_position == cp && msg_offset == mo )
    return;
  cursor_position= cp;
  msg_offset= mo;
  window->postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineEntry::go_home()
{
  if( cursor_position == 0 && msg_offset == 0 )
    return;
  cursor_position= 0;
  msg_offset= 0;
  window->postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineEntry::delete_char()
{
  if( cursor_position >= msg.length() )
    return;

  msg.remove(cursor_position);

  while( msg_offset > 0 )
    {
    if( msg.length() - msg_offset >= getCharsWidth() )
      break;
    --msg_offset;
    }

  window->postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineEntry::history_up()
{
  if( history_position >= history_size )
    return;

  if( history[history_position+1] == 0 )
    return;

  if( history_position == 0 )
    *(history[0])= msg; // save the current msg

  ++history_position;
  msg= *(history[history_position]);

  window->postRedraw();

  go_end();
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineEntry::history_down()
{
  if( history_position <= 0 )
    return;

  if( history[history_position-1] == 0 )
    return; // never happens:-)

  --history_position;
  msg= *(history[history_position]);

  window->postRedraw();

  go_end();
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineEntry::do_enter()
{
  if( msg.length() == 0 )
    return; // silly, empty line

  history_position= 0;
  *(history[0])= msg;

  MLString save_msg= msg;

// go something

  delete history[history_size];
  for( int i= history_size; i > 0; --i )
    history[i]= history[i-1];

  history[0]= new MLString("");
  msg= "";
  msg_offset= 0;
  cursor_position= 0;

  entering= false;

  window->postRedraw();

  if( callback != 0 )
    callback->handleEntryCallback(this,save_msg);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLStatusLineEntry::handleKey( Key& key )
{
  if( key == KeyCodes::HOME || key == KeyCodes::CTRL_A )
    {
    if( ! entering )
      {
      entering= true;
      window->postRedraw();
      }

    go_home();
    return true;
    }

  if( key == KeyCodes::END || key == KeyCodes::CTRL_E )
    {
    if( ! entering )
      {
      entering= true;
      window->postRedraw();
      }

    entering= true;
    go_end();
    return true;
    }

  if( key == KeyCodes::RIGHT || key == KeyCodes::CTRL_F )
    {
    if( ! entering )
      {
      entering= true;
      window->postRedraw();
      }

    go_right();
    return true;
    }

  if( key == KeyCodes::LEFT || key == KeyCodes::CTRL_B )
    {
    if( ! entering )
      {
      entering= true;
      window->postRedraw();
      }

    go_left();
    return true;
    }

  if( key == KeyCodes::UP )
    {
    if( ! entering )
      {
      entering= true;
      window->postRedraw();
      }

    history_up();
    return true;
    }

  if( key == KeyCodes::DOWN )
    {
    if( ! entering )
      {
      entering= true;
      window->postRedraw();
      }

    history_down();
    return true;
    }

  if( key == KeyCodes::DELETE || key == KeyCodes::CTRL_D )
    {
    if( ! entering )
      {
      entering= true;
      window->postRedraw();
      }

    delete_char();
    return true;
    }

  if( key == KeyCodes::BACKSPACE )
    {
    if( ! entering )
      {
      entering= true;
      window->postRedraw();
      }

    if( cursor_position > 0 )
      {
      go_left();
      delete_char();
      }
    return true;
    }

  if( key == KeyCodes::ENTER )
    {
    do_enter();
    return true;
    }

  if( key.isAscii() )
    {
    if( ! entering )
      {
      entering= true;
      window->postRedraw();
      }

    history_position= 0;

    msg= msg.substring(0,cursor_position-1) +
         MLString::singleChar( char(key.getBase()) ) +
         msg.substring(cursor_position,msg.length()-1);
    go_right();
    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

// sent by MLStatusBars::handleMLKeyboard()
// return false if the key event must be handle by the interface
boolean MLStatusLineEntry::handleMLKeyboard( MLEvent& event )
{
  if( highlighted && event.type == MLEvent::KEY_PRESS )
    {
    // this is for me!
    return handleKey(event.key);
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////
