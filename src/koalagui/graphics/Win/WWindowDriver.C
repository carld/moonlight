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
  WWindowDriver.C

  Windows 95/NT System Window

  Stephane Rehel
  July 6 1997

  For the Windows driver, we assume every window has its own context,
  and this context is hGLRC.

*/

#include <stdio.h>

#include "tools/SIList.h"

#include "SystemWindow.h"

#include "WWindowDriver.h"
#include "WGraphicsSystem.h"
#include "WMLCursor.h"

#include "GL/opengl.h"

#include "WinDebug.h"

/////////////////////////////////////////////////////////////////////////////

static const boolean only_one_context= false + 1;
static WGraphicsSystem* wGraphicsSystem= 0;
static HGLRC mainContext= HGLRC(0);

/////////////////////////////////////////////////////////////////////////////

WWindowDriver::WWindowDriver( SystemWindow* _systemWindow ):
  WindowDriver(_systemWindow)
{
  if( wGraphicsSystem == 0 )
    {
    GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

    assert( graphicsSystem != 0 );
    wGraphicsSystem= (WGraphicsSystem*) graphicsSystem;
    }

  hWnd= HWND(0);
  hDC= HDC(0);
  hGLRC= HGLRC(0);
}

/////////////////////////////////////////////////////////////////////////////

WWindowDriver::~WWindowDriver()
{
  destroyContext();

  if( hWnd != HWND(0) && hDC != HDC(0) )
    {
    ReleaseDC( hWnd, hDC );
    hDC= HDC(0);
    }

  if( hWnd != HWND(0) )
    {
    if( ! DestroyWindow(hWnd) )
      windebugf( "~WWindowDriver(): DestroyWindow(%p) failed! (err=%d)\n",
                 hWnd, GetLastError() );

    hWnd= HWND(0);
    }
}

/////////////////////////////////////////////////////////////////////////////

// _parent may be NULL
boolean WWindowDriver::create( SystemWindow* _parent,
                             int x, int y, int width, int height )

