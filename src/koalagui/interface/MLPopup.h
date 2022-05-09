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
  MLPopup.h

  Stephane Rehel
  November 19 1996
*/

#ifndef __MLPopup_h
#define __MLPopup_h

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __MLEventGrabber_h
#include "graphics/MLEventGrabber.h"
#endif

#ifndef __MLPopupHandler_h
#include "MLPopupHandler.h"
#endif

class SystemWindow;
class MLButtonsPanel;

/////////////////////////////////////////////////////////////////////////////

class MLPopup: public MLEventGrabber
{
public:
  // special labels
  static int OPEN;
  static int CLOSE;

  struct Item
    {
    MLString message;
    MLString accel_msg;

    int label;
    MLPopup* subPopup;
    boolean highlighted;
    boolean checkable;
    boolean checked;

    boolean enabled;

    Item();
    };

protected:
  int popupLabel;
  int nItems;
  int max_items;
  Item* items;
  MLPopupHandler* handler;
  int previouslySelected;

  SystemWindow* window;
  boolean hasChecks; // runtime
  MLButtonsPanel* buttonsPanel;
  unsigned long pressTime;

public:
  MLPopup( MLPopupHandler* _handler );
  virtual ~MLPopup();

  int add( const MLString& message, MLPopup* sub = 0 );
  int addCheck( const MLString& message );
  int getPopupLabel() const
    {
    return popupLabel;
    }

  // return 0 if not found
  Item* findItem( int label );
  boolean checked( int label );
  void check( int label, boolean yes );

  boolean isEnabled( int label );
  // return previous value
  boolean enable( int label, boolean yes = true );

  Item* getShortcutItem();
  boolean handleAction( Item* item );
  void open( int x, int y, MLButtonsPanel* _buttonsPanel,
             unsigned long _pressTime );

  int getNItems() const
    {
    return nItems;
    }

protected:
  void create( int x, int y );
  void drawItem( int i );
  void draw();
  void close();
  void quit( boolean unpress_button_panel = true );
  void setActiveItem( int i, boolean openSub );

  boolean keyPressed( MLEvent& event );
  virtual boolean eventGrabber( MLWindow*, MLEvent& );

  // return -1 if none
  int getSelected() const;

  void get_popup( int x, int y,
                  MLWindow* window,
                  MLPopup*& popup,
                  int& position,
                  int& item );

};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLPopup_h
