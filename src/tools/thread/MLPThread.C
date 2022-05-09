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
  MLPThread.C

  Stephane Rehel
  June 21 1997, from MLThread.C
*/

#include "MLThread.h"

#include <pthread.h>

#include <sys/time.h>
#include <unistd.h>

/////////////////////////////////////////////////////////////////////////////
/*
#include <signal.h>
static void sig_usr1_handler(int)
{
printf("sig usr1 caught!\n");
}
static boolean sig_usr1_init= false;
static void (*prev_sig_usr1)(int);
*/

/////////////////////////////////////////////////////////////////////////////

void MLRunnable::exitThread()
{
  pthread_exit(0);
}

/////////////////////////////////////////////////////////////////////////////

static void* ml_thread_create_data= 0;

static void* pthread_ML_start( void* _runnable )
{
  MLRunnable* runnable= (MLRunnable*) _runnable;

  runnable->runThread(::ml_thread_create_data);

  return 0;
}

boolean MLThread::create( MLRunnable* runnable, void* data /* = 0 */ )
{
/*
  if( ! ::sig_usr1_init )
    {
    prev_sig_usr1= signal(SIGUSR1,::sig_usr1_handler);
    ::sig_usr1_init= true;
    }
*/

  the_thread= (void*) new pthread_t;
  the_attr= (void*) new pthread_attr_t;
#ifdef DCE_COMPAT
  pthread_attr_create( (pthread_attr_t*) the_attr );
  ::ml_thread_create_data= data;
  return pthread_create( (pthread_t*) the_thread,
                         *(pthread_attr_t*) the_attr,
                         pthread_ML_start, (void*) runnable ) == 0;
#else
  pthread_attr_init( (pthread_attr_t*) the_attr );
  ::ml_thread_create_data= data;
  return pthread_create( (pthread_t*) the_thread,
                         (pthread_attr_t*) the_attr,
                         pthread_ML_start, (void*) runnable ) == 0;
#endif
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
  the_mutex= (void*) new pthread_mutex_t;

#ifdef DCE_COMPAT
  pthread_mutexattr_t mutexattr;
  pthread_mutexattr_create(&mutexattr);

  pthread_mutex_init( (pthread_mutex_t*) the_mutex, mutexattr );
#else
//  pthread_mutexattr_t mutexattr;
//  pthread_mutexattr_init(&mutexattr);

  pthread_mutex_init( (pthread_mutex_t*) the_mutex, 0 );
#endif

  _locked= false;
}

/////////////////////////////////////////////////////////////////////////////

MLMutex::~MLMutex()
{
  _locked= false;

  if( the_mutex != 0 )
    pthread_mutex_destroy( (pthread_mutex_t*) the_mutex );

  delete the_mutex;
  the_mutex= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLMutex::lock()
{
  pthread_mutex_lock( (pthread_mutex_t*) the_mutex );
  _locked= true;
}

/////////////////////////////////////////////////////////////////////////////

void MLMutex::unlock()
{
  if( ! _locked )
    return; // hmmm...
  _locked= false;
  pthread_mutex_unlock( (pthread_mutex_t*) the_mutex );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

MLThreadCond::MLThreadCond()
{
  the_mutex= (void*) new pthread_mutex_t;
  the_cond= (void*) new pthread_cond_t;

#ifdef DCE_COMPAT
  pthread_mutexattr_t mutexattr;
  pthread_mutexattr_create(&mutexattr);
  pthread_condattr_t condattr;
  pthread_condattr_create(&condattr);

  pthread_mutex_init( (pthread_mutex_t*) the_mutex, mutexattr );
  pthread_cond_init ( (pthread_cond_t *) the_cond , condattr );
#else
//  pthread_mutexattr_t mutexattr;
//  pthread_mutexattr_init(&mutexattr);
//  pthread_condattr_t condattr;
//  pthread_condattr_init(&condattr);

  pthread_mutex_init( (pthread_mutex_t*) the_mutex, 0 );
  pthread_cond_init ( (pthread_cond_t *) the_cond , 0 );
#endif
}

/////////////////////////////////////////////////////////////////////////////

MLThreadCond::~MLThreadCond()
{
  if( the_mutex != 0 )
    pthread_mutex_destroy( (pthread_mutex_t*) the_mutex );

  if( the_cond != 0 )
    pthread_cond_destroy( (pthread_cond_t*) the_mutex );

  delete the_cond;
  the_cond= 0;

  delete the_mutex;
  the_mutex= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLThreadCond::wait()
{
  pthread_cond_wait( (pthread_cond_t*)  the_cond,
                     (pthread_mutex_t*) the_mutex );

/*
  for(;;)
    {
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv,&tz);
    timespec t;
    t.tv_sec= tv.tv_sec;
    t.tv_nsec= tv.tv_usec * 1000 + int(1e9 * 0.5);

    int outcome= pthread_cond_timedwait( (pthread_cond_t*)  the_cond,
                                         (pthread_mutex_t*) the_mutex,
                                         &t );
    if( outcome != ETIMEDOUT )
      break;
    }
*/
}

/////////////////////////////////////////////////////////////////////////////

void MLThreadCond::signal()
{
  // first, lock this mutex, for predictable behaviour
//  pthread_mutex_lock( (pthread_mutex_t*) the_mutex );

  // then signal
  pthread_cond_signal( (pthread_cond_t*) the_cond );
}

/////////////////////////////////////////////////////////////////////////////

