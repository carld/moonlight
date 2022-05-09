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
   MLFileEntry.h

   Stephane Rehel
   April 3 1998
*/

#ifndef __MLFileEntry_h
#define __MLFileEntry_h

#include "boolean.h"
#include "MLString.h"

template<class T> class MLArray;

/////////////////////////////////////////////////////////////////////////////

class MLFileEntry
{
protected:
  MLString filename;

  long _length;
  long _lastModified;
  long user_id, group_id;

  boolean _exists, _isFile, _isDirectory, _isLink;
  boolean _canRead, _canWrite, _canExecute;

  long _inode;

  MLFileEntry();

public:
  MLFileEntry( const MLString& _filename );

  MLFileEntry( const MLFileEntry& file );

  virtual ~MLFileEntry()
    {}

  boolean stat( MLString* errorMessage = 0 );

  static MLString getUserName( long user_id );
  static MLString getUserName();
  static MLString getDateString( long date ); // seconds since 1970

  boolean canRead() const
    {
    return _canRead;
    }

  boolean canWrite() const
    {
    return _canWrite;
    }

  boolean canExecute() const
    {
    return _canExecute;
    }

  boolean exists() const
    {
    return _exists;
    }

  boolean remove();

  boolean isDirectory() const
    {
    return _isDirectory;
    }

  boolean isFile() const
    {
    return _isFile;
    }

  long lastModified() const
    {
    return _lastModified;
    }

  MLString lastModifiedString() const
    {
    return getDateString( lastModified() );
    }

  long length() const
    {
    return _length;
    }

  MLString getOwnerName() const
    {
    return getUserName(user_id);
    }

//  MLString getGroupName() const
//    {
//    return getGroupName(group_id);
//    }

  boolean list( MLArray<MLFileEntry*>& array,
                const MLString& pattern,
                MLString* errorMessage = 0,
                boolean alwaysIncludeDirs = true );

  boolean mkdir();
//  boolean mkdirs();

  boolean renameTo( const MLString& filename_2 );

  boolean isAccessibleDirectory() const;

  // returns false if failed
  static boolean changeToDirectory( const MLString& newDirectory );

  // if error, return FALSE
  static boolean sameFile( const MLString& file1, const MLString& file2 );

};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ListFiles_h
