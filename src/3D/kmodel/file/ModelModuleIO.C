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
  ModelModuleIO.C

  Stephane Rehel

  April 23 1997
*/

#include <stdio.h>

#include "ModelModuleIO.h"

#include "tools/file/MLWFile.h"
#include "tools/file/MLRFile.h"
#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"
#include "tools/file/MLFileInfo.h"
#include "tools/ListFiles.h"
#include "tools/zlib/MLLibZ.h"

#include "graphics/MLPicture.h"

#include "scene/file/MLDatabase.h"
#include "scene/file/MLSceneIO.h"
#include "scene/MLScene.h"
#include "scene/lighting/MLGlobalLighting.h"
#include "scene/renderer/MLRendererOptions.h"

#include "interface/MLIOProgressDialog.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelFlags.h"
#include "kmodel/MLModelVersion.h"

#include "kmodel/gdialogs/file/ReadingProgressDlg.h"
#include "kmodel/gdialogs/file/WritingProgressDlg.h"

#include "MLPackagedIO.h"

/////////////////////////////////////////////////////////////////////////////

// static
const unsigned int modelModuleSignature= 0x5f4d25e3;

static const int FILE_MAIN    = 1000;
static const int FILE_SNAPSHOT= 1001;
static const int FILE_MODULE  = 1002;
static const int FILE_FLAGS1  = 1003;
static const int FILE_FLAGS2  = 1004;
static const int FILE_FLAGS   = 1005;
static const int FILE_INFO    = 1006;

static const int flags_version= 3;

/////////////////////////////////////////////////////////////////////////////

ModelModuleIO::ModelModuleIO( ModelModule* _model )
{
  model= _model;
}

/////////////////////////////////////////////////////////////////////////////

ModelModuleIO::~ModelModuleIO()
{
  model= 0;
}

/////////////////////////////////////////////////////////////////////////////

void ModelModuleIO::writeFlags( MLWFileBlock& block )
{
  ModelFlags* flags= model->flags;

  block.begin(FILE_FLAGS);
    {
    block << int(::flags_version);

    block << int(flags->singleSelection);
    block << int(flags->showCamera);
    block << int(flags->showHierStruct);
    block << int(flags->showTesselation);
    block << int(flags->showNormals);
    block << int(flags->showHashBox);
    block << int(flags->writeSnapshot);

    block << int(flags->computeDirectLighting);
    block << int(flags->computeIndirectLighting);

    block << int(flags->showViewAxes);
    block << int(flags->showObjectAxes);
    block << int(flags->updateShadows);
    }
  block.end();
}

/////////////////////////////////////////////////////////////////////////////

void ModelModuleIO::writeInterfaceBlock( MLWFileBlock& block )
{
  MLWFileBlock block1;

  block1.open(block.getFile());
    {
    writeFlags(block1);
    }
  block1.close();

  block1.open(block.getFile());
    block1.begin(FILE_MODULE);
      {
      model->write(block1); // go to MLModule
      }
    block1.end();
  block1.close();
}

/////////////////////////////////////////////////////////////////////////////

void ModelModuleIO::writeMainBlock( MLWFileBlock& block )
{
  block.begin(FILE_SNAPSHOT);
    {
    MLPicture* snap= 0;

    if( model->flags->writeSnapshot )
      snap= model->getSnapshotOfScreen();
    if( snap == 0 )
      {
      block << int(0);
      }
     else
      {
      block << int(1);
      block << int(snap->getWidth());
      block << int(snap->getHeight());

      int length= snap->getWidth() * snap->getHeight() * 3;
      unsigned char* compressed= new unsigned char [ length ];
      int compressed_size= 0;
      boolean outcome= MLLibZ::deflate( (void*) snap->getData(), length,
                                      (void*) compressed, length,
                                      &compressed_size, 6 );
      block << int(outcome);
      if( outcome )
        {
        block << int(compressed_size);
        block.write( (const void*) compressed, compressed_size );
        }
       else
        block.write( (const void*) snap->getData(), length );
      delete compressed;
      compressed= 0;
      }
    delete snap;
    }
  block.end();

  block.begin(FILE_INFO);
    {
    MLFileInfo::write( MLModelVersion::version(),
                       MLModelVersion::build_cpu(),
                       MLModelVersion::build_vendor(),
                       MLModelVersion::build_os(),
                       block );
    }
  block.end();

  MLScene* scene= model->getScene();

  block.begin(FILE_MAIN);
    {
    block << scene->getName();
    writeInterfaceBlock(block);
    }
  block.end();
}

