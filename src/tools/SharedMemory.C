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
  SharedMemory.C

  Stephane Rehel
  Creation: February 25th, 1996
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "SharedMemory.h"


/////////////////////////////////////////////////////////////////////////////

void SharedMemory::destroy()
{
  if( addr != 0 )
    {
    shmdt( (char*) addr);
    addr= 0;
    }

  id= -1;
}

/////////////////////////////////////////////////////////////////////////////

// return NULL is error
void* SharedMemory::alloc( int size, long mode_flags /* = 0777 */ )
{
  if( size <= 0 )
    size= 1;

  if( id >= 0 )
    destroy();

  id= shmget( IPC_PRIVATE, size, IPC_CREAT | mode_flags );
  if( id < 0 )
    {
    id= -1;
    return 0;
    }

  addr= (void*)shmat( id, 0, 0 );
  if( addr == ((void*)-1) )
    {
    addr= 0;
    destroy();
    return 0;
    }

  return addr;
}

/////////////////////////////////////////////////////////////////////////////

void SharedMemory::removeID()
{
  shmid theID= id;

  destroy();

  if( theID != shmid(-1) )
    shmctl( id, IPC_RMID, 0 );
}

/////////////////////////////////////////////////////////////////////////////

// return NULL if error
void* SharedMemory::attach( shmid _id )
{
  assert( _id >= 0 );

  if( id == _id )
    {
    assert( addr != 0 );
    return addr;
    }
  destroy();

  id= _id;

  addr= (void*)shmat( id, 0, 0 );
  if( addr == ((void*)-1) )
    {
    addr= 0;
    destroy();
    return 0;
    }

  return addr;
}

/////////////////////////////////////////////////////////////////////////////

// this clears id
void SharedMemory::detach()
{
  if( addr != 0 )
    {
    shmdt( (char*) addr );
    addr= 0;
    }
  id= -1;
}

/////////////////////////////////////////////////////////////////////////////

