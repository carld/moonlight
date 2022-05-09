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
  MLCWorldRotate.C

  Stephane Rehel

  September 21 1997
*/

#include "kmodel/ModelModule.h"
#include "kmodel/ModelCommands.h"
#include "kmodel/ModelFlags.h"
#include "kmodel/ModelScene.h"

#include "MLCSetXForms.h"
#include "MLCWorldRotate.h"

/////////////////////////////////////////////////////////////////////////////

MLCWorldRotate::MLCWorldRotate()
{
  type= ModelCommands::WORLD_ROTATE;

  axis= Vector(0,0,1);
  angle= 0.;
}

/////////////////////////////////////////////////////////////////////////////

MLCWorldRotate::~MLCWorldRotate()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLCWorldRotate::doit( ModelModule* model )
{
  if( model == 0 )
    return true;

  ModelScene* modelScene= model->getModelScene();

  switch(model->getFlags()->modellingComponent)
    {
    case ModelFlags::COMPONENT_OBJ:
      {
      MLCSetXForms* undo= new MLCSetXForms;
      ModelCommand::createUndo(undo);
      undo->make( model->getScene(), indices );

      model->stopLightingProcess();

      modelScene->world_rotate( axis, angle );

      model->refreshScreen(ModelModule::REFRESH_TRANSFORM);
      break;
      }

    case ModelFlags::COMPONENT_VTX:
      {
      MLCWorldRotate* undo= new MLCWorldRotate;
      ModelCommand::createUndo(undo);
      undo->axis = axis;
      undo->angle=-angle;

      model->disableLighting();

      modelScene->world_rotateVertices(this,axis,angle);

      model->refreshScreen(ModelModule::REFRESH_GEOMETRY);
      break;
      }
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
