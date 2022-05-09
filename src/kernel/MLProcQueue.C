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
  MLProcQueue.C

  Stephane Rehel
  April 7 1998
*/

#include "MLProcQueue.h"
#include "MLRunnableProc.h"

/////////////////////////////////////////////////////////////////////////////

MLProcQueue::MLProcQueue( MLAppRoot* _approot )
{
  approot= _approot;
}

/////////////////////////////////////////////////////////////////////////////

MLProcQueue::~MLProcQueue()
{
  clear();
}

/////////////////////////////////////////////////////////////////////////////

void MLProcQueue::clear()
{
  while( ! queue.empty() )
    {
    MLRunnableProc* rp= queue.getFirst();
    unref(rp);
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLProcQueue::post( MLProcedure* body, MLProcArgs* args )
{
  if( body == 0 )
    return;

  MLRunnableProc* rp= new MLRunnableProc(body,args);
  ref(rp);

  post(rp);
}

/////////////////////////////////////////////////////////////////////////////

void MLProcQueue::post( MLRunnableProc* proc )
{
  if( proc == 0 )
    return;

  queue.append(proc);
}

/////////////////////////////////////////////////////////////////////////////

MLRunnableProc* MLProcQueue::peekLastPosted()
{
  return queue.empty() ? 0 : queue.getLast();
}

/////////////////////////////////////////////////////////////////////////////

// return 0 if the queue is empty
// returned value is referenced! unref it after use.
MLRunnableProc* MLProcQueue::getNext()
{
  if( queue.empty() )
    return 0;

  return queue.getFirst();
}

/////////////////////////////////////////////////////////////////////////////

// return false if the queue is empty
boolean MLProcQueue::runOne()
{
  MLRunnableProc* rp= getNext();

  if( rp == 0 )
    return false;

  MLNode* result= rp->run();

  unref(result);
  unref(rp);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return false if the queue is empty
boolean MLProcQueue::runAll()
{
  boolean e= true;

  while( runOne() )
    e= false;

  return e;
}

/////////////////////////////////////////////////////////////////////////////
