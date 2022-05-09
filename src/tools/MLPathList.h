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
  PathList.h

  Stephane Rehel

  March 7 1998

  For instance: ".:/home/foo:/usr/:" or ".;/home/foo;/usr/;"
*/

#ifndef __MLPathList_h
#define __MLPathList_h

#ifndef __boolean_h
#include "boolean.h"
#endif

#ifndef __SIList_h
#include "SIList.h"
#endif

class MLPath;
class MLString;

/////////////////////////////////////////////////////////////////////////////

class MLPathList
{
protected:
  SIList<MLPath> list;

public:
  MLPathList();

  MLPathList( const MLString& string );

  virtual ~MLPathList();

  void set( const MLString& string );

  MLPath* find( const MLPath& p );

  void append( const MLPath& path );

  MLString get() const;

  // return 0 if unable to find file in path list
  MLPath* findFile( const MLString& filename );

  int getNPaths() const;

  // i >= 1
  MLPath* getPath( int i );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLPathList_h
