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
   VEdit.C

   Creation: SR, August 8th, 1995
   Revisions:
     + SR July 5 1996
*/

#include "graphics/MLEvent.h"
#include "graphics/MLFont.h"
#include "graphics/KeyCodes.h"
#include "kw/kw.h"

#include "VOptions.h"
#include "VEdit.h"

/////////////////////////////////////////////////////////////////////////////

VEdit::VEdit( VWindow* _parent ): VWidget(_parent)
{
  editFont= vOptions->edit.font;
  string= MLString("");
  textpos= IPoint(0,0);
  selected= false;
}

/////////////////////////////////////////////////////////////////////////////

VEdit::~VEdit()
{}

/////////////////////////////////////////////////////////////////////////////

void VEdit::setFixedFont()
{
  editFont= vOptions->fixedFont;
}

/////////////////////////////////////////////////////////////////////////////

void VEdit::create( int x, int y, int _width )
{
  create( x, y, _width, MLString("") );
}

/////////////////////////////////////////////////////////////////////////////

void VEdit::create( int x, int y, int _width, const MLString& _string )
{
  string= MLString("");

  width= _width;
  freeWidth= width - 2 * (vOptions->edit.horizMargin + vOptions->edit.z);
  height= editFont->getHeight()+1
         + 2*(vOptions->edit.vertMargin + vOptions->edit.z);

  textpos= IPoint( vOptions->edit.z + vOptions->edit.horizMargin,
                   height/2 + editFont->getHeight()/2
                            - editFont->getDescent() );
  pos= visualPos= 0;

  VWindow::create( x, y, width, height ); // dummy width & height
  setFont( editFont );

  changeEdit(_string);
}

/////////////////////////////////////////////////////////////////////////////

