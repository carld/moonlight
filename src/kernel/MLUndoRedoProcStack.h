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
  MLUndoRedoProcStack.h

  Stephane Rehel

  March 3 1997

  Revision: May 13 1998
*/

#ifndef __MLUndoRedoProcStack_h
#define __MLUndoRedoProcStack_h

#include "tools/boolean.h"
#include "tools/IStack.h"

class MLRunnableProc;

/////////////////////////////////////////////////////////////////////////////

/** keeps a stack of procedures to provide
 *  capabilities to undo/redo them
 */
class MLUndoRedoProcStack
{
protected:
  IStack<MLRunnableProc> stack;

public:
  MLUndoRedoProcStack();

  virtual ~MLUndoRedoProcStack();

  boolean isEmpty() const
    {
    return stack.isEmpty();
    }

  void push( MLRunnableProc* proc );

  /**
   * pops item from stack, return value is still referenced!
   */
  MLRunnableProc* get();

  /**
   * return value is still referenced!
   */
  MLRunnableProc* peek();

  void clear();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLUndoRedoProcStack_h
