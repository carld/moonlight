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
  MLFReparent.C

  Stephane Rehel

  August 5 1997
*/

#include <string.h>

#include "MLFReparent.h"

#include "tools/IntegerArray.h"

#include "scene/MLScene.h"
#include "scene/MLObject.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelScene.h"
#include "kmodel/ModelModes.h"
#include "kmodel/ModelCommand.h"
#include "kmodel/ModelCommands.h"

#include "kmodel/modes/MLMSelect.h"

/////////////////////////////////////////////////////////////////////////////

MLFReparent::MLFReparent( MLModule* _module ):
  MLBaseFunction(_module)
{}

/////////////////////////////////////////////////////////////////////////////

MLFReparent::~MLFReparent()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLFReparent::_call()
{
  ModelModule* model= (ModelModule*) module;
  MLScene* scene= model->getScene();
  ModelScene* modelScene= model->getModelScene();

  module->enterMode(ModelModes::SELECT);
  MLString pre_string_1= "(1/2)";

  while( ! exitFunction )
    {
    model->printMessage( pre_string_1
                        +MLString(" Select child object to reparent") );

    if( ! interfaceYield() )
      break;
    if( exitFunction )
      break;

    MLMSelect* s= (MLMSelect*) model->getMode(ModelModes::SELECT);
    if( s == 0 )
      break;

    modelScene->removeAllHighlighting();
    modelScene->setHighlighted(s->objects);
    model->refreshScreen(ModelModule::REFRESH_SELECTION);

    if( s->objects.getSize() != 1 )
      {
      model->printMessage("Please select one single object",2);
      break;
      }

    int child_id= s->objects[0];
    MLObject* child= scene->getObject(child_id);
    if( child == 0 )
      continue;

    model->printMessage( MLString("(2/2) Select parent for '")
                        +child->getName().name
                        +MLString("'") );

    if( ! interfaceYield() )
      break;

    s= (MLMSelect*) model->getMode(ModelModes::SELECT);

    modelScene->removeAllHighlighting();
    modelScene->setHighlighted(s->objects);
    model->refreshScreen(ModelModule::REFRESH_SELECTION);

    if( s->objects.getSize() > 1 )
      {
      model->printMessage("Please select one single object",2);
      break;
      }

    int parent_id= (s->objects.getSize()==0) ? 0 : s->objects[0];

    if( parent_id == child_id )
      parent_id= 0; // reparent child to root
     else
      {
      MLObject* parent= scene->getObject(parent_id);
      if( parent != 0 )
        {
        if( parent->isChildOf(child) )
          {
          // sillyboy, you are trying to make a loop! Mmmhh.
          pre_string_1= "(Silly boy)";
          continue;
          }
        }
      }

    ModelCommand* cmd= new ModelCommand;
    cmd->type= ModelCommands::REPARENT;
    cmd->indices.alloc(2);
    cmd->indices[0]= child_id;
    cmd->indices[1]= parent_id;

    model->post(cmd);

    pre_string_1= "(1/2)";
    }

  if( modelScene->removeAllHighlighting() )
    model->refreshScreen(ModelModule::REFRESH_SELECTION);

  model->printMessage("");

  module->leaveMode();

  return true;
}

/////////////////////////////////////////////////////////////////////////////
