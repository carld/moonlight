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
  XMLGLPixmap.h

  Stephane Rehel
  July 22 1996
*/

#ifndef __XMLGLPixmap_h
#define __XMLGLPixmap_h

#ifndef __MLGLPixmap_h
#include "MLGLPixmap.h"
#endif

#include <X11/Xlib.h>

#include "GL/glx.h"

/////////////////////////////////////////////////////////////////////////////

class XMLGLPixmap: public MLGLPixmap
{
protected:
  Pixmap pm;
  GLXPixmap glxpm;

public:
  XMLGLPixmap();
  virtual ~XMLGLPixmap();

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

#endif // ifndef __XMLGLPixmap_h
