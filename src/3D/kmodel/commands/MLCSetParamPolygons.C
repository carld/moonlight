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
  MLCSetParamPolygons.C

  Stephane Rehel

  December 13 1997
*/

#include "scene/MLObject.h"
#include "scene/MLMesh.h"
#include "scene/MLScene.h"

#include "scene/formal/FormalRep.h"
#include "scene/formal/FormalParamSurf.h"

#include "scene/lighting/MLGlobalLighting.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelCommands.h"

#include "MLCSetParamPolygons.h"

/////////////////////////////////////////////////////////////////////////////

MLCSetParamPolygons::MLCSetParamPolygons()
{
  type= ModelCommands::SET_PARAM_POLYGONS;

  u_polygons= 10;
  v_polygons= 10;
  index= -1;
}

/////////////////////////////////////////////////////////////////////////////

MLCSetParamPolygons::~MLCSetParamPolygons()
{}

/////////////////////////////////////////////////////////////////////////////

MLMesh* MLCSetParamPolygons::checkValidFormal( ModelModule* model,
                                               int index )
{
  if( model == 0 )
    return 0;

  MLObject* o= model->getScene()->getObject(index);

  if( o == 0 )
    return 0;

  if( o->getObjectType() != MLObject::MESH )
    return 0;

  MLMesh* mesh= (MLMesh*) o;
  FormalRep* formalRep= mesh->getFormalRep();
  if( formalRep == 0 )
    return 0;

  if( ! formalRep->isParamSurf() )
    return 0;

  return mesh;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLCSetParamPolygons::doit( ModelModule* model )
{
  MLScene* scene= model->getScene();

  MLMesh* mesh= checkValidFormal( model, index );

  if( mesh == 0 )
    return true;
  FormalParamSurf* param= (FormalParamSurf*) (mesh->getFormalRep());

  if( param->nFacesU == u_polygons && param->nFacesV == v_polygons )
    return true;

  model->stopLightingProcess();

  param->nFacesU= u_polygons;
  param->nFacesV= v_polygons;

  mesh->regenerate();

  scene->getGlobalLighting()->object_is_transformed(mesh);

  model->refreshScreen( ModelModule::REFRESH_TRANSFORM |
                        ModelModule::REFRESH_GEOMETRY |
                        ModelModule::REFRESH_DIALOGS );

  return true;
}

/////////////////////////////////////////////////////////////////////////////

