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
  ModelEngine.C

  Stephane Rehel

  December 8 1996
*/

#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"

#include "interface/MLClickedMotionAction.h"
#include "interface/MLMode.h"

#include "ModelEngine.h"
#include "ModelCanvas.h"
#include "ModelModule.h"

#include "ModelModes.h"
#include "modes/MLMClick.h"

/////////////////////////////////////////////////////////////////////////////

ModelEngine::ModelEngine( ModelCanvas* _canvas ): MLEngine(_canvas)
{
  model= (ModelModule*) _canvas->getModule();
  scene= model->getScene();
}

/////////////////////////////////////////////////////////////////////////////

ModelEngine::~ModelEngine()
{}

/////////////////////////////////////////////////////////////////////////////

void ModelEngine::refreshScreen( unsigned what )
{
  if( what != 0 )
    MLEngine::postRefresh();
}

/////////////////////////////////////////////////////////////////////////////

void ModelEngine::refreshMainCamera( const MLCamera& )
{}

/////////////////////////////////////////////////////////////////////////////

boolean ModelEngine::handleAction( MLAction* action )
{
  if( ! MLEngine::validPointerAction(action) )
    return MLEngine::handleAction(action);

  int modeID= MLEngine::getMode()->getID();

  if( modeID == ModelModes::CLICK )
    {
    MLClickedMotionAction* a= (MLClickedMotionAction*) action;
    if( a->type == MLClickedMotionAction::START )
      {
      MLMClick* s= (MLMClick*) getMode();
      s->button= a->button;

      model->validateFunction();

      return true;
      }

    return true;
    }

  return MLEngine::handleAction(action);
}

/////////////////////////////////////////////////////////////////////////////

void ModelEngine::write( MLWFileBlock& block )
{
  MLEngine::write(block);
  block << int(0);
}

/////////////////////////////////////////////////////////////////////////////

void ModelEngine::read( MLRFileBlock& block )
{
  MLEngine::read(block);

  if( block.getInteger() == 0 )
    return;
}

/////////////////////////////////////////////////////////////////////////////

void ModelEngine::resetAll()
{
  MLEngine::resetAll();
}

/////////////////////////////////////////////////////////////////////////////
