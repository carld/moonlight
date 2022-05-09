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
   VEdit.h

   Creation: SR, August 8th, 1995
   Revisions:
     + SR July 5 1996
*/

#ifndef __VEdit_h
#define __VEdit_h

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __VWidget_h
#include "VWidget.h"
#endif

#ifndef __VWidgetEvent_h
#include "VWidgetEvent.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class VEdit: public VWidget, public VWidgetEventHandler
{
  friend class VWindow; // call the constructor

private:
  MLString string;
  int width, height, freeWidth;
  IPoint textpos;
  MLFont* editFont;

  int visualPos, pos;
  boolean selected;

public:
  VEdit( VWindow* _parent );
  virtual ~VEdit();

  void setFixedFont();

  void create( int x, int y, int _width, const MLString& _string );
  void create( int x, int y, int _width );

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
    return VWidget::EDIT;
    }
  void enableSelection( boolean yes = true );

  void changeEdit( const MLString& newString );

  const MLString& getEdit() const
    {
    return string;
    }

private:
  void go_left();
  void go_right();
  void buttonPress( int x );

protected:
  void drawEditText();
  void _drawEditText();
  void drawEdit();
  virtual boolean handleKey( Key key );

public:
  virtual void drawWidget();

  virtual void setCurrent( boolean yes );
  virtual boolean handleEvent( MLEvent& event );
  virtual boolean handleWidgetEvent( const VWidgetEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VEdit_h

