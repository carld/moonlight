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
  MLStatusBars.h

  Stephane Rehel

  December 8 1996
*/

#ifndef __MLStatusBars_h
#define __MLStatusBars_h

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

#ifndef __MLEventHandler_h
#include "graphics/MLEventHandler.h"
#endif

#ifndef __MLKeyboardHandler_h
#include "graphics/MLKeyboardHandler.h"
#endif

class SystemWindow;
class SoftWindow;
class MLStatusLine;
class MLString;

/////////////////////////////////////////////////////////////////////////////

class MLStatusBars: public MLEventHandler, public MLKeyboardHandler
{
  friend class Interface;

protected:
  SoftWindow* window;

  int nLines;
  MLStatusLine** lines;

public:
  MLStatusBars();

  virtual ~MLStatusBars();

  void create( SystemWindow* _root,
               const IPoint& position, const IVector& size );
  void map( boolean yes = true );
  void postRedraw();

  MLStatusLine* getLine( int i ) const
    {
    return (i>=0 && i<nLines) ? lines[i] : 0;
    }

private:
  void draw();

protected:
  virtual boolean eventHandler( MLWindow*, MLEvent& );

  // sent by Interface::handleMLKeyboard()
  // return false if the key event must be handle by the interface
  virtual boolean handleMLKeyboard( MLEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLStatusBars_h
