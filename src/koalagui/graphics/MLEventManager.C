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
  MLEventManager.C

  Stephane Rehel
  June 29, 1996
*/

#include "MLEventManager.h"
#include "MLEventHandler.h"
#include "MLEventGrabber.h"
#include "MLKeyboardHandler.h"
#include "MLWindow.h"
#include "SystemWindow.h"
#include "SoftWindow.h"
#include "GraphicsSystem.h"

/////////////////////////////////////////////////////////////////////////////

// used in dispatchEvents()
static MLWindow* pressWindow= 0;

/////////////////////////////////////////////////////////////////////////////

// static
MLEventManager* MLEventManager::eventManager= 0; // THE event manager

/////////////////////////////////////////////////////////////////////////////

MLEventManager::MLEventManager()
{
  assert( MLEventManager::eventManager == 0 );
  MLEventManager::eventManager= this;

  keyboardFocus= 0;
  eventGrabber= 0;

  // don't wait on events more than 1/20 second please
  wait_time_out= 1./20.;

  current_in_window= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLEventManager::~MLEventManager()
{
  while( ! events.empty() )
    {
    MLEvent* e= events.getLast();
    events.removeLast();
    delete e;
    }
}

/////////////////////////////////////////////////////////////////////////////

// focus may be NULL for no special focus
// return previous one
MLKeyboardHandler* MLEventManager::setKeyboardFocus(
                                         MLKeyboardHandler* _keyboardFocus )
{
  MLKeyboardHandler* prev= keyboardFocus;

  keyboardFocus= _keyboardFocus;

  return prev;
}

/////////////////////////////////////////////////////////////////////////////

// grabber may be NULL for ungrabbing
// return previous one
MLEventGrabber* MLEventManager::setEventGrabber(
                                            MLEventGrabber* _eventGrabber )
{
  MLEventGrabber* prev= eventGrabber;

  eventGrabber= _eventGrabber;

  return prev;
}

/////////////////////////////////////////////////////////////////////////////

void MLEventManager::setWaitTimeout( double wto )
{
  if( wto < 0 )
    wto= 0.;
  wait_time_out= wto;
}

/////////////////////////////////////////////////////////////////////////////

#ifndef NDEBUG
#include "KeyCodes.h"
#include <stdlib.h>
#endif

// return true if only_one and it remains events in the list
boolean MLEventManager::dispatch( boolean only_one /* = false */ )
{
  while( ! events.empty() )
    {
    MLEvent* event= events.getFirst();
    events.removeFirst();

    if( event == 0 )
      continue;

    if( event->window == 0 )
      goto next;

#ifndef NDEBUG
if( event->type == MLEvent::KEY_RELEASE && event->key == KeyCodes::CTRL_X )
  exit(0);
if( event->type == MLEvent::KEY_PRESS &&
    event->key == KeyCodes::X.control().alt() )
  system("/usr/X11/bin/xmag");
#endif

    if( event->type == MLEvent::KEY_PRESS ||
        event->type == MLEvent::KEY_RELEASE )
      {
      if( eventGrabber == 0 && keyboardFocus != 0 )
        {
        if( keyboardFocus->handleMLKeyboard(*event) )
          goto next;
        }
      }

    dispatchEvent( event->window, *event );

next:
    delete event;

    if( only_one )
      return !events.empty();
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

void MLEventManager::destroyWindow( MLWindow* window )
{
  if( window == 0 )
    return;

  removeEvents(window);

  if( pressWindow == window )
    pressWindow= 0;

  if( (void*)keyboardFocus == (void*)window )
    keyboardFocus= 0;

  if( (void*)eventGrabber == (void*)window )
    eventGrabber= 0;

  if( current_in_window == window )
    {
    current_in_window= 0;
    checkForEnterLeaveEvents();
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLEventManager::newWindow( MLWindow* window )
{
  if( window == 0 )
    return;

  checkForEnterLeaveEvents();
}

/////////////////////////////////////////////////////////////////////////////

void MLEventManager::removeEvents( MLWindow* window )
{
  // remove all events to this window
  IListIterator<MLEvent> li(events);
  IListElement<MLEvent>* ile= li.eol() ? 0 : li.nextElement();
  while( ile != 0 )
    {
    IListElement<MLEvent>* next= li.eol() ? 0 : li.nextElement();
    MLEvent* e= ile->element;
    if( e == 0 )
      continue;
    if( e->window == window )
      {
      events.remove(ile);
      delete e;
      }
    ile= next;
    }
}

/////////////////////////////////////////////////////////////////////////////

// return true if found any
boolean MLEventManager::removeTyped( MLWindow* window, MLEvent::Type eventType )
{
  if( window == 0 )
    return false;

  IListIterator<MLEvent> li(events);

  IListElement<MLEvent>* ile= li.eol() ? 0 : li.nextElement();

  boolean found= false;
  while( ile != 0 )
    {
    IListElement<MLEvent>* next= li.eol() ? 0 : li.nextElement();

    MLEvent* e= ile->element;
    if( e->window == window && e->type == eventType )
      {
      events.remove(ile);
      delete e;
      found= true;
      }

    ile= next;
    }

  return found;
}

/////////////////////////////////////////////////////////////////////////////

void MLEventManager::removeRedraw( SystemWindow* window )
{
  if( window == 0 )
    return;

  IListIterator<MLEvent> li(events);

  IListElement<MLEvent>* ile= li.eol() ? 0 : li.nextElement();

  while( ile != 0 )
    {
    IListElement<MLEvent>* next= li.eol() ? 0 : li.nextElement();

    MLEvent* e= ile->element;
    if( e->type == MLEvent::REDRAW )
      {
      boolean ok= false;
      if( e->window->isSystemWindow() )
        ok= (e->window == window);
       else
        ok= ((SoftWindow*)e->window)->systemParent == window;

      if(ok)
        {
        events.remove(ile);
        delete e;
        }
      }

    ile= next;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLEventManager::removeAnyRedraw()
{
  IListIterator<MLEvent> li(events);

  IListElement<MLEvent>* ile= li.eol() ? 0 : li.nextElement();

  while( ile != 0 )
    {
    IListElement<MLEvent>* next= li.eol() ? 0 : li.nextElement();

    MLEvent* e= ile->element;
    if( e->type == MLEvent::REDRAW )
      {
      events.remove(ile);
      delete e;
      }

    ile= next;
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLEventManager::findTyped( MLWindow* window, MLEvent::Type eventType )
{
  IListIterator<MLEvent> li(events);
  while( ! li.eol() )
    {
    MLEvent* e= li.next();
    if( e->type == eventType )
      {
      if( window == 0 )
        return true;
       else
        if( e->window == window )
          return true;
      }
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

MLEvent* MLEventManager::getTyped( MLWindow* window, MLEvent::Type eventType )
{
  IListIterator<MLEvent> li(events);
  while( ! li.eol() )
    {
    IListElement<MLEvent>* ile= li.nextElement();
    MLEvent* e= ile->element;
    if( e->window == window && e->type == eventType )
      {
      events.remove(ile);
      return e;
      }
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLEventManager::sendEvent( MLWindow* window, MLEvent& event,
                                 boolean with_grab /* = true */ )
{
  if( window == 0 )
    return true;

  if( (event.type != MLEvent::KEY_PRESS && event.type != MLEvent::KEY_RELEASE) &&
      ! window->mapped() )
    return true;

  if( eventGrabber != 0 && with_grab )
    return eventGrabber->eventGrabber(window,event);

  if( window->_eventHandler != 0 )
    return window->_eventHandler->eventHandler(window,event);

  return window->handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////

// protected
void MLEventManager::postEvent( MLEvent* event,
                                boolean ignore_unmapped /* = false */ )
{
  if( event == 0 )
    return;
  if( event->window == 0 )
    {
    return;
    delete event;
    }

  if( event->window->mapped() || ignore_unmapped )
    events.append(event);
   else
    delete event;
}

/////////////////////////////////////////////////////////////////////////////

// protected
boolean MLEventManager::dispatchEvent( MLWindow* window, MLEvent& event )
{
  if( window == 0 )
    return false;

//  boolean key_force= (eventGrabber != 0) &&
//                   (event.type == MLEvent::KEY_PRESS ||
//                    event.type == MLEvent::KEY_RELEASE);
boolean key_force= false;
  if( !key_force && !window->mapped() )
    return false;

  switch( event.type )
    {
    case MLEvent::REDRAW:
      {
      if( window->isSystemWindow() )
        {
        if( !event.privateEvent && ((SystemWindow*)window)->firstRedraw )
          {
          // this is the draw event generated by XCreateWindow
          ((SystemWindow*)window)->firstRedraw= false;
          return true;
          }
//        if( ! findTyped(window,MLEvent::REDRAW) )
          sendEvent(window,event);
        }
       else
        {
        register SoftWindow* soft= (SoftWindow*) window;

        int x1= event.position.x() - soft->absPosition.x();
        int x2= x1 + event.size.x() - 1;
        int y1= event.position.y() - soft->absPosition.y();
        int y2= y1 + event.size.y() - 1;

        if( !( x2<0 || x1>=window->size.x() ||
               y2<0 || y1>=window->size.y() ) )
          {
          MLEvent e;
          e.type= MLEvent::REDRAW;
          e.window= window;
          e.position= IPoint(0,0);
          e.size= window->size; // full redraw...
          e.button= event.button;
          e.key= event.key;
          e.state= event.state;
          sendEvent(window,event);
          }
        }

//      if( event.privateEvent )
        return true;

      IListIterator<SoftWindow> li(window->softChildren);
      while( ! li.eol() )
        dispatchEvent( li.next(), event );

      return true;
      }

    case MLEvent::MOUSE_PRESS:
    case MLEvent::MOUSE_RELEASE:
    case MLEvent::MOUSE_MOTION:
    case MLEvent::KEY_PRESS:
    case MLEvent::KEY_RELEASE:
      {
      static IPoint previousPosition(0,0);
#ifdef __WINDOWS__
      MLWindow* event_window= window;
#endif

      if( pressWindow != 0 )
        window= pressWindow;
       else
        {
        for( int i= window->softChildren.getNElements(); i >= 1; --i )
          {
          // event dispatching might have destroyed a window
          if( i > window->softChildren.getNElements() )
            i= window->softChildren.getNElements();
          SoftWindow* sw= window->softChildren.get(i);
          if( dispatchEvent( sw, event ) )
            return true;
          }
        }

      IPoint position;

      if( window->isSystemWindow() )
        position= event.position;
       else
        {
        register SoftWindow* soft= (SoftWindow*) window;

        position= IPoint( event.position.x() - soft->absPosition.x(),
                          event.position.y() - soft->absPosition.y() );

        if( pressWindow == 0 && !key_force )
          {
          if( position.x() < 0 ||
              position.x() >= window->size.x() ||
              position.y() < 0 ||
              position.y() >= window->size.y() )
            return false;
          }
        }

      if( pressWindow != 0 )
        {
#ifdef __WINDOWS__
        position= event.position + ( event_window->getAbsolutePosition()
                                    - pressWindow->getAbsolutePosition() );
#endif
        if( event.type == MLEvent::MOUSE_RELEASE )
          {
          boolean buttons;
          switch( event.button )
            {
            case 1: buttons= event.middle_pressed() || event.right_pressed();
                    break;
            case 2: buttons= event.left_pressed() || event.right_pressed();
                    break;
            case 3: buttons= event.left_pressed() || event.middle_pressed();
                    break;
            default: buttons= false;
            }
          if( ! buttons )
            pressWindow= 0;
          }
        }
       else
        {
        if( event.type == MLEvent::MOUSE_PRESS )
          {
          pressWindow= window;
          previousPosition= position;
          }
        }

      MLEvent e;
      e.type= event.type;
      e.time= event.time;
      e.window= window;
      e.position= position;
      if( pressWindow != 0 && event.type == MLEvent::MOUSE_MOTION )
        {
        e.size= position - previousPosition;
        previousPosition= position;
        }
       else
        e.size= event.size;
      e.button= event.button;
      e.key= event.key;
      e.state= event.state;

      sendEvent( e.window, e );

      return true;
      }

    default:
      {
      // private event?
      sendEvent(window,event);
      break;
      }
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

IList<SystemWindow>& MLEventManager::getSystemChildren( SystemWindow* sw )
{
  return sw->systemWindows;
}

/////////////////////////////////////////////////////////////////////////////

void MLEventManager::checkForEnterLeaveEvents()
{
  GraphicsSystem* gs= GraphicsSystem::getGraphicsSystem();
  if( gs == 0 )
    return;

  // leave this window
  MLEvent e;
  MLWindow* ciw= gs->getCursorWindow(&e);

  if( ciw == current_in_window )
    return;

  MLWindow* prev_current_in_window= current_in_window;
  current_in_window= ciw;

  if( prev_current_in_window != 0 )
    {
    // leave
    MLEvent* event= new MLEvent;
    event->type= MLEvent::MOUSE_LEAVE;
    event->window= prev_current_in_window;
    event->time= 0;
    event->privateEvent= false;
    event->position= IPoint(0,0);
    event->size= IVector(0,0);
    event->button= 0;
    event->key= Key(char(0));
    event->state= 0;

    prev_current_in_window->mousePointerInto= false;
    postEvent(event);
    }

  if( ciw != 0 )
    {
    // enter curr
    MLEvent* event= new MLEvent;
    event->type= MLEvent::MOUSE_ENTER;
    event->window= e.window;
    event->time= 0;
    event->privateEvent= false;
    event->position= e.position;
    event->size= IVector(0,0);
    event->button= 0;
    event->key= Key(char(0));
    event->state= e.state;

    ciw->mousePointerInto= true;
    postEvent(event);
    }
}

/////////////////////////////////////////////////////////////////////////////
