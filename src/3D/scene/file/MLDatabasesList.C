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
  MLDatabasesList.C

  Stephane Rehel
  April 20 1997
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tools/ListFiles.h"
#include "tools/MLPath.h"
#include "tools/MLPathList.h"
#include "tools/MLConfig.h"

#include "scene/file/MLDatabase.h"

#include "MLDatabasesList.h"

/////////////////////////////////////////////////////////////////////////////

static const char* currentDatabasePath= 0;

static MLConfig* ml_config= 0;
static MLString rc_group("");

/////////////////////////////////////////////////////////////////////////////

MLDatabasesList::MLDatabasesList()
{}

/////////////////////////////////////////////////////////////////////////////

MLDatabasesList::~MLDatabasesList()
{
  clear();
}

/////////////////////////////////////////////////////////////////////////////

void MLDatabasesList::clear()
{
  while( ! list.empty() )
    {
    MLDatabase* db= list.getLast();
    list.removeLast();
    delete db;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLDatabasesList::build()
{
/*
  clear();

  MLString mlrc= MLDatabasesList::getDatabasesResourceFilename();
  mlrc= Path::checkPathSeparators(mlrc);

  FILE* file= fopen( mlrc.get(), "r" );
  if( file == 0 )
    {
empty_list:
    // suggest a database
    list.append( new MLDatabase(
                    Path(ListFiles::expandFilename("~/moonlight/database/"))
                               )
               );
    return;
    }

  const int buffsize= 512;
  char buff[buffsize+1];

  while( fgets(buff,buffsize,file) != 0 )
    {
    if( strlen(buff) == 0 )
      continue;
    if( buff[ strlen(buff)-1 ] == '\n' )
      buff[ strlen(buff)-1 ]= '\0';

    MLString line= MLString(buff).trim().getFirstWord();
    if( line.length() == 0 )
      continue;
    if( line[0] == '#' )
      continue;

    line= ListFiles::expandFilename(line);
    Path path(line);

    boolean found= false;
    SIListIterator<MLDatabase> li(list);
    while( ! li.eol() )
      {
      MLDatabase* _db= li++;
      if( _db->getBasePath() == path.get() )
        {
        found= true;
        break;
        }
      }

    if( ! found )
      {
      MLDatabase* db= new MLDatabase(path);
      list.append(db);
      }
    }

  fclose(file);

  if( list.empty() )
    goto empty_list;
*/

  clear();

  MLString databases_var= "";

  if( ::ml_config != 0 )
    databases_var= ::ml_config->getVariable(::rc_group,"Databases");

  MLPathList pl;
  pl.set(databases_var);

  if( pl.getNPaths() == 0 )
    {
empty_list:
/*
    // suggest a database
    Path path( ListFiles::expandFilename("~/moonlight/database/") );
    list.append( new MLDatabase(path) );
*/
    return;
    }

  for( int i= 1; i <= pl.getNPaths(); ++i )
    {
    MLPath* path= pl.getPath(i);
    if( path == 0 )
      continue;

    MLDatabase* db= new MLDatabase(*path);
    list.append(db);
    }

  if( list.empty() )
    goto empty_list;
}

/////////////////////////////////////////////////////////////////////////////

// static
void MLDatabasesList::setConfig( MLConfig* config,
                                 const MLString& _rc_group )
{
  ::ml_config= config;
  ::rc_group= _rc_group;
}

/////////////////////////////////////////////////////////////////////////////

// static
void MLDatabasesList::setCurrent( const MLString& database_path )
{
  if( currentDatabasePath != 0 )
    free( (void*) currentDatabasePath );

  currentDatabasePath= strdup( database_path.get() );
}

/////////////////////////////////////////////////////////////////////////////

// return 1 if no current position
int MLDatabasesList::getCurrentDatabasePosition()
{
  assert( ! list.empty() );

  if( currentDatabasePath == 0 )
    return 1;

  int index= getDatabasePathPosition( MLString(currentDatabasePath) );
  if( index == 0 )
    index= 1; // not found

  return index;
}

/////////////////////////////////////////////////////////////////////////////

// return 0 if not found
int MLDatabasesList::getDatabasePathPosition( const MLString& path )
{
  if( list.empty() )
    return 0;

  int index= 1;
  SIListIterator<MLDatabase> li(list);
  while( ! li.eol() )
    {
    MLDatabase* db= li++;
    if( db->getBasePath() == path )
      return index;
    ++index;
    }

  return 0; // not found
}

/////////////////////////////////////////////////////////////////////////////

// return 0 if not found
MLDatabase* MLDatabasesList::getDatabase( const MLString& path )
{
  int index= getDatabasePathPosition(path);
  if( index == 0 )
    return 0;

  return list.get(index);
}

/////////////////////////////////////////////////////////////////////////////

// may return if no default database defined yet
MLDatabase* MLDatabasesList::getDefaultDatabase()
{
  if( currentDatabasePath == 0 )
    return 0;

  return getDatabase( MLString(currentDatabasePath) );
}

/////////////////////////////////////////////////////////////////////////////