/////////////////////////////////////////////////////////////////////////////

boolean ModelModuleIO::write( MLDatabase* database )
{
  MLString filename= ModelModuleIO::getProjectFilename(model,database);

  MLWFile file;
  if( ! file.create(filename) )
    return false;

  if( ! file.init(::modelModuleSignature) )
    {
    file.close();
    ListFiles::removeFile(filename);
    return false;
    }

  model->pushWaitCursor();

  MLWFileBlock& block= file.getMainBlock();
  writeMainBlock(block);

  file.close();

  model->popWaitCursor();

  MLIOProgressDialog* progress= new MLIOProgressDialog(model);
  progress->create( WritingProgressDlg::data,
                    WritingProgressDlg::progress,
                    WritingProgressDlg::label );
  progress->empty();
  progress->setMessage("Writing scene...");
  progress->flush();

  model->pushWaitCursor();

  MLScene* scene= model->getScene();

  boolean ok= MLSceneIO::write( database, scene, scene->getName(), progress );

  delete progress;
  progress= 0;

  model->popWaitCursor();

  return ok;
}

/////////////////////////////////////////////////////////////////////////////

void ModelModuleIO::readFlags_1( MLRFileBlock& block )
{
  ModelFlags* flags= model->flags;

  block.go(FILE_FLAGS1);
    {
    flags->singleSelection= block.getInteger();
    flags->showCamera=      block.getInteger();
    flags->showHierStruct=  block.getInteger();
    flags->showTesselation= block.getInteger();
    flags->showNormals=     block.getInteger();
    flags->showHashBox=     block.getInteger();
    flags->writeSnapshot=   block.getInteger();

    flags->computeDirectLighting=   block.getInteger();
    flags->computeIndirectLighting= block.getInteger();
    }

  if( block.go(FILE_FLAGS2) )
    {
    flags->showObjectAxes= block.getInteger();
    flags->updateShadows=  block.getInteger();
    }
}

/////////////////////////////////////////////////////////////////////////////

void ModelModuleIO::readFlags_2( MLRFileBlock& block )
{
  ModelFlags* flags= model->flags;

  flags->singleSelection= (block.getInteger() != 0);
  flags->showCamera=      (block.getInteger() != 0);
  flags->showHierStruct=  (block.getInteger() != 0);
  flags->showTesselation= (block.getInteger() != 0);
  flags->showNormals=     (block.getInteger() != 0);
  flags->showHashBox=     (block.getInteger() != 0);
  flags->writeSnapshot=   (block.getInteger() != 0);

  flags->computeDirectLighting=   (block.getInteger() != 0);
  flags->computeIndirectLighting= (block.getInteger() != 0);

  flags->showObjectAxes= (block.getInteger() != 0);
  flags->updateShadows=  (block.getInteger() != 0);
}

/////////////////////////////////////////////////////////////////////////////

void ModelModuleIO::readFlags_3( MLRFileBlock& block )
{
  ModelFlags* flags= model->flags;

  flags->singleSelection= (block.getInteger() != 0);
  flags->showCamera=      (block.getInteger() != 0);
  flags->showHierStruct=  (block.getInteger() != 0);
  flags->showTesselation= (block.getInteger() != 0);
  flags->showNormals=     (block.getInteger() != 0);
  flags->showHashBox=     (block.getInteger() != 0);
  flags->writeSnapshot=   (block.getInteger() != 0);

  flags->computeDirectLighting=   (block.getInteger() != 0);
  flags->computeIndirectLighting= (block.getInteger() != 0);

  flags->showViewAxes=   (block.getInteger() != 0);
  flags->showObjectAxes= (block.getInteger() != 0);
  flags->updateShadows=  (block.getInteger() != 0);
}

/////////////////////////////////////////////////////////////////////////////

void ModelModuleIO::read( MLRFileBlock& block )
{
  MLRFileBlock block1;

  block1.open(block.getFile());

  int version;
  if( block.go(FILE_FLAGS1) )
    version= 1;
   else
    {
    block.go(FILE_FLAGS);
    version= block.getInteger();
    }

  switch( version )
    {
    case 1: readFlags_1(block1); break;
    case 2: readFlags_2(block1); break;
    case 3: readFlags_3(block1); break;
    default:
      break;
    }
  block1.close();

  MLScene* scene= model->getScene();
  const ModelFlags* flags= model->getFlags();
  scene->getGlobalLighting()->update_shadows= flags->updateShadows;

  block1.open(block.getFile());
  block1.go(FILE_MODULE);
    model->read(block1); // go to MLModule
  block1.close();

  MLRendererOptions* options= model->getScene()->getRendererOptions();
  options->current_system_gamma_correction= model->getGammaCorrectionValue();

  model->updateFlags();
}

