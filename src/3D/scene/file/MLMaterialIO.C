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
  MLMaterialIO.C

  Stephane Rehel

  September 14 1997
*/

#include <stdio.h>

#include "MLMaterialIO.h"
#include "MLMaterialIDs.h"

#include "tools/MLFileEntry.h"
#include "tools/MLIOProgress.h"

#include "tools/file/MLWFile.h"
#include "tools/file/MLWFileBlock.h"

#include "tools/file/MLRFile.h"
#include "tools/file/MLRFileBlock.h"

#include "tools/file/MLFileInfo.h"

#include "scene/material/MLMaterial.h"
#include "scene/material/MLMaterialsIterator.h"

#include "scene/MLObject.h"
#include "scene/MLMesh.h"
#include "scene/MLChildrenIterator.h"
#include "scene/MLScene.h"
#include "scene/MLSceneVersion.h"

#include "MLDatabase.h"

/////////////////////////////////////////////////////////////////////////////

static const int major_version= 1;
static const int minor_version= 0;

/////////////////////////////////////////////////////////////////////////////

MLMaterialIO::MLMaterialIO( MLDatabase* _database, MLScene* _scene,
                            MLIOProgress* _progress /* = 0 */ ):
  MLIO(_progress)
{
  database= _database;
  scene= _scene;
}

/////////////////////////////////////////////////////////////////////////////

