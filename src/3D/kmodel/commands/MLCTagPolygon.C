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
  MLCTagPolygon.C

  Stephane Rehel

  November 23 1997
*/

#include "tools/3D/MLPolygon.h"

#include "scene/MeshRep.h"
#include "scene/MLObject.h"
#include "scene/MLMesh.h"
#include "scene/MLScene.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelCommands.h"

#include "MLCTagPolygon.h"

/////////////////////////////////////////////////////////////////////////////

MLCTagPolygon::MLCTagPolygon()
{
  type= ModelCommands::TAG_POLYGON;
  oper= ITOGGLE; // default value
  mesh_id= -1;
  polygon_id= -1;
}

/////////////////////////////////////////////////////////////////////////////

MLCTagPolygon::~MLCTagPolygon()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLCTagPolygon::doit( ModelModule* model )
{
  if( model == 0 )
    return true;

  MLScene* scene= model->getScene();

  MLMesh* mesh= (MLMesh*) scene->getObject( mesh_id, MLObject::MESH );
  if( mesh == 0 )
    return true; // ???

  MeshRep* meshrep= mesh->getMeshRep();
  if( meshrep == 0 )
    return true; // ???

  if( polygon_id < 1 || polygon_id > meshrep->polygons.getSize() )
    return true; // ???

  MLPolygon& p= meshrep->polygons[ polygon_id ];
  if( ! p.allocated() )
    return true; // ???

  MLCTagPolygon* undo= new MLCTagPolygon;
  ModelCommand::createUndo(undo);
  undo->mesh_id= mesh_id;
  undo->polygon_id= polygon_id;
  undo->oper= (oper==ITOGGLE) ? ITOGGLE : (oper==ISET) ? IUNSET : ISET;

  switch(oper)
    {
    case ITOGGLE: p.flags ^= MLPolygon::SELECTED; break;
    case ISET   : p.flags |= MLPolygon::SELECTED; break;
    case IUNSET : p.flags &=~MLPolygon::SELECTED; break;
    }

  model->refreshScreen( ModelModule::REFRESH_SELECTION );

  return true;
}

/////////////////////////////////////////////////////////////////////////////
