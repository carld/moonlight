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
  MLStatusLineText.C

  Stephane Rehel

  July 1 1997
*/

#include "tools/MLString.h"
#include "tools/Color.h"

#include "graphics/mlgl.h"
#include "graphics/SoftWindow.h"
#include "graphics/MLFont.h"

#include "MLModule.h"

#include "MLStatusLine.h"
#include "MLStatusLineText.h"

/////////////////////////////////////////////////////////////////////////////

class MLStatusMessage
{
public:
  MLString msg;
  int color;

  MLStatusMessage( const MLString& _msg, int _color = 1 );

  virtual ~MLStatusMessage()
    {}
};

MLStatusMessage::MLStatusMessage( const MLString& _msg, int _color /*=1*/ )
{
  msg= _msg;
  color= _color;
}

/////////////////////////////////////////////////////////////////////////////

MLStatusLineText::MLStatusLineText( MLModule* module,
                                    MLStatusLine* _line, int n_chars,
                                    MLStatusLineItem::Side _side,
                                    boolean _expandable /* = false */ ):
  MLStatusLineItem( module,
                    _line,
                    1 + MLStatusLine::getWidthFromChars(n_chars) + 2,
                    _side, _expandable )
{
  text_offset_if_pressed= false;

  messages.push( new MLStatusMessage("") );
}

/////////////////////////////////////////////////////////////////////////////

MLStatusLineText::~MLStatusLineText()
{
  while( ! messages.isEmpty() )
    delete messages.pop();
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineText::printMessage( const MLString& _message,
                                     int color /* = 1 */ )
{
  if( messages.isEmpty() )
    {
    pushMessage(_message,color);
    return;
    }

  MLString msg= cutMessage(_message);

  MLStatusMessage* m= messages.peek();

  if( msg == m->msg && m->color == color )
    return;

  m->msg= msg;
  m->color= color;

  if( window == 0 )
    return;

  if( ! window->mapped() )
    return;

  window->postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineText::pushMessage( const MLString& _message,
                                    int color /* = 1 */ )
{
  MLString msg= cutMessage(_message);

  MLStatusMessage* m= new MLStatusMessage(msg,color);
  messages.push(m);

  if( window == 0 )
    return;

  if( ! window->mapped() )
    return;

  window->postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineText::popMessage()
{
  if( ! messages.isEmpty() )
    messages.pop();

  if( ! window->mapped() )
    return;

  window->postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

MLString MLStatusLineText::cutMessage( const MLString& _msg ) const
{
  if( window == 0 )
    return _msg;

  MLString msg= _msg;

  MLFont* font= MLStatusLine::getFont();
  int width= font->getStringWidth(msg);
  width += 2 * MLStatusLine::getTextPosition().x();

  int i= msg.length() - 1;
  int max_width= window->getSize().x();
  while( width >= max_width && i >= 0 )
    {
    width -= font->getCharWidth( msg[i] );
    --i;
    }
  ++i;
  if( i < 0 ) i= 0;
  if( i < msg.length()-1 )
    msg= msg.substring(0,i);

  return msg;
}

/////////////////////////////////////////////////////////////////////////////

IPoint MLStatusLineText::getTextPosition() const
{
  IPoint position= MLStatusLine::getTextPosition();

  if( text_offset_if_pressed && pressed )
    position+= IVector(1,-1);

  return position;
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineText::_draw( int x_offset )
{
  MLStatusMessage* m= messages.peek();
  if( m == 0 )
    return;

  MLFont* font= MLStatusLine::getFont();

  IPoint position= getTextPosition();

  Color color;
  switch( m->color )
    {
    case 1: color= Color(1,1,1); break;
    case 2: color= Color(1,1,0); break;
    case 0:
    default:
      {
      extern unsigned char gray_level_2;
      color= Color(1,1,1) * double(gray_level_2) / 255.;
      break;
      }
    }

  if( MLStatusLineItem::highlight_when_cursor_enter &&
      MLStatusLineItem::cursor_highlighted )
    color= Color(1,1,0);

  mlColor(color);

  font->print( x_offset + position.x(), position.y(), m->msg );
  glFlush();
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineText::_draw()
{
  _draw(0);
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineText::draw()
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

