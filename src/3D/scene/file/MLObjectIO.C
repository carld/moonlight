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
  MLObjectIO.C

  Stephane Rehel
  April 13 1997
*/

#include <stdio.h>

#include "MLObjectIO.h"
#include "MLObjectIDs.h"

#include "MLMaterialIO.h"

#include "tools/MLFileEntry.h"
#include "tools/MLIOProgress.h"

#include "tools/file/MLWFile.h"
#include "tools/file/MLWFileBlock.h"

#include "tools/file/MLRFile.h"
#include "tools/file/MLRFileBlock.h"

#include "tools/file/MLFileInfo.h"

#include "scene/MLObject.h"
#include "scene/MLMesh.h"
#include "scene/MLChildrenIterator.h"
#include "scene/lighting/MLLight.h"
#include "scene/MLScene.h"
#include "scene/MLSceneVersion.h"

#include "MLDatabase.h"

/////////////////////////////////////////////////////////////////////////////

static const int major_version= 1;
static const int minor_version= 2;

/////////////////////////////////////////////////////////////////////////////

MLObjectIO::MLObjectIO( MLDatabase* _database, MLScene* _scene,
                        MLIOProgress* _progress /* = 0 */ ):
  MLIO(_progress)
{
  database= _database;
  scene= _scene;
}

/////////////////////////////////////////////////////////////////////////////

MLObjectIO::~MLObjectIO()
{}

/////////////////////////////////////////////////////////////////////////////

