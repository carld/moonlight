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
   VWidgetEvent.h

   Stephane Rehel
   Creation: August 9 1996 from VWidget.h
*/

#ifndef __VWidgetEvent_h
#define __VWidgetEvent_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __Key_h
#include "graphics/Key.h"
#endif

class VWidget;
class VWidgetEventHandler;

class MLEvent;

/////////////////////////////////////////////////////////////////////////////

class VWidgetEvent
{
public:
  enum TYPE
    {
    MLEVENT, SETBARPOS, SCROLLBARPOS,
             BEGINPRESS, ENDPRESS, REPEATPRESS,
             ENTERKEY, ENTEREDIT,
             PRESSRADIO, SETRANGEBARVALUE
    };
private:
  TYPE theType;
  MLEvent* event;
  VWidget* widget;
  int pos;
  Key key;
  double value;

  VWidgetEvent()
    {}

public:
  static const VWidgetEvent VMLEvent( VWidget* _widget, MLEvent& _event )
    {
    VWidgetEvent e;
    e.widget= _widget;
    e.theType= MLEVENT;
    e.event= &_event;
    return e;
    }
  static const VWidgetEvent SetBarPos( VWidget* _widget, int pos )
    {
    VWidgetEvent e;
    e.widget= _widget;
    e.theType= SETBARPOS;
    e.pos= pos;
    return e;
    }
  static const VWidgetEvent ScrollBarPos( VWidget* _widget, int pos )
    {
    VWidgetEvent e;
    e.widget= _widget;
    e.theType= SCROLLBARPOS;
    e.pos= pos;
    return e;
    }
  static const VWidgetEvent BeginPress( VWidget* _widget )
    {
    VWidgetEvent e;
    e.widget= _widget;
    e.theType= BEGINPRESS;
    return e;
    }
  static const VWidgetEvent EndPress( VWidget* _widget )
    {
    VWidgetEvent e;
    e.widget= _widget;
    e.theType= ENDPRESS;
    return e;
    }
  static const VWidgetEvent RepeatPress( VWidget* _widget )
    {
    VWidgetEvent e;
    e.widget= _widget;
    e.theType= REPEATPRESS;
    return e;
    }
  static const VWidgetEvent EnterKey( VWidget* _widget, Key _key )
    {
    VWidgetEvent e;
    e.widget= _widget;
    e.theType= ENTERKEY;
    e.key= _key;
    return e;
    }
  static const VWidgetEvent EnterEdit( VWidget* _widget )
    {
    VWidgetEvent e;
    e.widget= _widget;
    e.theType= ENTEREDIT;
    return e;
    }
  static const VWidgetEvent PressRadio( VWidget* _widget )
    {
    VWidgetEvent e;
    e.widget= _widget;
    e.theType= PRESSRADIO;
    return e;
    }
  static const VWidgetEvent SetRangeBarValue( VWidget* _widget, double _v )
    {
    VWidgetEvent e;
    e.widget= _widget;
    e.theType= SETRANGEBARVALUE;
    e.value= _v;
    return e;
    }

  TYPE type() const
    {
    return theType;
    }
  VWidget* getWidget() const
    {
//    assert( widget != 0 );
    return widget;
    }
  MLEvent* getMLEvent() const
    {
//    assert( theType == MLEVENT );
//    assert( event != 0 );
    return event;
    }
  int getPos() const
    {
    return pos;
    }
  Key getKey() const
    {
//    assert( theType == ENTERKEY );
    return key;
    }
  double getValue() const
    {
//    assert( theType == SETRANGEBARVALUE );
    return value;
    }
};

/////////////////////////////////////////////////////////////////////////////

class VWidgetEventHandler
{
  friend class VWidget;

public:
  VWidgetEventHandler()
    {}

protected:
  virtual boolean handlePrivateWidgetEvent( const VWidgetEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VWidgetEvent_h
