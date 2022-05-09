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
  MLDMeshTesselation.C

  Stephane Rehel

  December 13 1997
*/

#include <stdlib.h>

#include "MLDMeshTesselation.h"
#include "MeshTesselationDlg.h"

#include "kw/KWDialog.h"

#include "scene/MLObject.h"
#include "scene/MLMesh.h"
#include "scene/MLScene.h"

#include "scene/formal/FormalRep.h"
#include "scene/formal/FormalParamSurf.h"

#include "kmodel/ModelModule.h"

#include "kmodel/commands/MLCSetParamPolygons.h"

/////////////////////////////////////////////////////////////////////////////

MLDMeshTesselation::MLDMeshTesselation( ModelModule* _model ):
  MLScrollingDialog(_model)
{
  model= _model;
  data= MeshTesselationDlg::data;
  name= "MeshTesselation";

  MLScrollingDialog::updateFlags= ModelModule::REFRESH_DIALOGS   |
                                  ModelModule::REFRESH_NAMES     |
                                  ModelModule::REFRESH_SELECTION |
                                  ModelModule::REFRESH_TRANSFORM |
                                  ModelModule::REFRESH_GEOMETRY;
}

/////////////////////////////////////////////////////////////////////////////

MLDMeshTesselation::~MLDMeshTesselation()
{}

/////////////////////////////////////////////////////////////////////////////

MLMesh* MLDMeshTesselation::getValidFormal()
{
  if( kwDialog == 0 )
    return 0;

  MLObject* o= model->getScene()->getObject(model->getSingleSelected());

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

void MLDMeshTesselation::update()
{
  MLObject* o= model->getScene()->getObject(model->getSingleSelected());

  MLMesh* mesh= getValidFormal();
  FormalRep* formalRep= 0;

  boolean ok= (mesh != 0);

  if( ok )
    formalRep= mesh->getFormalRep();

  kwDialog->setAvailable( MeshTesselationDlg::object, ok );
  kwDialog->setAvailable( MeshTesselationDlg::type, ok );
  kwDialog->setAvailable( MeshTesselationDlg::u_polygons, ok );
  kwDialog->setAvailable( MeshTesselationDlg::v_polygons, ok );
  kwDialog->setAvailable( MeshTesselationDlg::total, ok );
  kwDialog->setAvailable( MeshTesselationDlg::apply, ok );

  if( !ok )
    {
    kwDialog->setText( MeshTesselationDlg::object, "" );
    kwDialog->setText( MeshTesselationDlg::type, "" );
    kwDialog->setText( MeshTesselationDlg::total, "" );
    return;
    }

  kwDialog->setText( MeshTesselationDlg::object, o->getName().name );
  kwDialog->setText( MeshTesselationDlg::type,
                     FormalRep::getName(formalRep->getType()) );

  FormalParamSurf* param= (FormalParamSurf*) formalRep;

  kwDialog->setValueEditRanges( MeshTesselationDlg::u_polygons,
                                double(param->minFacesU),
                                100000.,
                                0 // n digits
                              );
  kwDialog->setValueEditRanges( MeshTesselationDlg::v_polygons,
                                double(param->minFacesV),
                                100000.,
                                0 // n digits
                              );

  kwDialog->setValue( MeshTesselationDlg::u_polygons, param->nFacesU );
  kwDialog->setValue( MeshTesselationDlg::v_polygons, param->nFacesV );

  updateTotalPolygons();
}

/////////////////////////////////////////////////////////////////////////////

void MLDMeshTesselation::updateTotalPolygons()
{
  if( kwDialog == 0 )
    return;

  if( kwDialog->getAvailable( MeshTesselationDlg::u_polygons ) )
    {
    int u= atoi( kwDialog->getText(MeshTesselationDlg::u_polygons).get() );
    int v= atoi( kwDialog->getText(MeshTesselationDlg::v_polygons).get() );

    if( u * v < 1 )
      kwDialog->setText( MeshTesselationDlg::total, "" );
     else
      kwDialog->setText( MeshTesselationDlg::total, MLString::valueOf(u*v) );
    }
   else
    kwDialog->setText( MeshTesselationDlg::total, "" );
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDMeshTesselation::kwCallback( KWEvent& event )
{
  switch( event.type )
    {
    case KWEvent::KEY_PROCESSED:
      {
      if( event.widget == MeshTesselationDlg::u_polygons ||
          event.widget == MeshTesselationDlg::v_polygons )
        updateTotalPolygons();
       else
        return MLScrollingDialog::kwCallback(event);

      break;
      }

    case KWEvent::VALIDATE:
      {
      MLMesh* mesh= getValidFormal();
      if( mesh == 0 )
        return true;
      FormalParamSurf* param= (FormalParamSurf*) (mesh->getFormalRep());

      int u_polygons= kwDialog->getInteger( MeshTesselationDlg::u_polygons );
      int v_polygons= kwDialog->getInteger( MeshTesselationDlg::v_polygons );

      if( u_polygons == param->nFacesU && v_polygons == param->nFacesV )
        return true;

      MLCSetParamPolygons* cmd= new MLCSetParamPolygons;
      MLCSetParamPolygons* undo= new MLCSetParamPolygons;

      cmd->index= mesh->getID();

      cmd->createUndo(undo);

      cmd->u_polygons= u_polygons;
      cmd->v_polygons= v_polygons;

      undo->u_polygons= param->nFacesU;
      undo->v_polygons= param->nFacesV;

      model->post(cmd);

      break;
      }

    default:
      return MLScrollingDialog::kwCallback(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
