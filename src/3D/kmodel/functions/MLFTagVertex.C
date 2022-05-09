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
  MLFTagVertex.C

  Stephane Rehel

  October 28 1997
*/

#include "MLFTagVertex.h"

#include "scene/MLObject.h"
#include "scene/MLObjectsIterator.h"
#include "scene/MLScene.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelModes.h"
#include "kmodel/ModelCommands.h"
#include "kmodel/ModelFlags.h"

#include "kmodel/commands/MLCTagVertex.h"
#include "kmodel/commands/MLCSetVariable.h"

#include "kmodel/modes/MLMTagVertex.h"
#include "kmodel/MLTagVertexArray.h"


/////////////////////////////////////////////////////////////////////////////

MLFTagVertex::MLFTagVertex( MLModule* _module ):
  MLBaseFunction(_module)
{}

/////////////////////////////////////////////////////////////////////////////

MLFTagVertex::~MLFTagVertex()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLFTagVertex::_call()
{
  module->enterMode(ModelModes::TAG_VERTEX);

  ModelModule* model= (ModelModule*) module;

  if( model->getFlags()->modellingComponent != ModelFlags::COMPONENT_VTX )
    model->post( new MLCSetVariable( ModelFlags::MODELLING_COMPONENT,
                                     int(ModelFlags::COMPONENT_VTX) ) );

  model->pushMessage( "Tag Vertex: L(toggle)  M(select)  R(unselect)" );

  while( ! exitFunction )
    {
    if( ! interfaceYield() )
      break;

    MLMTagVertex* tv= (MLMTagVertex*) model->getMode(ModelModes::TAG_VERTEX);

    if( tv == 0 )
      break;

    if( tv->vertices->getNVertices() == 0 )
      continue;

    MLCTagVertex* cmd= new MLCTagVertex(model->getScene());

    switch(tv->button)
      {
      case 2:
        {
        cmd->type= ModelCommands::TAG_VERTEX_SET;
        cmd->set= *(tv->vertices);
        break;
        }
      case 3:
        {
        cmd->type= ModelCommands::TAG_VERTEX_SET;
        cmd->unset= *(tv->vertices);
        break;
        }
      case 1:
      default:
        {
        cmd->type= ModelCommands::TAG_VERTEX_TOGGLE;
        cmd->set= *(tv->vertices);
        break;
        }
      }

    model->post(cmd);
    }

  model->popMessage();

  module->leaveMode();

  return true;
}

/////////////////////////////////////////////////////////////////////////////
