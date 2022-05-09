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
  VTextList.C

  Stephane Rehel
  Creation: SR, July 23 1996
*/

#include "graphics/mlgl.h"
#include "graphics/MLFont.h"

#include "VWindow.h"
#include "VTextList.h"
#include "VOptions.h"

/////////////////////////////////////////////////////////////////////////////

VTextList::VTextList( VWindow* parent ): VList(parent)
{
  font= vOptions->list.font;
  margin= 3;
}

/////////////////////////////////////////////////////////////////////////////

VTextList::~VTextList()
{}

/////////////////////////////////////////////////////////////////////////////

void VTextList::setFixedFont()
{
  font= vOptions->fixedFont;
}

/////////////////////////////////////////////////////////////////////////////

void VTextList::create( int x, int y,
                        int width, int height, boolean scrolling )
{
  VList::create( x, y, width, height, scrolling, 1);//font->getHeight()+1 );
}

/////////////////////////////////////////////////////////////////////////////

// protected:
void VTextList::handleVListItem( VList*, VList::Item* i,
                                 int yBase, boolean selected )
{
  if( i == 0 )
    return;
  if( i->data == 0 )
    return;

  if( selected )
    vOptions->gray3();
   else
    vOptions->gray2();
  mlRect( 0, yBase, i->getWidth()-1, yBase+i->getHeight()-1 );

  if( ! isAvailable() )
    vOptions->gray1();
   else
    vOptions->gray0();

  const MLString& message= ((VTextListItemData*)i->data)->cutMessage;

  font->print( margin, yBase+font->getDescent()+1, message );
}

/////////////////////////////////////////////////////////////////////////////

boolean VTextList::appendMessage( const MLString& text )
{
  VTextListItemData* data= new VTextListItemData;
  data->message= text;

  VList::Item* i= VList::appendItem( font->getHeight()+1, this, data );
  if( i == 0 )
    {
    delete data;
    return false;
    }

  data->cutMessage= VWindow::cutString(font,text,i->getWidth()-2*margin);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// 1 <= i <= nElements
boolean VTextList::setMessage( int i, const MLString& text )
{
  if( i < 1 || i > items.getNElements() )
    return false;

  Item* item= items.get(i);

  VTextListItemData* data= (VTextListItemData*) item->data;
  if( data == 0 )
    return false;
  if( data->message == text )
    return true;

  data->message= text;
  data->cutMessage= VWindow::cutString(font,text,item->getWidth()-2*margin);

  postRedraw();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// 1 <= i <= nElements+1
boolean VTextList::insertMessage( int i, const MLString& text )
{
  if( i < 1 || i > items.getNElements()+1 )
    return false;

  if( i == items.getNElements()+1 )
    return appendMessage(text);

  VTextListItemData* data= new VTextListItemData;
  data->message= text;

  VList::Item* item= VList::insertItem( i, font->getHeight()+1, this, data );
  if( item == 0 )
    {
    delete data;
    return false;
    }

  data->cutMessage= VWindow::cutString(font,text,item->getWidth()-2*margin);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// 1 <= i <= nElements
boolean VTextList::deleteMessage( int i )
{
  if( i < 1 || i > items.getNElements() )
    return false;

  Item* item= items.get(i);

  VList::deleteItem( item->label );

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// 1 <= i <= nElements
const MLString& VTextList::getMessage( int i )
{
  if( i < 1 || i > items.getNElements() )
    return MLString::empty();

  Item* item= items.get(i);
  VTextListItemData* data= (VTextListItemData*) item->data;
  if( data == 0 )
    return MLString::empty();

  return data->message;
}

/////////////////////////////////////////////////////////////////////////////

int VTextList::getMessagePosition( const MLString& message )
{
  int index= 1;
  IListIterator<Item> li(items);
  while( ! li.eol() )
    {
    Item* item= li.next();
    ++index;

    VTextListItemData* data= (VTextListItemData*) item->data;
    if( data == 0 )
      continue;

    if( data->message == message )
      return index;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////
