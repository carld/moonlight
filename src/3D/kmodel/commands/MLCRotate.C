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
  MLCRotate.C

  Stephane Rehel

  November 23 1997
*/

#include "tools/IToggle.h"

#include "scene/MLScene.h"

#include "kmodel/ModelScene.h"
#include "kmodel/ModelCommands.h"
#include "kmodel/ModelFlags.h"
#include "kmodel/ModelModule.h"

#include "MLCRotate.h"

/////////////////////////////////////////////////////////////////////////////

MLCRotate::MLCRotate()
{
  ModelCommand::type= ModelCommands::ROTATE;
}

/////////////////////////////////////////////////////////////////////////////

MLCRotate::~MLCRotate()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLCRotate::doit( ModelModule* model )
{
  if( model == 0 )
    return true;

  ModelScene* modelScene= model->getModelScene();

  MLCRotate* undo= new MLCRotate;
  ModelCommand::createUndo(undo);

  switch(model->getFlags()->modellingComponent)
    {
    case ModelFlags::COMPONENT_OBJ:
      {
      model->stopLightingProcess();

      ModelCommand::undo->v= -v;

      modelScene->rotate(v);

      model->refreshScreen(ModelModule::REFRESH_TRANSFORM);
      break;
      }

    case ModelFlags::COMPONENT_VTX:
      {
      model->disableLighting();

      ModelCommand::undo->v= -v;

      modelScene->rotateVertices(this);

      model->refreshScreen(ModelModule::REFRESH_GEOMETRY);
      break;
      }
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