// Also write referenced materials
boolean MLObjectIO::write( MLObject* o, const MLString& filename )
{
  if( ! MLMaterialIO::writeReferenced( database, o, progress ) )
    return false;

  MLWFile file;
  if( ! file.create(filename) )
    return false;

  if( ! file.init(MLObjectID::signature) )
    {
    file.close();
    MLFileEntry f(filename);
    f.remove();
    return false;
    }

  MLWFileBlock& wb= file.getMainBlock();

  write(o,wb);

  file.close();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLObjectIO::write( MLObject* o, MLWFileBlock& block )
{
  block.begin(MLObjectID::DRIVER_VERSION);
    block << major_version;
    block << minor_version;
  block.end();

  block.begin(MLObjectID::OBJECT_INFO);
    MLFileInfo::write( MLSceneVersion::version(),
                       MLSceneVersion::build_cpu(),
                       MLSceneVersion::build_vendor(),
                       MLSceneVersion::build_os(),
                       block );
  block.end();

  assert( o->getScene() == scene );
  assert( o->getFather() == scene->getRootObject() );

  writeObject(o,block);
}

/////////////////////////////////////////////////////////////////////////////

// recursive
void MLObjectIO::writeObject( MLObject* o, MLWFileBlock& block )
{
  o->name.filename= getObjectFilename(database,scene->getName(),o->name);

  block.begin(MLObjectID::OBJECT_DEF);
    {
    block << scene->getName();
    block << o->getName();

    block << int(o->getObjectType());

    block << (unsigned int) o->flags;
/*
    if( o->abstract == 0 )
      block << int(0);
     else
      {
      block << int(1);

      block << o->abstract->getName();
      }
    block << o->getMaterial()->getName();
    block << int(o->inherit_material);
*/
    }
  block.end();

  block.begin(MLObjectID::OBJECT_PARAMS);
    block << o->hierBBox;
    block << int(o->bounded);
    block << o->globalBSphere.getCenter();
    block << double(o->globalBSphere.getRadius());
    block << o->local_xform.preXForm;
    block << o->local_xform.scale;
    block << o->local_xform.rotate;
    block << o->local_xform.translate;
    block << o->local_xform.transform;
    block << o->global_xform;
  block.end();

  block.begin(MLObjectID::OBJECT_CHILDREN);
    block << o->nChildren;

    MLChildrenIterator ci(o);
    while( ! ci.eol() )
      {
      MLObject* child= ci++;

      MLWFileBlock b;
      b.open(block.getFile());
        writeObject(child,b);
      b.close();
      }
  block.end();

  switch(o->getObjectType())
    {
    case MLObject::LIGHT:
      {
      // in MLLightIO.C
      writeLight( (MLLight*) o, block );
      break;
      }
    case MLObject::MESH:
      {
      // in MLMeshIO.C
      writeMesh( (MLMesh*) o, block );
      break;
      }
    default:
      break;
    }
}

/////////////////////////////////////////////////////////////////////////////

MLObject* MLObjectIO::read( MLRFileBlock& block )
{
  if( ! block.go(MLObjectID::DRIVER_VERSION) )
    return 0;

  int major, minor;
  block >> major;
  block >> minor;

  int version= major * 100 + minor;

  MLObject* o= 0;

  switch( version )
    {
    case 100: o= read_1_0(scene->getRootObject(),block); break;
    case 101: o= read_1_1(scene->getRootObject(),block); break;
    case 102: o= read_1_2(scene->getRootObject(),block); break;
    default: break;
    }

  scene->compileLights();

  if( o != 0 )
    o->updateMaterials();

  return o;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLObject* MLObjectIO::read( MLDatabase* database,
                            MLScene* scene,
                            const MLString& filename,
                            MLIOProgress* progress /*= 0*/ )
{
  if( database == 0 )
    return 0;

  MLRFile file;
  if( ! file.open(filename) )
    return 0;

  if( ! file.init(MLObjectID::signature) )
    {
    file.close();
    return 0;
    }

  MLRFileBlock& rb= file.getMainBlock();

  MLObjectIO oReader(database,scene,progress);

  MLObject* o= oReader.read(rb);

  file.close();

  return o;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLObject* MLObjectIO::read( MLDatabase* database,
                            MLScene* scene,
                            const MLName& name,
                            MLIOProgress* progress /*= 0*/ )
{
  MLString filename= getObjectFilename( database, scene->getName(), name );

  return read( database, scene, filename, progress );
}

/////////////////////////////////////////////////////////////////////////////

// static
boolean MLObjectIO::write( MLDatabase* database, MLObject* o,
                         MLIOProgress* progress /*= 0*/ )
{
  if( database == 0 )
    return true;

  MLString filename= getObjectFilename(database,o);

  MLObjectIO oWriter(database,o->getScene(),progress);

  return oWriter.write(o,filename);
}

/////////////////////////////////////////////////////////////////////////////

// static
// e.g. my_scene,1.2,hier-1.mlh
MLString MLObjectIO::getFilename( MLObject* o )
{
  return MLObjectIO::getFilename( o->getScene()->getName(), o->getName() );
}

/////////////////////////////////////////////////////////////////////////////

// static
// e.g. my_scene,1.2,hier-1.mlh
MLString MLObjectIO::getFilename( const MLName& sceneName,
                                 const MLName& name )
{
  MLString n= sceneName.name;
  n += MLDatabase::sepChar;
  n += MLDatabase::buildVersionString(sceneName);
  n += MLDatabase::sepChar;
  n += name.name;
  if( name.name_index != MLName::default_name_index )
    {
    // in case if we have two hierarchies named 'hier-1' for example
    // e.g. my_scene,1.2,hier-1,2.mlh
    n += MLDatabase::sepChar;
    n += MLString::valueOf( name.name_index );
    }

  n += MLDatabase::hierarchyExtension;

  return n;
}

/////////////////////////////////////////////////////////////////////////////

// e.g. /usr/ml/database/GEOM/my_scene,1.2,hier-1.mlh
// static
MLString MLObjectIO::getObjectFilename( MLDatabase* database,
                                       MLObject* o )
{
  return MLObjectIO::getObjectFilename( database,
                                        o->getScene()->getName(),
                                        o->getName() );
}

/////////////////////////////////////////////////////////////////////////////

// e.g. /usr/ml/database/GEOM/my_scene,1.2,hier-1.mlh
// static
MLString MLObjectIO::getObjectFilename( MLDatabase* database,
                                       const MLName& sceneName,
                                       const MLName& name )
{
  return   ((database==0) ? MLString("") : database->getHierPath())
         + MLObjectIO::getFilename(sceneName,name);
}

/////////////////////////////////////////////////////////////////////////////
