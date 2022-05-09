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
  MLStatusLineLogging.C

  Stephane Rehel

  June 4 1998
*/

#include "tools/Chrono.h"

#include "MLStatusLineEntryLogging.h"

/////////////////////////////////////////////////////////////////////////////

MLStatusLineEntryLogging::MLStatusLineEntryLogging( MLModule* _module )
{
  max_size= 1024;
  module= _module;
}

/////////////////////////////////////////////////////////////////////////////

MLStatusLineEntryLogging::~MLStatusLineEntryLogging()
{
  clear();
  module= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineEntryLogging::clear()
{
  while( ! entries.empty() )
    {
    Entry* e= entries.getFirst();
    entries.removeFirst();
    delete e;
    }
}

/////////////////////////////////////////////////////////////////////////////

long MLStatusLineEntryLogging::get_time() const
{
  return (long) Chrono::gettimeofday();
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLineEntryLogging::addEntry( const MLString& msg,
                                         boolean user_entry /* =false */ )
{
  if( !user_entry && entries.getNElements() > 0 )
    {
    Entry* e= entries.getLast();
    if( e->msg == msg && user_entry == e->user_entry )
      {
      ++(e->n_times);
      e->n_times_time= get_time();
      return;
      }
    }

  if( entries.getNElements() >= max_size )
    {
    Entry* e= entries.getFirst();
    entries.removeFirst();
    delete e;
    }

  Entry* e= new Entry;
  e->msg= msg;
  e->the_time= get_time();
  e->n_times= 1;
  e->n_times_time= e->the_time;
  e->user_entry= user_entry;

  entries.append(e);
}

/////////////////////////////////////////////////////////////////////////////

IList<MLStatusLineEntryLogging::Entry>&
MLStatusLineEntryLogging::getEntries()
{
  return entries;
}

/////////////////////////////////////////////////////////////////////////////
