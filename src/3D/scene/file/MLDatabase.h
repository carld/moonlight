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
  MLDatabase.h

  Stephane Rehel
  April 12 1997
*/

#ifndef __MLDatabase_h
#define __MLDatabase_h

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

class MLPath;
class MLAbstractMesh;
class MLName;

/////////////////////////////////////////////////////////////////////////////

class MLDatabase
{
protected:
  MLPath* path; // e.g. "/usr/ml/database/"

public:
  static const char sepChar; // ','

  static const char* geometryExtension; // abstract object
  static const char* hierarchyExtension;
  static const char* materialExtension;
  static const char* sceneExtension;
  static const char* projectExtension;

  static const char* GEOM_DIR_NAME; // "GEOMETRIES"
  static const char* HIER_DIR_NAME; // "HIERARCHIES"
  static const char* SCENE_DIR_NAME; // "SCENES"
  static const char* PROJECT_DIR_NAME; // "PROJECTS"
  static const char* IMAGE_DIR_NAME; // "IMAGES"
  static const char* MATERIAL_DIR_NAME; // "MATERIALS"

  static const char* packagedExtension;

public:
  MLDatabase( const MLPath& _path );

  virtual ~MLDatabase();

  static MLDatabase* create( const MLPath& path );

  static MLString buildVersionString( int major, int minor );

  static MLString buildVersionString( const MLName& name );

  const MLString& getBasePath() const;

  MLString getGeomPath() const;
  MLString getHierPath() const;
  MLString getScenePath() const;
  MLString getProjectPath() const;
  MLString getImagePath() const;
  MLString getMaterialPath() const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLDatabase_h
