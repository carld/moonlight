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
  MLStatusLine.h

  Stephane Rehel

  December 8 1996
*/

#ifndef __MLStatusLine_h
#define __MLStatusLine_h

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

#ifndef __MLEventHandler_h
#include "graphics/MLEventHandler.h"
#endif

#ifndef __MLKeyboardHandler_h
#include "graphics/MLKeyboardHandler.h"
#endif

class SoftWindow;
class MLFont;
class MLStatusLineItem;

/////////////////////////////////////////////////////////////////////////////

class MLStatusLine: public MLEventHandler, public MLKeyboardHandler
{
  friend class MLStatusBars;

protected:
  SoftWindow* window;
  IPoint position;
  int size_x;

  int nItems;
  MLStatusLineItem** items;

public:
  static MLFont* getFont();
  static int getHeight();
  static int getWidthFromChars( int n_chars );
  static IPoint getTextPosition();

  MLStatusLine( SoftWindow* root, const IPoint& _position, int _size_x );
  virtual ~MLStatusLine();

  void postRedraw();

  SoftWindow* getWindow() const
    {
    return window;
    }

  void addItem( MLStatusLineItem* item );

  void finalize();

protected:
  virtual boolean eventHandler( MLWindow*, MLEvent& );

  // sent by MLStatusBars::handleMLKeyboard()
  // return false if the key event must be handle by the interface
  virtual boolean handleMLKeyboard( MLEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLStatusLine_h
