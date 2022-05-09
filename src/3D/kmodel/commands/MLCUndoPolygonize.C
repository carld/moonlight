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
  MLCUndoPolygonize.C

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

#include "MLCUndoPolygonize.h"
#include "MLCPolygonize.h"

/////////////////////////////////////////////////////////////////////////////

MLCUndoPolygonize::MLCUndoPolygonize( int _nFormals )
{
  nFormals= (_nFormals<0) ? 0 : _nFormals;
  formals= new FormalRep* [ nFormals + 1 ];

  for( int i= 0; i <= nFormals; ++i )
    formals[i]= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLCUndoPolygonize::~MLCUndoPolygonize()
{
  delete formals;
  formals= 0;
  nFormals= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLCUndoPolygonize::doit( ModelModule* model )
{
  if( model == 0 )
    return true;

  MLScene* scene= model->getScene();

  MLCPolygonize* undo= new MLCPolygonize();
  undo->indices.alloc(nFormals);

  ModelCommand::createUndo(undo);

  for( int i= 1; i <= nFormals; ++i )
    {
    MLObject* o= scene->getObject(indices[i-1]);
    if( o == 0 )
      continue;

    if( o->getObjectType() == MLObject::MESH )
      {
      MLMesh* mesh= (MLMesh*) o;

      FormalRep* formalRep= formals[i];
      if( formalRep == 0 )
        continue;

      MLAbstractMesh* abstract= mesh->getAbstract();
      if( abstract != 0 )
        abstract->unpolygonize(formalRep);
      }
    }

  model->refreshScreen(ModelModule::REFRESH_GEOMETRY);

  return true;
}

/////////////////////////////////////////////////////////////////////////////
