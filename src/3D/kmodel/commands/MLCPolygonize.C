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
  MLCPolygonize.C

  Stephane Rehel

  December 14 1997
*/

#include "scene/MLScene.h"

#include "scene/MLAbstractMesh.h"

#include "scene/MLObject.h"
#include "scene/MLMesh.h"

#include "kmodel/ModelScene.h"
#include "kmodel/ModelCommands.h"
#include "kmodel/ModelModule.h"

#include "MLCPolygonize.h"
#include "MLCUndoPolygonize.h"

/////////////////////////////////////////////////////////////////////////////

MLCPolygonize::MLCPolygonize()
{
  ModelCommand::type= ModelCommands::POLYGONIZE;
}

/////////////////////////////////////////////////////////////////////////////

MLCPolygonize::~MLCPolygonize()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLCPolygonize::doit( ModelModule* model )
{
  if( model == 0 )
    return true;

  MLScene* scene= model->getScene();

  MLCUndoPolygonize* undo= new MLCUndoPolygonize(indices.getSize());

  ModelCommand::createUndo(undo);

  for( int i= 0; i < indices.getSize(); ++i )
    {
    MLObject* o= scene->getObject(indices[i]);
    if( o == 0 )
      continue;

    undo->formals[i+1]= 0;

    if( o->getObjectType() == MLObject::MESH )
      {
      MLMesh* mesh= (MLMesh*) o;
      FormalRep* formalRep= mesh->getFormalRep();

      undo->formals[i+1]= formalRep;

      MLAbstractMesh* abstract= mesh->getAbstract();
      if( abstract != 0 )
        abstract->polygonize();
      continue;
      }
    }

  model->refreshScreen(ModelModule::REFRESH_GEOMETRY);

  return true;
}

/////////////////////////////////////////////////////////////////////////////
