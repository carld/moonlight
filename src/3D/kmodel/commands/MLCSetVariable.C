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
  MLCSetVariable.C

  Stephane Rehel

  November 1st 1997
*/

#include "tools/IToggle.h"

#include "scene/MLScene.h"
#include "scene/lighting/MLGlobalLighting.h"

#include "kmodel/ModelScene.h"
#include "kmodel/ModelCommands.h"
#include "kmodel/ModelFlags.h"
#include "kmodel/ModelModule.h"

#include "MLCSetVariable.h"

/////////////////////////////////////////////////////////////////////////////

MLCSetVariable::MLCSetVariable()
{
  ModelCommand::type= ModelCommands::SET_VARIABLE;
  variable_id= ModelFlags::FIRST_VARIABLE_NAME;
}

/////////////////////////////////////////////////////////////////////////////

MLCSetVariable::MLCSetVariable( int _variable_id, int yes )
{
  ModelCommand::type= ModelCommands::SET_VARIABLE;
  variable_id= _variable_id;
  ModelCommand::index= yes;
}

/////////////////////////////////////////////////////////////////////////////

MLCSetVariable::~MLCSetVariable()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLCSetVariable::doit( ModelModule* model )
{
  ModelFlags* flags= model->getFlags();
  ModelScene* modelScene= model->getModelScene();

  MLCSetVariable* undo= new MLCSetVariable;
  ModelCommand::createUndo(undo);
  undo->type= ModelCommand::type;
  undo->index= ! ModelCommand::index;
  undo->variable_id= variable_id;

  int yes= (ModelCommand::index) ? 1 : 0;

  if( variable_id == ModelFlags::SINGLE_SELECTION )
    {
    if( (flags->singleSelection?1:0) == yes )
      return true;

    modelScene->getSelectedObjects(undo);

    flags->singleSelection= yes;

    modelScene->selectAll(IUNSET);
    modelScene->selectObjects(this,ISET);

    model->updateFlags();

    model->refreshScreen(ModelModule::REFRESH_SELECTION);
    return true;
    }

  if( variable_id == ModelFlags::COMPUTE_DIRECT_LIGHTING )
    {
    if( (flags->computeDirectLighting?1:0) == yes )
      return true;

    model->stopLightingProcess();

    flags->computeDirectLighting= yes;

    model->updateFlags();
    return true;
    }

  if( variable_id == ModelFlags::COMPUTE_INDIRECT_LIGHTING )
    {
    if( (flags->computeIndirectLighting?1:0) == yes )
      return true;

    model->stopLightingProcess();

    flags->computeIndirectLighting= yes;

    model->updateFlags();
    return true;
    }

  if( variable_id == ModelFlags::UPDATE_SHADOWS )
    {
    if( (flags->updateShadows?1:0) == yes )
      return true;

    model->stopLightingProcess();

    flags->updateShadows= yes;

    MLScene* scene= model->getScene();
    scene->getGlobalLighting()->update_shadows= flags->updateShadows;

    model->updateFlags();

// Then ask for a full reset?
    return true;
    }

  if( variable_id == ModelFlags::SHOW_VIEW_AXES )
    {
    if( (flags->showViewAxes?1:0) == yes )
      return true;
    flags->showViewAxes= yes;
    model->updateFlags();
    model->refreshScreen(ModelModule::REFRESH_WIRE);
    return true;
    }

  if( variable_id == ModelFlags::SHOW_OBJECT_AXES )
    {
    if( (flags->showObjectAxes?1:0) == yes )
      return true;
    flags->showObjectAxes= ! flags->showObjectAxes;
    model->updateFlags();
    model->refreshScreen(ModelModule::REFRESH_WIRE);
    return true;
    }

  if( variable_id == ModelFlags::SHOW_CAMERA )
    {
    if( (flags->showCamera?1:0) == yes )
      return true;
    flags->showCamera= yes;
    model->updateFlags();
    model->refreshScreen(ModelModule::REFRESH_GEOMETRY);
    return true;
    }

  if( variable_id == ModelFlags::SHOW_HIER_STRUCT )
    {
    if( (flags->showHierStruct?1:0) == yes )
      return true;
    flags->showHierStruct= yes;
    model->updateFlags();
    model->refreshScreen(ModelModule::REFRESH_GEOMETRY);
    return true;
    }

  if( variable_id == ModelFlags::SHOW_TESSELATION )
    {
    if( (flags->showTesselation?1:0) == yes )
      return true;
    flags->showTesselation = yes;
    model->updateFlags();
    model->refreshScreen(ModelModule::REFRESH_GEOMETRY);
    return true;
    }

  if( variable_id == ModelFlags::SHOW_NORMALS )
    {
    if( (flags->showNormals?1:0) == yes )
      return true;
    flags->showNormals = yes;
    model->updateFlags();
    model->refreshScreen(ModelModule::REFRESH_GEOMETRY);
    return true;
    }

  if( variable_id == ModelFlags::SHOW_HASHBOX )
    {
    if( (flags->showHashBox?1:0) == yes )
      return true;
    flags->showHashBox= yes;
    model->updateFlags();
    model->refreshScreen(ModelModule::REFRESH_GEOMETRY);
    return true;
    }

  if( variable_id == ModelFlags::DEFAULT_LIGHTS_DIRECT )
    {
    if( (flags->defaultLightsDirect?1:0) == yes )
      return true;
    flags->defaultLightsDirect= yes;
    model->updateFlags();
    return true;
    }

  if( variable_id == ModelFlags::DEFAULT_LIGHTS_GLOBAL )
    {
    if( (flags->defaultLightsGlobal?1:0) == yes )
      return true;
    flags->defaultLightsGlobal= yes;
    model->updateFlags();
    return true;
    }

  if( variable_id == ModelFlags::MODELLING_COMPONENT )
    {
    ModelFlags::MODELLING_COMPONENT_TYPE t=
                  ModelFlags::MODELLING_COMPONENT_TYPE(ModelCommand::index);
    if( flags->modellingComponent == t )
      return true;
    undo->index= int(flags->modellingComponent);
    flags->modellingComponent= t;
    model->updateFlags();
    model->refreshScreen(ModelModule::REFRESH_WIRE);
    return true;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
