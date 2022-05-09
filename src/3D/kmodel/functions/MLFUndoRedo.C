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
  MLFUndoRedo.C

  Stephane Rehel
  August 2 1997
*/

#include "MLFUndoRedo.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelModes.h"

#include "kmodel/modes/MLMClick.h"

/////////////////////////////////////////////////////////////////////////////

MLFUndoRedo::MLFUndoRedo( MLModule* _module ):
  MLBaseFunction(_module)
{}

/////////////////////////////////////////////////////////////////////////////

MLFUndoRedo::~MLFUndoRedo()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLFUndoRedo::_call()
{
  module->enterMode(ModelModes::CLICK);

  ModelModule* model= (ModelModule*) module;

  model->pushMessage( "Undo/Redo: L(undo)  R(Redo)" );

  while( ! exitFunction )
    {
    if( ! interfaceYield() )
      break;

    MLMClick* c= (MLMClick*) model->getMode(ModelModes::CLICK);
    if( c == 0 )
      break;

    switch( c->button )
      {
      case 1: model->postUndo(); break;
      case 3: model->postRedo(); break;
      default: break;
      }
    }

  model->popMessage();

  module->leaveMode();

  return true;
}

/////////////////////////////////////////////////////////////////////////////
