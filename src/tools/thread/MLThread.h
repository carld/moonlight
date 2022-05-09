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
  MLThread.h

  Stephane Rehel
  February 16 1997
*/

#ifndef __MLThread_h
#define __MLThread_h

#ifndef __boolean_H
#include "tools/boolean.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLRunnable
{
public:
  MLRunnable()
    {}

  virtual ~MLRunnable()
    {}

  virtual void runThread( void* )
    {}

  void exitThread();
};

/////////////////////////////////////////////////////////////////////////////

class MLThread
{
protected:
  void* the_thread;
  void* the_attr;

public:
  MLThread()
    {
    the_thread= 0;
    the_attr= 0;
    }
  virtual ~MLThread()
    {}

  boolean create( MLRunnable* runnable, void* data = 0 );

  static boolean available();
};

/////////////////////////////////////////////////////////////////////////////

class MLMutex
{
protected:
  boolean _locked;
  void* the_mutex;

public:
  MLMutex();

  virtual ~MLMutex();

  void lock();
  void unlock();

  boolean locked() const
    {
    return _locked;
    }

  operator int() const
    {
    return _locked;
    }
};

/////////////////////////////////////////////////////////////////////////////

class MLThreadCond
{
protected:
  void* the_mutex;
  void* the_cond;

public:
  MLThreadCond();

  virtual ~MLThreadCond();

  void wait();
  void signal();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef endif __MLThread_h

