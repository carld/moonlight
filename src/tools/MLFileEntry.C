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
   MLFileEntry.C

   Stephane Rehel
   April 3 1998
*/

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include "MLFileEntry.h"

#include "MLFilename.h"
#include "MLPath.h"

#include "MLArray.h"

/////////////////////////////////////////////////////////////////////////////

MLFileEntry::MLFileEntry( const MLString& _filename )
{
  filename= _filename;
  stat();
}

/////////////////////////////////////////////////////////////////////////////

MLFileEntry::MLFileEntry()
{
  filename= "";

  _length= 0;
  _lastModified= 0;
  user_id= group_id= 0;

  _exists= _isFile= _isDirectory= _isLink= false;
  _canRead= _canWrite= _canExecute= false;

  _inode= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLFileEntry::stat( MLString* errorMessage /* = 0 */ )
{
  struct stat file_info;
  if( ::stat( filename.get(), &file_info ) < 0 )
    {
    if( errorMessage != 0 )
      *errorMessage= strerror(errno);
    _exists= false;
    return false;
    }

  _exists= true;

  _inode= long(file_info.st_ino);

  user_id= long(file_info.st_uid);
  group_id= long(file_info.st_gid);
  _length= long(file_info.st_size);
#if defined(__sun__) || defined(__sgi__)
  _lastModified= long(file_info.st_mtim.tv_sec);
#else
  _lastModified= long(file_info.st_mtime); // SGI?
#endif
  _isFile= S_ISREG(file_info.st_mode) != 0;
  _isDirectory= S_ISDIR(file_info.st_mode) != 0;
  _isLink= S_ISLNK(file_info.st_mode) != 0;

  _canRead=    ::access( filename.get(), R_OK ) == 0;
  _canWrite=   ::access( filename.get(), W_OK ) == 0;
  _canExecute= ::access( filename.get(), X_OK ) == 0;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString MLFileEntry::getUserName( long user_id )
{
  struct passwd* entry= getpwuid( (uid_t) user_id );

  if( entry == 0 )
    return MLString("unknown");

  if( entry->pw_name == 0 )
    return MLString("unknown");

  return MLString(entry->pw_name);
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString MLFileEntry::getUserName()
{
  return MLFileEntry::getUserName( (long) getuid() );
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString MLFileEntry::getDateString( long date )
{
  time_t __date= (time_t) date;
  struct tm* t= localtime(&__date);
  if( t == 0 )
    return MLString("");

  char* months[12] = { "Jan",
                       "Feb",
                       "Mar",
                       "Apr",
                       "May",
                       "Jun",
                       "Jul",
                       "Aug",
                       "Sep",
                       "Oct",
                       "Nov",
                       "Dec" };

  // Returns "Apr 21 1997, 18:25"
  return MLString::printf( "%s %d %d, %02d:%02d",
                           months[t->tm_mon],
                           t->tm_mday,
                           t->tm_year + 1900,
                           t->tm_hour,
                           t->tm_min );
}

/////////////////////////////////////////////////////////////////////////////

/*
static boolean inGroups( gid_t gid )
{
#ifdef __WINDOWS__
  return true;
#else
  if( groups == 0 )
    {
    int ngroups= getgroups( 0, 0 );
    if( ngroups == -1 )
      return false;
    gid_t* groups= new gid_t [ ngroups ];
    getgroups( ngroups, groups );
    }

  boolean ok= false;
  for( int i= 0; i < ngroups; ++i )
    if( groups[i] == gid )
      {
      ok= true;
      break;
      }

  delete groups;

  return ok;
#endif
}
*/

/////////////////////////////////////////////////////////////////////////////

boolean MLFileEntry::list( MLArray<MLFileEntry*>& array,
                           const MLString& pattern,
                           MLString* errorMessage /* = 0 */,
                           boolean alwaysIncludeDirs /* = true */ )
{
  array.realloc(0);

  MLFilename f(filename.get());

  const MLPath& path= f.getPath();

/*
  int status;
  status= chdir(directory.get());
  if( status != 0 )
    return BAD_DIRECTORY;
   else
    {
    char d[2048];
    getcwd( d, 2048-1 );
    directory= d;
    }
*/

  DIR* current_directory= opendir( path.get().get() );
  if( current_directory == 0 )
    {
    if( errorMessage != 0 )
      *errorMessage= strerror(errno);
    return false;
    }

  for(;;)
    {
    struct dirent* entry= readdir(current_directory);
    if( entry == 0 )
      break;

    MLFileEntry* fe= new MLFileEntry;
    fe->filename= path.get() + MLString(entry->d_name);

    if( ! fe->stat(errorMessage) )
      {
      closedir(current_directory);
      return false;
      }

    if( ! (fe->isDirectory() && alwaysIncludeDirs) &&
        ! fe->filename.glob(pattern) )
      {
      delete fe;
      continue;
      }

    array.append(fe);
    } // end for(;;)

  closedir(current_directory);

  // lets sort them
  for( int i= 1; i <= array.length(); ++i )
    {
    MLFileEntry* fe_min= array[i];
    int i_min= i;
    for( int j= i+1; j <= array.length(); ++j )
      {
      MLFileEntry* fe= array[j];
      if( fe->filename.compareTo(fe_min->filename) < 0 )
        {
        fe_min= fe;
        i_min= j;
        }
      }

    if( i != i_min )
      {
      MLFileEntry* fe= array[i];
      array[i]= array[i_min];
      array[i_min]= fe;
      }
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// static
boolean MLFileEntry::remove()
{
  return ::unlink( filename.get() ) == 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLFileEntry::mkdir()
{
  return ::mkdir( filename.get(), 0 ) == 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLFileEntry::renameTo( const MLString& filename_2 )
{
  return ::rename( filename.get(), filename_2.get() ) == 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLFileEntry::isAccessibleDirectory() const
{
  return isDirectory() && canRead() && canExecute();
}

/////////////////////////////////////////////////////////////////////////////

// returns false if failed
// static
boolean MLFileEntry::changeToDirectory( const MLString& newDirectory )
{
  int status;
  status= chdir(newDirectory.get());

  return status == 0;
}

/////////////////////////////////////////////////////////////////////////////

// if error, return FALSE
boolean MLFileEntry::sameFile( const MLString& file1, const MLString& file2 )
{
  struct stat file1_info;
  struct stat file2_info;

  if( ::stat( file1.get(), &file1_info ) == -1 )
    return false;
  if( ::stat( file2.get(), &file2_info ) == -1 )
    return false;

  return (file1_info.st_ino == file2_info.st_ino) &&
         (file1_info.st_dev == file2_info.st_dev);
}

/////////////////////////////////////////////////////////////////////////////

