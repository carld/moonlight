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
  MLEventManager.h

  Stephane Rehel
  June 29, 1996
*/

#ifndef __MLEventManager_h
#define __MLEventManager_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __IList_h
#include "tools/IList.h"
#endif

#ifndef __MLEvent_h
#include "MLEvent.h"
#endif

class MLEvent;
class MLWindow;
class SystemWindow;

class MLKeyboardHandler;
class MLEventGrabber;

/////////////////////////////////////////////////////////////////////////////

class MLEventManager
{
private:
  static MLEventManager* eventManager; // THE event manager

protected:
  IList<MLEvent> events;
  MLKeyboardHandler* keyboardFocus;
  MLEventGrabber* eventGrabber;

  // time out in seconds
  double wait_time_out;

  // window in which the cursor is
  MLWindow* current_in_window;

public:
  MLEventManager();

  virtual ~MLEventManager();

  static MLEventManager* getEventManager()
    {
    return eventManager;
    }

  // focus may be NULL for no special focus
  // return previous one
  MLKeyboardHandler* setKeyboardFocus( MLKeyboardHandler* _keyboardFocus );

  // grabber may be NULL for ungrabbing
  // return previous one
  MLEventGrabber* setEventGrabber( MLEventGrabber* _eventGrabber );

  boolean empty() const
    {
    return events.empty();
    }

  void setWaitTimeout( double wto );

  // wait for events and get them
  // returns if no event after wait_time_out seconds
  // return false if timed out
  virtual boolean wait() = 0;

  // get events and return
  virtual void get() = 0;

  // Called by any thread for unblocking the main thread
//  virtual void unblockWaiting() = 0;

  boolean dispatch( boolean only_one = false );

  void destroyWindow( MLWindow* window );
  void newWindow( MLWindow* window );

  void removeEvents( MLWindow* window );

  // return true if found any
  boolean removeTyped( MLWindow* window, MLEvent::Type eventType );

  boolean findTyped( MLWindow* window, MLEvent::Type eventType );

  MLEvent* getTyped( MLWindow* window, MLEvent::Type eventType );

  void postEvent( MLEvent* event, boolean ignore_unmapped = false );

  void removeRedraw( SystemWindow* window );

  void removeAnyRedraw();

  boolean sendEvent( MLWindow* window, MLEvent& event,
                     boolean with_grab = true );

protected:
  boolean dispatchEvent( MLWindow* window, MLEvent& event );

  IList<SystemWindow>& getSystemChildren( SystemWindow* sw );

public:
  void checkForEnterLeaveEvents();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLEventManager_h

