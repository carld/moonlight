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
  MLPackagedIO.C

  Stephane Rehel

  December 17 1997
*/

#include "tools/MLDArray.h"
#include "tools/MLIOProgress.h"
#include "tools/MLFileEntry.h"
#include "tools/MLFilename.h"
#include "tools/file/MLWFile.h"
#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFile.h"
#include "tools/file/MLRFileBlock.h"
#include "tools/file/MLFileInfo.h"

#include "scene/MLAbstractMesh.h"
#include "scene/material/MLMaterial.h"
#include "scene/material/MLMaterialsIterator.h"
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

#include "kmodel/gdialogs/file/ReadingProgressDlg.h"
#include "kmodel/gdialogs/file/WritingProgressDlg.h"

#include "ModelModuleIO.h"
#include "MLPackagedIO.h"

/////////////////////////////////////////////////////////////////////////////

// static
const unsigned int MLPackagedIO::signature= 0x5f4d25e5;

static const int major_version= 1;
static const int minor_version= 0;

/////////////////////////////////////////////////////////////////////////////

MLPackagedIO::MLPackagedIO( ModelModule* _model )
{
  model= _model;
}

/////////////////////////////////////////////////////////////////////////////

MLPackagedIO::~MLPackagedIO()
{
  model= 0;
}

/////////////////////////////////////////////////////////////////////////////

static void getReferencedAbstracts( MLScene* scene,
                                    MLDArray<int>& refAbstracts )
{
  refAbstracts.clear();

  MLMeshesIterator mi(scene);
  while( ! mi.eol() )
    {
    MLMesh* m= mi++;
    MLAbstractMesh* ao= m->getAbstract();
    if( ao == 0 )
      continue;

    boolean found= false;
    for( int i= 1; i <= refAbstracts.getSize(); ++i )
      if( refAbstracts[i] == ao->getID() )
        {
        found= true;
        break;
        }

    if( ! found )
      refAbstracts.append()= ao->getID();
    }
}

/////////////////////////////////////////////////////////////////////////////

static void getReferencedMaterials( MLScene* scene,
                                    MLDArray<int>& refMaterials )
{
  refMaterials.clear();

  MLMaterialsIterator mi(scene);

  // Then write only referenced materials
  while( ! mi.eol() )
    {
    MLMaterial* m= mi++;
    if( m == 0 )
      continue;

    boolean ok= false;
    MLObjectsIterator oi(scene);
    while( ! oi.eol() )
      {
      MLObject* o= oi++;
      if( o == 0 )
        continue;

      if( o->getObjectType() == MLObject::MESH )
        {
        MLMesh* mesh= (MLMesh*) o;
        if( ! mesh->inheritMaterial() )
          if( mesh->getMaterial() == m )
            {
            ok= true;
            break;
            }
        }
      }

    if( ok || m->getID() == 0 )
      refMaterials.append()= m->getID();
    }
}


/////////////////////////////////////////////////////////////////////////////

