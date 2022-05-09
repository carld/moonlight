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
  MLFOrientObject.C

  Stephane Rehel

  September 21 1997
*/

#include "MLFOrientObject.h"

#include "scene/MLScene.h"
#include "scene/MLObject.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelModes.h"
#include "kmodel/ModelFlags.h"
#include "kmodel/ModelScene.h"

#include "kmodel/modes/MLMRay.h"

#include "kmodel/commands/MLCWorldRotate.h"

/////////////////////////////////////////////////////////////////////////////

MLFOrientObject::MLFOrientObject( MLModule* _module ):
  MLBaseFunction(_module)
{}

/////////////////////////////////////////////////////////////////////////////

MLFOrientObject::~MLFOrientObject()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLFOrientObject::_call()
{
  ModelModule* model= (ModelModule*) module;

  if( model->getFlags()->modellingComponent != ModelFlags::COMPONENT_OBJ )
    return true;

  module->enterMode(ModelModes::RAY);

  if( model->getSingleSelected() == 0 )
    {
    model->printMessage( "Please select one single object", 2 );
    return true;
    }

  model->pushMessage( "Orient object: L(-Z)  R(-Z)" );

  while( ! exitFunction )
    {
    if( ! interfaceYield() )
      break;

    MLMRay* r= (MLMRay*) model->getMode(ModelModes::RAY);
    if( r == 0 )
      break;

    if( r->button != 1 && r->button != 3 )
      continue;

    if( r->hit_polygon == 0 )
      continue;

    Vector axis= Vector(0,0,-1);
    if( r->button == 3 )
      axis= -axis;

    int object_index= model->getSingleSelected();
    MLObject* o= model->getScene()->getObject(object_index);
    if( o == 0 )
      break;

    axis= o->getGlobalXForm().o2w(axis);
    axis.setNormalized();

    Point origin= o->getGlobalXForm().o2w(Point(0,0,0));
    Vector direction= r->hit_point - origin;
//    direction= o->getGlobalXForm().w2o(direction);
    direction.setNormalized();

    Vector rot_axis= axis ^ direction;
    if( rot_axis.norm() < 1e-20 )
      continue;

    rot_axis.setNormalized();
    double angle= -acos(axis | direction);

    MLCWorldRotate* cmd= new MLCWorldRotate;
    model->getModelScene()->getSelectedObjects(cmd);

    cmd->axis= rot_axis;
    cmd->angle= angle;

    cmd->lonelyCommand= 0;

    model->post(cmd);
    }

  model->popMessage();

  module->leaveMode();

  return true;
}

/////////////////////////////////////////////////////////////////////////////
