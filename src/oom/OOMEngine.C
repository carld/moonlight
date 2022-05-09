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
  OOMEngine.C

  Stephane Rehel

  October 19 1997
*/

#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"

#include "interface/MLClickedMotionAction.h"
#include "interface/MLMode.h"

#include "OOMEngine.h"
#include "OOMCanvas.h"
#include "OOMModule.h"

#include "OOMModes.h"
#include "modes/OOMMClick.h"

/////////////////////////////////////////////////////////////////////////////

OOMEngine::OOMEngine( OOMCanvas* _canvas ): MLEngine(_canvas)
{
  oomModule= (OOMModule*) _canvas->getModule();
}

/////////////////////////////////////////////////////////////////////////////

OOMEngine::~OOMEngine()
{}

/////////////////////////////////////////////////////////////////////////////

void OOMEngine::refreshScreen( unsigned what )
{
  if( what != 0 )
    MLEngine::postRefresh();
}

/////////////////////////////////////////////////////////////////////////////

boolean OOMEngine::handleAction( MLAction* action )
{
  if( ! MLEngine::validPointerAction(action) )
    return MLEngine::handleAction(action);

  int modeID= MLEngine::getMode()->getID();

  if( modeID == OOMModes::CLICK )
    {
    MLClickedMotionAction* a= (MLClickedMotionAction*) action;
    if( a->type == MLClickedMotionAction::START )
      {
      /* ... */
      return true;
      }

    return true;
    }

  return MLEngine::handleAction(action);
}

/////////////////////////////////////////////////////////////////////////////

void OOMEngine::write( MLWFileBlock& block )
{
  MLEngine::write(block);
  block << int(0);
}

/////////////////////////////////////////////////////////////////////////////

void OOMEngine::read( MLRFileBlock& block )
{
  MLEngine::read(block);

  if( block.getInteger() == 0 )
    return;
}

/////////////////////////////////////////////////////////////////////////////

void OOMEngine::resetAll()
{
  MLEngine::resetAll();
}

/////////////////////////////////////////////////////////////////////////////
