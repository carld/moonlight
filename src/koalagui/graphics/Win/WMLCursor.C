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
  WMLCursor.C

  Stephane Rehel
  July 23 1997
*/

#include "WMLCursor.h"
#include "WGraphicsSystem.h"

#include "WinDebug.h"

/////////////////////////////////////////////////////////////////////////////

WMLCursor::WMLCursor()
{
  cursor= HCURSOR(0);
}

/////////////////////////////////////////////////////////////////////////////

WMLCursor::~WMLCursor()
{
  destroy();
}

/////////////////////////////////////////////////////////////////////////////

void WMLCursor::destroy()
{
  if( cursor != HCURSOR(0) )
    {
    DestroyCursor(cursor);
    cursor= HCURSOR(0);
    }
}

/////////////////////////////////////////////////////////////////////////////

void WMLCursor::build()
{
  destroy();
  if( ANDPlane == 0 || XORPlane == 0 )
    return;

  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();
  if( graphicsSystem == 0 )
    return;

  HINSTANCE hInstance= ((WGraphicsSystem*) graphicsSystem)->getInstance();
  cursor= CreateCursor( hInstance,
                        xHotSpot, yHotSpot,
                        width, height,
                        ANDPlane, XORPlane );

  if( cursor == HCURSOR(0) )
    windebugf( "failed creating %dx%d cursor (err=%d)\n",
               width, height, GetLastError() );
}

/*
#include "WWindowDriver.h"
void WMLCursor::build()
{
  destroy();
  if( ANDPlane == 0 || XORPlane == 0 )
    return;

  HDC hDC0= GetDC(GetTopWindow(0));
  HBITMAP hbmMask= CreateCompatibleBitmap( hDC0, width, height * 2 );
  HBITMAP hbmColor= CreateCompatibleBitmap( hDC0, width, height );

  HDC hDC= CreateCompatibleDC(hDC0);

  SelectObject( hDC, hbmMask );
  int i, j;
  for( i= 0; i < width; ++i )
    for( j= 0; j < height; ++j )
      {
      int index= (i / 8) + j * line_size;
      unsigned char bit= 1 << (7 - (i & 7));

      int set= (ANDPlane [ index ] & bit) != 0;
      if( set )
        SelectObject( hDC, GetStockObject(WHITE_PEN) );
       else
        SelectObject( hDC, GetStockObject(BLACK_PEN) );
      MoveToEx( hDC, i, j, 0 );
      LineTo( hDC, i, j );

      set= (XORPlane [ index ] & bit) != 0;
      if( set )
        SelectObject( hDC, GetStockObject(WHITE_PEN) );
       else
        SelectObject( hDC, GetStockObject(BLACK_PEN) );
      MoveToEx( hDC, i, j + height, 0 );
      LineTo( hDC, i, j + height );
      }

  DeleteDC(hDC);
  ReleaseDC( GetTopWindow(0), hDC0 );

  ICONINFO ii;
  ii.fIcon= FALSE;
  ii.xHotspot= xHotSpot;
  ii.yHotspot= yHotSpot;
  ii.hbmMask= hbmMask;
  ii.hbmColor= HBITMAP(0);

  cursor= CreateIconIndirect( &ii );

  if( cursor == HCURSOR(0) )
    windebugf( "failed creating %dx%d cursor (err=%d)\n",
               width, height, GetLastError() );
}
*/

/////////////////////////////////////////////////////////////////////////////

void WMLCursor::getDefaultSize( int* width, int* height ) const
{
  if( width != 0 )
    *width = GetSystemMetrics(SM_CXICON);
  if( height != 0 )
    *height= GetSystemMetrics(SM_CYICON);
}

/////////////////////////////////////////////////////////////////////////////

