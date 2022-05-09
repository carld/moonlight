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
  VTextBox.h

  Stephane Rehel
  July 16 1996
*/

#ifndef __VTextBox_h
#define __VTextBox_h

#ifndef __VWidget_h
#include "VWidget.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class VTextBox: public VWidget
{
public:
  enum Justify { LEFT= 1, CENTER= 2, RIGHT= 3 };

protected:
  MLString _message;
  HotCharMessage hcm;
  int x0;
  Justify justify;
  MLFont* tbFont;

public:
  VTextBox( VWindow* parent );
  virtual ~VTextBox();

  void setFixedFont();
  void create( int x, int y, int width,
               const MLString& _message,
               Justify _justify = CENTER );

  virtual VWidget::TYPE type() const
    {
    return VWidget::TEXTBOX;
    }

  void changeMessage( const MLString& message );
  const MLString& getMessage() const
    {
    return hcm.message;
    }

  virtual void drawWidget();
  virtual char getHotChar() const;

protected:
  virtual void drawTextBox();
  virtual boolean handleEvent( MLEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VTextBox_h
