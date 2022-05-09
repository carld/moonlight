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
  MLSceneIO.C

  Stephane Rehel
  April 15 1997
*/

#include <stdio.h>

#include "MLSceneIO.h"
#include "MLSceneIDs.h"

#include "tools/MLDArray.h"
#include "tools/MLIOProgress.h"

#include "tools/file/MLWFile.h"
#include "tools/file/MLWFileBlock.h"

#include "tools/file/MLRFile.h"
#include "tools/file/MLRFileBlock.h"

#include "tools/file/MLFileInfo.h"

#include "tools/MLFileEntry.h"

#include "scene/MLObject.h"
#include "scene/MLMesh.h"
#include "scene/MLAbstractMesh.h"
#include "scene/MLObjectsIterator.h"
#include "scene/MLMeshesIterator.h"
#include "scene/MLChildrenIterator.h"
#include "scene/lighting/MLPoolTable.h"
#include "scene/lighting/MLLightPool.h"
#include "scene/lighting/MLLight.h"
#include "scene/lighting/MLPolygonEnergySource.h"
#include "scene/lighting/MLMeshLighting.h"
#include "scene/lighting/MLMeshPoolLighting.h"
#include "scene/renderer/MLRendererOptions.h"
#include "scene/MLScene.h"
#include "scene/MLSceneVersion.h"

#include "MLAbstractMeshIO.h"
#include "MLObjectIO.h"
#include "MLDatabase.h"

/////////////////////////////////////////////////////////////////////////////

static const int major_version= 1;
static const int minor_version= 3;

/////////////////////////////////////////////////////////////////////////////

MLSceneIO::MLSceneIO( MLDatabase* _database,
                      MLIOProgress* _progress /*= 0*/ ):
  MLIO(_progress)
{
  database= _database;
}

/////////////////////////////////////////////////////////////////////////////