MLMaterialIO::~MLMaterialIO()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLMaterialIO::write( MLMaterial* m, const MLString& filename )
{
  MLWFile file;
  if( ! file.create(filename) )
    return false;

  if( ! file.init(MLMaterialID::signature) )
    {
    file.close();
    MLFileEntry f(filename);
    f.remove();
    return false;
    }

  MLWFileBlock& wb= file.getMainBlock();

  write(m,wb);

  file.close();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLMaterialIO::write( MLMaterial* m, MLWFileBlock& block )
{
  block.begin(MLMaterialID::DRIVER_VERSION);
    block << major_version;
    block << minor_version;
  block.end();

  block.begin(MLMaterialID::MATERIAL_INFO);
    MLFileInfo::write( MLSceneVersion::version(),
                       MLSceneVersion::build_cpu(),
                       MLSceneVersion::build_vendor(),
                       MLSceneVersion::build_os(),
                       block );
  block.end();

  assert( m->getScene() == scene );

  writeMaterial(m,block);
}

/////////////////////////////////////////////////////////////////////////////

void MLMaterialIO::writeMaterial( MLMaterial* m, MLWFileBlock& block )
{
  m->name.filename= getMaterialFilename(database,scene->getName(),m->name);

  block.begin(MLMaterialID::MATERIAL_NAMES);
    {
    block << scene->getName();
    block << m->getName();
    }
  block.end();

  block.begin(MLMaterialID::MATERIAL_DEF);
    {
    block << int(m->direct_shaded);
    block << int(m->rad_contributor);

    block << int(m->generate_shadows);
    block << int(m->get_shadows);
    }
  block.end();

  block.begin(MLMaterialID::MATERIAL_EMITTANCE);
    {
    const MLMaterialEmittance& c= m->emittance;
    block << int(c.color_model);
    block << c.color;
    block << double(c.factor);
    block << int(c.use_texture);
    block << c.image_name;
    block << int(c.x_offset);
    block << int(c.y_offset);
    block << double(c.x_scale);
    block << double(c.y_scale);
    block << int(c.use_alpha);
    }
  block.end();

  block.begin(MLMaterialID::MATERIAL_DIFFUSE);
    {
    const MLMaterialDiffuse& c= m->diffuse;
    block << int(c.color_model);
    block << c.color;
    block << double(c.factor);
    block << int(c.use_texture);
    block << c.image_name;
    block << int(c.x_offset);
    block << int(c.y_offset);
    block << double(c.x_scale);
    block << double(c.y_scale);
    block << int(c.use_alpha);
    }
  block.end();

  block.begin(MLMaterialID::MATERIAL_SPECULAR);
    {
    const MLMaterialSpecular& c= m->specular;
    block << double(c.shininess);
    block << int(c.color_model);
    block << c.color;
    block << double(c.factor);
    block << int(c.use_texture);
    block << c.image_name;
    block << int(c.x_offset);
    block << int(c.y_offset);
    block << double(c.x_scale);
    block << double(c.y_scale);
    block << int(c.use_alpha);
    }
  block.end();

  block.begin(MLMaterialID::MATERIAL_REFLECTION);
    {
    const MLMaterialReflection& c= m->reflection;
    block << int(c.color_model);
    block << c.color;
    block << double(c.factor);
    block << int(c.use_texture);
    block << c.image_name;
    block << int(c.x_offset);
    block << int(c.y_offset);
    block << double(c.x_scale);
    block << double(c.y_scale);
    block << int(c.use_alpha);
    }
  block.end();

  block.begin(MLMaterialID::MATERIAL_TRANSMISSION);
    {
    const MLMaterialTransmission& c= m->transmission;
    block << double(c.trans_n);
    block << int(c.color_model);
    block << c.color;
    block << double(c.factor);
    block << int(c.use_texture);
    block << c.image_name;
    block << int(c.x_offset);
    block << int(c.y_offset);
    block << double(c.x_scale);
    block << double(c.y_scale);
    block << int(c.use_alpha);
    }
  block.end();
}

/////////////////////////////////////////////////////////////////////////////

// return an unregistered material!
MLMaterial* MLMaterialIO::read( MLRFileBlock& block )
{
  if( ! block.go(MLMaterialID::DRIVER_VERSION) )
    return 0;

  int major, minor;
  block >> major;
  block >> minor;

  int version= major * 100 + minor;

  switch( version )
    {
    case 100: return read_1_0(block); break;
    default: break;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

// return an unregistered material!
// static
MLMaterial* MLMaterialIO::read( MLDatabase* database,
                                MLScene* scene,
                                const MLString& filename,
                                MLIOProgress* progress /*= 0*/ )
{
  MLRFile file;
  if( ! file.open(filename) )
    return 0;

  if( ! file.init(MLMaterialID::signature) )
    {
    file.close();
    return 0;
    }

  MLRFileBlock& rb= file.getMainBlock();

  MLMaterialIO mReader(database,scene,progress);

  MLMaterial* m= mReader.read(rb);

  file.close();

  return m;
}

/////////////////////////////////////////////////////////////////////////////

// return an unregistered material!
// static
MLMaterial* MLMaterialIO::read( MLDatabase* database,
                                MLScene* scene,
                                const MLName& name,
                                MLIOProgress* progress /*= 0*/ )
{
  MLString filename= getMaterialFilename( database, scene->getName(), name );

  return read( database, scene, filename, progress );
}

/////////////////////////////////////////////////////////////////////////////

// static
boolean MLMaterialIO::write( MLDatabase* database,
                           MLMaterial* m,
                           MLIOProgress* progress /*= 0*/ )
{
  if( m == 0 || database == 0 )
    return true;

  MLString filename= getMaterialFilename(database,m);

  MLMaterialIO mWriter(database,m->getScene(),progress);

  return mWriter.write(m,filename);
}

/////////////////////////////////////////////////////////////////////////////

static boolean is_material_referenced( MLMaterial* m, MLObject* o )
{
  if( o == 0 )
    return false;

  if( o->getObjectType() == MLObject::MESH )
    {
    MLMesh* mesh= (MLMesh*) o;
    if( ! mesh->inheritMaterial() )
      if( mesh->getMaterial() == m )
        return true;
    }

  MLChildrenIterator ci(o);
  while( ! ci.eol() )
    if( is_material_referenced(m,ci++) )
      return true;

  return false;
}

// Write all referenced materials of this hierarchy
// static
boolean MLMaterialIO::writeReferenced( MLDatabase* database,
                                     MLObject* o,
                                     MLIOProgress* progress /*= 0*/ )
{
  if( o == 0 || database == 0 )
    return true;

  MLScene* scene= o->getScene();

  boolean ok= true;

  // First let's write the default material
  ok= ok && MLMaterialIO::write( database,
                                 scene->getDefaultMaterial(),
                                 progress );

  // Then write only referenced materials
  MLMaterialsIterator mi(scene);
  mi++; // skip default material
  while( ! mi.eol() )
    {
    MLMaterial* m= mi++;
    if( m == 0 )
      continue;
    if( ! ::is_material_referenced(m,o) )
      continue;

    ok= ok && MLMaterialIO::write( database, m, progress );
    }

  return ok;
}

/////////////////////////////////////////////////////////////////////////////

// static
// e.g. my_scene,1.2,material-1.mlm
MLString MLMaterialIO::getFilename( MLMaterial* m )
{
  return MLMaterialIO::getFilename( m->getScene()->getName(),
                                    m->getName() );
}

/////////////////////////////////////////////////////////////////////////////

// static
// e.g. my_scene,1.2,material-1.mlm
MLString MLMaterialIO::getFilename( const MLName& sceneName,
                                   const MLName& name )
{
  MLString n= sceneName.name;
  n += MLDatabase::sepChar;
  n += MLDatabase::buildVersionString(sceneName);
  n += MLDatabase::sepChar;
  n += name.name;
  if( name.name_index != MLName::default_name_index )
    {
    // in case if we have two hierarchies named 'material-1' for example
    // e.g. my_scene,1.2,material-1,2.mlm
    n += MLDatabase::sepChar;
    n += MLString::valueOf( name.name_index );
    }

  n += MLDatabase::materialExtension;

  return n;
}

/////////////////////////////////////////////////////////////////////////////

// e.g. /usr/ml/database/GEOM/my_scene,1.2,material-1.mlm
// static
MLString MLMaterialIO::getMaterialFilename( MLDatabase* database,
                                           MLMaterial* m )
{
  return MLMaterialIO::getMaterialFilename( database,
                                            m->getScene()->getName(),
                                            m->getName() );
}

/////////////////////////////////////////////////////////////////////////////

// e.g. /usr/ml/database/GEOM/my_scene,1.2,material-1.mlm
// static
MLString MLMaterialIO::getMaterialFilename( MLDatabase* database,
                                         const MLName& sceneName,
                                         const MLName& name )
{
  return   ((database==0) ? MLString("") : database->getMaterialPath())
         + MLMaterialIO::getFilename(sceneName,name);
}

/////////////////////////////////////////////////////////////////////////////
