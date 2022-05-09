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
  VLine.h

  Stephane Rehel
  July 15 1996
*/

#ifndef __VLine_h
#define __VLine_h

#ifndef __VWidget_h
#include "VWidget.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class VLine: public VWidget
{
protected:
  IVector Y;
  IPoint p1, p2;

public:
  VLine( VWindow* parent, const IVector& _Y );
  virtual ~VLine();

  void create( const IPoint& _p1, const IPoint& _p2 );

  virtual VWidget::TYPE type() const
    {
    return VWidget::LINE;
    }

  boolean horizontal() const;

  virtual void drawWidget();

protected:
  virtual void drawTheLine();
  virtual boolean handleEvent( MLEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VLine_h
