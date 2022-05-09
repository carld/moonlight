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

  March 5 1998
*/

#include <math.h>

#include "scene/MLObject.h"
#include "scene/MLMesh.h"
#include "scene/MLScene.h"

#include "scene/formal/FormalRep.h"
#include "scene/formal/FormalText.h"

#include "scene/lighting/MLGlobalLighting.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelCommands.h"

#include "MLCSetPlainText.h"

/////////////////////////////////////////////////////////////////////////////

MLCSetPlainText::MLCSetPlainText()
{
  type= ModelCommands::SET_PLAIN_TEXT;

  text= "";
  ttf_name= "";
  precision= 20.;

  front= true;
  side= true;
  back= true;

  extrude_height= 1. / 5.;

  reversed_sides= false;

  index= -1;
}

/////////////////////////////////////////////////////////////////////////////

MLCSetPlainText::~MLCSetPlainText()
{}

/////////////////////////////////////////////////////////////////////////////

MLMesh* MLCSetPlainText::checkValidFormal( ModelModule* model,
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

  if( formalRep->getType() != FormalRep::TEXT )
    return 0;

  return mesh;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLCSetPlainText::doit( ModelModule* model )
{
  MLScene* scene= model->getScene();

  MLMesh* mesh= checkValidFormal( model, index );

  if( mesh == 0 )
    return true;

  FormalText* ft= (FormalText*) mesh->getFormalRep();

  if( fabs(extrude_height) < 1e-10 )
    {
    extrude_height= 0.;
    side= false; // don't be silly please
    }

  if( text == ft->text &&
      ttf_name == ft->ttf_name &&
      fabs(precision - ft->precision) < 1e-5 &&
      (front?1:0) == (ft->front?1:0) &&
      (side?1:0) == (ft->side?1:0) &&
      (back?1:0) == (ft->back?1:0) &&
      fabs(extrude_height - ft->extrude_height) < 1e-5 &&
      (reversed_sides?1:0) == (ft->reversed_sides?1:0) )
    return true;

  model->stopLightingProcess();

  ft->text= text;
  ft->ttf_name= ttf_name;
  ft->precision= precision;
  ft->front= front;
  ft->side= side;
  ft->back= back;
  ft->extrude_height= extrude_height;
  ft->reversed_sides= reversed_sides;

  mesh->regenerate();

  scene->getGlobalLighting()->object_is_transformed(mesh);

  model->refreshScreen( ModelModule::REFRESH_TRANSFORM |
                        ModelModule::REFRESH_GEOMETRY |
                        ModelModule::REFRESH_DIALOGS );

  return true;
}

/////////////////////////////////////////////////////////////////////////////
