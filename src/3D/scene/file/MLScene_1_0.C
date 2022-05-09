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
  MLScene_1_0.C

  boolean MLSceneIO::read_1_0( MLScene* scene, MLRFileBlock& );

  Stephane Rehel
  April 15 1997
*/

#include <stdio.h>

#include "MLSceneIO.h"
#include "MLSceneIDs.h"

#include "tools/file/MLRFile.h"
#include "tools/file/MLRFileBlock.h"

#include "scene/MLObject.h"
#include "scene/MLMesh.h"
#include "scene/MLChildrenIterator.h"
#include "scene/lighting/MLLightPool.h"
#include "scene/lighting/MLPoolTable.h"
#include "scene/lighting/MLLight.h"
#include "scene/lighting/MLGlobalLighting.h"
#include "scene/lighting/MLPolygonEnergySource.h"
#include "scene/lighting/MLMeshLighting.h"
#include "scene/lighting/MLMeshPoolLighting.h"
#include "scene/MLScene.h"

#include "MLAbstractMeshIO.h"
#include "MLObjectIO.h"
#include "MLDatabase.h"

/////////////////////////////////////////////////////////////////////////////

boolean MLSceneIO::read_1_0( MLScene* scene, MLRFileBlock& block )
{
  int i;

  MLName sceneName;

  block.go(MLSceneID::SCENE_PARAMS);
    {
    block >> sceneName;
    }

  scene->name= sceneName;

  // First, read hierarchies
  block.go(MLSceneID::SCENE_HIER);
    {
    int nHier;
    block >> nHier;

    for( i= 1; i <= nHier; ++i )
      {
      MLName oname;
      block >> oname;

      oname.filename= MLObjectIO::getObjectFilename( database,
                                                     sceneName,
                                                     oname );

      MLObject* o= MLObjectIO::read(database,scene,oname.filename);
if( o == 0 )
{
 // warning
 fprintf( stderr, "unable to open hier '%s' file '%s'\n",
         oname.name.get(),
         oname.filename.get());
}
      }
    }

  // Abstracts have been read in MLObjectIO::read()

  // Then read pools
  block.go(MLSceneID::SCENE_POOLS);
    {
    int nPools;
    block >> nPools;

    MLPoolTable* table= scene->getPoolTable();

    for( i= 1; i <= nPools; ++i )
      {
      MLString poolName;
      block >> poolName;

      boolean newPool= false;
      MLLightPool* pool= table->getPool(poolName);
      if( pool == 0 )
        {
        // this a new pool for this scene
        pool= new MLLightPool(scene);
        pool->name= poolName;
        newPool= true;
        }

      block >> pool->color;
      pool->intensity= block.getDouble();
      boolean dirty= block.getInteger();
      boolean dirtyIndirect= block.getInteger();

      int nLights;
      block >> nLights;

      for( int j= 1; j <= nLights; ++j )
        {
        MLName lightName;
        block >> lightName;

        lightName.filename= MLObjectIO::getObjectFilename( database,
                                                           sceneName,
                                                           lightName );

        MLObject* object= scene->getObjectByFilename(lightName.filename);
        if( object != 0 )
          {
          if( object->getObjectType() != MLObject::LIGHT )
            object= 0;
          }
        MLLight* light= (MLLight*) object;
        if( light == 0 )
          continue; // didn't find this light ???!!!

if( light->getPool()->getName() != MLLightPool::defaultPoolName )
  { /* WARNING !! */ }

        MLGlobalLighting* glighting= scene->getGlobalLighting();

        glighting->change_light_pool( light, pool );
        }

      if( newPool )
        {
        // ok, no change to pool lighting
        pool->dirty= dirty;
        pool->dirtyIndirect= dirtyIndirect;
        }
      }
    }

  block.go(MLSceneID::SCENE_LIGHTING);
    {
    for(;;)
      {
      int oneMoreObject;
      block >> oneMoreObject;
      if( ! oneMoreObject )
        break;

      MLName oname;
      block >> oname;
      MLObject* o= scene->getObject(oname);
      if( o == 0 )
{
printf( "Unexpected error: unknown object '%s'\n", oname.name.get() );
        continue;
}

      if( o->getObjectType() != MLObject::MESH )
        continue;

      MLMesh* m= (MLMesh*) o;

      MLMeshLighting* lighting= m->getLighting();
      if( lighting == 0 )
        break; // idem

      // Read indirect PES if there's a valid one
      int hasIndirectPES;
      block >> hasIndirectPES;

      if( hasIndirectPES )
        {
        MLPolygonEnergySource* pes= new MLPolygonEnergySource(scene);

        int nPools;
        block >> nPools;
        int valid= (pes->nPools == nPools);
        readArray( block, pes->energy, 0, pes->nPools-1 );

        pes->max_abs_power= block.getDouble();
        MLName name;
        block >> name;
        pes->mesh= (MLMesh*) scene->getObject(name,MLObject::MESH);
        if( pes->mesh != 0 )
          pes->object_index= pes->mesh->getID();

        block >> pes->polygon;
        block >> pes->world_location;
        block >> pes->world_normal;

        pes->polygonArea= block.getDouble();

        if( valid && lighting->indirectPES != 0 )
          lighting->setIndirectPES(pes);
         else
          delete pes;
        }

      // Read final lighting
      int size;
      block >> size;
      lighting->finalLighting.realloc(size);
      readArray( block, lighting->finalLighting, 1, size );

      // Then read object lighting for each pool
      for(;;)
        {
        int oneMorePool;
        block >> oneMorePool;
        if( ! oneMorePool )
          break;

        MLString poolName;
        block >> poolName;
        MLLightPool* pool= scene->getPool(poolName);
        assert( pool != 0 );

        MLMeshPoolLighting* poolLighting=
                                         lighting->prepareForShooting(pool);
        assert( poolLighting != 0 );

        block >> size;
        poolLighting->ffDirect.realloc(size);
        readArray( block, poolLighting->ffDirect, 1, size );

        block >> size;
        poolLighting->eDirect.realloc(size);
        readArray( block, poolLighting->eDirect, 1, size );

        block >> size;
        poolLighting->indirect.realloc(size);
        readArray( block, poolLighting->indirect, 1, size );

        block >> size;
        poolLighting->unshotEnergy.realloc(size);
        readArray( block, poolLighting->unshotEnergy, 1, size );
        } // end of pools for this object

      } // end of objects
    } // end of SCENE_LIGHTING

  return true;
}

/////////////////////////////////////////////////////////////////////////////
