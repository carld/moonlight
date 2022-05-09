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
  MLFInstantiate.C

  Stephane Rehel

  October 4 1997
*/

#include <string.h>

#include "MLFInstantiate.h"

#include "tools/IntegerArray.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelModes.h"
#include "kmodel/ModelCommand.h"
#include "kmodel/ModelCommands.h"
#include "kmodel/ModelFlags.h"

#include "kmodel/modes/MLMSelect.h"

/////////////////////////////////////////////////////////////////////////////

MLFInstantiate::MLFInstantiate( MLModule* _module ):
  MLBaseFunction(_module)
{}

/////////////////////////////////////////////////////////////////////////////

MLFInstantiate::~MLFInstantiate()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLFInstantiate::_call()
{
  module->enterMode(ModelModes::SELECT);

  ModelModule* model= (ModelModule*) module;

  if( model->getFlags()->singleSelection )
    model->pushMessage( "Select object to instantiate" );
   else
    model->pushMessage( "Select object(s) to instantiate" );

  while( ! exitFunction )
    {
    if( ! interfaceYield() )
      break;

    MLMSelect* s= (MLMSelect*) model->getMode(ModelModes::SELECT);
    if( s == 0 )
      break;
    if( s->objects.getSize() == 0 )
      continue;

    ModelCommand* cmd= new ModelCommand;
    cmd->type= ModelCommands::INSTANTIATE;
    cmd->indices= s->objects;

    if( model->getFlags()->singleSelection )
      cmd->indices.truncate(1);

    model->post(cmd);
    }

  model->popMessage();

  module->leaveMode();

  return true;
}

/////////////////////////////////////////////////////////////////////////////
