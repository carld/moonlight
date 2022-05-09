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
  MLDatabase.C

  Stephane Rehel
  April 12 1997
*/

#include "tools/MLPath.h"

#include "scene/MLName.h"

#include "MLDatabase.h"
#include "MLAbstractMeshIO.h"

/////////////////////////////////////////////////////////////////////////////

// static

const char MLDatabase::sepChar= ',';

const char* MLDatabase::geometryExtension= ".mlg"; // abstract object
const char* MLDatabase::hierarchyExtension= ".mlh";
const char* MLDatabase::materialExtension= ".mlm";
const char* MLDatabase::sceneExtension= ".mls";
const char* MLDatabase::projectExtension= ".mlp";

const char* MLDatabase::GEOM_DIR_NAME= "GEOMETRIES";
const char* MLDatabase::HIER_DIR_NAME= "HIERARCHIES";
const char* MLDatabase::SCENE_DIR_NAME= "SCENES";
const char* MLDatabase::PROJECT_DIR_NAME= "PROJECTS";
const char* MLDatabase::IMAGE_DIR_NAME= "IMAGES";
const char* MLDatabase::MATERIAL_DIR_NAME= "MATERIALS";

const char* MLDatabase::packagedExtension= ".mlk";

/////////////////////////////////////////////////////////////////////////////

MLDatabase::MLDatabase( const MLPath& _path )
{
  path= new MLPath(_path);
}

/////////////////////////////////////////////////////////////////////////////

MLDatabase::~MLDatabase()
{
  delete path;
  path= 0;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLDatabase* MLDatabase::create( const MLPath& path )
{
/*
  if( ! path.isAbsolute() )
    return 0;
*/
  return new MLDatabase(path);
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString MLDatabase::buildVersionString( int major, int minor )
{
//  if( major < 0 ) major= 0;
//  if( minor < 0 ) minor= 0;

  return MLString::valueOf(major) + "." + MLString::valueOf(minor);
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString MLDatabase::buildVersionString( const MLName& name )
{
  return buildVersionString(name.major_version,name.minor_version);
}

/////////////////////////////////////////////////////////////////////////////

const MLString& MLDatabase::getBasePath() const
{
  return path->get();
}

/////////////////////////////////////////////////////////////////////////////

MLString MLDatabase::getGeomPath() const
{
  MLPath p= *path;
  p += GEOM_DIR_NAME;
  return p.get();
}

/////////////////////////////////////////////////////////////////////////////

MLString MLDatabase::getHierPath() const
{
  MLPath p= *path;
  p += HIER_DIR_NAME;
  return p.get();
}

/////////////////////////////////////////////////////////////////////////////

MLString MLDatabase::getScenePath() const
{
  MLPath p= *path;
  p += SCENE_DIR_NAME;
  return p.get();
}

/////////////////////////////////////////////////////////////////////////////

MLString MLDatabase::getProjectPath() const
{
  MLPath p= *path;
  p += PROJECT_DIR_NAME;
  return p.get();
}

/////////////////////////////////////////////////////////////////////////////

MLString MLDatabase::getImagePath() const
{
  MLPath p= *path;
  p += IMAGE_DIR_NAME;
  return p.get();
}

/////////////////////////////////////////////////////////////////////////////

MLString MLDatabase::getMaterialPath() const
{
  MLPath p= *path;
  p += MATERIAL_DIR_NAME;
  return p.get();
}

/////////////////////////////////////////////////////////////////////////////

