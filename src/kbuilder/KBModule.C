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
  KBModule.C

  Stephane Rehel

  October 19 1997
*/

#include <stdio.h>

#include "tools/MLCommandLine.h"

#include "graphics/SystemWindow.h"

#include "interface/MLAllocator.h"
#include "interface/MLInterfaceCommand.h"
#include "interface/MLScrollingDialog.h"
#include "interface/MLMode.h"
#include "interface/MLStatusLineText.h"

#include "KBCanvas.h"
#include "KBModes.h"
#include "KBFunctions.h"
#include "KBCommand.h"
#include "KBStatusBars.h"
#include "KBPopups.h"
#include "KBModuleKeyboardHandler.h"

#include "KBModule.h"

/////////////////////////////////////////////////////////////////////////////

KBModule::KBModule( Interface* _interface ): MLModule(_interface)
{
  mcanvas= 0;
  keyboardHandler= 0;

  status= 0;

  popups= new KBPopups(this);

  MLModule::name= MLString("KBuilder");
  MLModule::color= Color(0.25,0.20,0.05);

  KBModes::init(this,modes);
  KBFunctions::init(this);

  KBCommand::initKBCommands();
}

/////////////////////////////////////////////////////////////////////////////

KBModule::~KBModule()
{
  delete mcanvas; mcanvas= 0;

  delete status;
  status= 0;

  delete popups;
  popups= 0;

  MLModule::setModuleKeyboardHandler(0);

  delete keyboardHandler;
  keyboardHandler= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean KBModule::init( MLAppRoot* _approot )
{
  if( ! MLModule::init(_approot) )
    return false;

  status= new KBStatusBars(this,statusBars);
  status->init();

  mcanvas= new KBCanvas(this);
  mcanvas->create( CANVAS_1234 );

  popups->init(MLModule::buttonsPanel);

  updatePopupsFlags();

  createDialog("kBuilder");

  keyboardHandler= new KBModuleKeyboardHandler(this);
  MLModule::setModuleKeyboardHandler(keyboardHandler);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void KBModule::updatePopupsFlags()
{
  if( popups != 0 )
    popups->updateFlags();
}

/////////////////////////////////////////////////////////////////////////////

void KBModule::updateStatusFlags()
{
  if( status != 0 )
    status->update();
}

/////////////////////////////////////////////////////////////////////////////

void KBModule::updateFlags()
{
  updateStatusFlags();
  updatePopupsFlags();
}

/////////////////////////////////////////////////////////////////////////////

void KBModule::idle()
{}

/////////////////////////////////////////////////////////////////////////////

void KBModule::map()
{
  if( _mapped )
    return;

  MLModule::map();

  if( mcanvas != 0 )
    mcanvas->map();
}

/////////////////////////////////////////////////////////////////////////////

void KBModule::draw()
{
  if( ! _mapped )
    return;

  if( mcanvas != 0 )
    mcanvas->titleDraw();

  MLModule::draw();
}

/////////////////////////////////////////////////////////////////////////////

Canvas* KBModule::getBuilderCanvas() const
{
  if( mcanvas == 0 )
    return 0;

  return mcanvas->getBuilderCanvas();
}

/////////////////////////////////////////////////////////////////////////////

// color: 0 = gray
//        1 = white
//        2 = yellow
void KBModule::printMessage( const MLString& message, int color /* = 1 */ )
{
  if( status->message == 0 )
    return;

  status->message->printMessage(message,color);
}

/////////////////////////////////////////////////////////////////////////////

// color: 0 = gray
//        1 = white
//        2 = yellow
void KBModule::pushMessage( const MLString& message, int color /* = 1 */ )
{
  if( status->message == 0 )
    return;

  status->message->pushMessage(message,color);
}

/////////////////////////////////////////////////////////////////////////////

void KBModule::popMessage()
{
  if( status->message == 0 )
    return;

  status->message->popMessage();
}

/////////////////////////////////////////////////////////////////////////////

void KBModule::printWidgetMessage( const MLString& message )
{
  if( status->widgetMessage == 0 )
    return;

  status->widgetMessage->printMessage(message);
}

/////////////////////////////////////////////////////////////////////////////

void KBModule::handleCommand( MLCommand* cmd )
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
/*
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
*/
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

  if( cmd->getBase() == KBCommand::BASE )
    {
    handleKBCommand( (KBCommand*) cmd );
    return;
    }

  MLModule::handleCommand(cmd);
}

/////////////////////////////////////////////////////////////////////////////

void KBModule::enterMode( MLMode* mm )
{
  MLModule::enterMode(mm);

  if( mm != 0 && status->modeInfo != 0 )
    status->modeInfo->pushMessage(mm->getName());
}

/////////////////////////////////////////////////////////////////////////////

void KBModule::leaveMode( MLMode* mm /* = 0 */ )
{
  if( status->modeInfo != 0 )
    status->modeInfo->popMessage();

  MLModule::leaveMode(mm);
}

/////////////////////////////////////////////////////////////////////////////

int KBModule::getCanvasIndex( MLCanvas* c ) const
{
  if( c == mcanvas ) return 1;

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void KBModule::refreshScreen( unsigned int what )
{
  if( mcanvas != 0 )
    mcanvas->refreshScreen(what);

  MLModule::refreshScreen(what);
}

/////////////////////////////////////////////////////////////////////////////

void KBModule::resetAll()
{
  updatePopupsFlags();
  updateStatusFlags();

  MLModule::resetAll();

  refreshScreen(REFRESH_ALL);
}

/////////////////////////////////////////////////////////////////////////////