boolean MLPackagedIO::write( const MLString& filename )
{
  if( model == 0 || filename.length() == 0 )
    return false;

  MLDatabase* database= 0;
  MLScene* scene= model->getScene();

  MLName sceneName= scene->getName();
  sceneName.filename= MLFilename(filename).getFilename();
  sceneName.name= MLFilename(filename).getFilename();
  scene->setName(sceneName);

  MLWFile file;
  if( ! file.create(filename) )
    return false;

  if( ! file.init(MLPackagedIO::signature) )
    {
    file.close();
    MLFileEntry(filename).remove();
    return false;
    }

  model->pushWaitCursor();

  // let's get referenced abstracts
  MLDArray<int> refAbstracts;
  getReferencedAbstracts(scene,refAbstracts);

  // let's get referenced materials
  MLDArray<int> refMaterials;
  getReferencedMaterials(scene,refMaterials);

  MLWFileBlock& mainBlock= file.getMainBlock();

  mainBlock.begin(PKG_DRIVER_VERSION);
    {
    mainBlock << major_version;
    mainBlock << minor_version;
    }
  mainBlock.end();

  mainBlock.begin(PKG_FILE_INFO);
    {
    MLFileInfo::write( MLModelVersion::version(),
                       MLModelVersion::build_cpu(),
                       MLModelVersion::build_vendor(),
                       MLModelVersion::build_os(),
                       mainBlock );
    }
  mainBlock.end();

  mainBlock.begin(PKG_SCENE_INFO);
    {
    mainBlock << scene->getName();
    }
  mainBlock.end();

  mainBlock.begin(PKG_PROJECT);
    {
    MLWFileBlock block;
    block.open(&file);
      {
      ModelModuleIO* modelIO= new ModelModuleIO(model);
      modelIO->writeMainBlock(block);
      delete modelIO;
      modelIO= 0;
      }
    block.close();
    }
  mainBlock.end();

  MLIOProgressDialog* progress= new MLIOProgressDialog(model);
  progress->create( WritingProgressDlg::data,
                    WritingProgressDlg::progress,
                    WritingProgressDlg::label );
  progress->empty();
  progress->setMessage("Writing packaged file...");
  progress->flush();

  boolean ok= true;

  mainBlock.begin(PKG_GEOMETRIES);
    {
    mainBlock << int(refAbstracts.getSize());
    for( int i= 1; i <= refAbstracts.getSize(); ++i )
      {
      int id= refAbstracts[i];
      MLAbstractMesh* ao= scene->getAbstractObject(id);
      // assert ao != 0

      MLWFileBlock block;
      block.open(&file);
        {
        MLAbstractMeshIO a_oi( database, scene, progress );
        //ok= ok &&
        a_oi.write( ao, block );
        if( ! ok )
          break;
        }
      block.close();
      }
    }
  mainBlock.end();

  if( ! ok )
    ;// beuh beuh

  mainBlock.begin(PKG_MATERIALS);
    {
    mainBlock << int(refMaterials.getSize());
    for( int i= 1; i <= refMaterials.getSize(); ++i )
      {
      int id= refMaterials[i];
      MLMaterial* m= scene->getMaterial(id);
      // assert m != 0

      MLWFileBlock block;
      block.open(&file);
        {
        MLMaterialIO m_io( database, scene, progress );
        //ok= ok &&
        m_io.write( m, block );
        if( ! ok )
          break;
        }
      block.close();
      }
    }
  mainBlock.end();

  if( ! ok )
    ;// beuh beuh

  mainBlock.begin(PKG_HIERARCHIES);
    {
    MLScene* scene= model->getScene();

    int nHier= 0;
    MLChildrenIterator ci(scene->getRootObject());

    while( ! ci.eol() )
      {
      if( ci++ != 0 )
        ++nHier;
      }

    mainBlock << int(nHier);

    ci.reset();
    while( ! ci.eol() )
      {
      MLObject* o= ci++;
      if( o == 0 )
        continue;

      MLWFileBlock block;
      block.open(&file);
        {
        MLObjectIO o_io( database, scene, progress );
        //ok=
        o_io.write( o, block );
//        if( ! ok )
//          break;
        }
      block.close();
      }
    }
  mainBlock.end();

  mainBlock.begin(PKG_SCENE);
    {
    MLWFileBlock block;
    block.open(&file);
      {
      MLSceneIO s_io( database, progress );
      //ok= ok &&
      s_io.write( scene, block );
      if( ! ok )
        ;//beuh beuhbreak;
      }
    block.close();
    }
  mainBlock.end();

  file.close();

  model->popWaitCursor();

  delete progress;
  progress= 0;

  return ok;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLPackagedIO::read( const MLString& filename )
{
  if( model == 0 || filename.length() == 0 )
    return false;

  MLRFile file;
  if( ! file.open(filename) )
    return false;

  if( ! file.init(MLPackagedIO::signature) )
    {
    file.close();
    return false;
    }

  model->pushWaitCursor();

  MLRFileBlock& mainBlock= file.getMainBlock();

  int major= 0,
      minor= 0;
  mainBlock.go(PKG_DRIVER_VERSION);
    {
    mainBlock >> major;
    mainBlock >> minor;
    }

#if 0
  if( mainBlock.go(PKG_FILE_INFO) )
    {
    MLFileInfo fi;
    fi.read(mainBlock);
    printf( "Software version: %s\n", fi.version.get() );
    printf( "         Creator: %s\n", fi.user.get() );
    printf( "      OS-Machine: %s-%s\n", fi.sysname.get(), fi.machine.get() );
    printf( "            Date: %s\n", fi.getTimeOfDay().get() );
    }
#endif

  MLIOProgressDialog* progress= new MLIOProgressDialog(model);
  progress->create( WritingProgressDlg::data,
                    WritingProgressDlg::progress,
                    WritingProgressDlg::label );
  progress->empty();
  progress->setMessage("Reading packaged file...");
  progress->flush();

  boolean ok= true;

  int version= major * 100 + minor;
  switch( version )
    {
    case 100: ok= read_1_0(mainBlock,progress); break;
    default:
      ok= false;
      break;
    }

  file.close();

  model->popWaitCursor();

  delete progress;
  progress= 0;

  return ok;
}

/////////////////////////////////////////////////////////////////////////////
