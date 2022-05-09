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
  KWFileDialogManager.h

  Stephane Rehel

  August 11 1997
*/

#ifndef __KWFileDialogManager_h
#define __KWFileDialogManager_h

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

class KWDialog;
class MLPath;
class ListFiles;
class KWEvent;

/////////////////////////////////////////////////////////////////////////////

class KWFileDialogManager
{
protected:
  KWDialog* dialog;
  unsigned int directories;
  unsigned int files;
  unsigned int location;
  unsigned int selection;
  unsigned int ok;
  unsigned int cancel;
  unsigned int size;
  unsigned int date;
  unsigned int user;

  MLPath* path;
  MLString pattern;
  ListFiles* listfiles;

  MLString filename;

public:
  KWFileDialogManager();
  virtual ~KWFileDialogManager();

  void init( KWDialog* _dialog,
             unsigned int _directories,
             unsigned int _files,
             unsigned int _location,
             unsigned int _selection,
             unsigned int _ok,
             unsigned int _cancel,
             unsigned int _size = 0,
             unsigned int _date = 0,
             unsigned int _user = 0 );

  // return true if user has entered a real filename, and return
  // the effective full filename
  virtual boolean validate();

  void updateDirectories();

protected:
  void _updateDirectories();

public:
  void setPattern( const MLString& _pattern );
  void setPath( const MLString& _path );

  void updateSizeDateUser();

  boolean handleKWEvent( KWEvent& event );

  const MLString& getFilename() const
    {
    return filename;
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __KWFileDialogManager_h
