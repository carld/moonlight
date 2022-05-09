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
  MLFRotate.C

  Stephane Rehel

  September 21 1997
*/

#include <string.h>

#include "MLFRotate.h"

#include "tools/IntegerArray.h"

#include "kmodel/commands/MLCWorldRotate.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelModes.h"
#include "kmodel/ModelCommand.h"
#include "kmodel/ModelCommands.h"
#include "kmodel/ModelScene.h"

#include "kmodel/modes/MLMDrag.h"

/////////////////////////////////////////////////////////////////////////////

MLFRotate::MLFRotate( MLModule* _module ):
  MLBaseFunction(_module)
{}

/////////////////////////////////////////////////////////////////////////////

MLFRotate::~MLFRotate()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLFRotate::_call()
{
  module->enterMode(ModelModes::DRAG);

  ModelModule* model= (ModelModule*) module;

  model->pushMessage("Rotate: L(XYZ)" );

  while( ! exitFunction )
    {
    if( ! interfaceYield() )
      break;

    MLMDrag* d= (MLMDrag*) model->getMode(ModelModes::DRAG);
    if( d == 0 )
      break;

    Vector axis(0,0,0);
    double angle= 0.;
    if( d->ortho_camera )
      {
      Vector2 v1= Vector2( double(d->position1_x), double(d->position1_y) );
      Vector2 v2= Vector2( double(d->position2_x), double(d->position2_y) );
      double norm1= v1.norm();
      double norm2= v2.norm();
      if( norm1 < 1e-4 || norm2 < 1e-4 )
        continue;

      v1 /= norm1;
      v2 /= norm2;

      axis= -(d->view_vector.getNormalized());
      angle= acos( v1 | v2 );
      if( (v1^v2) < 0. )
        angle= -angle;
      }
     else
      {
      axis= d->v ^ d->view_vector;
      double axis_norm= axis.norm();
      if( axis_norm < 1e-20 )
        continue;

      axis /= axis_norm;

      const double speed= M_PI/2.;
      angle= -speed * d->screenVector.norm();
      }

    MLCWorldRotate* cmd= new MLCWorldRotate;
    cmd->axis= axis;
    cmd->angle= angle;
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
