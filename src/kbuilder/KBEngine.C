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
  KBEngine.C

  Stephane Rehel

  October 19 1997
*/

#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"

#include "interface/MLClickedMotionAction.h"
#include "interface/MLMode.h"

#include "KBEngine.h"
#include "KBCanvas.h"
#include "KBModule.h"

#include "KBModes.h"
#include "modes/KBMClick.h"

/////////////////////////////////////////////////////////////////////////////

KBEngine::KBEngine( KBCanvas* _canvas ): MLEngine(_canvas)
{
  kbModule= (KBModule*) _canvas->getModule();
}

/////////////////////////////////////////////////////////////////////////////

KBEngine::~KBEngine()
{}

/////////////////////////////////////////////////////////////////////////////

void KBEngine::refreshScreen( unsigned what )
{
  if( what != 0 )
    MLEngine::postRefresh();
}

/////////////////////////////////////////////////////////////////////////////

boolean KBEngine::handleAction( MLAction* action )
{
  if( ! MLEngine::validPointerAction(action) )
    return MLEngine::handleAction(action);

  int modeID= MLEngine::getMode()->getID();

  if( modeID == KBModes::CLICK )
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

void KBEngine::write( MLWFileBlock& block )
{
  MLEngine::write(block);
  block << int(0);
}

/////////////////////////////////////////////////////////////////////////////

void KBEngine::read( MLRFileBlock& block )
{
  MLEngine::read(block);

  if( block.getInteger() == 0 )
    return;
}

/////////////////////////////////////////////////////////////////////////////

void KBEngine::resetAll()
{
  MLEngine::resetAll();
}

/////////////////////////////////////////////////////////////////////////////
