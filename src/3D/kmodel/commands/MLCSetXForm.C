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
  MLCSetXForm.C

  Stephane Rehel

  August 6 1997
*/

#include "scene/MLObject.h"
#include "scene/MLScene.h"
#include "scene/lighting/MLGlobalLighting.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelCommands.h"

#include "MLCSetXForm.h"

/////////////////////////////////////////////////////////////////////////////

MLCSetXForm::MLCSetXForm()
{
  type= ModelCommands::SET_TRANSFORMATION;

  scale= Vector(1,1,1);
  rotate= Vector(0,0,0);
  translate= Vector(0,0,0);
  set_preXForm= false;
}

/////////////////////////////////////////////////////////////////////////////

MLCSetXForm::~MLCSetXForm()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLCSetXForm::doit( ModelModule* model )
{
  MLScene* scene= model->getScene();

  MLObject* o= scene->getObject(index);
  if( o == 0 )
    return true;

  model->stopLightingProcess();

  o->scale(scale);
  o->rotate(rotate);
  o->translate(translate);

  if( set_preXForm )
    o->local_xform.preXForm= preXForm;

  scene->getGlobalLighting()->object_is_transformed(o);

  scene->updateTransformations();

  model->refreshScreen( ModelModule::REFRESH_TRANSFORM |
                        ModelModule::REFRESH_GEOMETRY |
                        ModelModule::REFRESH_DIALOGS );

  return true;
}

/////////////////////////////////////////////////////////////////////////////

