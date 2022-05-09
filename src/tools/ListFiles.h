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
   ListFiles.h

   Creation: SR, August 23rd, 1995
   Revisions:

*/

#ifndef __ListFiles_h
#define __ListFiles_h

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef __IList_h
#include "IList.h"
#endif

#ifndef __MLString_h
#include "MLString.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class ListFiles
{
public:
  struct FileItem
    {
    MLString name;
    mode_t   mode;
    uid_t    uid;
    gid_t    gid;
    off_t    size;
    time_t   mtime;

/*
for SunOS 5.4:
#if (defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE <= 2)) || \
        defined(_XOPEN_SOURCE)
typedef struct  _timespec {
        time_t          _tv_sec;
        long            _tv_nsec;
} timespec_t;
typedef struct _timespec  timestruc_t;
*/

    boolean    isDirectory, isLink;
    boolean    canRead, canWrite, canExecute;

    MLString getUser();
    MLString getDateString();
    };
  enum STATUS { SUCCESS, BAD_DIRECTORY, UNABLE_TO_OPENDIR };

protected:
  int ngroups;
  gid_t* groups;

  IList<ListFiles::FileItem> files;

public:
  ListFiles();

  virtual ~ListFiles();

  static MLString getUserName( unsigned long uid );
  static MLString getUserName();
  static MLString getDateString( time_t t );

  boolean inGroups( gid_t gid );

  IList<ListFiles::FileItem>& getFiles()
    {
    return files;
    }

  STATUS list( MLString& directory, const MLString& pattern,
               boolean alwaysIncludeDirs = true );

  static boolean canRead( const MLString& filename );
  static boolean canWrite( const MLString& filename );
  static boolean canExecute( const MLString& filename );
  static boolean exist( const MLString& filename );

  static boolean isAccessibleDirectory( const MLString& filename );

  static boolean removeFile( const MLString& filename );

  static void splitFilename( const MLString& long_filename,
                             MLString& path,
                             MLString& filename );

  static MLString expandFilename( const MLString& filename,
                                 boolean expand_current = true );

  // for '/etc/rc.d/toto.foo.bar' returns: '.bar'
  // for 'foo' returns: ''
  // for 'foo.' returns: '.'
  // for '.a' returns: '.a'
  static MLString getExtension( const MLString& filename );

  // always finish by a '/'
  static MLString getHomeDirectory();

  // always finish by a '/'
  static MLString getCurrentDirectory();

  // returns false if failed
  static boolean changeToDirectory( const MLString& newDirectory );

  // if error, return FALSE
  static boolean sameFile( const MLString& file1, const MLString& file2 );

  // if error, return 0.
  // in seconds
  static double getLastModTime( const MLString& filename );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ListFiles_h
