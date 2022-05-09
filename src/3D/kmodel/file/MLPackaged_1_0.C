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
  MLPackaged_1_0.C

  Stephane Rehel

  December 17 1997
*/

#include "tools/MLIOProgress.h"

#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"

#include "scene/MLAbstractMesh.h"
#include "scene/material/MLMaterial.h"
#include "scene/file/MLDatabase.h"
#include "scene/file/MLAbstractMeshIO.h"
#include "scene/file/MLMaterialIO.h"
#include "scene/file/MLObjectIO.h"
#include "scene/file/MLSceneIO.h"
#include "scene/MLObject.h"
#include "scene/MLChildrenIterator.h"
#include "scene/MLMesh.h"
#include "scene/MLMeshesIterator.h"
#include "scene/MLScene.h"

#include "interface/MLIOProgressDialog.h"

#include "kmodel/ModelModule.h"
#include "kmodel/MLModelVersion.h"

#include "ModelModuleIO.h"
#include "MLPackagedIO.h"

/////////////////////////////////////////////////////////////////////////////

boolean MLPackagedIO::read_1_0( MLRFileBlock& mainBlock,
                              MLIOProgress* progress )
{
  MLScene* scene= model->getScene();
  MLDatabase* database= 0;
  MLRFile* file= mainBlock.getFile();

  boolean ok= true;

  MLName sceneName;
  mainBlock.go(PKG_SCENE_INFO);
    {
    mainBlock >> sceneName;
    scene->setName(sceneName);
    }

  mainBlock.go(PKG_GEOMETRIES);
    {
    int nAbstracts= mainBlock.getInteger();
    for( int i= 1; i <= nAbstracts; ++i )
      {
      MLRFileBlock block;
      block.open(file);
        {
        MLAbstractMesh* ao= MLAbstractMeshIO::read( database,
                                                        scene,
                                                        block,
                                                        progress );

        ok= (ao != 0);
        if( ! ok )
          break;
        }
      block.close();
      }
    }

  if( ! ok )
    goto failed;

  mainBlock.go(PKG_MATERIALS);
    {
    int nMaterials= mainBlock.getInteger();
    for( int i= 1; i <= nMaterials; ++i )
      {
      MLRFileBlock block;
      block.open(file);
        {
        MLMaterialIO m_io( database, scene, progress );
        MLMaterial* m= m_io.read(block);
        if( m != 0 )
          m->registerID();

        ok= (m != 0);
        if( ! ok )
          break;
        }
      block.close();
      }
    }

  if( ! ok )
    goto failed;

  mainBlock.go(PKG_PROJECT);
    {
    MLRFileBlock block;
    block.open(file);
      {
      ModelModuleIO* modelIO= new ModelModuleIO(model);
      modelIO->readMainBlock(block,0,progress,false);
      delete modelIO;
      modelIO= 0;
      }
    block.close();
    }

  mainBlock.go(PKG_HIERARCHIES);
    {
    int nHier= mainBlock.getInteger();
    for( int i= 1; i <= nHier; ++i )
      {
      MLRFileBlock block;
      block.open(file);
        {
        MLObjectIO o_io( database, scene, progress );
        ok= (o_io.read(block) != 0);
        if( ! ok )
          break;
        }
      block.close();
      }
    }

  if( ! ok )
    goto failed;

  mainBlock.go(PKG_SCENE);
    {
    MLRFileBlock block;
    block.open(file);
      {
      MLSceneIO s_io( database, progress );
      ok= s_io.read( block, scene );
      }
    block.close();
    }

  return true;

failed:
  // cleanup scene?
  return false;
}

/////////////////////////////////////////////////////////////////////////////
