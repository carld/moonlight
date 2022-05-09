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
  MLProcManager.C

  Stephane Rehel
  May 13 1998
*/

#include <stdio.h>

#include "MLProcManager.h"
#include "MLProcQueue.h"
#include "MLUndoRedoProcStack.h"
#include "MLRunnableProc.h"

static MLRunnableProc* groupStart= 0;
static MLRunnableProc* groupEnd= 0;

/////////////////////////////////////////////////////////////////////////////

MLProcManager::MLProcManager( MLAppRoot* _approot )
{
  approot= _approot;

  procQueue= new MLProcQueue(approot);
  undoStack= new MLUndoRedoProcStack();
  redoStack= new MLUndoRedoProcStack();

  if( ::groupStart == 0 )
    ::groupStart= new MLRunnableProc;

  ref(::groupStart);

  if( ::groupEnd == 0 )
    ::groupEnd= new MLRunnableProc;

  ref(::groupEnd);
}

/////////////////////////////////////////////////////////////////////////////

MLProcManager::~MLProcManager()
{
  delete undoStack;
  undoStack= 0;

  delete redoStack;
  redoStack= 0;

  delete procQueue;
  procQueue= 0;

  unref(::groupStart);
  unref(::groupEnd);
}

/////////////////////////////////////////////////////////////////////////////

// clears undo, redo, and procQueue
void MLProcManager::clear()
{
  procQueue->clear();
  undoStack->clear();
  redoStack->clear();
}

/////////////////////////////////////////////////////////////////////////////

// post a procedure without undo
void MLProcManager::post( MLProcedure* body, MLProcArgs* args )
{
  MLRunnableProc* proc= new MLRunnableProc(body,args);
  post(proc);
}

/////////////////////////////////////////////////////////////////////////////

void MLProcManager::post( MLRunnableProc* proc )
{
  if( proc == 0 )
    return;

  procQueue->post(proc);
}

/////////////////////////////////////////////////////////////////////////////

void MLProcManager::groupStart()
{
  MLRunnableProc* last= procQueue->peekLastPosted();

  if( last == ::groupStart )
    return;

  procQueue->post(::groupStart);
}

/////////////////////////////////////////////////////////////////////////////

void MLProcManager::groupEnd()
{
  MLRunnableProc* last= procQueue->peekLastPosted();

  if( last == ::groupEnd )
    return;

  procQueue->post(::groupEnd);
}

/////////////////////////////////////////////////////////////////////////////

void MLProcManager::runOne( MLRunnableProc* proc, boolean doit )
{
  if( proc == 0 )
    return;

  if( proc == ::groupStart || proc == ::groupEnd )
    {
    if( doit )
      undoStack->push(proc);
     else
      redoStack->push(proc);

    return;
    }

  MLNode* result= proc->run();
  unref(result);

  MLRunnableProc* undo_proc= proc->getUndo();
  if( undo_proc != 0 )
    {
    if( doit )
      undoStack->push(undo_proc);
     else
      redoStack->push(undo_proc);
    }

  unref(proc);
}

/////////////////////////////////////////////////////////////////////////////

// return false is no more procedure
boolean MLProcManager::runNextProc()
{
  if( procQueue->isEmpty() )
    return false;

  MLRunnableProc* rp= procQueue->getNext();

  if( rp == 0 )
    return false;

  if( rp == ::groupEnd )
    {
    // this is an error!
#ifndef NDEBUG
    fprintf(stderr,"MLProcManager: i got a proc group end without any start!?\n");
#endif
    unref(rp);
    return runNextProc();
    }

  boolean into_group= (rp == ::groupStart);

  if( ! into_group )
    {
    runOne(rp,true);
    return true;
    }

  for(;;)
    {
    runOne(rp,true);

    if( rp == ::groupEnd )
      break; // fine

    rp= procQueue->getNext();
    if( rp == 0 )
      {
      // diabolo, it is empty
#ifndef NDEBUG
      fprintf(stderr,"MLProcManager: a group end is missing!?\n");
#endif
      ref(::groupEnd);
      rp= ::groupEnd;
      continue;
      }

    if( rp == ::groupStart )
      {
      // this is an error!
#ifndef NDEBUG
      fprintf(stderr,"MLProcManager: i got a proc group start into a group!?\n");
#endif
      continue;
      }
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLProcManager::clearUndos()
{
  undoStack->clear();
}

/////////////////////////////////////////////////////////////////////////////

// returns false if no more undo procedures available
boolean MLProcManager::undo()
{
  if( undoStack->isEmpty() )
    return false;

  MLRunnableProc* rp= undoStack->get();

  if( rp == 0 )
    return false;

  if( rp == ::groupStart )
    {
    // this is an error!
#ifndef NDEBUG
    fprintf(stderr,"MLProcManager::undo(): i got a proc group start without any start!?\n");
#endif
    unref(rp);
    return undo();
    }

  boolean into_group= (rp == ::groupEnd);

  if( ! into_group )
    {
    runOne(rp,false);
    return true;
    }

  for(;;)
    {
    runOne(rp,false);

    if( rp == ::groupStart )
      break; // fine

    rp= undoStack->get();
    if( rp == 0 )
      {
      // diabolo, it is empty
#ifndef NDEBUG
      fprintf(stderr,"MLProcManager::undo(): a group start is missing!?\n");
#endif
      ref(::groupStart);
      rp= ::groupStart;
      continue;
      }

    if( rp == ::groupEnd )
      {
      // this is an error!
#ifndef NDEBUG
      fprintf(stderr,"MLProcManager::undo(): i got a proc group end into a group!?\n");
#endif
      continue;
      }
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// returns false if no more redo procedures available
boolean MLProcManager::redo()
{
  if( redoStack->isEmpty() )
    return false;

  MLRunnableProc* rp= redoStack->get();

  if( rp == 0 )
    return false;

  if( rp == ::groupEnd )
    {
    // this is an error!
#ifndef NDEBUG
    fprintf(stderr,"MLProcManager::redo(): i got a proc group end without any start!?\n");
#endif
    unref(rp);
    return redo();
    }

  boolean into_group= (rp == ::groupStart);

  if( ! into_group )
    {
    runOne(rp,true);
    return true;
    }

  for(;;)
    {
    runOne(rp,true);

    if( rp == ::groupEnd )
      break; // fine

    rp= redoStack->get();
    if( rp == 0 )
      {
      // diabolo, it is empty
#ifndef NDEBUG
      fprintf(stderr,"MLProcManager::redo(): a group end is missing!?\n");
#endif
      ref(::groupEnd);
      rp= ::groupEnd;
      continue;
      }

    if( rp == ::groupStart )
      {
      // this is an error!
#ifndef NDEBUG
      fprintf(stderr,"MLProcManager::redo(): i got a proc group start into a group!?\n");
#endif
      continue;
      }
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
