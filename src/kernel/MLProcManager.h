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
  MLProcManager.h

  Stephane Rehel
  May 13 1998
*/

#ifndef __MLProcManager_h
#define __MLProcManager_h

#include "tools/boolean.h"

class MLAppRoot;
class MLUndoRedoProcStack;
class MLProcQueue;
class MLProcedure;
class MLProcArgs;
class MLRunnableProc;

/////////////////////////////////////////////////////////////////////////////

class MLProcManager
{
protected:
  MLAppRoot* approot;

  MLUndoRedoProcStack* undoStack;
  MLUndoRedoProcStack* redoStack;

  MLProcQueue* procQueue;

public:
  MLProcManager( MLAppRoot* _approot );

  virtual ~MLProcManager();

  // clears undo, redo, and procQueue
  void clear();

  // post a procedure without undo
  void post( MLProcedure* body, MLProcArgs* args );

  void post( MLRunnableProc* proc );

  void groupStart();
  void groupEnd();

protected:
  void runOne( MLRunnableProc* proc, boolean doit );

public:
  // return false is no more procedure
  boolean runNextProc();

  void clearUndos();

  // returns false if no more undo procedures available
  boolean undo();

  // returns false if no more redo procedures available
  boolean redo();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLProcManager_h
