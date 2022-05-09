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
   VRangeBar.h

   Creation: SR, April 9th, 1996, at home
   Revisions:
     + SR July 5 1996
*/

#ifndef __VRangeBar_h
#define __VRangeBar_h

#ifndef __VWidget_h
#include "VWidget.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class VRangeBar: public VWidget
{
private:
  int width, height;

  int lx;
  double value;
  double min_value, max_value;
  IPoint top, bottom;
  VWidget* activateWidget;

public:
  VRangeBar( VWindow* _parent );

  VRangeBar( VWindow* _parent, VWidget* _activateWidget );

  virtual ~VRangeBar();

  void create( int x, int y, int _width, int _height,
               double _value,
               double _min_value, double _max_value );

  void setRanges( double _min_value, double _max_value );

  double getMinValue() const;
  double getMaxValue() const;

  void drawRangeBar();

  virtual VWidget::TYPE type() const
    {
    return VWidget::RANGEBAR;
    }

  virtual void drawWidget();

private:
  int value_to_lx( double value ) const;
  double lx_to_value( int lx ) const;

public:
  // value can be outside range!!
  void changeValue( double _value );

  double getValue() const
    {
    return value;
    }

protected:
  virtual boolean handleEvent( MLEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VRangeBar_h

