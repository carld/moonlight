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
  VGroup.h

  Stephane Rehel
  July 15 1996
*/

#ifndef __VGroup_h
#define __VGroup_h

#ifndef __VWidget_h
#include "VWidget.h"
#endif

class MLFont;

/////////////////////////////////////////////////////////////////////////////

class VGroup: public VWidget
{
protected:
  HotCharMessage hcm;
  MLFont* labelFont;
  IPoint labelPos;
  IPoint textPos;
  IRectangle widgetsArea;
  IPoint p1, p2;

public:
  VGroup( VWindow* parent );
  virtual ~VGroup();

  void create( int x, int y, int width, int height,
               const MLString& _message );

  virtual VWidget::TYPE type() const
    {
    return VWidget::GROUP;
    }

  const IRectangle& getWidgetsArea() const
    {
    return widgetsArea;
    }

  void changeMessage( const MLString& message );
  const MLString& getMessage() const
    {
    return hcm.message;
    }

  virtual void drawWidget();
  virtual char getHotChar() const;

protected:
  virtual void drawGroup();
  virtual boolean handleEvent( MLEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VGroup_h
