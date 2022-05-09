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
  MLCSetParamSegments.C

  Stephane Rehel

  January 24 1998
*/

#include "scene/MLObject.h"
#include "scene/MLCurve.h"
#include "scene/MLScene.h"

#include "scene/formal/FormalCurveRep.h"
#include "scene/formal/FormalParamCurve.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelCommands.h"

#include "MLCSetParamSegments.h"

/////////////////////////////////////////////////////////////////////////////

MLCSetParamSegments::MLCSetParamSegments()
{
  type= ModelCommands::SET_PARAM_SEGMENTS;

  segments= 10;

  index= -1;
}

/////////////////////////////////////////////////////////////////////////////

MLCSetParamSegments::~MLCSetParamSegments()
{}

/////////////////////////////////////////////////////////////////////////////

MLCurve* MLCSetParamSegments::checkValidFormal( ModelModule* model,
                                                int index )
{
  if( model == 0 )
    return 0;

  MLObject* o= model->getScene()->getObject(index);

  if( o == 0 )
    return 0;

  if( o->getObjectType() != MLObject::CURVE )
    return 0;

  MLCurve* curve= (MLCurve*) o;
  FormalCurveRep* formalRep= curve->getFormalRep();
  if( formalRep == 0 )
    return 0;

  if( ! formalRep->isParamCurve() )
    return 0;

  return curve;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLCSetParamSegments::doit( ModelModule* model )
{
  MLCurve* curve= checkValidFormal( model, index );

  if( curve == 0 )
    return true;

  FormalParamCurve* param= (FormalParamCurve*) (curve->getFormalRep());

  if( param->nSegments == segments )
    return true;

  param->nSegments= segments;

  curve->regenerate();

  model->refreshScreen( ModelModule::REFRESH_TRANSFORM |
                        ModelModule::REFRESH_GEOMETRY |
                        ModelModule::REFRESH_DIALOGS );

  return true;
}

/////////////////////////////////////////////////////////////////////////////
