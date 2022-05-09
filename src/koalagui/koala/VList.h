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
  VList.h

  Stephane Rehel
  Creation: SR, July 29th, 1995
  Revisions:
    + SR July 5 1996
    + rewrote
*/

#ifndef __VList_h
#define __VList_h

#ifndef __IList_h
#include "tools/IList.h"
#endif

#ifndef __IRectangle_h
#include "tools/IRectangle.h"
#endif

#ifndef __VWidget_h
#include "VWidget.h"
#endif

#ifndef __VWidgetEvent_h
#include "VWidgetEvent.h"
#endif

class MLGLPixmap;

class VListItemHandler;
class VListItemData;

/////////////////////////////////////////////////////////////////////////////

class VList: public VWidget, public VWidgetEventHandler
{
public:
  typedef int Label;
  struct Item
    {
    Label label;
    VListItemHandler* handler;
    VListItemData* data;
    boolean hidden;
    int height;
    IPoint p1, p2; // position in virtual window
    IRectangle pressArea; // local pressing area

    int getWidth() const { return p2.x()-p1.x()+1; }
    int getHeight() const { return height; }

    void* user_data;
    };

protected:
  static int labelsCounter;

  MLGLPixmap* image;
  IList<Item> items;
  int imageWidth, imageHeight, imagePos, gimagePos;
  int preciseVirtualHeight, virtualHeight;
  Label bar; // may be Label(0)
  boolean needRedraw, needImageRedraw;
  boolean hasBar;
  IPoint drawImageOrigin;

  VVertBar* scrollBar;
  boolean updatePositionFlag;
  int scrollingGranularity;
  boolean rightBar;

public:
  VList( VWindow* parent );
  virtual ~VList();

  void setRightBar();
  void create( int x, int y, int width, int height, boolean scrolling,
               int _scrollingGranularity );

  void enableBar( boolean yes );

  virtual VWidget::TYPE type() const
    {
    return VWidget::LIST;
    }

  virtual void drawWidget();
  virtual VWidget** getWidgetsList( int* nWidgets );

  void drawList();

protected:
  void drawListImage();
  void _drawListImage();

protected:
  void redrawItem( Item* i );
public:
  void redrawImage();
  void refresh();

  Item* newItem( int height,
                 VListItemHandler* handler,
                 VListItemData* data,
                 boolean hidden = false,
                 void* user_data = 0 );

  Item* appendItem( int height,
                    VListItemHandler* handler,
                    VListItemData* data,
                    boolean hidden = false,
                    void* user_data = 0 );

  // 1 <= i <= nElement+1
  Item* insertItem( int i,
                    int height,
                    VListItemHandler* handler,
                    VListItemData* data,
                    boolean hidden = false,
                    void* user_data = 0 );

  boolean deleteItem( Label label );

protected:
  void updateItemsPositions();
private:
  void updateItemsList();

public:
  Item* findItem( Label label );
  Item* findItem( void* user_data );
  boolean setBar( Label label );

  void setInteger( int i );
  int getInteger(); // may return 0
  void* getUserData( int i ); // may return 0
  int getUserDataIndex( void* user_data ); // may return 0
  int getLength() const;

protected:
  void setImagePos( int newImagePos );
  virtual boolean handleEvent( MLEvent& event );

private:
  // handles private widget events from VScrollBar
  virtual boolean handlePrivateWidgetEvent( const VWidgetEvent& event );
  void visibleItem( Label label );

protected:
  virtual boolean handleKey( Key key );

  virtual boolean handleClick( int label, int x, int y );
  virtual boolean handleDoubleClick( int label );
};

/////////////////////////////////////////////////////////////////////////////

class VListItemHandler
{
public:
  VListItemHandler() {}
  virtual ~VListItemHandler() {}
  virtual void handleVListItem( VList*, VList::Item*,
                                int yBase, boolean selected ) = 0;
};

/////////////////////////////////////////////////////////////////////////////

class VListItemData
{
public:
  VListItemData() {}
  virtual ~VListItemData() {}
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VList_h
