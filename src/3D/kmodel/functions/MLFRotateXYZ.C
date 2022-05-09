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
  MLFRotateXYZ.C

  Stephane Rehel
  August 2 1997
*/

#include <string.h>

#include "MLFRotateXYZ.h"

#include "tools/IntegerArray.h"

#include "kmodel/commands/MLCRotate.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelModes.h"
#include "kmodel/ModelCommand.h"
#include "kmodel/ModelCommands.h"
#include "kmodel/ModelScene.h"

#include "kmodel/modes/MLMDrag.h"

/////////////////////////////////////////////////////////////////////////////

MLFRotateXYZ::MLFRotateXYZ( MLModule* _module ):
  MLBaseFunction(_module)
{}

/////////////////////////////////////////////////////////////////////////////

MLFRotateXYZ::~MLFRotateXYZ()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLFRotateXYZ::_call()
{
  module->enterMode(ModelModes::DRAG);

  ModelModule* model= (ModelModule*) module;

  model->pushMessage( "Rotate: L(X)  M(Y)  R(Z)" );

  while( ! exitFunction )
    {
    if( ! interfaceYield() )
      break;

    MLMDrag* d= (MLMDrag*) model->getMode(ModelModes::DRAG);
    if( d == 0 )
      break;

    MLCRotate* cmd= new MLCRotate;
    cmd->lonelyCommand= 0;

    model->getModelScene()->getSelectedObjects(cmd);

    if( cmd->indices.getSize() == 0 )
      {
      delete cmd;
      continue;
      }

    double angle= d->screenVector.norm() * M_PI / 2.;
    if( ! d->growing )
      angle= -angle;
    Vector t;
    switch( d->button )
      {
      case 3:  t= Vector(0,0,angle); break;
      case 2:  t= Vector(0,angle,0); break;
      case 1:
      default: t= Vector(angle,0,0); break;
      }

    cmd->v= t;

    model->post(cmd);
    }

  model->popMessage();

  module->leaveMode();

  return true;
}

/////////////////////////////////////////////////////////////////////////////
