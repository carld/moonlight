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
  MLDObjectInfo.C

  Stephane Rehel

  March 5 1997
*/

#include "kmodel/ModelModule.h"

#include "MLDObjectInfo.h"
#include "ObjectInfoDlg.h"

#include "kw/KWDialog.h"
#include "scene/material/MLMaterial.h"
#include "scene/MLMesh.h"
#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

MLDObjectInfo::MLDObjectInfo( ModelModule* _model ):
  MLScrollingDialog(_model)
{
  model= _model;
  data= ObjectInfoDlg::data;
  name= "Object Info";

  MLScrollingDialog::updateFlags= ModelModule::REFRESH_DIALOGS   |
                                  ModelModule::REFRESH_NAMES     |
                                  ModelModule::REFRESH_SELECTION |
                                  ModelModule::REFRESH_GEOMETRY;
}

/////////////////////////////////////////////////////////////////////////////

MLDObjectInfo::~MLDObjectInfo()
{}

/////////////////////////////////////////////////////////////////////////////

void MLDObjectInfo::update()
{
  int object_index= model->getSingleSelected();
  MLMesh* mesh= (MLMesh*)
                model->getScene()->getObject(object_index,MLObject::MESH);

  if( mesh == 0 )
    {
    kwDialog->setText( ObjectInfoDlg::object, "" );
    kwDialog->setText( ObjectInfoDlg::material, "" );
    kwDialog->setText( ObjectInfoDlg::nVertices, "" );
    kwDialog->setText( ObjectInfoDlg::nSVertices, "" );
    kwDialog->setText( ObjectInfoDlg::nEdges, "" );
    kwDialog->setText( ObjectInfoDlg::nPolygons, "" );
    kwDialog->setText( ObjectInfoDlg::nTessTriangles, "" );
    kwDialog->setText( ObjectInfoDlg::nTriangles, "" );
    kwDialog->setText( ObjectInfoDlg::allocated, "" );

    return;
    }

  int nVertices, nSVertices, nEdges,
      nPolygons, nTessTriangles, nTriangles;

  mesh->getInfo( nVertices, nSVertices, nEdges,
              nPolygons, nTessTriangles, nTriangles );

  int size= mesh->getSizeOf() / 1024;
  MLMaterial* m= mesh->getMaterial();

  kwDialog->setText( ObjectInfoDlg::object,         mesh->getName().name );
  kwDialog->setText( ObjectInfoDlg::material,       m->getName().name );
  kwDialog->setText( ObjectInfoDlg::nVertices,      MLString::valueOf(nVertices) );
  kwDialog->setText( ObjectInfoDlg::nSVertices,     MLString::valueOf(nSVertices) );
  kwDialog->setText( ObjectInfoDlg::nEdges,         MLString::valueOf(nEdges) );
  kwDialog->setText( ObjectInfoDlg::nPolygons,      MLString::valueOf(nPolygons) );
  kwDialog->setText( ObjectInfoDlg::nTessTriangles, MLString::valueOf(nTessTriangles) );
  kwDialog->setText( ObjectInfoDlg::nTriangles,     MLString::valueOf(nTriangles) );
  kwDialog->setText( ObjectInfoDlg::allocated,      MLString::valueOf(size) );
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDObjectInfo::kwCallback( KWEvent& event )
{
  switch( event.type )
    {
    default:
      return MLScrollingDialog::kwCallback(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
