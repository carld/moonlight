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
  MLStatusLine.C

  Stephane Rehel

  December 8 1996
*/

#include "MLStatusLine.h"
#include "MLStatusLineItem.h"

#include "graphics/mlgl.h"
#include "graphics/SoftWindow.h"
#include "graphics/MLEvent.h"
#include "graphics/MLFont.h"
#include "graphics/MLFonts.h"

/////////////////////////////////////////////////////////////////////////////

static const int maxItems= 64;

/////////////////////////////////////////////////////////////////////////////

// static
MLFont* MLStatusLine::getFont()
{
  return smallFont;
}

/////////////////////////////////////////////////////////////////////////////

// static
int MLStatusLine::getHeight()
{
  return getFont()->getHeight() + 4 + 1;
}

/////////////////////////////////////////////////////////////////////////////

// static
int MLStatusLine::getWidthFromChars( int n_chars )
{
  return 2*2 + getFont()->getCharWidth('M')*n_chars;
}

/////////////////////////////////////////////////////////////////////////////

// static
IPoint MLStatusLine::getTextPosition()
{
  return IPoint( 2 + 1, 3 + getFont()->getDescent() );
}

/////////////////////////////////////////////////////////////////////////////

MLStatusLine::MLStatusLine( SoftWindow* root,
                            const IPoint& _position, int _size_x )
{
  size_x= _size_x;

  nItems= 0;
  items= new MLStatusLineItem* [ maxItems + 1 ];
  for( int i= 0; i <= maxItems; ++i )
    items[i]= 0;

  position= _position;
  window= new SoftWindow;
  window->create( root, position.x(), position.y(), size_x, getHeight() );
  window->setEventHandler(this);
}

/////////////////////////////////////////////////////////////////////////////

MLStatusLine::~MLStatusLine()
{
  for( int i= nItems; i >= 1; --i )
    {
    MLStatusLineItem* item= items[i];
    items[i]= 0;
    delete item;
    }

  delete window;
  window= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLine::addItem( MLStatusLineItem* item )
{
  if( item == 0 )
    return;

  int i;
  for( i= 1; i <= nItems; ++i )
    if( items[i] == item )
      return;

  if( nItems >= maxItems )
    return;

  ++nItems;
  items[ nItems ]= item;
}

/////////////////////////////////////////////////////////////////////////////

static const int margin_between_items= 1;

void MLStatusLine::finalize()
{
  int nLeft= 0, nRight= 0;
  int widthLeft= 0, widthRight= 0;
  int nExpandables= 0;

  int i;
  for( i= 1; i <= nItems; ++i )
    {
    MLStatusLineItem* item= items[i];
    if( item == 0 )
      continue;
    if( item->expandable )
      {
      ++nExpandables;
      continue;
      }
    if( item->side == MLStatusLineItem::LEFT )
      {
      ++nLeft;
      widthLeft += item->size_x;
      }
     else
      {
      ++nRight;
      widthRight += item->size_x;
      }
    }

  widthLeft += nLeft * margin_between_items;
  widthRight+= nRight* margin_between_items;

  int size_expandable= 0;
  if( nExpandables > 0 )
    {
    size_expandable= size_x - widthLeft - widthRight
                            - (nExpandables-1)*margin_between_items;

    size_expandable /= nExpandables;
    if( size_expandable <= 0 )
      size_expandable= 1;
    }

  int xLeft= 0;
  int xRight= size_x-1;
  int xExpandable= widthLeft;
  for( i= 1; i <= nItems; ++i )
    {
    MLStatusLineItem* item= items[i];
    if( item == 0 )
      continue;
    if( item->expandable )
      {
      item->size_x= size_expandable;
      item->x_position= xExpandable;
      xExpandable += size_expandable + margin_between_items;
      continue;
      }

    if( item->side == MLStatusLineItem::LEFT )
      {
      item->x_position= xLeft;
      xLeft+= item->size_x + margin_between_items;
      }
     else
      {
      item->x_position= xRight - (item->size_x-1);
      xRight-= item->size_x + margin_between_items;
      }
    }

  for( i= 1; i <= nItems; ++i )
    {
    MLStatusLineItem* item= items[i];
    if( item == 0 )
      continue;
    item->create();
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLine::postRedraw()
{
  if( window == 0 )
    return;
  if( ! window->mapped() )
    return;

  for( int i= 1; i <= nItems; ++i )
    {
    MLStatusLineItem* item= items[i];
    if( item == 0 )
      continue;
    item->postRedraw();
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLStatusLine::eventHandler( MLWindow*, MLEvent& )
{
/*
  if( w != window || window == 0 )
    return false;

  if( e.type == MLEvent::REDRAW )
    {
    draw();
    return true;
    }
*/
  return false;
}

/////////////////////////////////////////////////////////////////////////////

// sent by MLStatusBars::handleMLKeyboard()
// return false if the key event must be handle by the interface
boolean MLStatusLine::handleMLKeyboard( MLEvent& event )
{
  for( int i= 1; i <= nItems; ++i )
    {
    MLStatusLineItem* item= items[i];
    if( item == 0 )
      continue;

    if( item->handleMLKeyboard(event) )
      return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////
