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
  pthreads_test.C

  Stephane Rehel
  February 19 1997
*/

//#define _MIT_POSIX_THREADS 1

#include <stdio.h>
#include <stdlib.h>

#include "tools/Chrono.h"

#define USE_ML_THREAD_CLASSES

#ifndef USE_ML_THREAD_CLASSES

#include <pthread.h>

/////////////////////////////////////////////////////////////////////////////

pthread_mutexattr_t mutexattr;
pthread_mutex_t     mutex;
pthread_condattr_t  condattr;
pthread_cond_t      cond;

pthread_mutexattr_t mutexattr2;
pthread_mutex_t     mutex2;
pthread_condattr_t  condattr2;
pthread_cond_t      cond2;

pthread_mutexattr_t mutexattr3;
pthread_mutex_t     mutex3;
pthread_condattr_t  condattr3;
pthread_cond_t      cond3;

pthread_attr attr;
pthread_t tid;

enum STATE { RUN, CANCEL, SLEEP } state;

int nMsg= 0;

/////////////////////////////////////////////////////////////////////////////

void* thread_func( void* )
{
  printf( "  thread running\n" );

  for(;;)
    {
    printf( "  Waiting for a message (%d)\n", nMsg );

    while( nMsg <= 0 )
      {
      pthread_cond_wait(&cond,&mutex);
      if( state == RUN )
        break;
      pthread_cond_signal(&cond2);
      }

    while( nMsg > 0 )
      {
      --nMsg;
      printf( "  Working...\n");
      int n= 10000000;
      int i;
      for( i= 1; i <= n; ++i )
        {
        if( state == SLEEP )
          pthread_cond_wait(&cond3,&mutex3);
        if( state == CANCEL )
          break;
        }

      if( state == CANCEL )
        {
        pthread_cond_signal(&cond2);
        printf("  *stopped (%d)\n", i-1 );
        break;
        }
       else
        printf("  Finished.\n");
      }
    }

  printf("  thread exiting\n" );

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

STATE old_state;
void get_asleep()
{
  old_state= state;
  state= SLEEP;
}

void wakeup()
{
  state= old_state;
  pthread_cond_signal(&cond3);
}

/////////////////////////////////////////////////////////////////////////////

int main()
{
  int ok;

  state= RUN;

/*
#define init_cond(cond,mutex) { \
  pthread_condattr_init (&condattr); \
  pthread_cond_init     (&cond,&condattr); \
  pthread_mutexattr_init(&mutexattr); \
  pthread_mutex_init    (&mutex,&mutexattr); }
*/

#define init_cond(cond,mutex) { \
             pthread_cond_init     (&cond,0); \
             pthread_mutex_init    (&mutex,0); }

  init_cond(cond,mutex);
  init_cond(cond2,mutex2);
  init_cond(cond3,mutex3);

  pthread_attr_init(&attr);
  ok= pthread_create(&tid,&attr,thread_func,0);
  printf( "ok= %d\n", ok );
  Chrono c; c.start();
  Chrono::delay(0.1); // 0.01 seems not to be enough on SunOS 5.5.1

  printf("Created. Delay= %f\n",c.peek());

  double maxd= 0., totald= 0.;
  int nd= 0;

  for(;;)
    {
    printf( "ENTER ME-->" ); fflush(stdout);
get_asleep();
Chrono::delay(0.5);
wakeup();
    getchar();

    // if we have to stop the thread
      Chrono c; c.start();
      state= CANCEL;
      pthread_cond_signal(&cond3);
      pthread_cond_signal(&cond);
      pthread_cond_wait(&cond2,&mutex2);
      double d= c.peek();
      if( d > maxd ) maxd= d;
      totald+= d; ++nd;
      printf( "Cancelling delay= %f (max=%f avg=%f)\n",
              d, maxd, totald/double(nd) );
    state= RUN;
    ++nMsg;
    pthread_cond_signal(&cond);
    }

  printf( "program exiting\n");

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

#else

#include "tools/thread/MLThread.h"

/////////////////////////////////////////////////////////////////////////////

MLThreadCond cond, cond2, cond3;
MLThread ex_thread;

enum STATE { RUN, CANCEL, SLEEP } state;

int nMsg= 0;

/////////////////////////////////////////////////////////////////////////////

class Example: public MLRunnable
{
public:
  Example() {}
  ~Example() {}
  void runThread(void*);
};

void Example::runThread(void*)
{
  printf( "  thread running\n" );

  for(;;)
    {
    printf( "  Waiting for a message (%d)\n", nMsg );

    while( nMsg <= 0 )
      {
      cond.wait();
      if( state == RUN )
        break;
      cond2.signal();
      }

    while( nMsg > 0 )
      {
      --nMsg;
      printf( "  Working...\n");
      int n= 10000000;
      int i;
      for( i= 1; i <= n; ++i )
        {
        if( state == SLEEP )
          cond3.wait();
        if( state == CANCEL )
          break;
        }

      if( state == CANCEL )
        {
        cond2.signal();
        printf("  *stopped (%d)\n", i-1 );
        break;
        }
       else
        printf("  Finished.\n");
      }
    }

  printf("  thread exiting\n" );
}

/////////////////////////////////////////////////////////////////////////////

STATE old_state;
void get_asleep()
{
  old_state= state;
  state= SLEEP;
}

void wakeup()
{
  state= old_state;
  cond3.signal();
}

/////////////////////////////////////////////////////////////////////////////

int main()
{
  int ok;

  state= RUN;

  Example ex;

  ok= ex_thread.create(&ex);

  printf( "ok= %d\n", ok );
  Chrono c; c.start();
  Chrono::delay(0.1); // 0.01 seems not to be enough on SunOS 5.5.1

  printf("Created. Delay= %f\n",c.peek());

  double maxd= 0., totald= 0.;
  int nd= 0;

  for(;;)
    {
    printf( "ENTER ME-->" ); fflush(stdout);
//get_asleep();
//Chrono::delay(0.5);
//wakeup();
    getchar();

    // if we have to stop the thread
      Chrono c; c.start();
      state= CANCEL;
      cond3.signal();
      cond.signal();
      cond2.wait();

      double d= c.peek();
      if( d > maxd ) maxd= d;
      totald+= d; ++nd;
      printf( "Cancelling delay= %f (max=%f avg=%f)\n",
              d, maxd, totald/double(nd) );
    state= RUN;
    ++nMsg;
    cond.signal();
    }

  printf( "program exiting\n");

  return 0;
}

#endif

