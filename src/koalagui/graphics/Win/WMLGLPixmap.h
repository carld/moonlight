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
  WMLGLPixmap.h

  Stephane Rehel
  July 6 1997
*/

#ifndef __WMLGLPixmap_h
#define __WMLGLPixmap_h

#ifndef __MLGLPixmap_h
#include "MLGLPixmap.h"
#endif

#include "mlwindows.h"

/////////////////////////////////////////////////////////////////////////////

class WMLGLPixmap: public MLGLPixmap
{
protected:
  HDC hDC;
  HGLRC hGLRC;
  BITMAPINFO bmi;
  HBITMAP hBitmap;
  VOID* pvBits;

//  Pixmap pm;
//  GLXPixmap glxpm;

public:
  WMLGLPixmap();
  virtual ~WMLGLPixmap();

  void create( int width, int height );

  void current();

  void print( MLWindow* dest,
              int src_x, int src_y,
              int width, int height,
              int dest_x, int dest_y );

  void print( MLGLPixmap* _dest,
              int src_x, int src_y,
              int width, int height,
              int dest_x, int dest_y );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __WMLGLPixmap_h
