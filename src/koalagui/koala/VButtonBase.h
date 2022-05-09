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
   VButtonBase.h

   Abstract Class

   Creation: SR, July 28th, 1995
   Revisions:
     + SR July 5 1996
*/

#ifndef __VButtonBase_h
#define __VButtonBase_h

#ifndef __VWidget_h
#include "VWidget.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class VButtonBase: public VWidget
{
protected:
  int width, height;

  enum STATE
    {
    RELEASED= 1, PRESSED
    };
  STATE state;

public:
  VButtonBase( VWindow* _parent );
  virtual ~VButtonBase();

  virtual void create( int x, int y, int _width, int _height );
  virtual void create( const IPoint& p1, const IPoint& p2 );

  int getWidth() const
    {
    return width;
    }
  int getHeight() const
    {
    return height;
    }

  // from VPixmapWindow
  virtual void setSize( const IVector& newSize );

  void press();
  void unpress();

protected:
  virtual void drawButton() = 0;
  void _drawButtonBase();

public:
  virtual void drawWidget();

  virtual boolean handleEvent( MLEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VButtonBase_h