void VEdit::enableSelection( boolean yes /* = true */ )
{
//  if( (selected?1:0) == (yes?1:0) )
//    return;
  selected= yes;
  if( string.length() == 0 )
    selected= false;
  postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void VEdit::changeEdit( const MLString& newString )
{
  if( string == newString )
    return;
  string= newString;
  pos= visualPos= 0;
//  while( pos < int(string.length()) )
//    go_right();

  if( isCurrent() )
    enableSelection(true);
   else
    postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void VEdit::go_left()
{
  if( pos == 0 )
    {
    visualPos= 0;
    return;
    }
  if( pos == visualPos )
    visualPos= max( 0, visualPos-1 );
  --pos;

  if( pos > 0 && visualPos == pos && pos == int(string.length()) )
    {
    int w= 0;
    for(;;)
      {
      w+= editFont->getCharWidth( string[visualPos-1] );
      if( w >= freeWidth )
        break;
      --visualPos;
      if( visualPos == 0 )
        break;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

void VEdit::go_right()
{
  if( pos == int(string.length()) )
    return;

  ++pos;
  int visual_width= editFont->getStringWidth( string.get()+visualPos,
                                              pos-visualPos );

  while( visual_width >= freeWidth )
    {
    visual_width -= editFont->getCharWidth( string[visualPos] );
    ++visualPos;
    assert( visualPos <= pos );
    }
}

/////////////////////////////////////////////////////////////////////////////

void VEdit::drawEditText()
{
  if( ! beginDraw() )
    return;

  _drawEditText();
  endDraw();
}

/////////////////////////////////////////////////////////////////////////////

void VEdit::_drawEditText()
{
  if( visualPos > int(string.length()) )
    visualPos= string.length();

  int w= 0;
  int len= 0;
  for(;;)
    {
    if( visualPos + len >= string.length() )
      break; // end of string
    char ch= string[visualPos + len];
    int cw= editFont->getCharWidth(ch);
    if( w + cw >= freeWidth )
      break;
    w+= cw;
    ++len;
    }

  gray3();
  fillRectangle( IPoint(2,2), IPoint(width-3,height-3) );

  if( len > 0 )
    {
    boolean _selected= selected && isAvailable();
    if( _selected )
      {
      setForeground( getGray3() );
      setBackground( getGray1() );
      drawImageString( textpos.x(), textpos.y(),
                       string.get() + visualPos, len );
      }
     else
      {
      if( isAvailable() )
        gray0();
       else
        gray1();
      setBackground( getGray3() );
      drawString( textpos.x(), textpos.y(), string.get() + visualPos, len );
      }
    }

  if( isCurrent() && isAvailable() && !selected )
    {
    // draw cursor
    gray0();
    int x= (visualPos==pos)
          ? 0
          : editFont->getStringWidth(string.get()+visualPos,pos-visualPos);
    x+= textpos.x();
    int y1= textpos.y()-editFont->getAscent();
    int y2= textpos.y()+editFont->getDescent()-1;

    drawVertLine( x, y1, y2 );
    drawHorizLine( x-2, x+2, y1 );
    drawHorizLine( x-2, x+2, y2 );
    }
}

/////////////////////////////////////////////////////////////////////////////

void VEdit::drawEdit()
{
  if( ! beginDraw() )
    return;
int yy= 0;
#ifdef __WINDOWS__
++yy;
#endif
  gray1();
  drawHorizLine( 0, width-1+yy, 0 );
  drawHorizLine( 1, width-2, 1 );
  drawVertLine( 0, 0, height-2 );
  drawVertLine( 1, 1, height-3 );
  gray2();
  drawHorizLine( 1, width-2, height-2 );
  drawVertLine( width-2, 2, height-2 );
  gray3();
  drawHorizLine( 0, width-1, height-1 );
  drawVertLine( width-1, 1, height-1 );
  drawVertLine( width-1, 0, 0 );

  if( isCurrent() )
    {
    gray0();
    drawRectangle( IPoint(0,0), IPoint(width-1,height-1) );
    }

  _drawEditText();

  endDraw();
}

/////////////////////////////////////////////////////////////////////////////

boolean VEdit::handleKey( Key key )
{
  if( key.isAscii() )
    {
    if( selected )
      {
      string= MLString("");
      pos= visualPos= 0;
      }
    pos= min( int(pos), int(string.length()) );

    string=   string.substring(0,pos-1)
            + MLString::singleChar(char(key.getBase()))
            + string.substring(pos,string.length()-1);
    go_right();

    goto next;
    }

  if( key == KeyCodes::ENTER )
    {
    callPrivateWidgetEventHandler( VWidgetEvent::EnterEdit(this) );
    return true;
    }

  if( key == KeyCodes::LEFT )
    {
    go_left();
    goto next;
    }
  if( key == KeyCodes::RIGHT )
    {
    go_right();
    goto next;
    }
  if( key == KeyCodes::HOME )
    {
    while( pos > 0 )
      go_left();
    goto next;
    }
  if( key == KeyCodes::END )
    {
    while( pos < int(string.length()) )
      go_right();
    goto next;
    }
  if( key == KeyCodes::DELETE )
    {
    if( selected )
      {
      string= "";
      pos= visualPos= 0;
      }
    string.remove(pos);
    goto next;
    }
  if( key == KeyCodes::BACKSPACE )
    {
    if( selected )
      {
      string= "";
      pos= visualPos= 0;
      }
    if( pos > 0 )
      {
      if( pos > 0 )
        string.remove(pos-1);
      go_left();
      }
    goto next;
    }
  if( key == KeyCodes::CTRL_Y )
    {
    string= "";
    pos= visualPos= 0;
    goto next;
    }

  return false;

next:
  selected= false;
  drawEditText();
  
  flush();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void VEdit::setCurrent( boolean yes )
{
  VWidget::setCurrent(yes);
  enableSelection(yes); // always do this event if already current
}

/////////////////////////////////////////////////////////////////////////////

boolean VEdit::handleEvent( MLEvent& event )
{
//  if( callPrivateWidgetEventHandler( VWidgetEvent::VXEvent(this,event) ) )
//    return true;

  if( event.type == MLEvent::REDRAW )
    {
    drawEdit();
    return true;
    }

  if( ! isAvailable() )
    return VWindow::handleEvent(event);

  switch( event.type )
    {
    case MLEvent::KEY_PRESS:
      handleWidgetEvent( VWidgetEvent::EnterKey(this,event.key) );
      break;

    case MLEvent::MOUSE_PRESS:
      {
      static double oldClickTime= 0.;
      double t= double(event.time) / 1000.;
      boolean doubleClick= (t - oldClickTime) < vOptions->doubleClickDelay;
      oldClickTime= t;

      if( doubleClick )
        enableSelection();
       else
        {
        if( ! isCurrent() )
          setCurrent(true);
         else
          buttonPress( event.position.x()
                      -vOptions->edit.z - vOptions->edit.horizMargin );
        }
      }
      break;

    default:
      return VWindow::handleEvent(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void VEdit::buttonPress( int x )
{
  selected= false;

  int i= 0;
  int w= 0;
  for(;;)
    {
    int j= visualPos + i;
    if( j >= int(string.length()) )
      break;
    w+= editFont->getCharWidth( string[j] );
    if( w >= x )
      break;
    ++i;
    }
  i+= visualPos;
  i= max( 0, i );
  i= min( int(string.length()), int(i) );

  while( i < pos )
    go_left();
  while( i > pos )
    go_right();

  drawEditText();
}

/////////////////////////////////////////////////////////////////////////////

boolean VEdit::handleWidgetEvent( const VWidgetEvent& event )
{
  if( event.type() != VWidgetEvent::ENTERKEY )
    return false;

  return handleKey( event.getKey() );
}

/////////////////////////////////////////////////////////////////////////////

void VEdit::drawWidget()
{
  removeRedraw();
  drawEdit();
}

/////////////////////////////////////////////////////////////////////////////