/////////////////////////////////////////////////////////////////////////////

boolean ModelModuleIO::readMainBlock( MLRFileBlock& block,
                                    MLDatabase* database,
                                    MLIOProgress* progress,
                                    boolean read_scene /* = false */ )
{
#if defined(PRINT_INFO) && !defined(NDEBUG)
  if( block.go(FILE_INFO) )
    {
    MLFileInfo fi;
    fi.read(block);
    printf( "Software version: %s\n", fi.version.get() );
    printf( "         Creator: %s\n", fi.user.get() );
    printf( "          Target: %s-%s-%s\n", fi.cpu.get(),
                                            fi.vendor.get(),
                                            fi.os.get() );
    printf( "            Date: %s\n", fi.getTimeOfDay().get() );
    }
#endif

  block.go(FILE_MAIN);
    {
    MLName sceneName;
    MLScene* scene= model->getScene();

    block >> sceneName;
    if( read_scene )
      {
      if( ! MLSceneIO::read( database, scene, sceneName, progress ) )
        return false;
      }

    read(block);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean ModelModuleIO::read( const MLString& filename, MLDatabase* database )
{
  MLRFile file;
  if( ! file.open(filename) )
    return false;

  if( ! file.init(::modelModuleSignature) )
    {
    file.close();
    return false;
    }

  model->stopLightingProcess();

  model->resetAll();

  MLIOProgressDialog* progress= new MLIOProgressDialog(model);
  progress->create( ReadingProgressDlg::data,
                    ReadingProgressDlg::progress,
                    ReadingProgressDlg::label );
  progress->empty();
  progress->setMessage("Reading scene...");
  progress->flush();

  MLRFileBlock& block= file.getMainBlock();

  model->pushWaitCursor();

  if( ! readMainBlock(block,database,progress,true) )
    {
    file.close();
    delete progress;
    model->popWaitCursor();
    return false;
    }

  file.close();

  delete progress;
  progress= 0;

  model->popWaitCursor();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLPicture* ModelModuleIO::readSnapshot( const MLString& filename )
{
  MLRFile file;
  if( ! file.open(filename) )
    return 0;

  MLRFileBlock tmp_block;
  MLRFileBlock* block= 0;

  if( file.init(::modelModuleSignature) )
    {
    block= &(file.getMainBlock());
    }
   else
    {
    if( ! file.init(MLPackagedIO::signature) )
      return 0;

    MLRFileBlock& mainBlock= file.getMainBlock();
    if( ! mainBlock.go(MLPackagedIO::PKG_PROJECT) )
      return 0;

    if( ! tmp_block.open(&file) )
      return 0;

    block= &tmp_block;
    }

  if( ! block->go(FILE_SNAPSHOT) )
    return 0;

  int hasSnap;
  *block >> hasSnap;
  if( ! hasSnap )
    return 0;

  int width, height;
  *block >> width >> height;

  MLPicture* snap= new MLPicture;
  snap->createRGB(width,height);

  int data_are_compressed;
  *block >> data_are_compressed;
  if( data_are_compressed )
    {
    int compressed_size;
    *block >> compressed_size;
    unsigned char* compressed= new unsigned char [ compressed_size ];
    block->read( (void*) compressed, compressed_size );
    MLLibZ::inflate( (void*) compressed, compressed_size,
                     (void*) snap->getData(), width*height*3 );
    delete compressed;
    }
   else
    block->read( (void*) snap->getData(), width*height*3 );

  return snap;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString ModelModuleIO::getProjectFilename( ModelModule* model,
                                           MLDatabase* database )
{
  const MLName& sceneName= model->scene->getName();
  MLString n= sceneName.name;
  n += MLDatabase::sepChar;
  n += MLDatabase::buildVersionString(sceneName);
  n += MLDatabase::projectExtension;

  return ((database==0) ? MLString("") : database->getProjectPath())
         + n;
}

/////////////////////////////////////////////////////////////////////////////
