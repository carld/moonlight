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
  MLImporter.h

  Stephane Rehel

  April 27 1997
*/

#ifndef __MLImporter_h
#define __MLImporter_h

#include <stdio.h>

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

class MeshRep;
class MLScene;
class Point;
class MLCommandLine;
class MLDatabase;

/////////////////////////////////////////////////////////////////////////////

class MLImporter
{
public:
  boolean verbose;
  boolean reverseFaces;
  boolean compressed;
  MLScene* scene;
  MLString in_filename;
  MLString filename_ext;
  MLString scene_name;
  MLDatabase* database;
  boolean layers2objects;
  boolean no_write;

public:
  MLImporter();
  virtual ~MLImporter();

  virtual void finalize();

  virtual void addPolygon( MeshRep* mesh,
                           int nVertices, const Point* points );

  void convertName( MLString& name );

  int parseOptions( MLCommandLine& cmd );

  FILE* open_file() const;
  void close_file( FILE* file ) const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // __MLImporter_h
