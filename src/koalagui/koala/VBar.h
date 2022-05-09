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
   VBar.h

   Creation: SR, July 28th, 1995
   Revisions:
     + SR July 5 1996
*/

#ifndef __VBar_h
#define __VBar_h

#ifndef __VWidget_h
#include "VWidget.h"
#endif

#ifndef __VWidgetEvent_h
#include "VWidgetEvent.h"
#endif

class VScrollingDialogs;

/////////////////////////////////////////////////////////////////////////////

class VBar: public VWidget, public VWidgetEventHandler
{
  friend class VScrollingDialogs;
  friend class VList;

protected:
  int scrollBar_width;

  IVector X, Y;

  int width, height;
  int scrollingHeight;
  IPoint top, bottom;

  struct Image
    {
    int totalLength;
    int visualLength;
    int pos;
    } image;

  struct Bar
    {
    int width, height;
    int freeHeight;
    int y;
    VBlankButton* button;
    } bar;

private:
  boolean inTop;
  boolean inBottom;

public:
  VBar( VWindow* _parent, const IVector& _X );

  virtual ~VBar();

  void create( int x, int y, int _height,
               int totalLength, int visualLength );

  virtual VWidget::TYPE type() const
    {
    return VWidget::BAR;
    }

  // 1 <= getPosition() <= totalLength
  int getPosition() const
    {
    return image.pos + 1;
    }
  int getTotalLength() const
    {
    return image.totalLength;
    }

  // 1 <= i <= totalLength
  void changePosition( int i );

  // 0 <= tl
  void changeTotalLength( int tl );

  // 1 <= vl
  void changeVisualLength( int vl );

protected:
  boolean vertical() const
    {
    return X.x() != 0;
    }
  int coord( const IPoint& p ) const
    {
    return vertical() ? p.y() : p.x();
    }
  int coord( const IVector& v ) const
    {
    return vertical() ? v.y() : v.x();
    }
  int coordx( const IPoint& p ) const
    {
    return vertical() ? p.x() : p.y();
    }
  int coordx( const IVector& v ) const
    {
    return vertical() ? v.x() : v.y();
    }
  virtual boolean handlePrivateWidgetEvent( const VWidgetEvent& event );
  void drawBarBackground();

protected:
  void _drawBarBackground();

public:
  virtual void drawWidget();
  virtual VWidget** getWidgetsList( int* nWidgets );

  virtual boolean handleEvent( MLEvent& event );

  void up( int dy = 1 );
  void down( int dy = 1 );
  void page_down();
  void page_up();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VBar_h

