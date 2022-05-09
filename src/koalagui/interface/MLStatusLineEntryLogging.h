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
  MLStatusLineLogging.h

  Stephane Rehel

  June 4 1998
*/

#ifndef __MLStatusLineEntryLogging_h
#define __MLStatusLineEntryLogging_h

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __IList_h
#include "tools/IList.h"
#endif

class MLModule;

/////////////////////////////////////////////////////////////////////////////

class MLStatusLineEntryLogging
{
protected:
  MLModule* module;

public:
  struct Entry
    {
    long the_time;
    MLString msg;

    int n_times;
    long n_times_time;

    boolean user_entry;
    };

protected:
  int max_size;
  IList<Entry> entries;

public:
  MLStatusLineEntryLogging( MLModule* _module );

  virtual ~MLStatusLineEntryLogging();

  void clear();

private:
  long get_time() const;

public:
  void addEntry( const MLString& msg, boolean user_entry = false );

  IList<Entry>& getEntries();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLStatusLineEntryLogging_h
