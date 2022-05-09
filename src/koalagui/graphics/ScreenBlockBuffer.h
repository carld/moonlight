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
  ScreenBlockBuffer.h

  Stephane Rehel
  July 20 1997
*/

#ifndef __ScreenBlockBuffer_h
#define __ScreenBlockBuffer_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

class SystemWindow;

/////////////////////////////////////////////////////////////////////////////

class ScreenBlockBuffer
{
protected:
  int x1, y1, x2, y2;
  unsigned char* data;

public:
  ScreenBlockBuffer();
  virtual ~ScreenBlockBuffer();

protected:
  void destroy();

public:
  void get( SystemWindow* sw );

  // (x,y) from left-bottom
  void get( int x, int y, int width, int height );

  void restore();

  boolean put( SystemWindow* sw );

protected:
  boolean putRec( SystemWindow* sw );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ScreenBlockBuffer_h
