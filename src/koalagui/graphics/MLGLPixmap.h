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
  MLGLPixmap.h

  Stephane Rehel
  July 22 1996
*/

#ifndef __MLGLPixmap_h
#define __MLGLPixmap_h

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

class MLWindow;

/////////////////////////////////////////////////////////////////////////////

class MLGLPixmap
{
protected:
  IVector size;

public:
  MLGLPixmap();
  virtual ~MLGLPixmap();

  virtual void create( int width, int height ) = 0;

  const IVector& getSize() const
    {
    return size;
    }

  virtual void current() = 0;
  void pixelMode();

  void currentPixel();

  virtual void print( MLWindow* dest,
                      int src_x, int src_y,
                      int width, int height,
                      int dest_x, int dest_y ) = 0;

  virtual void print( MLGLPixmap* dest,
                      int src_x, int src_y,
                      int width, int height,
                      int dest_x, int dest_y ) = 0;

  void print( MLWindow* dest, int dest_x, int dest_y );
  void print( MLGLPixmap* dest, int dest_x, int dest_y );

  static MLGLPixmap* newMLGLPixmap();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLGLPixmap_h
