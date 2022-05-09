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
  ModelModule.C

  Stephane Rehel

  November 15 1996
*/

#include <stdio.h>

#include "GL/opengl.h"

#include "tools/MLCommandLine.h"
#include "tools/thread/MLThread.h"
#include "tools/MLLog.h"

#include "graphics/SystemWindow.h"

#include "interface/MLAllocator.h"
#include "interface/MLInterfaceCommand.h"
#include "interface/MLScrollingDialog.h"
#include "interface/MLScrollingDialogs.h"
#include "interface/MLMode.h"
#include "interface/MLStatusLineEntryLogging.h"

#include "ModelCanvas.h"

#include "scene/MLCamera.h"
#include "scene/MLObject.h"
#include "scene/MLScene.h"
#include "scene/file/MLDatabasesList.h"

#include "ModelColors.h"
#include "ModelModes.h"
#include "ModelFunctions.h"
#include "ModelCommand.h"
#include "ModelScene.h"
#include "ModelCommandLine.h"

#include "MLLightingProcess.h"

#include "MLStatusLightingSwitch.h"
#include "ModelStatusBars.h"
#include "ModelLogListener.h"
#include "ModelStatusLineEntry.h"

#include "ModelFlags.h"
#include "ModelPopups.h"

#include "ModelModule.h"

#include "render/ModelRender.h"
#include "dialogs/MLDModesList.h"

/////////////////////////////////////////////////////////////////////////////

ModelModule::ModelModule( Interface* _interface ): MLModule(_interface)
{
  modelRender= 0;

  scene=  0;

  flags= 0;
  popups= 0;

  mcanvas[0]= mcanvas[1]= mcanvas[2]= mcanvas[3]= mcanvas[4]= 0;

  modelScene= 0;

  lightingThread= 0;
  lightingProcess= 0;

  status= 0;

  modelLogListener= 0;
  modelCommandLine= 0;
  entryLogging= 0;
  log= 0;
}

/////////////////////////////////////////////////////////////////////////////

