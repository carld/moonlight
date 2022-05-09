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
  OOMModule.C

  Stephane Rehel

  November 3 1997
*/

#include <stdio.h>

#include "tools/MLCommandLine.h"

#include "graphics/SystemWindow.h"

#include "interface/MLAllocator.h"
#include "interface/MLInterfaceCommand.h"
#include "interface/MLScrollingDialog.h"
#include "interface/MLMode.h"
#include "interface/MLStatusLineText.h"

#include "OOMCanvas.h"
#include "OOMModes.h"
#include "OOMFunctions.h"
#include "OOMColors.h"
#include "OOMCommand.h"
#include "OOMStatusBars.h"
#include "OOMPopups.h"

#include "OOMModule.h"

#include "model/OOModel.h"

/////////////////////////////////////////////////////////////////////////////

OOMModule::OOMModule( Interface* _interface ): MLModule(_interface)
{
  mcanvas[0]= mcanvas[1]= mcanvas[2]= mcanvas[3]= mcanvas[4]= 0;

  model= new OOModel;

  status= 0;

  popups= new OOMPopups(this);

  MLModule::name= MLString("OOM");
  MLModule::color= Color(1,1,1)*0.20;

  OOMModes::init(this,modes);
  OOMFunctions::init(this);
  OOMColors::init(this);

  OOMCommand::initOOMCommands();
}

/////////////////////////////////////////////////////////////////////////////

OOMModule::~OOMModule()
{
  delete mcanvas[1]; mcanvas[1]= 0;
  delete mcanvas[2]; mcanvas[2]= 0;
  delete mcanvas[3]; mcanvas[3]= 0;
  delete mcanvas[4]; mcanvas[4]= 0;

  delete status;
  status= 0;

  delete popups;
  popups= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean OOMModule::init( MLAppRoot* _approot )
{
  if( ! MLModule::init(_approot) )
    return false;

  status= new OOMStatusBars(this,statusBars);
  status->init();

  mcanvas[1]= new OOMCanvas(this);
  mcanvas[1]->create( CANVAS_1 );

  mcanvas[2]= new OOMCanvas(this);
  mcanvas[2]->create( CANVAS_2 );

  mcanvas[3]= new OOMCanvas(this);
  mcanvas[3]->create( CANVAS_3 );

  mcanvas[4]= new OOMCanvas(this);
  mcanvas[4]->create( CANVAS_4 );

  popups->init(buttonsPanel);
  updatePopupsFlags();

  createDialog("Welcome");

//  MLInterfaceCommand* cmd= new MLInterfaceCommand;
//  cmd->type= MLInterfaceCommand::MAXIMIZE_CANVAS;
//  mcanvas[1]->post(cmd);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void OOMModule::updatePopupsFlags()
{
  if( popups != 0 )
    popups->updateFlags();
}

/////////////////////////////////////////////////////////////////////////////

void OOMModule::updateStatusFlags()
{
  if( status != 0 )
    status->update();
}

/////////////////////////////////////////////////////////////////////////////

void OOMModule::updateFlags()
{
  updateStatusFlags();
  updatePopupsFlags();
}

/////////////////////////////////////////////////////////////////////////////

void OOMModule::idle()
{}

/////////////////////////////////////////////////////////////////////////////

void OOMModule::map()
{
  if( _mapped )
    return;

  MLModule::map();

  if( mcanvas[1] != 0 ) mcanvas[1]->map();
  if( mcanvas[2] != 0 ) mcanvas[2]->map();
  if( mcanvas[3] != 0 ) mcanvas[3]->map();
  if( mcanvas[4] != 0 ) mcanvas[4]->map();
}

/////////////////////////////////////////////////////////////////////////////

void OOMModule::draw()
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
void OOMModule::printMessage( const MLString& message, int color /* = 1 */ )
{
  if( status->message == 0 )
    return;

  status->message->printMessage(message,color);
}

/////////////////////////////////////////////////////////////////////////////

// color: 0 = gray
//        1 = white
//        2 = yellow
void OOMModule::pushMessage( const MLString& message, int color /* = 1 */ )
{
  if( status->message == 0 )
    return;

  status->message->pushMessage(message,color);
}

/////////////////////////////////////////////////////////////////////////////

void OOMModule::popMessage()
{
  if( status->message == 0 )
    return;

  status->message->popMessage();
}

/////////////////////////////////////////////////////////////////////////////

void OOMModule::handleCommand( MLCommand* cmd )
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

  if( cmd->getBase() == OOMCommand::BASE )
    {
    handleOOMCommand( (OOMCommand*) cmd );
    return;
    }

  MLModule::handleCommand(cmd);
}

/////////////////////////////////////////////////////////////////////////////

void OOMModule::enterMode( MLMode* mm )
{
  MLModule::enterMode(mm);

  if( mm != 0 && status->modeInfo != 0 )
    status->modeInfo->pushMessage(mm->getName());
}

/////////////////////////////////////////////////////////////////////////////

void OOMModule::leaveMode( MLMode* mm /* = 0 */ )
{
  if( status->modeInfo != 0 )
    status->modeInfo->popMessage();

  MLModule::leaveMode(mm);
}

/////////////////////////////////////////////////////////////////////////////

int OOMModule::getCanvasIndex( MLCanvas* c ) const
{
  if( c == mcanvas[1] ) return 1;
  if( c == mcanvas[2] ) return 2;
  if( c == mcanvas[3] ) return 3;
  if( c == mcanvas[4] ) return 4;

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void OOMModule::refreshScreen( unsigned int what )
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

void OOMModule::resetAll()
{
  updatePopupsFlags();
  updateStatusFlags();

  MLModule::resetAll();

  refreshScreen(REFRESH_ALL);
}

/////////////////////////////////////////////////////////////////////////////
