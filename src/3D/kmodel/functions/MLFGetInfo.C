// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//                 2002       Patrick Mauritz
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
  MLFGetInfo.C

  Stephane Rehel
  August 2 1997
*/

#include <string.h>

#include "MLFGetInfo.h"

#include "tools/IntegerArray.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelScene.h"
#include "kmodel/ModelModes.h"
#include "kmodel/ModelCommand.h"
#include "kmodel/ModelCommands.h"
#include "kmodel/ModelFlags.h"

#include "kmodel/modes/MLMSelect.h"

/////////////////////////////////////////////////////////////////////////////

MLFGetInfo::MLFGetInfo( MLModule* _module ):
  MLBaseFunction(_module)
{}

/////////////////////////////////////////////////////////////////////////////

MLFGetInfo::~MLFGetInfo()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLFGetInfo::_call()
{
  module->enterMode(ModelModes::SELECT);

  ModelModule* model= (ModelModule*) module;
  ModelScene* modelScene= model->getModelScene();

  model->pushMessage( "Select object for information" );

  while( ! exitFunction )
    {
    if( ! interfaceYield() )
      break;

    MLMSelect* s= (MLMSelect*) model->getMode(ModelModes::SELECT);
    if( s == 0 )
      break;

    if( s->objects.getSize() > 1 )
      {
      // error
//      model->printMessage( "Use SingleSelection for info", 2 );
//      continue;
      s->objects.truncate(1);
      }

    if( s->objects.getSize() == 0 )
      continue;

    modelScene->removeAllHighlighting();
    modelScene->setHighlighted(s->objects);
    model->refreshScreen(ModelModule::REFRESH_SELECTION);

    ModelCommand* cmd= new ModelCommand;
    cmd->type= ModelCommands::GET_INFO;
    cmd->indices= s->objects;

    model->post(cmd);
    }

  if( modelScene->removeAllHighlighting() )
    model->refreshScreen(ModelModule::REFRESH_SELECTION);

  model->popMessage();

  module->leaveMode();

  return true;
}

/////////////////////////////////////////////////////////////////////////////
