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
  MLCanvasAction.h

  Created by an engine, associated with a canvas title, drawn and
  clickable on a canvas title

  Stephane Rehel

  November 27 1996
*/

#ifndef __MLCanvasAction_h
#define __MLCanvasAction_h

#ifndef __MLAction_h
#include "MLAction.h"
#endif

#ifndef __MLEventHandler_h
#include "graphics/MLEventHandler.h"
#endif

class MLCanvas;
class SoftWindow;

/////////////////////////////////////////////////////////////////////////////

class MLCanvasAction: public MLAction, public MLEventHandler
{
  friend class MLCanvas;
  friend class MLCanvasActionsPool;
protected:
  MLCanvas* canvas;
  SoftWindow* window;
  int width;
  int left;
  int x;
  boolean pressed;
  boolean highlight_when_cursor_enter;
  boolean cursor_highlighted;

public:
  MLCanvasAction( MLCanvas* _canvas );

  virtual ~MLCanvasAction();

  int getWidth() const
    {
    return width;
    }

  void map( boolean yes = true );

  virtual void destroyWindow();

  virtual void createWindow() = 0;

  virtual void draw() = 0;

protected:
  virtual boolean eventHandler( MLWindow*, MLEvent& );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLCanvasAction_h
