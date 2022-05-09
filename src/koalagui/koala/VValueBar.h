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
   VValueBar.h

   Stephane Rehel
   August 13 1996
*/

#ifndef __VValueBar_h
#define __VValueeBar_h

#ifndef __VWidget_h
#include "VWidget.h"
#endif

#ifndef __VValueEdit_h
#include "VValueEdit.h"
#endif

class VRangeBar;

/////////////////////////////////////////////////////////////////////////////

class VValueBar: public VValueEdit
{
private:
  VRangeBar* bar;

public:
  VValueBar( VWindow* _parent );

  virtual ~VValueBar();

  void create( int x, int y, int _totalWidth,
               int _editWidth,
               double _barMin,
               double _barMax,
               double _editMinValue,
               double _editMaxValue,
               int    _editNDigits,
               double _editValue );

  void setRanges( double _barMin,
                  double _barMax,
                  double _editMinValue,
                  double _editMaxValue,
                  int    _editNDigits );

  virtual VWidget::TYPE type() const
    {
    return VWidget::VALUEBAR;
    }

  boolean handleWidgetEvent( const VWidgetEvent& event );

  void changeValue( double _value );

  virtual IRectangle getWidgetRectangle() const;
  virtual void setWidgetPosition( const IPoint& pos );
  virtual void drawWidget();
  virtual VWidget** getWidgetsList( int* nWidgets );

private:
  boolean handlePrivateWidgetEvent( const VWidgetEvent& event );
  virtual boolean handleKey( Key key );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VValueBar_h

