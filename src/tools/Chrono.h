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
  Chrono.h

  From: Classes pour la gestion de l'heure et de la date
  From: SCFR. le 17 juin 92

  Stephane Rehel
  Revision: February 27th, 1996
*/


#ifndef __Chrono_h
#define __Chrono_h

#ifndef __boolean_h
#include "boolean.h"
#endif

#ifndef __MLString_h
#include "MLString.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class Chrono
{
private:
  double start_time;
  double end_time;
  double pause_time, pause_delay;
  boolean paused;

public:
  Chrono();

  virtual ~Chrono()
    {}

  static double clock();
  static void delay( double seconds );

  void start()
    {
    start_time= clock();
    pause_time= 0.;
    pause_delay= 0.;
    paused= false;
    }
  void stop()
    {
    if( paused )
      restart();
    end_time= clock();
    }
  void end() { stop(); } // compatibility only...

  double peek( ) const
    {
    return clock() - start_time - pause_delay;
    }
  double elapsed() const
    {
    return end_time - start_time - pause_delay;
    }
  double delta() { return elapsed(); } // compatibility only...

  void pause()
    {
    pause_time= clock();
    paused= true;
    }

  void restart()
    {
    if( ! paused )
      return;
    pause_delay += clock() - pause_time;
    pause_time= 0.;
    paused= false;
    }

  void endPause() { restart(); } // compatibility only...

  double getStartTime() const;
  double getStopTime() const;
  double getPauseDelay() const;

  // interval is a number of seconds
  static void split( double interval,
                     int* h, int* m, int* sec, int* msec );

  static MLString getString( double seconds );

  // in seconds, since Jan 1 1970
  static long gettimeofday();

  // return "00:00:00"
  static MLString getHoursMinutesSeconds( long timeofday );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __Chrono_h
