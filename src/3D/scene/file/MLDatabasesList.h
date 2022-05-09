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
  MLDatabasesList.h

  Stephane Rehel
  April 20 1997
*/

#ifndef __MLDatabasesList_h
#define __MLDatabasesList_h

#ifndef __SIList_h
#include "tools/SIList.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

class MLDatabase;
class MLConfig;

/////////////////////////////////////////////////////////////////////////////

class MLDatabasesList
{
protected:
  SIList<MLDatabase> list;

public:
  MLDatabasesList();
  virtual ~MLDatabasesList();

  void clear();
  void build();

  SIList<MLDatabase>& get()
    {
    return list;
    }

  static void setConfig( MLConfig* config, const MLString& _rc_group );

  static void setCurrent( const MLString& database_path );

  // return 1 if no current position
  int getCurrentDatabasePosition();

  // return 0 if not found
  int getDatabasePathPosition( const MLString& path );

  // return 0 if not found
  MLDatabase* getDatabase( const MLString& path );

  // may return if no default database defined yet
  MLDatabase* getDefaultDatabase();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLDatabasesList_h
