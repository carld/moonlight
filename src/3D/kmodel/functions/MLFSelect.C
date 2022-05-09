// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//                 2002       Patrick Mauritz
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
  MLFSelect.C

  Stephane Rehel

  August 2 1997
*/

#include <string.h>

#include "MLFSelect.h"

#include "tools/IntegerArray.h"

#include "scene/MLObject.h"
#include "scene/MLObjectsIterator.h"
#include "scene/MLScene.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelModes.h"
#include "kmodel/ModelCommand.h"
#include "kmodel/ModelCommands.h"
#include "kmodel/ModelFlags.h"

#include "kmodel/modes/MLMSelect.h"

#include "kmodel/commands/MLCSetVariable.h"

/////////////////////////////////////////////////////////////////////////////

MLFSelect::MLFSelect( MLModule* _module ):
  MLBaseFunction(_module)
{}

/////////////////////////////////////////////////////////////////////////////

MLFSelect::~MLFSelect()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLFSelect::_call()
{
  module->enterMode(ModelModes::SELECT);

  ModelModule* model= (ModelModule*) module;

  if( model->getFlags()->modellingComponent != ModelFlags::COMPONENT_OBJ )
    model->post( new MLCSetVariable( ModelFlags::MODELLING_COMPONENT,
                                     int(ModelFlags::COMPONENT_OBJ) ) );

  model->pushMessage( "Select: L(object)  M(object+children)" );

  while( ! exitFunction )
    {
    if( ! interfaceYield() )
      break;

    MLMSelect* s= (MLMSelect*) model->getMode(ModelModes::SELECT);
    if( s == 0 )
      break;

    ModelCommand* cmd= new ModelCommand;
    cmd->type= ModelCommands::SELECT_TOGGLE;
    cmd->indices= s->objects;

    if( s->button == 2 &&
        s->objects.getSize() > 0 &&
        !model->getFlags()->singleSelection )
      {
      // select children also

      MLScene* scene= model->getScene();

      int i, j;
      // remove children that has been selected in the same picking
      for( i= 0; i < cmd->indices.getSize(); ++i )
        {
        if( cmd->indices[i] == 0 )
          continue;
        MLObject* o= scene->getObject(cmd->indices[i]);
        if( o == 0 )
          {
          cmd->indices[i]= 0;
          continue;
          }
        int j;
        for( j= i+1; j < cmd->indices.getSize(); ++j )
          {
          if( cmd->indices[j] == 0 )
            continue;
          MLObject* c= scene->getObject(cmd->indices[j]);
          if( c == 0 ) continue;
          if( c == o || c->isChildOf(o) || o->isChildOf(c) )
            cmd->indices[j]= 0;
          }
        }

      // clean zero objects ids
      j= 0;
      for( i= 0; i < cmd->indices.getSize(); ++i )
        if( cmd->indices[i] != 0 )
          cmd->indices[j++]= cmd->indices[i];
      cmd->indices.truncate(j);

      // find out how many children we've got to add
      MLObjectsIterator oi(scene);
      int nChildren= 0;
      while( ! oi.eol() )
        {
        MLObject* c= oi++;
        for( i= 0; i < cmd->indices.getSize(); ++i )
          if( c->isChildOf(scene->getObject(cmd->indices[i])) )
            ++nChildren;
        }

      // allocate a new indices array
      IntegerArray indices;
      indices.alloc( cmd->indices.getSize() + nChildren );
      for( i= 0; i < cmd->indices.getSize(); ++i )
        indices[i]= cmd->indices[i];

      // set children ids
      j= cmd->indices.getSize();
      oi.reset();
      while( ! oi.eol() )
        {
        MLObject* c= oi++;
        for( i= 0; i < cmd->indices.getSize(); ++i )
          if( c->isChildOf(scene->getObject(cmd->indices[i])) )
            indices[j++]= c->getID();
        }

      cmd->indices= indices;
      }

    model->post(cmd);
    }

  model->popMessage();

  module->leaveMode();

  return true;
}

/////////////////////////////////////////////////////////////////////////////
