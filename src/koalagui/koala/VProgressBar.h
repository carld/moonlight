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
   VProgressBar.h

   Stephane Rehel
   August 12 1996
*/

#ifndef __VProgressBar_h
#define __VProgressBar_h

#ifndef __VRangeBar_h
#include "VRangeBar.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class VProgressBar: public VRangeBar
{
private:
  int perc;

public:
  VProgressBar( VWindow* _parent );

  virtual ~VProgressBar();

  virtual void create( int x, int y, int _width, int _height );

  void drawProgressBar();

  virtual VWidget::TYPE type() const
    {
    return VWidget::PROGRESSBAR;
    }

  virtual void drawWidget();

public:
  // old_value <= value <= 1
  void changeValue( double _value );

protected:
  virtual boolean handleEvent( MLEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VProgressBar_h

