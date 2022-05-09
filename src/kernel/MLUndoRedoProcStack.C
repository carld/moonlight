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
  MLUndoRedoProcStack.C

  Stephane Rehel

  March 3 1997

  Revision: May 13 1998
*/

#include "MLUndoRedoProcStack.h"
#include "MLRunnableProc.h"

/////////////////////////////////////////////////////////////////////////////

MLUndoRedoProcStack::MLUndoRedoProcStack()
{}

/////////////////////////////////////////////////////////////////////////////

MLUndoRedoProcStack::~MLUndoRedoProcStack()
{
  clear();
}

/////////////////////////////////////////////////////////////////////////////

// This function is called by the interface AND by the lighting
// process. Let's use a mutex.
void MLUndoRedoProcStack::push( MLRunnableProc* proc )
{
  ::ref(proc); // i want to keep it

  stack.push(proc);
}

/////////////////////////////////////////////////////////////////////////////

// return value is still referenced!
MLRunnableProc* MLUndoRedoProcStack::get()
{
  MLRunnableProc* answer= 0;

  if( ! stack.isEmpty() )
    answer= stack.pop();

  return answer;
}

/////////////////////////////////////////////////////////////////////////////

// return value is still referenced!
MLRunnableProc* MLUndoRedoProcStack::peek()
{
  return stack.isEmpty() ? 0 : stack.peek();
}

/////////////////////////////////////////////////////////////////////////////

void MLUndoRedoProcStack::clear()
{
  while( ! stack.isEmpty() )
    {
    MLRunnableProc* cmd= stack.pop();
    ::unref(cmd);
    }
}

/////////////////////////////////////////////////////////////////////////////
