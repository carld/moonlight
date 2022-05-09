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
  VFolderList.h

  Stephane Rehel

  January 2nd 1998
*/

#ifndef __VFolderList_h
#define __VFolderList_h

#ifndef __VWidget_h
#include "VWidget.h"
#endif

#ifndef __VList_h
#include "VList.h"
#endif

class MLFont;
class MLImage;

/////////////////////////////////////////////////////////////////////////////

class VFolderList: public VList, public VListItemHandler
{
protected:
  MLFont* font;
  int margin;

  MLImage* open_folder;
  MLImage* closed_folder;

  class VFolderListItemData: public VListItemData
    {
    public:
      MLString message;
      MLString cutMessage;

      VFolderListItemData* parent;

      boolean isFolder;
      boolean isClosed;

      int label;
      int index; // index of this item if parent folder

      int depth;

      int max_items;
      int nItems;
      VFolderListItemData** items; // this could be items[] or other folders[]

      VFolderListItemData( VFolderListItemData* _parent, boolean _isFolder );

      virtual ~VFolderListItemData();

      private:
        void assume( int size );
      public:
        void append( VFolderListItemData* it );
        void insert( int i, VFolderListItemData* it );
        void remove( int i );
    };

  // this is just a list item data.
  VFolderListItemData* rootFolder;

public:
  VFolderList( VWindow* parent );
  virtual ~VFolderList();

  void setFixedFont();
  void setPlusMinusFolder( boolean yes = true );

  void create( int x, int y, int width, int height, boolean scrolling );

  virtual VWidget::TYPE type() const
    {
    return VWidget::FOLDERLIST;
    }

  int getInteger(); // may return 0

protected:
  VFolderListItemData* findFolderItem( int label );
  VFolderListItemData* findFolder( int folder_id );

  int getFolderTotalLength( VFolderListItemData* data );
  boolean findFolderItemPosition( VFolderListItemData* data,
                                int label,
                                int& position );

  // folder_id==0 for root folder
  int insertFolderItem( int folder_id, int i, const MLString& text,
                        void* user_data,
                        boolean isFolder );
  // folder_id==0 for root folder
  int appendFolderItem( int folder_id, const MLString& text,
                        void* user_data,
                        boolean isFolder );

public:
  // folder_id==0 for root folder
  // returns new folder id
  int insertFolder( int folder_id, int i, const MLString& text,
                    void* user_data = 0 );

  // folder_id==0 for root folder
  // 1 <= i <= nFolderElements(folder)+1
  boolean insertItem( int folder_id, int i, const MLString& text,
                    void* user_data = 0 );

  // folder_id==0 for root folder
  // returns new folder id
  int appendFolder( int folder_id, const MLString& text,
                    void* user_data = 0 );

  // folder_id==0 for root folder
  boolean appendItem( int folder_id, const MLString& text,
                    void* user_data = 0 );

  // folder_id==0 for root folder
  // 0 <= i <= nFolderElements(folder)
  // i==0 for folder text
  boolean setItem( int folder_id, int i, const MLString& text,
                 void* user_data = 0 );

  // folder_id==0 for root folder
  // 1 <= i <= nFolderElements
  boolean deleteItem( int folder_id, int i );

  // folder_id!=0
  boolean deleteFolder( int folder_id );

  // folder_id==0 for root folder
  // 1 <= i <= nFolderElements
  // i==0 for folder text
  const MLString& getItem( int folder_id, int i );

  // folder_id==0 for root folder
  // 1 <= i <= nFolderElements
  // i==0 for folder text
  void* getUserData( int folder_id, int i );

  // may return 0
  int getUserDataIndex( int folder_id, void* user_data );

  // may return 0
  int getFolderID( int i );

  // may return 0
  int getFolderID( int folder_id, int i );

  int getFolderLength( int folder_id );

  // folder_id==0 for root folder
//  int getItemPosition( int folder_id, const MLString& message );

  boolean openFolder( int folder_id, boolean yes = true );
  boolean closeFolder( int folder_id );

private:
  // recursive
  void _openFolder( VList::Item* item, boolean yes );

  // recursive
  void closeAllFolders( VFolderListItemData* folder );

public:
  void closeAllFolders( int folder_id = 0 );

protected:
  void handleVListItem( VList*, VList::Item*,
                        int yBase, boolean selected );


  virtual boolean handleClick( int label, int x, int y );
  virtual boolean handleDoubleClick( int label );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VTextList_h
