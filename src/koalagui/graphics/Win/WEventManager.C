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
  WEventManager.C

  Must be created after WGraphicsSystem is created

  Stephane Rehel
  July 9 1997
*/

#include "SystemWindow.h"
#include "mlgl.h"

#include "WEventManager.h"
#include "WGraphicsSystem.h"
#include "WWindowDriver.h"
#include "WKey.h"

#include "MLEvent.h"

#include "WinDebug.h"

/////////////////////////////////////////////////////////////////////////////

static WGraphicsSystem* wgs= 0;

/////////////////////////////////////////////////////////////////////////////

WEventManager::WEventManager()
{
  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  assert( graphicsSystem != 0 );
  wgs= (WGraphicsSystem*) graphicsSystem;

  HWND hRoot= wgs->getRootWindowID();

  if( hRoot != HWND(0) )
    {
    UINT outcome= SetTimer( hRoot,
                            UINT(1), // idTimer
                            UINT( MLEventManager::wait_time_out * 1000. + .5 ), // uTimeout
                            TIMERPROC(0) );
    if( outcome == 0 )
      windebugf( "Warning: unable to create timer\n" );
    }
}

/////////////////////////////////////////////////////////////////////////////

WEventManager::~WEventManager()
{}

/////////////////////////////////////////////////////////////////////////////

long FAR PASCAL _export
WEventManager_WndProc( HWND hWnd, UINT message, DWORD wParam, LONG lParam )
{
  switch(message)
    {
    case WM_SIZE:
      {
      if( hWnd != wgs->getRootWindowID() )
        break;
      int sizeType= int(wParam);
      if( sizeType == SIZE_MINIMIZED )
        wgs->iconify(true);
      if( sizeType == SIZE_RESTORED )
        wgs->iconify(false);
      return 0;
      }
/*
Problems with popups: they get activated, and they get the focus
Main window receives the activate msg!
    case WM_ACTIVATE:
      {
      int fActive= LOWORD(wParam);
      BOOL fMinimized= HIWORD(wParam);
      if( !fMinimized && (fActive==WA_ACTIVE || fActive==WA_CLICKACTIVE) )
        {
        if( MLEventManager::getEventManager() == 0 )
          break;
        if( ((WEventManager*)eventManager)->findWindow(hWnd) != 0 )
          break; // this one of my window (popup, etc)

        SystemWindow* root= wgs->getRootWindow();
        root->currentPixel();
        glDrawBuffer(GL_FRONT);
        root->clear(0,0,0);
        glFlush();
        wgs->refreshScreen();
        }
      return 0;
      }
*/
    default:
      break;
    }

  return DefWindowProc( hWnd, message, wParam, lParam );
}

/////////////////////////////////////////////////////////////////////////////

