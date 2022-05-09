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
   VWidget.h

   Creation: SR, July 28th, 1995
   Revisions:
     + SR July 5 1996
     + SR August 9 1996
*/

#ifndef __VWidget_h
#define __VWidget_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

#ifndef __IRectangle_h
#include "tools/IRectangle.h"
#endif

#ifndef __Key_h
#include "graphics/Key.h"
#endif

#ifndef __VWindow_h
#include "VWindow.h"
#endif

/*
#ifndef __KWCallbackClass_h
#include "kw/KWCallbackClass.h"
#endif
*/

class KWEvent;
class VWidgetEvent;
class VWidgetEventHandler;

class MLFont;

/////////////////////////////////////////////////////////////////////////////

class VDialog;
class VWidgetsSet;

class VWidget: public VWindow
{
  friend class VDialog;
  friend class VWidgetsSet;
  friend class KWDialog;

private:
  VWidgetEventHandler* widgetEventHandler;
  boolean available, current;
  VDialog* dialog;
  boolean staticWidget;
  VWidget* linkWidget;

private:
  int id;
  VCard* card;
  KWDialog* kwDialog;

public:
  enum TYPE
    {
    FIRST= 0,

    LABEL       =  1,
    BLANKBUTTON =  2,
    BUTTON      =  3,
    ARROW       =  4,
    BAR         =  5,
    SCROLLBAR   =  6,
    LIST        =  7,
    TEXTLIST    =  8,
    SCROLLLIST  =  9,
    CHECKBOX    = 10,
    EDIT        = 11,
    VALUEEDIT   = 12,
    CIRCLEBUTTON= 13,
    RADIOBUTTON = 14,
    RADIOBOX    = 15,
    TEXTIO      = 16,
    COMMANDLINE = 17,
    RANGEBAR    = 18,
    VALUEBAR    = 19,
    WIDGETS_SET = 20,
    GROUP       = 21,
    LINE        = 22,
    TEXTBOX     = 23,
    DIALOG      = 24,
    AREA        = 25,
    PROGRESSBAR = 26,
    PICTURE     = 27,
    CHOOSER     = 28,
    CARD        = 29,
    FOLDERLIST  = 30,

    LAST
    };

  VWidget( VWindow* parent );
  virtual ~VWidget();

  virtual TYPE type() const= 0;

  void setWidgetEventHandler( VWidgetEventHandler* weh );

  boolean isAvailable() const
    {
    return available;
    }
  virtual void setAvailable( boolean yes = true );

  virtual void drawWidget() = 0;
  virtual VWidget** getWidgetsList( int* nWidgets );
  virtual void postDrawWidget();

  boolean activable() const
    {
    return dialog != 0;
    }

  virtual void setCurrent( boolean yes );
  boolean isCurrent() const;

  virtual char getHotChar() const;

  void setLinkWidget( VWidget* _linkWidget );

  int getWidgetID() const
    {
    return id;
    }
  void setWidgetID( int _id )
    {
    id= _id;
    }
  // may return 0
  VWidget* getLinkWidget() const
    {
    return linkWidget;
    }

  boolean isStaticWidget() const
    {
    return staticWidget;
    }
  virtual IRectangle getWidgetRectangle() const;
  virtual void setWidgetPosition( const IPoint& pos );

protected:
  virtual void map( boolean yes = true );

  virtual boolean handleWidgetEvent( const VWidgetEvent& event );
  boolean callPrivateWidgetEventHandler( const VWidgetEvent& event );

  class HotCharMessage
    {
    public:
      MLString message;
      char ch;
      int x1, x2;
      int width;
      HotCharMessage();
      HotCharMessage( const MLString& msg, MLFont* font );
      ~HotCharMessage() {}
      void set( const MLString& msg, MLFont* font );
      void print( VWindow* window, const IPoint& pos );
    };

  virtual boolean handleKey( Key key );

  virtual boolean kwCall( int );
  virtual boolean kwCall( KWEvent& );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VWidget_h
