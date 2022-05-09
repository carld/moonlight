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
  MLFTranslate.C

  Stephane Rehel
  August 2 1997
*/

#include <string.h>

#include "MLFTranslate.h"

#include "tools/IntegerArray.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelModes.h"
#include "kmodel/ModelCommands.h"
#include "kmodel/ModelScene.h"

#include "kmodel/commands/MLCTranslate.h"

#include "kmodel/modes/MLMDrag.h"

/////////////////////////////////////////////////////////////////////////////

MLFTranslate::MLFTranslate( MLModule* _module ):
  MLBaseFunction(_module)
{}

/////////////////////////////////////////////////////////////////////////////

MLFTranslate::~MLFTranslate()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLFTranslate::_call()
{
  module->enterMode(ModelModes::DRAG);

  ModelModule* model= (ModelModule*) module;

  model->pushMessage("Translate: L(XYZ)" );

  while( ! exitFunction )
    {
    if( ! interfaceYield() )
      break;

    MLMDrag* d= (MLMDrag*) model->getMode(ModelModes::DRAG);
    if( d == 0 )
      break;

    MLCTranslate* cmd= new MLCTranslate;
    cmd->v= d->v;
    cmd->lonelyCommand= 0;

    model->getModelScene()->getSelectedObjects(cmd);

    if( cmd->indices.getSize() == 0 )
      {
      delete cmd;
      continue;
      }

    model->post(cmd);
    }

  model->popMessage();

  module->leaveMode();

  return true;
}

/////////////////////////////////////////////////////////////////////////////
