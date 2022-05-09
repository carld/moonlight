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
   VScrollBar.h

   Creation: SR, July 28th, 1995
   Revisions:
     + SR, July 29th 1995: Global revision...:-)
     + SR July 5 1996
*/

#ifndef __VScrollBar_h
#define __VScrollBar_h

#ifndef __VWindow_h
#include "VWindow.h"
#endif

#ifndef __VWidget_h
#include "VWidget.h"
#endif

#ifndef __VBar_h
#include "VBar.h"
#endif

class VScrollList;

/////////////////////////////////////////////////////////////////////////////

class VScrollBar: public VBar
{
  friend class VScrollList; // for calling VScrollBar::handlePrivateWidgetEvent()

private:
  int width, height;

  VArrow *upArrow, *downArrow;

public:
  VScrollBar( VWindow* _parent, const IVector& _X );

  virtual ~VScrollBar();

  void create( int x, int y, int _height,
               int totalLength, int visualLength );

  int getWidth() const
    {
    return width;
    }
  int getHeight() const
    {
    return height;
    }
  virtual VWidget::TYPE type() const
    {
    return VWidget::SCROLLBAR;
    }

  virtual IRectangle getWidgetRectangle() const;
  virtual void setWidgetPosition( const IPoint& pos );
  virtual void drawWidget();
  virtual VWidget** getWidgetsList( int* nWidgets );

private:
  // private widget event handler for:
  //   + VBar
  //   + up VArrow
  //   + down VArrow
  boolean handlePrivateWidgetEvent( const VWidgetEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VScrollBar_h

