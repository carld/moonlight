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
   VCheckBox.h

   Creation: SR, August 2nd, 1995
   Revisions:
     + SR July 5 1996
*/

#ifndef __VCheckBox_h
#define __VCheckBox_h

#ifndef __VCheckButton_h
#include "VCheckButton.h"
#endif

#ifndef __VLabel_h
#include "VLabel.h"
#endif

#ifndef __VWidgetEvent_h
#include "VWidgetEvent.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class VCheckBox: public VLabel, public VWidgetEventHandler
{
private:
  boolean checkedFlag;
  VCheckButton* box;

public:
  VCheckBox( VWindow* _parent );

  virtual ~VCheckBox();

  void create( int x, int y, int width, int height,
               const MLString& _message,
               VLabel::Justify justify = VLabel::LEFT,
               boolean _checked = false );

  virtual VWidget::TYPE type() const
    {
    return VWidget::CHECKBOX;
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
  boolean handlePrivateWidgetEvent( const VWidgetEvent& event );

protected:
  virtual boolean handleKey( Key key );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VCheckBox_h