MLSceneIO::~MLSceneIO()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLSceneIO::write( MLScene* scene, const MLString& filename )
{
  MLWFile file;
  if( ! file.create(filename) )
    return false;

  if( ! file.init(MLSceneID::signature) )
    {
    file.close();
    MLFileEntry(filename).remove();
    return false;
    }

  MLWFileBlock& wb= file.getMainBlock();

  write(scene,wb);

  file.close();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLSceneIO::write( MLScene* scene, MLWFileBlock& block )
{
  block.begin(MLSceneID::DRIVER_VERSION);
    block << major_version;
    block << minor_version;
  block.end();

  block.begin(MLSceneID::SCENE_INFO);
    MLFileInfo::write( MLSceneVersion::version(),
                       MLSceneVersion::build_cpu(),
                       MLSceneVersion::build_vendor(),
                       MLSceneVersion::build_os(),
                       block );
  block.end();

  MLDArray<int> refAbstracts;
  int i;

  // First, write objects as hierarchies
  block.begin(MLSceneID::SCENE_PARAMS);
    {
    block << scene->getName();
    }
  block.end();

  block.begin(MLSceneID::SCENE_HIER);
    {
    block << int(scene->getRootObject()->getNChildren());

    MLMeshesIterator mi(scene);
    while( ! mi.eol() )
      {
      MLMesh* m= mi++;
      MLAbstractMesh* ao= m->getAbstract();
      if( ao == 0 )
        continue;

      boolean found= false;
      for( i= 1; i <= refAbstracts.getSize(); ++i )
        if( refAbstracts[i] == ao->getID() )
          {
          found= true;
          break;
          }

      if( ! found )
        refAbstracts.append()= ao->getID();
      }

    MLChildrenIterator ci(scene->getRootObject());
    while( ! ci.eol() )
      {
      MLObject* o= ci++;

      block << o->getName();

      MLObjectIO::write(database,o);
      }
    }
  block.end();

  // Then write referenced abstracts as geometries
  block.begin(MLSceneID::SCENE_ABSTRACTS);
    {
    block << int(refAbstracts.getSize());
    for( i= 1; i <= refAbstracts.getSize(); ++i )
      {
      int id= refAbstracts[i];
      MLAbstractMesh* ao= scene->getAbstractObject(id);

      block << ao->getName();

      boolean outcome= MLAbstractMeshIO::write(database,ao,progress);
      if( ! outcome )
        {
        fprintf( stderr, "unable to write geometry '%s'\n",
                 ao->getName().name.get());
        }
      }
    }
  block.end();

  // Then write pools
  block.begin(MLSceneID::SCENE_POOLS);
    {
    MLPoolTable* table= scene->getPoolTable();

    block << int(table->getNPools());

    for( i= 1; i <= table->getNPools(); ++i )
      {
      MLLightPool* pool= table->getPool(i);

      block << pool->getName(); // !this is an MLString!
      block << pool->color;
      block << double(pool->intensity);
      block << int(pool->dirty);
      block << int(pool->dirtyIndirect);

      block << pool->getNLights();

      for( int j= 1; j <= pool->getNLights(); ++j )
        block << pool->getLight(j)->getName();
      }
    }
  block.end();

  block.begin(MLSceneID::SCENE_LIGHTING);
    {
    MLMeshesIterator mi(scene);

    int total_svertices= 0;
    while( ! mi.eol() )
      {
      MLMesh* m= mi++;
      if( m == scene->getRootObject() )
        continue;
      MLMeshLighting* lighting= m->getLighting();
      if( lighting == 0 )
        continue;

      total_svertices+= m->getSVerticesSize();
      }

    progress->setRange( 0., double(total_svertices) );
    progress->empty();
    progress->flush();

    block << int(total_svertices);

    mi.reset();
    while( ! mi.eol() )
      {
      MLMesh* m= mi++;
      if( m == scene->getRootObject() )
        continue;
      MLMeshLighting* lighting= m->getLighting();
      if( lighting == 0 )
        continue;

      progress->setMessage( MLString("Writing object ") + m->getName().name );

      block << int(1);
      block << m->getName();
      block << int(lighting->dirty);
      block << int(lighting->dirtyFinal);

      // Write indirect PES if there's a valid one
      boolean writePES= false;
      if( lighting->indirectPES != 0 )
        if( lighting->indirectPES->isValid() )
          writePES= true;
      if( writePES )
        {
        block << int(1);
        MLPolygonEnergySource* pes= lighting->indirectPES;

        block << pes->nPools;
        writeArray( block, pes->energy, 0, pes->nPools-1 );

        block << double(pes->max_abs_power);
        block << pes->mesh->getName();
        block << pes->polygon;
        block << pes->world_location;
        block << pes->world_normal;
        block << double(pes->polygonArea);
        }
       else
        block << int(0);

      // Write final lighting
      block << lighting->finalLighting.getSize();
      writeArray( block, lighting->finalLighting,
                  1, lighting->finalLighting.getSize() );

      // Then write object lighting for each pool
      MLMeshPoolLightingArray& lightings= lighting->lightings;
      for( i= 1; i <= lightings.getSize(); ++i )
        {
        MLMeshPoolLighting* poolLighting= lightings[i];
        if( poolLighting == 0 )
          continue;
        MLLightPool* pool= poolLighting->pool;
        if( pool == 0 )
          continue;

        block << int(1);
        block << pool->getName(); // !this is an MLString!

        block << int(poolLighting->dirty);

        block << poolLighting->ffDirect.getSize();
        writeArray( block, poolLighting->ffDirect,
                    1, poolLighting->ffDirect.getSize() );

        block << poolLighting->eDirect.getSize();
        writeArray( block, poolLighting->eDirect,
                    1, poolLighting->eDirect.getSize() );

        block << poolLighting->indirect.getSize();
        writeArray( block, poolLighting->indirect,
                    1, poolLighting->indirect.getSize() );

        block << poolLighting->unshotEnergy.getSize();
        writeArray( block, poolLighting->unshotEnergy,
                    1, poolLighting->unshotEnergy.getSize() );
        }

      block << int(0); // end of pools for this object

      progress->add( double(m->getSVerticesSize()) );
      }

    block << int(0); // end of objects
    } // end of SCENE_LIGHTING
  block.end();

  // Write the renderer options
  writeRendererOptions(scene,block);

  progress->full();
  progress->flush();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLSceneIO::read( MLRFileBlock& block, MLScene* scene )
{
  if( ! block.go(MLSceneID::DRIVER_VERSION) )
    return false;

  int major, minor;
  block >> major;
  block >> minor;

  int version= major * 100 + minor;

  boolean ok= false;

  switch( version )
    {
    case 100: ok= read_1_0(scene,block); break;
    case 101: ok= read_1_1(scene,block); break;
    case 102: ok= read_1_2(scene,block); break;
    case 103: ok= read_1_3(scene,block); break;
    default: break;
    }

  // Read the renderer options
  readRendererOptions(scene,block);

/*
  // confirm abstract objects filenames
  for( i= 1; i <= refAbstracts.getSize(); ++i )
    {
    int id= refAbstracts[i];
    MLAbstractMesh* ao= scene->getAbstractObject(id);
    if( ao == 0 )
      continue; // ?

    ao->name.filename= MLAbstractMeshIO::getAbstractObjectFilename
                                                         ( database, ao );
    }
*/

  return ok;
}

/////////////////////////////////////////////////////////////////////////////

// static
boolean MLSceneIO::read( MLDatabase* database,
                       MLScene* scene,
                       const MLString& filename,
                       MLIOProgress* progress /*= 0*/ )
{
  MLRFile file;
  if( ! file.open(filename) )
    return false;

  if( ! file.init(MLSceneID::signature) )
    {
    file.close();
    return false;
    }

  MLRFileBlock& rb= file.getMainBlock();

  MLSceneIO sReader(database,progress);

  boolean outcome= sReader.read(rb,scene);

  file.close();

  return outcome;
}

/////////////////////////////////////////////////////////////////////////////

// static
boolean MLSceneIO::read( MLDatabase* database,
                       MLScene* scene,
                       const MLName& sceneName,
                       MLIOProgress* progress /*= 0*/ )
{
  MLString filename= getSceneFilename( database, sceneName );

  return read( database, scene, filename, progress );
}

/////////////////////////////////////////////////////////////////////////////

// static
boolean MLSceneIO::write( MLDatabase* database,
                        MLScene* scene,
                        const MLName& sceneName,
                        MLIOProgress* progress /*= 0*/ )
{
  MLString filename= getSceneFilename(database,sceneName);

  MLSceneIO sWriter(database,progress);

  return sWriter.write(scene,filename);
}

/////////////////////////////////////////////////////////////////////////////

// e.g. my_scene,1.2.mls
// static
MLString MLSceneIO::getFilename( const MLName& sceneName )
{
  MLString n= sceneName.name;
  n += MLDatabase::sepChar;
  n += MLDatabase::buildVersionString(sceneName);

  n += MLDatabase::sceneExtension;

  return n;
}

/////////////////////////////////////////////////////////////////////////////

// e.g. /usr/ml/database/GEOM/my_scene,1.2.mls
// static
MLString MLSceneIO::getSceneFilename( MLDatabase* database,
                                     const MLName& sceneName )
{
  return   ((database==0) ? MLString("") : database->getScenePath())
         + MLSceneIO::getFilename(sceneName);
}

/////////////////////////////////////////////////////////////////////////////
