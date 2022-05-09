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
  MLFTagPolygon.C

  Stephane Rehel

  November 23 1997
*/

#include "MLFTagPolygon.h"

#include "tools/3D/MLPolygon.h"

#include "scene/MLObject.h"
#include "scene/MeshRep.h"
#include "scene/MLMesh.h"
#include "scene/MLObjectsIterator.h"
#include "scene/MLScene.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelModes.h"
#include "kmodel/ModelCommands.h"
#include "kmodel/ModelFlags.h"

#include "kmodel/commands/MLCTagPolygon.h"
#include "kmodel/commands/MLCSetVariable.h"

#include "kmodel/modes/MLMRaySelected.h"

/////////////////////////////////////////////////////////////////////////////

MLFTagPolygon::MLFTagPolygon( MLModule* _module ):
  MLBaseFunction(_module)
{}

/////////////////////////////////////////////////////////////////////////////

MLFTagPolygon::~MLFTagPolygon()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLFTagPolygon::_call()
{
  module->enterMode(ModelModes::RAY_SELECTED);

  ModelModule* model= (ModelModule*) module;

  if( model->getFlags()->modellingComponent != ModelFlags::COMPONENT_POL )
    model->post( new MLCSetVariable( ModelFlags::MODELLING_COMPONENT,
                                     int(ModelFlags::COMPONENT_POL) ) );

  model->pushMessage( "Tag Polygon: L(toggle)  M(select)  R(unselect)" );

  while( ! exitFunction )
    {
    if( ! interfaceYield() )
      break;

    MLMRaySelected* r= (MLMRaySelected*)
                       model->getMode(ModelModes::RAY_SELECTED);

    if( r == 0 )
      break;

    if( r->hit_polygon == 0 || r->hit_mesh == 0 )
      continue;

    if( r->hit_mesh->isFormal() )
      continue;

    MLCTagPolygon* cmd= new MLCTagPolygon;
    cmd->mesh_id= r->hit_mesh->getID();
    assert( r->hit_mesh->getMeshRep() != 0 );
    cmd->polygon_id= r->hit_polygon->getFatherPolygon(
                     r->hit_mesh->getMeshRep()->polygons.getArray() ).index;

    switch(r->button)
      {
      case 2:  cmd->oper= ISET; break;
      case 3:  cmd->oper= IUNSET; break;
      case 1:
      default: cmd->oper= ITOGGLE; break;
      }

    model->post(cmd);
    }

  model->popMessage();

  module->leaveMode();

  return true;
}

/////////////////////////////////////////////////////////////////////////////
