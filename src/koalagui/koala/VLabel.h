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
   VLabel.h

   Creation: SR, July 27th, 1995
   Revisions:
     + SR July 5 1996
*/

#ifndef __VLabel_h
#define __VLabel_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __VWidget_h
#include "VWidget.h"
#endif

class MLFont;
class MLEvent;

/////////////////////////////////////////////////////////////////////////////

class VLabel: public VWidget
{
public:
  enum Justify { LEFT= 1, CENTER= 2, RIGHT= 3 };

private:
  HotCharMessage hcm;
  MLString message;
  MLFont* labelFont;
  Justify justify;
  IPoint textpos;
  int margin;
  int justMargin;

public:
  VLabel( VWindow* _parent );

  virtual ~VLabel();

  void setFixedFont();
  void setBigFont();
  void setMargin( int _justMargin );
  void create( int x, int y, int _width, int _height,
               const MLString& _message,
               Justify _justify = LEFT );

  virtual VWidget::TYPE type() const
    {
    return VWidget::LABEL;
    }

  const MLString& getMessage() const
    {
    return hcm.message;
    }

  void changeMessage( const MLString& _message );

  void drawWidget();

  void drawDotRectangle();

protected:
  virtual void drawLabel();

public:
  virtual boolean handleEvent( MLEvent& event );
  virtual char getHotChar() const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VLabel_h