// return false if timed out
boolean WEventManager::wait()
{
  GdiFlush();
  glFlush();

  MSG msg;
  do{
    BOOL outcome= GetMessage( &msg, HWND(0), UINT(0), UINT(0) );
    if( outcome == FALSE )
      return true; // got WM_QUIT msg

    if( outcome == -1 )
      continue;

    handleEvent(&msg);
    }
  while( PeekMessage( &msg, HWND(0), UINT(0), UINT(0), PM_NOREMOVE ) );

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void WEventManager::get()
{
  GdiFlush();
  glFlush();

  MSG msg;
  while( PeekMessage( &msg, HWND(0), UINT(0), UINT(0), PM_NOREMOVE ) )
    {
    BOOL outcome= GetMessage( &msg, HWND(0), UINT(0), UINT(0) );
    if( outcome == FALSE )
      return; // got WM_QUIT msg

    if( outcome != -1 )
      handleEvent(&msg);
    }
}

/////////////////////////////////////////////////////////////////////////////

// protected:
void WEventManager::handleEvent( MSG* msg )
{
  if( msg == 0 )
    return;

  HWND hWnd= msg->hwnd;

  SystemWindow* window= findWindow(hWnd);
  if( window == 0 )
    {
    DispatchMessage(msg);
    return;
    }

  MLEvent* e= new MLEvent;
  e->privateEvent= false;
  e->window= window;

  boolean ok= true;

  switch( msg->message )
    {
    case WM_ERASEBKGND:
      {
      ok= false;
      break;
      }
    case WM_PAINT:
      {
      if( wgs->doRedrawEvents() )
        {
        // validate rectangle
        DefWindowProc( hWnd, msg->message, msg->wParam, msg->lParam );

        ok= false;
        break;
        }

      RECT rect;

      if( ! GetUpdateRect( hWnd, &rect, FALSE ) )
        {
        ok= false;
        break;
        }

      e->type= MLEvent::REDRAW;
      e->time= (unsigned long) msg->time;
//      e->position= IPoint( event->xexpose.x, event->xexpose.y );
//      e->size= IVector( event->xexpose.width, event->xexpose.height );

      e->position= IPoint(0,0);
      e->size= window->getSize();
      e->button= 0;
      e->state= 0;

      // validate rectangle
      DefWindowProc( hWnd, msg->message, msg->wParam, msg->lParam );

      break;
      }

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
      {
      e->type= (msg->message==WM_LBUTTONDOWN ||
                msg->message==WM_MBUTTONDOWN ||
                msg->message==WM_RBUTTONDOWN ) ? MLEvent::MOUSE_PRESS
                                               : MLEvent::MOUSE_RELEASE;
      e->time= (unsigned long) msg->time;
      e->position= IPoint( LOWORD(msg->lParam), HIWORD(msg->lParam) );
      e->size= IVector(0,0);
      e->button= (msg->message==WM_LBUTTONDOWN || msg->message==WM_LBUTTONUP) ? 1 :
                 (msg->message==WM_MBUTTONDOWN || msg->message==WM_MBUTTONUP) ? 2 :
                 3;
      e->state= 0;
      if( (msg->wParam & MK_LBUTTON) != 0 )
        e->state |= int(MLEvent::LEFT_BUTTON);
      if( (msg->wParam & MK_MBUTTON) != 0 )
        e->state |= int(MLEvent::MIDDLE_BUTTON);
      if( (msg->wParam & MK_RBUTTON) != 0 )
        e->state |= int(MLEvent::RIGHT_BUTTON);

      if( (msg->wParam & MK_SHIFT) != 0 )
        e->state |= int(MLEvent::SHIFT);
      if( (msg->wParam & MK_CONTROL) != 0 )
        e->state |= int(MLEvent::CONTROL);
//      if( (event->xbutton.state & Mod1Mask) != 0 )
//        e->state |= int(MLEvent::ALT);

      // For 2 buttons mouse users:
      //  Emulate middle button by pressing SHIFT + CONTROL
      if( (e->state & int(MLEvent::SHIFT)) != 0 &&
          (e->state & int(MLEvent::CONTROL)) != 0 )
        {
        if( (e->state & int(MLEvent::LEFT_BUTTON)) != 0 ||
            (e->state & int(MLEvent::RIGHT_BUTTON)) != 0 )
          e->state= int(MLEvent::MIDDLE_BUTTON);
        e->button= 2;
        }

      break;
      }

    case WM_MOUSEMOVE:
      {
      e->type= MLEvent::MOUSE_MOTION;
      e->time= (unsigned long) msg->time;
      e->position= IPoint( LOWORD(msg->lParam), HIWORD(msg->lParam) );
      e->size= IVector(0,0);
      e->button= 0;
      e->state= 0;
      if( (msg->wParam & MK_LBUTTON) != 0 )
        e->state |= int(MLEvent::LEFT_BUTTON);
      if( (msg->wParam & MK_MBUTTON) != 0 )
        e->state |= int(MLEvent::MIDDLE_BUTTON);
      if( (msg->wParam & MK_RBUTTON) != 0 )
        e->state |= int(MLEvent::RIGHT_BUTTON);

      if( e->state == 0 )
        {
        // simple move of cursor
        ((WWindowDriver*)(window->getDriver()))->_setCursor();
        ok= false;

        MLEventManager::checkForEnterLeaveEvents();
        break;
        }

      if( (msg->wParam & MK_SHIFT) != 0 )
        e->state |= int(MLEvent::SHIFT);
      if( (msg->wParam & MK_CONTROL) != 0 )
        e->state |= int(MLEvent::CONTROL);
//      if( (event->xmotion.state & Mod1Mask) != 0 )
//        e->state |= int(MLEvent::ALT);

      // For 2 buttons mouse users:
      //  Emulate middle button by pressing SHIFT + CONTROL
      if( (e->state & int(MLEvent::SHIFT)) != 0 &&
          (e->state & int(MLEvent::CONTROL)) != 0 )
        {
        if( (e->state & int(MLEvent::LEFT_BUTTON)) != 0 ||
            (e->state & int(MLEvent::RIGHT_BUTTON)) != 0 )
          e->state= int(MLEvent::MIDDLE_BUTTON);
        }

      break;
      }

    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
      {
      if( ! translate_wkey_event( msg, e->key ) )
        {
        ok= false;
        break;
        }
      e->type= (msg->message==WM_KEYDOWN || msg->message==WM_SYSKEYDOWN)
               ? MLEvent::KEY_PRESS
               : MLEvent::KEY_RELEASE;
      e->time= (unsigned long) msg->time;
      e->position= IPoint( msg->pt.x, msg->pt.y );
      e->size= IVector(0,0);
      e->button= 0;
      e->state= 0;
      if( GetKeyState(VK_LBUTTON) < 0 )
        e->state |= int(MLEvent::LEFT_BUTTON);
      if( GetKeyState(VK_MBUTTON) < 0 )
        e->state |= int(MLEvent::MIDDLE_BUTTON);
      if( GetKeyState(VK_RBUTTON) < 0 )
        e->state |= int(MLEvent::RIGHT_BUTTON);

      if( GetKeyState(VK_SHIFT) < 0 )
        e->state |= int(MLEvent::SHIFT);
      if( GetKeyState(VK_CONTROL) < 0 )
        e->state |= int(MLEvent::CONTROL);
//      if( (event->xkey.state & Mod1Mask) != 0 )
//        e->state |= int(MLEvent::ALT);

      break;
      }

    case WM_CREATE:
      {
      ok= false;
      break;
      }

    default:
      {
//      DefWindowProc( hWnd, msg->message, msg->wParam, msg->lParam );

DispatchMessage(msg);

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
SystemWindow* WEventManager::findWindow( HWND hWnd )
{
  if( hWnd == HWND(0) )
    return 0;

  assert( wgs != 0 );

  SystemWindow* root= wgs->getRootWindow();
  assert( root != 0 );

  return (SystemWindow*) _findWindow( root, hWnd );
}

/////////////////////////////////////////////////////////////////////////////

// private
SystemWindow* WEventManager::_findWindow( SystemWindow* sw, HWND hWnd )
{
  WWindowDriver* wwd= (WWindowDriver*) sw->getDriver();
  if( wwd->hWnd == hWnd )
    return sw;

  SIListIterator<SystemWindow> li( MLEventManager::getSystemChildren(sw) );
  while( ! li.eol() )
    {
    SystemWindow* w= li.next();
    if( w == 0 )
      continue;

    SystemWindow* found= _findWindow( w, hWnd );
    if( found != 0 )
      return found;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

// Called by any thread for unblocking the main thread
/*
void WEventManager::unblockWaiting()
{
  if( wgs == 0 )
    return;
  SystemWindow* root= wgs->getRootWindow();
  if( root == 0 )
    return;
  WWindowDriver* wd= (WWindowDriver*) (root->getDriver());
  if( wd == 0 )
    return;
  HWND hWnd= wd->getWindowID();
  if( hWnd == HWND(0) )
    return;

  DWORD main_thread_id= GetWindowThreadProcessId(hWnd,0);
  if( main_thread_id == DWORD(0) )
    {
    windebugf( "unblockWaiting(): unable to get main thread id! (err=%d)\n",
               GetLastError() );
    return;
    }

  static UINT msg= 0;

  if( msg == 0 )
    {
    msg= RegisterWindowMessage( "ml_unblockWaiting_protocol" );
    if( msg == 0 )
      windebugf( "unblockWaiting(): unable to register msg! (err=%d)\n",
                 GetLastError() );
    }

  if( ! PostThreadMessage( main_thread_id,
                           msg,
                           WPARAM(0),
                           LPARAM(0) ) )
    {
    windebugf( "unblockWaiting(): unable to post thread message! (err=%d)\n",
               GetLastError() );
    }
}
*/

/////////////////////////////////////////////////////////////////////////////

