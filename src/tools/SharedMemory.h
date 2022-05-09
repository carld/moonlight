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
  SharedMemory.h

  Stephane Rehel
  Creation: February 25th, 1996
*/

#ifndef __SharedMemory_h
#define __SharedMemory_h

#include <assert.h>

typedef int shmid;

/////////////////////////////////////////////////////////////////////////////

class SharedMemory
{
private:
  shmid id;
  void* addr;

public:
  SharedMemory()
    {
    id= 0;
    addr= 0;
    }
  virtual ~SharedMemory()
    {
    destroy();
    }

  void destroy();

  // return NULL is error
  void* alloc( int size, long mode_flags = 0777 );

  // return NULL if error
  void* attach( shmid id );

  // this clears id
  void detach();

  // terminate operations with this shared memory segment
  void removeID();

  shmid getID() const
    {
    assert( id >= 0 );
    return id;
    }

  void* getAddr() const
    {
    assert( addr != 0 );
    return addr;
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __SharedMemory_h

