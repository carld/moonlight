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
  MLDCurveTesselation.C

  Stephane Rehel

  January 24 1998
*/

#include <stdlib.h>

#include "MLDCurveTesselation.h"
#include "CurveTesselationDlg.h"

#include "kw/KWDialog.h"

#include "scene/MLObject.h"
#include "scene/MLCurve.h"
#include "scene/MLScene.h"

#include "scene/formal/FormalCurveRep.h"
#include "scene/formal/FormalParamCurve.h"

#include "kmodel/ModelModule.h"

#include "kmodel/commands/MLCSetParamSegments.h"

/////////////////////////////////////////////////////////////////////////////

MLDCurveTesselation::MLDCurveTesselation( ModelModule* _model ):
  MLScrollingDialog(_model)
{
  model= _model;
  data= CurveTesselationDlg::data;
  name= "CurveTesselation";

  MLScrollingDialog::updateFlags= ModelModule::REFRESH_DIALOGS   |
                                  ModelModule::REFRESH_NAMES     |
                                  ModelModule::REFRESH_SELECTION |
                                  ModelModule::REFRESH_TRANSFORM |
                                  ModelModule::REFRESH_GEOMETRY;
}

/////////////////////////////////////////////////////////////////////////////

MLDCurveTesselation::~MLDCurveTesselation()
{}

/////////////////////////////////////////////////////////////////////////////

MLCurve* MLDCurveTesselation::getValidFormal()
{
  if( kwDialog == 0 )
    return 0;

  MLObject* o= model->getScene()->getObject(model->getSingleSelected());

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

void MLDCurveTesselation::update()
{
  MLObject* o= model->getScene()->getObject(model->getSingleSelected());

  MLCurve* curve= getValidFormal();
  FormalCurveRep* formalRep= 0;

  boolean ok= (curve != 0);

  if( ok )
    formalRep= curve->getFormalRep();

  kwDialog->setAvailable( CurveTesselationDlg::object, ok );
  kwDialog->setAvailable( CurveTesselationDlg::type, ok );
  kwDialog->setAvailable( CurveTesselationDlg::segments, ok );
  kwDialog->setAvailable( CurveTesselationDlg::apply, ok );

  if( !ok )
    {
    kwDialog->setText( CurveTesselationDlg::object, "" );
    kwDialog->setText( CurveTesselationDlg::type, "" );
    return;
    }

  kwDialog->setText( CurveTesselationDlg::object, o->getName().name );
  kwDialog->setText( CurveTesselationDlg::type,
                     FormalCurveRep::getName(formalRep->getType()) );

  FormalParamCurve* param= (FormalParamCurve*) formalRep;

  kwDialog->setValueEditRanges( CurveTesselationDlg::segments,
                                double(param->minSegments),
                                100000.,
                                0 // n digits
                              );

  kwDialog->setValue( CurveTesselationDlg::segments, param->nSegments );
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDCurveTesselation::kwCallback( KWEvent& event )
{
  switch( event.type )
    {
    case KWEvent::KEY_PROCESSED:
      {
      if( event.widget == CurveTesselationDlg::segments )
{}//        updateTotalPolygons();
       else
        return MLScrollingDialog::kwCallback(event);

      break;
      }

    case KWEvent::VALIDATE:
      {
      MLCurve* curve= getValidFormal();
      if( curve == 0 )
        return true;
      FormalParamCurve* param= (FormalParamCurve*) (curve->getFormalRep());

      int segments= kwDialog->getInteger( CurveTesselationDlg::segments );

      if( segments == param->nSegments )
        return true;

      MLCSetParamSegments* cmd= new MLCSetParamSegments;
      MLCSetParamSegments* undo= new MLCSetParamSegments;

      cmd->index= curve->getID();

      cmd->createUndo(undo);

      cmd->segments= segments;

      undo->segments= param->nSegments;

      model->post(cmd);

      break;
      }

    default:
      return MLScrollingDialog::kwCallback(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