{
  assert( wGraphicsSystem != 0 );

  if( hWnd != HWND(0) )
    {
    windebugf( "warning: WWindowDriver::create(): hWnd != 0 \n" );
    return false;
    }

  if( systemWindow == 0 )
    return false;

  HWND parentID= HWND(0);
  if( _parent != 0 )
    parentID= ((WWindowDriver*) (_parent->getDriver()))->hWnd;

  hWnd= wGraphicsSystem->createWindow( parentID,
                                       x, y, width, height,
                                       hasSaveUnder() );
  if( hWnd == HWND(0) )
    {
    windebugf( "WWindowDriver::create(): unable to create window (err=%d)\n",
               GetLastError() );
    return false;
    }

//printf("hWnd=%p pos(%d,%d) size(%d,%d)\n", hWnd, x,y,width,height);

  hDC= GetDC(hWnd); // get main DC
  if( hDC == HDC(0) )
    {
    windebugf( "WWindowDriver::create(): unable to get DC (err=%d)\n",
               GetLastError() );
    return false;
    }

  wGraphicsSystem->initOpenGL(hDC);

  createContext();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void WWindowDriver::createContext()
{
  if( hWnd == HWND(0) || hDC == HDC(0) )
    {
    windebugf( "warning: WWindowDriver::createContext(), hWnd=%p hDC=%p\n",
               hWnd, hDC );
    return;
    }

  if( hGLRC != HGLRC(0) )
    {
    if( wGraphicsSystem->getRootWindow()->getDriver() != this )
      windebugf( "warning: WWindowDriver::createContext(), hGLRC!=0 (hWnd=%p)\n",
                 hWnd );
    return;
    }

  if( only_one_context )
    {
    if( ::mainContext != HGLRC(0) )
      {
      // share main context
      hGLRC= ::mainContext;
      return;
      }
    }

  hGLRC= wglCreateContext(hDC);
  if( ::mainContext == HGLRC(0) )
    ::mainContext= hGLRC;
   else
    {
    if( ! only_one_context )
      wglShareLists( ::mainContext, hGLRC );
    }

  if( hGLRC == HGLRC(0) )
    windebugf( "Unable to create hGLRC context for window %p (err=%d)\n",
               hWnd, GetLastError() );
}

/////////////////////////////////////////////////////////////////////////////

void WWindowDriver::destroyContext()
{
  if( hGLRC == HGLRC(0) || hDC == HDC(0) )
    return;

  if( wglGetCurrentDC() == hDC )
    wglMakeCurrent( hDC, HGLRC(0) );
  if( wglGetCurrentContext() == hGLRC )
    wglMakeCurrent( HDC(0), hGLRC );

  if( ! only_one_context )
    {
    if( ! wglDeleteContext(hGLRC) )
      windebugf( "WWindowDriver::destroyContext(hWnd=%p hGLRC=%p) failed! (err=%d)\n",
                 hWnd, hGLRC, GetLastError() );
    }

  hGLRC= HGLRC(0);
  return;
}

/////////////////////////////////////////////////////////////////////////////

void WWindowDriver::current() // current for opengl-drawing
{
  if( hDC == HDC(0) || hGLRC == HGLRC(0) )
    {
    windebugf( "warning: makeCurrent on hWnd=%p, hDC=%p, hGLRC=%p\n",
               hWnd, hDC, hGLRC );
    return;
    }

  // Sometimes, i lose the window handle of the DC... Seems very strange.
  // Don't know why! So i check it here, and ask for another DC again!
  // /SR July 17 1997
  if( WindowFromDC(hDC) != hWnd )
    {
//windebugf("Warning: WindowFromDC(hDC) != hWnd !!??\n");
/*
fprintf( stderr,"pos(%d %d) size(%d %d)\n",
        systemWindow->getPosition().x(),
        systemWindow->getPosition().y(),
        systemWindow->getSize().x(),
        systemWindow->getSize().y() );
*/
    ReleaseDC(hWnd,hDC);
    hDC= GetDC(hWnd);
    if( hDC == HDC(0) )
      {
      windebugf( "WWindowDriver::current(): unable to get DC (err=%d)\n",
                 GetLastError() );
      return;
      }
    wGraphicsSystem->initOpenGL(hDC);
    }

  if( ! wglMakeCurrent( hDC, hGLRC ) )
    {
    windebugf( "glwMakeCurrent(%p) failed on hWnd %p (err=%d)!\n",
               hGLRC, hWnd, GetLastError() );
    return;
    }

//printf("+hWnd=%p hDC=%p hGLRC=%p\n", WindowFromDC(wglGetCurrentDC()),
//                                     wglGetCurrentDC(),
//                                     wglGetCurrentContext() );

  glDrawBuffer(GL_FRONT);
}

/////////////////////////////////////////////////////////////////////////////

void WWindowDriver::swapBuffers()
{
  if( hDC == HDC(0) )
    {
    windebugf( "SwapBuffers() with hDC=0 !\n");
    return;
    }

//if( WindowFromDC(hDC) != hWnd )
//  printf( "OOHH GOD!! (%p %p)\n",hWnd,WindowFromDC(hDC));

  GdiFlush();
  glFlush();
  if( ! SwapBuffers( hDC ) )
    windebugf( "SwapBuffers(hDC=%p) failed!\n", hDC );
}

/////////////////////////////////////////////////////////////////////////////

void WWindowDriver::map( boolean yes /* = true */ )
{
  if( hWnd == HWND(0) )
    return;

  int nCmdShow= yes ? SW_RESTORE : SW_HIDE;

  ShowWindow( hWnd, nCmdShow );

  GdiFlush();
  glFlush();
}

/////////////////////////////////////////////////////////////////////////////

void WWindowDriver::setPosition( const IPoint& newPosition )
{
  if( hWnd == HWND(0) )
    return;

  RECT rect;
  if( ! GetClientRect( hWnd, &rect ) )
    {
    windebugf( "WWindowDriver::setPosition(): GetClientRect(%p) failed! (err=%d)\n",
               hWnd, GetLastError() );
    return;
    }

  int width = rect.right - rect.left + 1;
  int height= rect.bottom- rect.top  + 1;

  if( ! MoveWindow( hWnd, newPosition.x(), newPosition.y(), width, height,
                    FALSE // don't send WM_PAINT message
                  ) )
    {
    windebugf( "WWindowDriver::setPosition(): MoveWindow(%p) failed! (err=%d)\n",
               hWnd, GetLastError() );
    }
}

/////////////////////////////////////////////////////////////////////////////

void WWindowDriver::setSize( const IVector& newSize )
{
  if( hWnd == HWND(0) )
    return;

  RECT rect;
  if( ! GetClientRect( hWnd, &rect ) )
    {
    windebugf( "WWindowDriver::setSize(): GetClientRect(%p) failed! (err=%d)\n",
               hWnd, GetLastError() );
    return;
    }

  int x= rect.left;
  int y= rect.top;

  if( ! MoveWindow( hWnd, x, y, newSize.x(), newSize.y(),
                    FALSE // don't send WM_PAINT message
                  ) )
    {
    windebugf( "WWindowDriver::setSize(): MoveWindow(%p) failed! (err=%d)\n",
               hWnd, GetLastError() );
    }
}

/////////////////////////////////////////////////////////////////////////////

// static
HGLRC WWindowDriver::getMainContext()
{
  return ::mainContext;
}

/////////////////////////////////////////////////////////////////////////////

void WWindowDriver::_setCursor()
{
  if( cursor == 0 )
    return;

  WMLCursor* wcursor= (WMLCursor*) cursor;
  HCURSOR c= wcursor->get();

  if( c == HCURSOR(0) )
    return;

  SetCursor(c);
}

/////////////////////////////////////////////////////////////////////////////
/*
static void xor_horiz_line( HDC hDC, int x1, int x2, int y )
{
  RECT r;
  r.left= x1;
  r.right= x2 + 1;
  r.top= y;
  r.bottom= y + 1;
  InvertRect( hDC, &r );
}
static void xor_vert_line( HDC hDC, int x, int y1, int y2 )
{
  RECT r;
  r.left= x;
  r.right= x + 1;
  r.top= y1;
  r.bottom= y2 + 1 ;
  InvertRect( hDC, &r );
}
*/
static void xor_rect( HDC hDC, int x1, int y1, int x2, int y2 )
{
  MoveToEx( hDC, x1, y1, 0 );
  LineTo( hDC, x2, y1 );
  LineTo( hDC, x2, y2 );
  LineTo( hDC, x1, y2 );
  LineTo( hDC, x1, y1 );
}
void WWindowDriver::drawXORFrame( int _x1, int _y1, int _x2, int _y2,
                                  boolean undo )
{
  if( hWnd == HWND(0) )
    return;

  static int first= 1;
  static HPEN xor1Pen= HPEN(0);
  static HPEN xor2Pen= HPEN(0);

  if( first )
    {
    COLORREF xor1Pixel, xor2Pixel;
    first= 0;

    switch( wGraphicsSystem->getDepth() )
      {
      case 8:
        {
        xor1Pixel= RGB(0x7f,0x7f,0x7f);
        xor2Pixel= RGB(0xff,0xff,0xff);
        break;
        }
      case 16:
        {
        xor1Pixel= RGB(175,168,175);
        xor2Pixel= RGB(0xff,0xff,0xff);
        break;
        }
      case 24:
      default:
        {
        BYTE g= 170;
        xor1Pixel= RGB(g,g,g);
        xor2Pixel= RGB(0xff,0xff,0xff);
        break;
        }
      }

    xor1Pen= CreatePen( PS_SOLID, 1, xor1Pixel );
    if( xor1Pen == HPEN(0) )
      xor1Pen= GetStockObject(WHITE_PEN);
    xor2Pen= CreatePen( PS_SOLID, 1, xor2Pixel );
    if( xor2Pen == HPEN(0) )
      xor2Pen= GetStockObject(WHITE_PEN);
    }

  int x1= min( _x1, _x2 );
  int x2= max( _x1, _x2 );
  int y1= min( _y1, _y2 );
  int y2= max( _y1, _y2 );

//  XSetForeground( display, gc, undo ? xor2Pixel : xor1Pixel );
//  XDrawRectangle( display, window, gc, x1, y1, x2-x1+1, y2-y1+1 );
//  XSetForeground( display, gc, undo ? xor1Pixel : xor2Pixel );
//  XDrawRectangle( display, window, gc, x1, y1, x2-x1+1, y2-y1+1 );

  HDC hDC= GetDC(hWnd);
  if( hDC == HDC(0) )
    return;

  int rop2= GetROP2(hDC);
  SetROP2(hDC,R2_NOTXORPEN);
  HGDIOBJ prev_obj= SelectObject( hDC, xor1Pen );

/*
xor_horiz_line(hDC,x1,x2,y1);
  xor_horiz_line(hDC,x1,x2,y2);
  xor_vert_line(hDC,x1,y1,y2);
  xor_vert_line(hDC,x2,y1,y2);
*/

  xor_rect(hDC,x1,y1,x2,y2);
  SelectObject( hDC, xor2Pen );
  xor_rect(hDC,x1,y1,x2,y2);

  if( prev_obj != HGDIOBJ(0) )
    SelectObject( hDC, prev_obj );

  SetROP2(hDC,rop2);

  ReleaseDC(hWnd,hDC);
}

/////////////////////////////////////////////////////////////////////////////
