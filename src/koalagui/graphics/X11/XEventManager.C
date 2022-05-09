// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//                 2002       Patrick Mauritz
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
  XEventManager.C

  Must be created after XGraphicsSystem is created

  Stephane Rehel
  June 29, 1996
*/

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#if defined(__AIX__) || defined(___AIX) || defined(_AIX) || \
    defined(__QNX__) || defined(___AIXV3) || defined(AIXV3) || \
    defined(_SEQUENT_)
// for select() in delay()
#include <sys/select.h>
#endif

#include "SystemWindow.h"

#include "XEventManager.h"
#include "XGraphicsSystem.h"
#include "XWindowDriver.h"
#include "XKey.h"

#include "MLEvent.h"

/////////////////////////////////////////////////////////////////////////////

static Display* display= 0;
static XGraphicsSystem* xgs= 0;

/////////////////////////////////////////////////////////////////////////////

XEventManager::XEventManager()
{
  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  assert( graphicsSystem != 0 );
  xgs= (XGraphicsSystem*) graphicsSystem;

  display= xgs->getDisplay();
}

/////////////////////////////////////////////////////////////////////////////

XEventManager::~XEventManager()
{}

/////////////////////////////////////////////////////////////////////////////

// return false if timed out
boolean XEventManager::wait()
{
/*
  XEvent event;

  XSync(display,False);
  do{
    XNextEvent( display, &event );
    handleEvent(&event);
    }
  while( XPending(display) );
*/

  int x_fd= XConnectionNumber(display);

  fd_set in_fdset;
  FD_ZERO(&in_fdset);
  FD_SET(x_fd,&in_fdset);

  XSync(display,False);
  if( ! XPending(display) )
    {
    timeval timeout;
    timeout.tv_sec= long(wait_time_out);
    timeout.tv_usec=long((wait_time_out-double(timeout.tv_sec)) * 1e6);
    int outcome;

    // wait for events
#ifdef __hpux
    outcome= select( x_fd + 1, (int*) &in_fdset, 0, 0, &timeout );
#else
    outcome= select( x_fd + 1, &in_fdset, 0, 0, &timeout );
#endif
    if( outcome < 0 )
      {
      // error!!
      return false;
      }
    if( outcome == 0 )
      return false; // timed out!
    }

  XEvent event;
  do{
    XNextEvent( display, &event );
    handleEvent(&event);
    }
  while( XPending(display) );

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void XEventManager::get()
{
  XEvent event;

  XSync(display,False);
  while( XPending(display) )
    {
    XNextEvent( display, &event );
    handleEvent(&event);
    }
}

/////////////////////////////////////////////////////////////////////////////

// protected:
void XEventManager::handleEvent( XEvent* event )
{
  if( event == 0 )
    return;

  Window windowID= event->xany.window;

  SystemWindow* window= findWindow(windowID);
  if( window == 0 )
    return;

  MLEvent* e= new MLEvent;
  e->privateEvent= false;
  e->window= window;

  boolean ok= true;

  switch( event->type )
    {
    case MapNotify:
      {
      if( windowID != xgs->getRootWindowID() )
        break;

      xgs->iconify(false);
      ok= false;
      break;
      }

    case UnmapNotify:
      {
      if( windowID != xgs->getRootWindowID() )
        break;
      xgs->iconify(true);
      ok= false;
      break;
      }

    case Expose:
      {
      if( xgs->doRedrawEvents() )
        {
        ok= false;
        break;
        }

      if( event->xexpose.count != 0 )
        {
        ok= false;
        break;
        }

      e->type= MLEvent::REDRAW;
      e->time= 0;
//      e->position= IPoint( event->xexpose.x, event->xexpose.y );
//      e->size= IVector( event->xexpose.width, event->xexpose.height );

      e->position= IPoint(0,0);
      e->size= window->getSize();
      e->button= 0;
      e->state= 0;
      break;
      }

    case ButtonPress:
    case ButtonRelease:
      {
      e->type= (event->type==ButtonPress) ? MLEvent::MOUSE_PRESS
                                          : MLEvent::MOUSE_RELEASE;
      e->time= (unsigned long) event->xbutton.time;
      e->position= IPoint( event->xbutton.x, event->xbutton.y );
      e->size= IVector(0,0);
      e->button= event->xbutton.button;
      e->state= 0;
      if( (event->xbutton.state & Button1Mask) != 0 )
        e->state |= int(MLEvent::LEFT_BUTTON);
      if( (event->xbutton.state & Button2Mask) != 0 )
        e->state |= int(MLEvent::MIDDLE_BUTTON);
      if( (event->xbutton.state & Button3Mask) != 0 )
        e->state |= int(MLEvent::RIGHT_BUTTON);

      if( (event->xbutton.state & ShiftMask) != 0 )
        e->state |= int(MLEvent::SHIFT);
      if( (event->xbutton.state & ControlMask) != 0 )
        e->state |= int(MLEvent::CONTROL);
      if( (event->xbutton.state & Mod1Mask) != 0 )
        e->state |= int(MLEvent::ALT);
      break;
      }

    case MotionNotify:
      {
      e->type= MLEvent::MOUSE_MOTION;
      e->time= (unsigned long) event->xmotion.time;
      e->position= IPoint( event->xmotion.x, event->xmotion.y );
      e->size= IVector(0,0);
      e->button= 0;
      e->state= 0;
      if( (event->xmotion.state & Button1Mask) != 0 )
        e->state |= int(MLEvent::LEFT_BUTTON);
      if( (event->xmotion.state & Button2Mask) != 0 )
        e->state |= int(MLEvent::MIDDLE_BUTTON);
      if( (event->xmotion.state & Button3Mask) != 0 )
        e->state |= int(MLEvent::RIGHT_BUTTON);

      if( e->state != 0)
        {
        if( (event->xmotion.state & ShiftMask) != 0 )
          e->state |= int(MLEvent::SHIFT);
        if( (event->xmotion.state & ControlMask) != 0 )
          e->state |= int(MLEvent::CONTROL);
        if( (event->xmotion.state & Mod1Mask) != 0 )
          e->state |= int(MLEvent::ALT);
        }
       else
        ok=false;

      MLEventManager::checkForEnterLeaveEvents();
      break;
      }

    case KeyPress:
    case KeyRelease:
      {
      if( ! translate_xkey_event( &event->xkey, e->key ) )
        {
        ok= false;
        break;
        }
      e->type= (event->type==KeyPress) ? MLEvent::KEY_PRESS
                                       : MLEvent::KEY_RELEASE;
      e->time= (unsigned long) event->xkey.time;
      e->position= IPoint( event->xkey.x_root, event->xkey.y_root );
      e->size= IVector(0,0);
      e->button= 0;
      e->state= 0;
      if( (event->xkey.state & Button1Mask) != 0 )
        e->state |= int(MLEvent::LEFT_BUTTON);
      if( (event->xkey.state & Button2Mask) != 0 )
        e->state |= int(MLEvent::MIDDLE_BUTTON);
      if( (event->xkey.state & Button3Mask) != 0 )
        e->state |= int(MLEvent::RIGHT_BUTTON);

      if( (event->xkey.state & ShiftMask) != 0 )
        e->state |= int(MLEvent::SHIFT);
      if( (event->xkey.state & ControlMask) != 0 )
        e->state |= int(MLEvent::CONTROL);
      if( (event->xkey.state & Mod1Mask) != 0 )
        e->state |= int(MLEvent::ALT);
      break;
      }

    default:
      {
      ok= false;
      break;
      }
    }

  if( ok )
    MLEventManager::postEvent(e);
   else
    delete e;
}

/////////////////////////////////////////////////////////////////////////////

// protected:
SystemWindow* XEventManager::findWindow( Window windowID )
{
  if( windowID == Window(0) )
    return 0;

  assert( xgs != 0 );

  SystemWindow* root= xgs->getRootWindow();
  assert( root != 0 );

  return (SystemWindow*) _findWindow( root, windowID );
}

/////////////////////////////////////////////////////////////////////////////

// private
SystemWindow* XEventManager::_findWindow( SystemWindow* sw,
                                          Window windowID )
{
  XWindowDriver* xwd= (XWindowDriver*) sw->getDriver();
  if( xwd->window == windowID )
    return sw;

  IListIterator<SystemWindow> li( MLEventManager::getSystemChildren(sw) );
  while( ! li.eol() )
    {
    SystemWindow* w= li.next();
    if( w == 0 )
      continue;

    SystemWindow* found= _findWindow( w, windowID );
    if( found != 0 )
      return found;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

// Called by any thread for unblocking the main thread
/*
void XEventManager::unblockWaiting()
{
  if( xgs == 0 )
    return;

  Display* display2= xgs->getDisplay2();
  Atom unblock_protocol= xgs->getUnblockProtocol();

  if( display2 == 0 || unblock_protocol == Atom(0) )
    return;

  SystemWindow* sw= xgs->getRootWindow();
  if( sw == 0 )
    return;

  XWindowDriver* wd= (XWindowDriver*) sw->getDriver();
  if( wd == 0 )
    return;
  Window main_windowID= wd->getWindowID();
  if( main_windowID == 0 )
    return;

  XClientMessageEvent client_event;
  client_event.type= ClientMessage;
  client_event.window= main_windowID;
  client_event.message_type= unblock_protocol;
  client_event.format= 32;
  client_event.data.l[0]= unblock_protocol;
  client_event.data.l[1]= CurrentTime;

  XSendEvent( display2, main_windowID, False, 0L, (XEvent*) &client_event );
  XSync(display2,False);
}
*/

/////////////////////////////////////////////////////////////////////////////
