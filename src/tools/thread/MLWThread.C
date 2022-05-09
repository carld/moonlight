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
  MLWThread.C

  Stephane Rehel
  July 23 1997
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "MLThread.h"

#include <windows.h>

struct ThreadStruct
  {
  HANDLE handle;
  DWORD id;

  ThreadStruct()
    {
    handle= 0;
    id= 0;
    }
  };

struct MutexStruct
  {
  HANDLE handle;
  char* name;

  MutexStruct()
    {
    handle= 0;
    name= 0;
    }
  };

struct EventStruct
  {
  HANDLE handle;
  char* name;

  EventStruct()
    {
    handle= 0;
    name= 0;
    }
  };

/////////////////////////////////////////////////////////////////////////////

void MLRunnable::exitThread()
{
  ExitThread(0);
}

/////////////////////////////////////////////////////////////////////////////

static void* ml_thread_create_data= 0;

static unsigned int win_thread_ML_start( void* _runnable )
{
  MLRunnable* runnable= (MLRunnable*) _runnable;

  runnable->runThread(::ml_thread_create_data);

  return 0;
}

boolean MLThread::create( MLRunnable* runnable, void* data /* = 0 */ )
{
  ThreadStruct* ts= new ThreadStruct;
  the_thread= (void*) ts;

  ::ml_thread_create_data= data;
  ts->handle= (void*) CreateThread( LPSECURITY_ATTRIBUTES(0), // threadAttributes
                                    DWORD(0), // stackSize
                                    win_thread_ML_start, // startAddress
                                    LPVOID(runnable), // parameter
                                    DWORD(0), // creationFlags
                                    &(ts->id) // threadId
                                  );

  if( ts->handle == 0 )
    fprintf( stderr, "thread: unable to create (err=%d)\n", GetLastError() );

  return ts->handle != 0;
}

/////////////////////////////////////////////////////////////////////////////

// static
boolean MLThread::available()
{
  return true;
}

/////////////////////////////////////////////////////////////////////////////

MLMutex::MLMutex()
{
  MutexStruct* ms= new MutexStruct;
  the_mutex= (void*) ms;

  char str[64];
  sprintf( str, "mutex_%p", ms );

  ms->name= strdup(str);
  ms->handle= (void*) CreateMutex( LPSECURITY_ATTRIBUTES(0), // semaphoreAttributes
                                   FALSE, // initialOwner
                                   ms->name );
	
/*	
  the_mutex= (void*) CreateSemaphore( LPSECURITY_ATTRIBUTES(0), // semaphoreAttributes
                                      LONG(0), // initialCount
                                      LONG(1<<31), // maximumCount
                                      LPCTSTR(0) // name
                                    );
*/
//  pthread_mutexattr_t mutexattr;
//  pthread_mutexattr_init(&mutexattr);

  if( ms->handle == 0 )
    fprintf( stderr, "mutex(): unable to create (err=%d)\n", GetLastError() );
    
  _locked= false;
}

/////////////////////////////////////////////////////////////////////////////

MLMutex::~MLMutex()
{
  _locked= false;

  if( the_mutex != 0 )
    {
    MutexStruct* ms= (MutexStruct*) the_mutex;
    if( ms->handle != 0 )
      CloseHandle(ms->handle);
    if( ms->name != 0 )
      free( (void*) ms->name );

    delete ms;
    the_mutex= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLMutex::lock()
{
  if( the_mutex == 0 )
    return;
  MutexStruct* ms= (MutexStruct*) the_mutex;
  if( ms->handle == 0 || ms->name == 0 )
    return;

  HANDLE h= OpenMutex( MUTEX_ALL_ACCESS, // access
                       TRUE, // inherit
                       ms->name // mutexName
                     );
  if( h == 0 )
    {
    fprintf( stderr, "lock(): unable to open mutex (err=%d)!\n", GetLastError() );
    return;
    }

  if( WaitForSingleObject( h, INFINITE ) == WAIT_FAILED )
    fprintf( stderr, "mutex_lock(): wait failed! (err=%d)\n", GetLastError() );

  CloseHandle(h);

  _locked= true;
}

/////////////////////////////////////////////////////////////////////////////

void MLMutex::unlock()
{
  if( ! _locked )
    return; // hmmm...
  _locked= false;

  if( the_mutex == 0 )
    return;
  MutexStruct* ms= (MutexStruct*) the_mutex;
  if( ms->handle == 0 || ms->name == 0 )
    return;

  HANDLE h= OpenMutex( MUTEX_ALL_ACCESS, // access
                       FALSE, // inherit
                       ms->name // mutexName
                     );

  if( h == 0 )
    {
    fprintf( stderr, "unlock(): unable to open mutex (err=%d)!\n", GetLastError() );
    return;
    }

  if( ! ReleaseMutex(h) )
    fprintf( stderr, "unlock: unable to release mutex! (err=%d)\n", GetLastError() );

  CloseHandle(h);

  _locked= true;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

MLThreadCond::MLThreadCond()
{
  EventStruct* es= new EventStruct;
  the_cond= (void*) es;

  char str[64];
  sprintf( str, "mutex_%p", es );

  es->name= strdup(str);
  es->handle= (void*) CreateEvent( LPSECURITY_ATTRIBUTES(0), // semaphoreAttributes
                                   FALSE, // manualReset
                                   FALSE, // initialState (signaled?)
                                   es->name // name
                                 );

  if( es->handle == 0 )
    fprintf( stderr, "cond(): unable to create event! (err=%d)\n", GetLastError() );

  the_mutex= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLThreadCond::~MLThreadCond()
{
  if( the_cond != 0 )
    {     
    EventStruct* es= (EventStruct*) the_cond;
    if( es->handle != 0 )
      CloseHandle(es->handle);
    if( es->name != 0 )
      free( (void*) es->name );

    delete es;
    the_cond= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLThreadCond::wait()
{
  if( the_cond == 0 )
    return;

  EventStruct* es= (EventStruct*) the_cond;
  if( es->handle == 0 || es->name == 0 )
    return;

  HANDLE h= OpenEvent( EVENT_ALL_ACCESS, // access
                       FALSE, // inherit
                       es->name // eventName
                     );
  if( h == 0 )
    {
    fprintf( stderr, "wait(): unable to open event (err=%d)\n", GetLastError() );
    return;
    }

  if( WaitForSingleObject( h, INFINITE ) == WAIT_FAILED )
    fprintf( stderr, "cond_wait(): wait failed (err=%d)\n", GetLastError() );

  CloseHandle(h);
}

/////////////////////////////////////////////////////////////////////////////

void MLThreadCond::signal()
{
  if( the_cond == 0 )
    return;

  EventStruct* es= (EventStruct*) the_cond;
  if( es->handle == 0 || es->name == 0 )
    return;

  HANDLE h= OpenEvent( EVENT_ALL_ACCESS, // access
                       FALSE, // inherit
                       es->name // eventName
                     );
  if( h == 0 )
    {
    fprintf( stderr, "cond_signal(): unable to open event (err=%d)\n", GetLastError() );
    return;
    }

  if( ! PulseEvent(h) )
    fprintf( stderr, "cond_signal(): unable to pulse event (err=%d)\n", GetLastError() );

  CloseHandle(h);
}

/////////////////////////////////////////////////////////////////////////////

