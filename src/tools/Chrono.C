// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//                 2002       Patrick Mauritz
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
  Chrono.C

  From: Classes pour la gestion de l'heure et de la date
  From: SCFR. le 17 juin 92

  Stephane Rehel
  Creation: February 27th, 1996
*/

#include "config.h"

#include "Chrono.h"

#include <stdio.h>

#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#if defined(__AIX__) || defined(___AIX) || defined(_AIX) || \
    defined(__QNX__) || defined(___AIXV3) || defined(AIXV3) || \
    defined(_SEQUENT_)
// for select() in delay()
#include <sys/select.h>
#endif

#ifdef __CYGWIN32__
// for select()
#include <sys/socket.h>
#include <windows.h>
#endif

/////////////////////////////////////////////////////////////////////////////

// static
double Chrono::clock()
{
#ifdef __WINDOWS__
  // gettimeofday seems not to be implemented in cygwin32 (cf. _syslist.h)
  // Use: GetTickCount() = number of milliseconds since windows was started
  return double(GetTickCount()) * double(1e-3);
#else
  static struct timeval tv;
  static struct timezone tz;

  ::gettimeofday( &tv, &tz );

  return double(tv.tv_sec) + double(tv.tv_usec)/1e6;
#endif
}

/////////////////////////////////////////////////////////////////////////////

Chrono::Chrono()
{
  start_time= end_time= 0.;
  pause_time= 0.;
  pause_delay= 0.;
  paused= false;
}

/////////////////////////////////////////////////////////////////////////////

double Chrono::getStartTime() const
{
  return start_time;
}

/////////////////////////////////////////////////////////////////////////////

double Chrono::getStopTime() const
{
  return end_time;
}

/////////////////////////////////////////////////////////////////////////////

double Chrono::getPauseDelay() const
{
  return pause_delay;
}

/////////////////////////////////////////////////////////////////////////////

// static
void Chrono::delay( double seconds )
{
  if( seconds <= 0. )
    return;
#ifdef __WINDOWS__
  Sleep( int(seconds * 1000. + .5) );
#else
  struct timeval timer;

  timer.tv_sec= long(seconds);
  timer.tv_usec= long( (seconds-double(timer.tv_sec)) * 1e6 );
  select(0,0,0,0,&timer);
#endif
}

/////////////////////////////////////////////////////////////////////////////

// interval is a number of seconds
// static
void Chrono::split( double interval,
                    int* h, int* m, int* sec, int* msec )
{
  if( interval < 0. )
    interval= 0.;

  int _h, _m, _sec, _msec;
  if( h == 0 ) h= &_h;
  if( m == 0 ) m= &_m;
  if( sec == 0 ) sec= &_sec;
  if( msec== 0 ) msec= &_msec;

  long int_seconds= long(interval);
  *msec= int( (interval - double(int_seconds))*1000. );

  *h= int(int_seconds / 3600);
  int_seconds -= long(*h) * 3600;

  *m= int(int_seconds / 60);
  int_seconds -= long(*m) * 60;

  *sec= int(int_seconds);
}

/////////////////////////////////////////////////////////////////////////////

// static
// return "0 h 0 min 0 s 0 ms\n"
MLString Chrono::getString( double seconds )
{
  int h, m, sec, msec;
  split( seconds, &h, &m, &sec, &msec );

  return MLString::printf( "%d h %d min %d s %d ms", h, m, sec, msec );
}

/////////////////////////////////////////////////////////////////////////////

// static
// in seconds, since Jan 1 1970
long Chrono::gettimeofday()
{
  return long(Chrono::clock()+.5);
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString Chrono::getHoursMinutesSeconds( long timeofday )
{
  time_t __date= (time_t) timeofday;
  struct tm* t= localtime(&__date);
  if( t == 0 )
    return MLString::empty();

  return MLString::printf( "%02d:%02d:%02d",
                           t->tm_hour, t->tm_min, t->tm_sec );
}

/////////////////////////////////////////////////////////////////////////////