ModelModule::~ModelModule()
{
  if( lightingProcess != 0 )
    {
    lightingProcess->quitThread();
    delete lightingProcess;
    lightingProcess= 0;
    }

  delete lightingThread;
  lightingThread= 0;

  for( int i= 1; i <= 4; ++i )
    {
    ModelCanvas* mc= mcanvas[i];
    if( mc == 0 )
      continue;

    mcanvas[i]= 0;
    delete mc;
    }

  delete modelScene;
  modelScene= 0;

  delete scene;
  scene= 0;

  delete modelCommandLine;
  modelCommandLine= 0;

  delete entryLogging;
  entryLogging= 0;

  delete status;
  status= 0;

  delete flags;
  flags= 0;

  delete popups;
  popups= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean ModelModule::init()
{
  scene= new MLScene(kernel);
  if( ! scene->init() )
    return false;

  scene->setConfigGroup(rc_group);

  log= scene->getLog();

  if( ! MLModule::init( (MLAppRoot*) scene ) )
    return false;;

  flags= new ModelFlags(this);
  popups= new ModelPopups(this);

  resetFlags();

  mcanvas[0]= mcanvas[1]= mcanvas[2]= mcanvas[3]= mcanvas[4]= 0;

  lightingThread= new MLThread;
  lightingProcess= new MLLightingProcess(this);

  MLModule::name= MLString("Scene");
  MLModule::color= Color(0.10,0.10,0.25);

  ModelColors::init(this);
  ModelModes::init(this,modes);
  ModelFunctions::init(this);

  ModelCommand::initModelCommands();

//  MLCommandLine& command= getKernel()->getCommandLine();

  rc_group= "Moonlight 3D";

  MLDatabasesList::setConfig( MLModule::getConfig(), rc_group );

  status= new ModelStatusBars(this,statusBars);
  status->init();

  entryLogging= new MLStatusLineEntryLogging(this);

  modelCommandLine= new ModelCommandLine(this);
  status->logs->setCallback(modelCommandLine);

  modelLogListener= new ModelLogListener(this,status->logs);
  log->setListener(modelLogListener);

  scene->getLog()->setListener(modelLogListener);

  modelScene= new ModelScene(this,scene);

  mcanvas[1]= new ModelCanvas(this);
  mcanvas[1]->create( CANVAS_1 );

  mcanvas[2]= new ModelCanvas(this);
  mcanvas[2]->create( CANVAS_2 );

  mcanvas[3]= new ModelCanvas(this);
  mcanvas[3]->create( CANVAS_3 );

  mcanvas[4]= new ModelCanvas(this);
  mcanvas[4]->create( CANVAS_4 );

//  popups->init(MLModule::buttonsPanel);
  popups->init(buttonsPanel);
  updatePopupsFlags();

  // Before entering the multi-threaded part, do a glFlush()
  // This may fix the XError #0 ... ?!? (why not...)
  // /kepler May 28 1998
  glFlush();

  lightingThread->create(lightingProcess);

  createDialog("Welcome");
  modelScene->resetAll();

  enableLighting();

  log->printf("Welcome to Moonlight 3D.\n");

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void ModelModule::updatePopupsFlags()
{
  if( popups != 0 )
    popups->updateFlags();
}

/////////////////////////////////////////////////////////////////////////////

void ModelModule::updateStatusFlags()
{
  if( status != 0 )
    status->update();
}

/////////////////////////////////////////////////////////////////////////////

void ModelModule::updateFlags()
{
  updateStatusFlags();
  updatePopupsFlags();
}

/////////////////////////////////////////////////////////////////////////////

void ModelModule::idle()
{
  if( scene == 0 )
    return;

  if( modelRender != 0 )
    {
    if( modelRender->isRendering() )
      return; // don't disturb it
    }

  if( lightingProcess == 0 )
    {
    status->dlt->setStatus(LS_DISABLED);
    status->ilt->setStatus(LS_DISABLED);
    return;
    }

  boolean run_it= false;

  if( scene->dirtyPoolsLighting ||
      scene->dirtyObjectsLighting )
    {
    scene->dirtyFinalLighting= false;

    if( flags->computeDirectLighting )
      run_it= true;

    goto update_status;
    }

  if( scene->dirtyFinalLighting )
    {
    scene->dirtyFinalLighting= false;
    refreshScreen(REFRESH_ILLUM);
    }

  if( flags->computeIndirectLighting )
    {
    // could be in the diffusing process, no matter.
    run_it= true;
    }

update_status:

  if( run_it )
    {
    // Check for boundings/hashboxes to update
    modelScene->updateBoundingHashBox();
    runLightingProcess();
    }

  if( flags->computeDirectLighting )
    {
    status->dlt->setStatus( lightingProcess->inDirectDiffusing()
                            ? (LS_WORKING1 + (lightingProcess->getStep() & 1))
                            : LS_ASLEEP );
    }
   else
    status->dlt->setStatus(LS_DISABLED);

  if( flags->computeIndirectLighting )
    {
    status->ilt->setStatus( lightingProcess->inIndirectDiffusing()
                            ? (LS_WORKING1 + (lightingProcess->getStep() & 1))
                            : LS_ASLEEP );
    }
   else
    status->ilt->setStatus(LS_DISABLED);
}

/////////////////////////////////////////////////////////////////////////////

void ModelModule::map()
{
  if( _mapped )
    return;

  MLModule::map();

//*** error when a canvas if maximized...
  if( mcanvas[1] != 0 ) mcanvas[1]->map();
  if( mcanvas[2] != 0 ) mcanvas[2]->map();
  if( mcanvas[3] != 0 ) mcanvas[3]->map();
  if( mcanvas[4] != 0 ) mcanvas[4]->map();
}

/////////////////////////////////////////////////////////////////////////////

void ModelModule::draw()
{
  if( ! _mapped )
    return;

  if( mcanvas[1] != 0 ) mcanvas[1]->titleDraw();
  if( mcanvas[2] != 0 ) mcanvas[2]->titleDraw();
  if( mcanvas[3] != 0 ) mcanvas[3]->titleDraw();
  if( mcanvas[4] != 0 ) mcanvas[4]->titleDraw();

  MLModule::draw();
}

/////////////////////////////////////////////////////////////////////////////

// color: 0 = gray
//        1 = white
//        2 = yellow
void ModelModule::printMessage( const MLString& message, int color /* = 1 */ )
{
  if( status->message == 0 )
    return;

  status->message->printMessage(message,color);
}

/////////////////////////////////////////////////////////////////////////////

// color: 0 = gray
//        1 = white
//        2 = yellow
void ModelModule::pushMessage( const MLString& message, int color /* = 1 */ )
{
  if( status->message == 0 )
    return;

  status->message->pushMessage(message,color);
}

/////////////////////////////////////////////////////////////////////////////

void ModelModule::popMessage()
{
  if( status->message == 0 )
    return;

  status->message->popMessage();
}

/////////////////////////////////////////////////////////////////////////////

void ModelModule::handleCommand( MLCommand* cmd )
{
  if( cmd == 0 )
    return;

  if( cmd->getBase() == MLInterfaceCommand::BASE )
    {
    MLInterfaceCommand* icmd= (MLInterfaceCommand*) cmd;
    switch( icmd->getType() )
      {
      case MLInterfaceCommand::MINIMIZE_CANVAS:
      case MLInterfaceCommand::MAXIMIZE_CANVAS:
        {
        MLCanvas* c= MLAllocator::getCanvas(icmd->canvas);
        if( c != 0 )
          {
          for( int i= 1; i <= 4; ++i )
            {
            if( mcanvas[i] == 0 || mcanvas[i] == c )
              continue;
            if( icmd->getType() == MLInterfaceCommand::MAXIMIZE_CANVAS )
              mcanvas[i]->unmap();
             else
              mcanvas[i]->map();
            }
          }
        MLModule::handleCommand(icmd);
        return;
        }

      case MLInterfaceCommand::POPUP:
        {
        handlePopupCommand(icmd->index);
        return;
        }

      default:
        break;
      }
    }

  if( cmd->getBase() == ModelCommand::BASE )
    {
    handleModelCommand( (ModelCommand*) cmd );
    return;
    }

  MLModule::handleCommand(cmd);
}

/////////////////////////////////////////////////////////////////////////////

void ModelModule::enterMode( MLMode* mm )
{
  MLModule::enterMode(mm);

  if( mm != 0 && status->modeInfo != 0 )
    status->modeInfo->pushMessage(mm->getName());
}

/////////////////////////////////////////////////////////////////////////////

void ModelModule::leaveMode( MLMode* mm /* = 0 */ )
{
  if( status->modeInfo != 0 )
    status->modeInfo->popMessage();

  MLModule::leaveMode(mm);
}

/////////////////////////////////////////////////////////////////////////////

int ModelModule::getCanvasIndex( MLCanvas* c ) const
{
  if( c == mcanvas[1] ) return 1;
  if( c == mcanvas[2] ) return 2;
  if( c == mcanvas[3] ) return 3;
  if( c == mcanvas[4] ) return 4;

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void ModelModule::refreshScreen( unsigned int what )
{
  for( int i= 1; i <= 4; ++i )
    {
    if( mcanvas[i] == 0 )
      continue;
    mcanvas[i]->refreshScreen(what);
    }

  MLModule::refreshScreen(what);
}

/////////////////////////////////////////////////////////////////////////////

void ModelModule::refreshMainCamera( const MLCamera& camera )
{
  for( int i= 1; i <= 4; ++i )
    {
    if( mcanvas[i] == 0 )
      continue;
    mcanvas[i]->refreshMainCamera(camera);
    }
}

/////////////////////////////////////////////////////////////////////////////

const MLCamera& ModelModule::getMainCamera() const
{
  if( mcanvas[2] == 0 )
    {
    static MLCamera tmp;
    MLCamera c;
    tmp= c; // force initialization
    return tmp;
    }
  return mcanvas[2]->getPerspectiveCamera();
}

/////////////////////////////////////////////////////////////////////////////

// f==0 if leaving current
void ModelModule::enteringFunction( MLFunction* f )
{
  MLScrollingDialog* sd= MLModule::dialogs->getDialog("ModesList");
  if( sd == 0 )
    return;

  MLDModesList* modesList= (MLDModesList*) sd;
  modesList->update(f);
}

/////////////////////////////////////////////////////////////////////////////

// return 0 if none selected or more than a single one
int ModelModule::getSingleSelected()
{
  return modelScene->getSingleSelected();
}

/////////////////////////////////////////////////////////////////////////////

boolean ModelModule::isProjectEmpty()
{
  if( scene->getRootObject()->getNChildren() > 0 )
    return false;

  if( scene->getNMaterials() != 0 )
    return false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void ModelModule::stopLightingProcess()
{
  if( lightingProcess == 0 )
    return;

  lightingProcess->stop();
}

/////////////////////////////////////////////////////////////////////////////

void ModelModule::runLightingProcess()
{
  if( lightingProcess == 0 )
    return;

  lightingProcess->compute();
}

/////////////////////////////////////////////////////////////////////////////

void ModelModule::disableLighting()
{
  if( !flags->computeDirectLighting && !flags->computeIndirectLighting )
    return;

  stopLightingProcess();

  flags->computeDirectLighting= false;
  flags->computeIndirectLighting= false;

  updateFlags();
}

/////////////////////////////////////////////////////////////////////////////

void ModelModule::enableLighting()
{
  if( flags->computeDirectLighting )
    return;

  stopLightingProcess();

  flags->computeDirectLighting= true;

  updateFlags();
}

/////////////////////////////////////////////////////////////////////////////

void ModelModule::resetFlags()
{
  flags->reset();
}

/////////////////////////////////////////////////////////////////////////////

void ModelModule::resetAll()
{
  stopLightingProcess();

  resetFlags();

  delete modelRender;
  modelRender= 0;

  updatePopupsFlags();
  updateStatusFlags();

  modelScene->resetAll();

  MLModule::resetAll();

  refreshScreen(REFRESH_ALL);
}

/////////////////////////////////////////////////////////////////////////////
