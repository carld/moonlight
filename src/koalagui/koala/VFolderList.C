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
  VFolderList.C

  Stephane Rehel

  January 2nd 1998
*/

#include "graphics/mlgl.h"
#include "graphics/MLFont.h"
#include "graphics/MLPicture.h"

#include "VWindow.h"
#include "VFolderList.h"
#include "VOptions.h"

#include "MiniPlusFolderPicture.h"
#include "MiniMinusFolderPicture.h"
#include "MiniOpenFileFolderPicture.h"
#include "MiniClosedFileFolderPicture.h"
#include "MiniEmptyBoxFolderPicture.h"

/////////////////////////////////////////////////////////////////////////////

static MLImage* miniPlusFolder= 0;
static MLImage* miniMinusFolder= 0;
static MLImage* miniEmptyBoxFolder= 0;
static MLImage* miniOpenFileFolder= 0;
static MLImage* miniClosedFileFolder= 0;

/////////////////////////////////////////////////////////////////////////////

VFolderList::VFolderListItemData::VFolderListItemData(
                                            VFolderListItemData* _parent,
                                            boolean _isFolder )
{
  message= "";
  cutMessage= "";

  // parent is 0 for specifying that this item is the root item
  parent= _parent;

  max_items= 0;
  nItems= 0;
  items= 0;

  isFolder= _isFolder;
  isClosed= true;

  label= 0;
  depth= 0;
  index= 0;
}
/////////////////////////////////////////////////////////////////////////////
VFolderList::VFolderListItemData::~VFolderListItemData()
{
  delete items;
  items= 0;
  nItems= 0;
  max_items= 0;
}
/////////////////////////////////////////////////////////////////////////////
void VFolderList::VFolderListItemData::assume( int size )
{
  if( items == 0 )
    {
    max_items= 10;
    nItems= 0;
    items= new VFolderListItemData* [ max_items + 1 ];
    for( int i= 0; i <= max_items; ++i )
      items[i]= 0;
    }

  if( size >= max_items )
    {
    int new_max_items= max_items + max_items / 2;
    VFolderListItemData** new_items=
                             new VFolderListItemData* [ new_max_items + 1 ];
    for( int i= 0; i <= max_items; ++i )
      new_items[i]= items[i];

    for( int i= max_items+1; i <= new_max_items; ++i )
      new_items[i]= 0;

    delete items;
    items= new_items;
    max_items= new_max_items;
    }
}
/////////////////////////////////////////////////////////////////////////////
void VFolderList::VFolderListItemData::append( VFolderListItemData* it )
{
  assume( nItems + 1 );

  ++nItems;
  items[ nItems ]= it;
}
/////////////////////////////////////////////////////////////////////////////
void VFolderList::VFolderListItemData::insert( int i,
                                               VFolderListItemData* it )
{
  if( i < 1 || i > nItems+1 )
    return;

  assume( nItems + 1 );

  for( int j= nItems; j >= i; --j )
    {
    items[j+1]= items[j];
    items[j+1]->index= j+1;
    }

  ++nItems;
  items[i]= it;
  items[i]->index= i;
}
/////////////////////////////////////////////////////////////////////////////
void VFolderList::VFolderListItemData::remove( int i )
{
  if( i < 1 || i > nItems )
    return;

  for( int j= i; j < nItems; ++j )
    {
    items[j]= items[j+1];
    items[j]->index= j;
    }

  items[nItems]= 0;
  --nItems;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

VFolderList::VFolderList( VWindow* parent ): VList(parent)
{
  if( miniPlusFolder == 0 )
    {
    miniPlusFolder= MiniPlusFolderPicture::get();
    miniPlusFolder->upsidedown();
    }
  if( miniMinusFolder == 0 )
    {
    miniMinusFolder= MiniMinusFolderPicture::get();
    miniMinusFolder->upsidedown();
    }

  if( miniOpenFileFolder == 0 )
    {
    miniOpenFileFolder= MiniOpenFileFolderPicture::get();
    miniOpenFileFolder->upsidedown();
    }
  if( miniClosedFileFolder == 0 )
    {
    miniClosedFileFolder= MiniClosedFileFolderPicture::get();
    miniClosedFileFolder->upsidedown();
    }
  if( miniEmptyBoxFolder == 0 )
    {
    miniEmptyBoxFolder= MiniEmptyBoxFolderPicture::get();
    miniEmptyBoxFolder->upsidedown();
    }

  setPlusMinusFolder(true); // default

  VFolderList::rootFolder= new VFolderListItemData(0,true);
  VFolderList::rootFolder->isClosed= false;

  font= vOptions->list.font;
  margin= 3;
}

/////////////////////////////////////////////////////////////////////////////

VFolderList::~VFolderList()
{
  delete rootFolder;
  rootFolder= 0;
}

/////////////////////////////////////////////////////////////////////////////

void VFolderList::setFixedFont()
{
  font= vOptions->fixedFont;
}

/////////////////////////////////////////////////////////////////////////////

void VFolderList::setPlusMinusFolder( boolean yes /* = true */ )
{
  if( yes )
    {
    open_folder= ::miniMinusFolder;
    closed_folder= ::miniPlusFolder;
    }
   else
    {
    open_folder= ::miniOpenFileFolder;
    closed_folder= ::miniClosedFileFolder;
    }
}

/////////////////////////////////////////////////////////////////////////////

void VFolderList::create( int x, int y,
                          int width, int height, boolean scrolling )
{
  VList::create( x, y, width, height, scrolling, 1);//font->getHeight()+1 );
}

/////////////////////////////////////////////////////////////////////////////

// protected:
void VFolderList::handleVListItem( VList*, VList::Item* i,
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

  VFolderListItemData* fi= (VFolderListItemData*) (i->data);
  if( fi == 0 )
    return;

  const MLString& message= fi->cutMessage;

  int depth_margin= ::miniPlusFolder->getWidth() + 3;
  if( closed_folder == miniClosedFileFolder )
    depth_margin += 6;

  int _margin = margin + fi->depth * depth_margin;
  int y_text= yBase+font->getDescent()+1;
  font->print( _margin, y_text, message );

  int x_link= _margin - depth_margin + closed_folder->getWidth()/2;
  int y_link= y_text-1 + closed_folder->getHeight()/2;

  if( closed_folder == miniClosedFileFolder )
    y_link-= 5;
   else
    --y_link;

  glLineStipple(1,0x5555);
  glEnable(GL_LINE_STIPPLE);
  glColor3i(0,0,0);
  glBegin(GL_LINES);
    {
    int y_link1;
    if( fi->index == fi->parent->nItems )
      {
      y_link1= y_link;
      if( (y_link & 1) != 0 )
        ++y_link1;
      }
     else
      {
      y_link1= yBase;
      y_link1 &= ~1;
      }

    int y_link2;
    if( fi->index == 1 && fi->parent == VFolderList::rootFolder )
      y_link2= y_link;
     else
      y_link2= yBase + i->getHeight() - 1;
    y_link2 &= ~1;

    glVertex2i( x_link, y_link1 );
    glVertex2i( x_link, y_link2 );
    glVertex2i( x_link+1, y_link );
    glVertex2i( x_link+5, y_link );

    if( fi->depth > 1 )
      {
      y_link1= yBase;
      y_link2= yBase + i->getHeight() - 1;
      y_link1 &= ~1;
      y_link2 &= ~1;
      VFolderListItemData* parent= fi->parent;
      for( int k= 1; k < fi->depth; ++k )
        {
        x_link -= depth_margin;

        if( parent->index < parent->parent->nItems )
          {
          glVertex2i( x_link, y_link1 );
          glVertex2i( x_link, y_link2 );
          }

        parent= parent->parent;
        }
      }
    }
  glEnd();
  glDisable(GL_LINE_STIPPLE);

  if( fi->isFolder )
    {
    MLImage* mini= fi->isClosed ? closed_folder : open_folder;

    if( closed_folder != miniClosedFileFolder )
      {
      if( fi->nItems == 0 )
        mini= 0;// ::miniEmptyBoxFolder;
      }

    if( mini != 0 )
      {
      int y= y_link - open_folder->getHeight()/2 + 1;
      MLPicture::draw( mini, _margin - depth_margin, y );
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

VFolderList::VFolderListItemData* VFolderList::findFolderItem( int label )
{
  if( label == 0 )
    return VFolderList::rootFolder;

  VList::Item* i= VList::findItem( VList::Label(label) );
  if( i == 0 )
    return 0;

  VFolderListItemData* fi= (VFolderListItemData*) (i->data);

  return fi;
}

/////////////////////////////////////////////////////////////////////////////

VFolderList::VFolderListItemData* VFolderList::findFolder( int folder_id )
{
  VFolderListItemData* fi= findFolderItem(folder_id);
  if( fi == 0 )
    return 0;

  if( ! fi->isFolder )
    return 0;

  return fi;
}

/////////////////////////////////////////////////////////////////////////////

int VFolderList::getFolderTotalLength( VFolderListItemData* data )
{
  if( data == 0 )
    return 0;

  int length= 1;

  if( ! data->isFolder )
    return length;

  for( int i= 1; i <= data->nItems; ++i )
    {
    VFolderListItemData* d= data->items[i];
    if( d == 0 )
      continue;
    length += getFolderTotalLength(d);
    }

  return length;
}

/////////////////////////////////////////////////////////////////////////////

boolean VFolderList::findFolderItemPosition( VFolderListItemData* data,
                                           int label,
                                           int& position )
{
  if( data->label == label )
    return true;

  ++position;

  if( ! data->isFolder )
    return false;

  for( int i= 1; i <= data->nItems; ++i )
    {
    VFolderListItemData* d= data->items[i];
    if( d == 0 )
      continue;

    if( findFolderItemPosition(d,label,position) )
      return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

// folder_id==0 for root folder
// 1 <= i <= nFolderElements(folder)+1
int VFolderList::insertFolderItem( int folder_id, int i,
                                   const MLString& text,
                                   void* user_data,
                                   boolean isFolder )
{
  VFolderListItemData* folder= findFolder(folder_id);

  if( folder == 0 )
    return 0;

  if( i == -1 )
    i= folder->nItems+1;

  if( i < 1 || i > folder->nItems+1 )
    return 0;

  boolean closed= isFolder ? true : folder->isClosed;

  VFolderListItemData* data= new VFolderListItemData(folder,isFolder);
  data->message= text;
  data->isClosed= closed;

  int position= 0;
  findFolderItemPosition(rootFolder,folder->label,position);
  ++position;

  for( int j= 1; j < i; ++j )
    {
    VFolderListItemData* d= folder->items[j];
    if( d == 0 )
      continue;
    position += getFolderTotalLength(d);
    }

  VList::Item* item= VList::insertItem( position,
                                        font->getHeight()+1,
                                        this,
                                        data,
                                        folder->isClosed,
                                        user_data );

  if( item == 0 )
    {
    delete data;
    return 0;
    }
  data->label= item->label;

  data->cutMessage= VWindow::cutString(font,text,item->getWidth()-2*margin);

  folder->insert( i, data );

  data->depth= folder->depth + 1;

  if( ! folder->isClosed )
    VList::updateItemsPositions();

  return item->label;
}

/////////////////////////////////////////////////////////////////////////////

// folder_id==0 for root folder
int VFolderList::insertFolder( int folder_id, int i, const MLString& text,
                               void* user_data /* = 0 */ )
{
  if( i == -1 )
    return 0;

  return insertFolderItem( folder_id, i, text, user_data, true );
}

/////////////////////////////////////////////////////////////////////////////

// folder_id==0 for root folder
boolean VFolderList::insertItem( int folder_id, int i,
                               const MLString& text,
                               void* user_data /* = 0 */ )
{
  if( i == -1 )
    return 0;

  return insertFolderItem( folder_id, i, text, user_data, false ) != 0;
}

/////////////////////////////////////////////////////////////////////////////

// folder_id==0 for root folder
int VFolderList::appendFolderItem( int folder_id, const MLString& text,
                                   void* user_data,
                                   boolean isFolder )
{
  return insertFolderItem( folder_id, -1, text, user_data, isFolder );
}

/////////////////////////////////////////////////////////////////////////////

// folder_id==0 for root folder
int VFolderList::appendFolder( int folder_id, const MLString& text,
                               void* user_data /* = 0 */ )
{
  return appendFolderItem( folder_id, text, user_data, true );
}

/////////////////////////////////////////////////////////////////////////////

// folder_id==0 for root folder
boolean VFolderList::appendItem( int folder_id, const MLString& text,
                               void* user_data /* = 0 */ )
{
  return appendFolderItem( folder_id, text, user_data, false ) != 0;
}

/////////////////////////////////////////////////////////////////////////////

// folder_id==0 for root folder
// 0 <= i <= nFolderElements(folder)
// i==0 for folder text
boolean VFolderList::setItem( int folder_id, int i, const MLString& text,
                            void* user_data /* = 0 */ )
{
  VFolderListItemData* folder= findFolder(folder_id);

  if( folder == 0 )
    return false;

  VFolderListItemData* data= 0;

  if( i == 0 )
    data= folder;
   else
    {
    if( i < 1 || i > folder->nItems )
      return false;

    data= folder->items[i];
    if( data == 0 )
      return false;
    }

  if( data->message == text )
    return true;

  MLString old_message= data->message;
  MLString old_cut_message= data->cutMessage;

  data->message= text;

  VList::Item* item= VList::findItem( VList::Label(data->label) );
  if( item != 0 )
    {
    data->cutMessage= VWindow::cutString(font,text,item->getWidth()-2*margin);
    item->user_data= user_data;
    }

  if( !data->isClosed &&
      (old_message!=data->message || old_cut_message!=data->cutMessage) )
    VList::updateItemsPositions();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// folder_id==0 for root folder
// 1 <= i <= nFolderElements
boolean VFolderList::deleteItem( int folder_id, int i )
{
  VFolderListItemData* folder= findFolder(folder_id);

  if( folder == 0 )
    return false;

  if( i < 1 || i > folder->nItems )
    return false;

  boolean closed= folder->isClosed;

  folder->remove(i);
  VList::deleteItem( folder->items[i]->label );

  if( ! closed )
    VList::updateItemsPositions();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// folder_id!=0
boolean VFolderList::deleteFolder( int folder_id )
{
  VFolderListItemData* folder= findFolder(folder_id);
  if( folder == 0 )
    return false;
  if( folder == rootFolder )
    return false;

  int infinite_loop= 0;
  while( folder->nItems > 0 )
    {
    if( ++infinite_loop > 1000000 )
      break;
    VFolderListItemData* fi= folder->items[ folder->nItems ];
    if( fi == 0 )
      continue;

    if( fi->isFolder )
      deleteFolder( fi->label );
     else
      deleteItem( folder_id, folder->nItems );
    }

  folder->parent->remove(folder->index);

  VList::deleteItem( folder->label );

  VList::updateItemsPositions();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// folder_id==0 for root folder
// 1 <= i <= nFolderElements
// i==0 for folder text
const MLString& VFolderList::getItem( int folder_id, int i )
{
  VFolderListItemData* folder= findFolder(folder_id);

  if( folder == 0 )
    return MLString::empty();

  if( i < 1 || i > folder->nItems )
    return MLString::empty();

  VFolderListItemData* data= folder->items[i];
  if( data == 0 )
    return MLString::empty();

  return data->message;
}

/////////////////////////////////////////////////////////////////////////////

// folder_id==0 for root folder
// 1 <= i <= nFolderElements
// i==0 for folder text
void* VFolderList::getUserData( int folder_id, int i )
{
  VFolderListItemData* folder= findFolder(folder_id);

  if( folder == 0 )
    return 0;

  VFolderListItemData* data= 0;

  if( i == 0 )
    data= folder;
   else
    {
    if( i < 1 || i > folder->nItems )
      return 0;

    data= folder->items[i];
    if( data == 0 )
      return 0;
    }

  VList::Item* item= VList::findItem( VList::Label(data->label) );
  if( item == 0 )
    return 0;

  return item->user_data;
}

/////////////////////////////////////////////////////////////////////////////

// may return 0
int VFolderList::getUserDataIndex( int folder_id, void* user_data )
{
  VFolderListItemData* folder= findFolder(folder_id);

  if( folder == 0 )
    return 0;

  for( int i= 1; i <= folder->nItems; ++i )
    {
    VFolderListItemData* data= folder->items[i];
    if( data == 0 )
      continue;

    VList::Item* item= VList::findItem( VList::Label(data->label) );
    if( item == 0 )
      continue;
    if( item->user_data == user_data )
      return i;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

// may return 0
int VFolderList::getFolderID( int i )
{
  if( i < 1 || i > items.getNElements() )
    return 0;

  VList::Item* item= VList::items.get(i);
  if( item == 0 )
    return 0;

  VFolderListItemData* data= (VFolderListItemData*) (item->data);
  if( data == 0 )
    return 0;
  if( ! data->isFolder )
    return 0;

  return data->label;
}

/////////////////////////////////////////////////////////////////////////////

// may return 0
int VFolderList::getFolderID( int folder_id, int i )
{
  VFolderListItemData* folder= findFolder(folder_id);

  if( folder == 0 )
    return 0;

  if( i == 0 )
    return folder->label;

  if( i < 1 || i > folder->nItems )
    return 0;

  VFolderListItemData* data= folder->items[i];
  if( data == 0 )
    return 0;

  return data->label;
}

/////////////////////////////////////////////////////////////////////////////

int VFolderList::getFolderLength( int folder_id )
{
  VFolderListItemData* folder= findFolder(folder_id);

  if( folder == 0 )
    return 0;

  return folder->nItems;
}

/////////////////////////////////////////////////////////////////////////////

boolean VFolderList::openFolder( int folder_id, boolean yes /* = true */ )
{
  if( folder_id == 0 )
    yes= true;

  VFolderListItemData* folder= findFolder(folder_id);

  if( folder == 0 )
    return false;

  if( (yes?1:0) == (folder->isClosed?0:1) )
    return true;

  VList::Item* item= VList::findItem(folder->label);
  if( item  == 0 )
    return false;

  boolean hidden= item->hidden;

  folder->isClosed= !yes;

  _openFolder( item, yes );

  item->hidden= hidden;

  VList::updateItemsPositions();
  VList::postRedraw();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean VFolderList::closeFolder( int folder_id )
{
  return openFolder(folder_id,false);
}

/////////////////////////////////////////////////////////////////////////////

// private recursive
void VFolderList::_openFolder( VList::Item* item, boolean yes )
{
  if( item == 0 )
    return;

  item->hidden= !yes;

  VFolderListItemData* data= (VFolderListItemData*) (item->data);
  if( data == 0 )
    return;

  for( int i= 1; i <= data->nItems; ++i )
    {
    VList::Item* it= VList::findItem( data->items[i]->label );
    if( it == 0 )
      continue;

    it->hidden= !yes;
    VFolderListItemData* fi= (VFolderListItemData*) (it->data);
    if( fi == 0 )
      continue;

    if( ! fi->isFolder )
      continue;
    if( fi->isClosed )
      continue;
    _openFolder(it,yes);
    }
}

/////////////////////////////////////////////////////////////////////////////

void VFolderList::closeAllFolders( VFolderListItemData* folder )
{
  if( folder == 0 )
    return;

  if( folder != rootFolder )
    closeFolder(folder->label);

  for( int i= 1; i <= folder->nItems; ++i )
    closeAllFolders( folder->items[i] );
}

/////////////////////////////////////////////////////////////////////////////

void VFolderList::closeAllFolders( int folder_id )
{
  closeAllFolders( findFolder(folder_id) );
}

/////////////////////////////////////////////////////////////////////////////

boolean VFolderList::handleClick( int label, int x, int y )
{
  if( label == 0 )
    return false;

  VFolderListItemData* folder= findFolder(label);
  if( folder == 0 )
    return false;

  int depth_margin= ::miniPlusFolder->getWidth() + 3;
  if( closed_folder == miniClosedFileFolder )
    depth_margin += 6;
  int _margin = margin + folder->depth * depth_margin;
  int y_text= font->getDescent()+1;

//  int x_link= _margin - depth_margin + closed_folder->getWidth()/2;
  int y_link= y_text-1 + closed_folder->getHeight()/2;

  if( closed_folder == miniClosedFileFolder )
    y_link-= 5;
   else
    --y_link;

  MLImage* mini= folder->isClosed ? closed_folder : open_folder;

  int x1= _margin - depth_margin;
  int x2= _margin - depth_margin + mini->getWidth() - 1;
  int y1= y_link - open_folder->getHeight()/2 + 1;
  int y2= y1 + mini->getHeight() - 1;

  if( x >= x1 && x <= x2 && y >= y1 && y <= y2 )
    {
    openFolder( label, folder->isClosed );
    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean VFolderList::handleDoubleClick( int label )
{
  if( label == 0 )
    return false;

  VFolderListItemData* folder= findFolder(label);
  if( folder == 0 )
    return false;

  openFolder( label, folder->isClosed );

  return true;
}

/////////////////////////////////////////////////////////////////////////////
