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
  MLCanvasActionsPool.h

  Stephane Rehel
  December 1 1996
*/

#ifndef __MLCanvasActionsPool_h
#define __MLCanvasActionsPool_h

class MLCanvas;
class MLCanvasAction;
class MLAction;

/////////////////////////////////////////////////////////////////////////////

class MLCanvasActionsPool
{
protected:
  MLCanvas* canvas;

  MLCanvasAction** actions;
  int nActions;

  boolean _mapped;

public:
  MLCanvasActionsPool( MLCanvas* _canvas );
  virtual ~MLCanvasActionsPool();

  boolean mapped() const
    {
    return _mapped;
    }

  void map();
  void unmap();

  void addLeftAction( MLCanvasAction* action );
  void addRightAction( MLCanvasAction* action );

protected:
  void addAction( MLCanvasAction* action );

public:
  void removeAction( MLCanvasAction* action );

  void destroyActionsWindows();
  void reconfigure();
  void draw();

  boolean find( MLAction* action ) const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLCanvasActionsPool_h

