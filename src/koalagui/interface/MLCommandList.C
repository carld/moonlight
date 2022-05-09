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
  MLCommandList.C

  Stephane Rehel

  November 29 1996
*/

#include "tools/thread/MLThread.h"

#include "MLCommandList.h"

#include "MLCommand.h"

/////////////////////////////////////////////////////////////////////////////

MLCommandList::MLCommandList()
{
  mutex= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLCommandList::~MLCommandList()
{
  if( mutex != 0 )
    {
    mutex->unlock(); // safe
    delete mutex;
    mutex= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLCommandList::setMTSafe()
{
  if( mutex == 0 )
    mutex= new MLMutex;
}

/////////////////////////////////////////////////////////////////////////////

// This function is called by the interface AND by the lighting
// process. Let's use a mutex.
void MLCommandList::post( MLCommand* command )
{
  if( command == 0 )
    return;

  if( mutex != 0 )
    mutex->lock();

  command->type |= command->base;
  ::ref(command); // i want to keep it

  list.append(command);

  if( mutex != 0 )
    mutex->unlock();
}

/////////////////////////////////////////////////////////////////////////////

MLCommand* MLCommandList::get()
{
  if( mutex != 0 )
    mutex->lock();

  MLCommand* answer= 0;

  if( ! list.empty() )
    {
    answer= list.getFirst();
    list.removeFirst();
    }

  if( mutex != 0 )
    mutex->unlock();

  return answer;
}

/////////////////////////////////////////////////////////////////////////////

MLCommand* MLCommandList::peek()
{
  if( mutex != 0 )
    mutex->lock();

  MLCommand* answer= list.empty() ? 0 : list.getFirst();

  if( mutex != 0 )
    mutex->unlock();

  return answer;
}

/////////////////////////////////////////////////////////////////////////////

void MLCommandList::clear()
{
  if( mutex != 0 )
    mutex->lock();

  while( ! list.empty() )
    {
    MLCommand* cmd= list.getFirst();
    list.removeFirst();
    if( cmd != 0 )
      ::unref(cmd);
    }

  if( mutex != 0 )
    mutex->unlock();
}

/////////////////////////////////////////////////////////////////////////////
