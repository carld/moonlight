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
   VRadioButton.h

   Creation: SR, August 9th, 1995
   Revisions:
     + SR July 5 1996
*/

#ifndef __VRadioButton_h
#define __VRadioButton_h

#ifndef __VLabel_h
#include "VLabel.h"
#endif

#ifndef __VWidgetEvent_h
#include "VWidgetEvent.h"
#endif

class VCircleButton;

/////////////////////////////////////////////////////////////////////////////

class VRadioButton: public VLabel, public VWidgetEventHandler
{
private:
  boolean checkedFlag;
  VCircleButton* box;

public:
  VRadioButton( VWindow* _parent );
  virtual ~VRadioButton();

  void create( int x, int y, int width, int height,
               const MLString& _message,
               VLabel::Justify justify = VLabel::LEFT,
               boolean _checked = false );

  virtual VWidget::TYPE type() const
    {
    return VWidget::RADIOBUTTON;
    }

  void check( boolean yes = true );
  void uncheck();

  boolean checked() const
    {
    return checkedFlag;
    }

  virtual void drawWidget();
  virtual VWidget** getWidgetsList( int* nWidgets );
  virtual IRectangle getWidgetRectangle() const;
  virtual void setWidgetPosition( const IPoint& pos );

private:
  virtual boolean handlePrivateWidgetEvent( const VWidgetEvent& event );
  virtual boolean handleEvent( MLEvent& event );

protected:
  virtual boolean handleKey( Key key );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VRadioButton_h
