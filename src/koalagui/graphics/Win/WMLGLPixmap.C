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
  WMLGLPixmap.C

  Stephane Rehel
  July 6 1997
*/

#include <stdio.h>
#include <string.h>

#include "GL/opengl.h"

#include "MLWindow.h"
#include "SystemWindow.h"
#include "SoftWindow.h"
#include "GraphicsSystem.h"

#include "WGraphicsSystem.h"
#include "WWindowDriver.h"
#include "WMLGLPixmap.h"

#include "WinDebug.h"

/////////////////////////////////////////////////////////////////////////////

static WGraphicsSystem* wgs= 0;
static HGLRC mainPixmapContext= HGLRC(0);
static int initialize_lists= 1;

/////////////////////////////////////////////////////////////////////////////

WMLGLPixmap::WMLGLPixmap()
{
  if( wgs == 0 )
    {
    GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

    wgs= (WGraphicsSystem*) graphicsSystem;
    }

  hDC= HDC(0);
  hGLRC= HGLRC(0);
  hBitmap= HBITMAP(0);
  pvBits= 0;
}

/////////////////////////////////////////////////////////////////////////////

WMLGLPixmap::~WMLGLPixmap()
{
//printf( "***WMLGLPixmap!!\n");
  if( wglGetCurrentDC() == hDC && wglGetCurrentContext() == hGLRC )
    wglMakeCurrent( HDC(0), HGLRC(0) );

  if( hGLRC != HGLRC(0) )
    {
//    if( ! wglDeleteContext(hGLRC) )
//      windebugf( "~MLGLPixmap(): wglDeleteContext() failed (err=%d)\n",
//                 GetLastError() );
    hGLRC= HGLRC(0);
    }

  if( hDC != HDC(0) )
    {
    if( ! DeleteDC(hDC) )
      windebugf( "~MLGLPixmap(): DeleteDC() failed (err=%d)\n",
                 GetLastError() );
    hDC= HDC(0);
    }

  if( hBitmap != HBITMAP(0) )
    {
    DeleteObject( HGDIOBJ(hBitmap) );
    hBitmap= HBITMAP(0);
    }

/*
if( glxpm != GLXPixmap(0) )
    {
    glXDestroyGLXPixmap( xgs->getDisplay(), glxpm );
    glxpm= GLXPixmap(0);
    }

  if( pm != Pixmap(0) )
    {
    XFreePixmap(xgs->getDisplay(),pm);
    pm= Pixmap(0);
    }
*/
}

/////////////////////////////////////////////////////////////////////////////

void WMLGLPixmap::create( int width, int height )
{
  if( hBitmap != HBITMAP(0) )
    {
    windebugf( "warning: WMLGLPixmap::create(): hBitmap!=0\n" );
    return;
    }

  width= max( 1, width );
  height= max( 1, height );
  MLGLPixmap::size= IVector(width,height);

//  Display* display= xgs->getDisplay();
  WWindowDriver* wd= (WWindowDriver*) wgs->getRootWindow()->getDriver();
  if( hDC == HDC(0) )
    {
    HDC rhDC= wd->getDC(); //GetDC( wd->getWindowID() );
    if( rhDC == HDC(0) )
      {
      windebugf( "WMLGLPixmap: no hDC for root?! (err=%d)\n",
               GetLastError() );
      return; // failed
      }
    hDC= CreateCompatibleDC(rhDC);
    if( hDC == HDC(0) )
      {
      windebugf( "WMLGLPixmap: no compatible hDC with root?! (err=%d)\n",
                       GetLastError() );
      return; // failed
      }
    }

/*
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight= -height; // top-down bitmap
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = GetDeviceCaps(hDC, BITSPIXEL);
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biSizeImage = 0;
  bmi.bmiHeader.biXPelsPerMeter = 0;
  bmi.bmiHeader.biYPelsPerMeter = 0;
  bmi.bmiHeader.biClrUsed = 0;
  bmi.bmiHeader.biClrImportant = 0;

  UINT fwUsage = (bmi.bmiHeader.biBitCount <= 8) ? DIB_PAL_COLORS : DIB_RGB_COLORS;

//  int scanWidth = (((width * nBypp)+ 3) & ~3);

  hBitmap= CreateDIBitmap( hDC,
                           &(bmi.bmiHeader),
                           DWORD(0), // fdwInit
                           0, // CONST VOID* lpbInit
                           &bmi,
                           fwUsage );

//hBitmap= CreateCompatibleBitmap( hDC, width, height );
*/

  memset( (void*) &(bmi.bmiHeader), 0, sizeof(BITMAPINFOHEADER) );
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight= height;
  bmi.bmiHeader.biPlanes= 1;

  bmi.bmiHeader.biBitCount= GetDeviceCaps( hDC, BITSPIXEL );
  bmi.bmiHeader.biCompression= BI_RGB;

  pvBits= 0;
  hBitmap= CreateDIBSection( hDC,
                             (BITMAPINFO*) &(bmi.bmiHeader),
                             DIB_PAL_COLORS,
                             (VOID**) &pvBits,
                             HANDLE(0), // hSection
                             DWORD(0) // dwOffset
                           );

  if( hBitmap == HBITMAP(0) )
    {
    windebugf( "warning: unable to create DIBitmap %dx%d (err=%d)\n",
               width, height, GetLastError() );
    return;
    }

  if( SelectObject( hDC, HGDIOBJ(hBitmap) ) == HGDIOBJ(0) )
    windebugf( "SelectObject(hBitmap) failed! (err=%d)\n",
               GetLastError() );

  if( ! wgs->initDIBOpenGL(hDC) )
    return;

  if( ::mainPixmapContext == HGLRC(0) )
    {
    ::mainPixmapContext= wglCreateContext(hDC);

    if( ::mainPixmapContext == HGLRC(0) )
      {
      windebugf( "WMLGLPixmap: wglCreateContext failed! (err=%d)\n",
                 GetLastError() );
      return; // failed
      }
/*
    if( ! wglShareLists( hGLRC, WWindowDriver::getMainContext() ) )
      {
      windebugf( "WMLGLPixmap::create(): wglShareLists() failed! (err=%d)\n",
                 GetLastError() );
      }
*/
    }

  hGLRC= ::mainPixmapContext;
}

