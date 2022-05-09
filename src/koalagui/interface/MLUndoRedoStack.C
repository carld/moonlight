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
  MLUndoRedoStack.C

  Stephane Rehel

  March 3 1997
*/

#include "MLUndoRedoStack.h"
#include "MLCommand.h"

/////////////////////////////////////////////////////////////////////////////

MLUndoRedoStack::MLUndoRedoStack()
{}

/////////////////////////////////////////////////////////////////////////////

MLUndoRedoStack::~MLUndoRedoStack()
{
  clear();
}

/////////////////////////////////////////////////////////////////////////////

// This function is called by the interface AND by the lighting
// process. Let's use a mutex.
void MLUndoRedoStack::push( MLCommand* command )
{
  if( command == 0 )
    return;

  ::ref(command); // i want to keep it

  stack.push(command);
}

/////////////////////////////////////////////////////////////////////////////

MLCommand* MLUndoRedoStack::get()
{
  MLCommand* answer= 0;

  if( ! stack.isEmpty() )
    answer= stack.pop();

  return answer;
}

/////////////////////////////////////////////////////////////////////////////

MLCommand* MLUndoRedoStack::peek()
{
  return stack.isEmpty() ? 0 : stack.peek();
}

/////////////////////////////////////////////////////////////////////////////

void MLUndoRedoStack::clear()
{
  while( ! stack.isEmpty() )
    {
    MLCommand* cmd= stack.pop();
    if( cmd != 0 )
      ::unref(cmd);
    }
}

/////////////////////////////////////////////////////////////////////////////