/////////////////////////////////////////////////////////////////////////////

void WMLGLPixmap::current()
{
  if( hDC == HDC(0) || hGLRC == HGLRC(0) )
    {
    windebugf( "warning: WMLGLPixmap::current() with hDC=%p hGLRC=%d\n",
               hDC, int(hGLRC) );
    return;
    }

  if( ! wglMakeCurrent( hDC, hGLRC ) )
    windebugf( "MLGLPixmap::glwMakeCurrent() failed (err=%d)\n",
               GetLastError() );

  if( ::initialize_lists )
    {
    ::initialize_lists= 0;
    wgs->initDefaultLists();
    }

  glViewport( 0, 0, size.x(), size.y() );
  glDrawBuffer(GL_FRONT);
}

/////////////////////////////////////////////////////////////////////////////

void WMLGLPixmap::print( MLWindow* dest,
                      int src_x, int src_y,
                      int width, int height,
                      int dest_x, int dest_y )
{
  if( dest == 0 || hDC == HDC(0) || hGLRC == HGLRC(0) )
    return;

  SystemWindow* s_dest;
  if( dest->isSystemWindow() )
    s_dest= (SystemWindow*) dest;
   else
    {
    s_dest= ((SoftWindow*) dest)->getSystemParent();
    const IPoint& absPosition= ((SoftWindow*) dest)->getAbsPosition();
    dest_x+= absPosition.x();
    dest_y+= absPosition.y();
    }
/*
  WWindowDriver* wd= (WWindowDriver*) (s_dest->getDriver());
  HDC destDC= wd->getDC();
  if( ! BitBlt( destDC, dest_x, dest_y, width, height,
                hDC, src_x, src_y, SRCCOPY ) )
    {
    windebugf( "MLGLPixmap::BitBlt(window) failed! (err=%d)\n",
               GetLastError() );
    return;
    }
*/

  WWindowDriver* wd= (WWindowDriver*) (s_dest->getDriver());
  HWND hWnd= wd->getWindowID();
  HDC destDC= GetDC(hWnd);

  if( ! BitBlt( destDC, dest_x, dest_y, width, height,
                hDC, src_x, src_y, SRCCOPY ) )
    {
    windebugf( "MLGLPixmap::BitBlt(window) failed! (err=%d)\n",
               GetLastError() );
    return;
    }

  ReleaseDC(hWnd,hDC);
}

/////////////////////////////////////////////////////////////////////////////

void WMLGLPixmap::print( MLGLPixmap* _dest,
                      int src_x, int src_y,
                      int width, int height,
                      int dest_x, int dest_y )
{
  if( _dest == 0 || hDC == HDC(0) || hGLRC == HGLRC(0) )
    return;

  WMLGLPixmap* dest= (WMLGLPixmap*) _dest;

  if( dest->hBitmap == HBITMAP(0) )
    return;

  if( ! BitBlt( dest->hDC, dest_x, dest_y, width, height,
                hDC, src_x, src_y, SRCCOPY ) )
    {
    windebugf( "MLGLPixmap::BitBlt(pixmap) failed! (err=%d)\n",
               GetLastError() );
    return;
    }
}

/////////////////////////////////////////////////////////////////////////////

