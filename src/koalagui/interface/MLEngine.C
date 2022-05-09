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
  MLEngine.C

  Stephane Rehel

  November 14 1996
*/

#include "graphics/SystemWindow.h"
#include "graphics/MLEvent.h"

#include "MLAllocator.h"

#include "MLEngine.h"
#include "MLCanvas.h"
#include "MLModule.h"
#include "MLMode.h"
#include "KoalaNodeTypes.h"

#include "MLMotionAction.h"
#include "MLClickedMotionAction.h"
#include "MLXORRectAction.h"

#include "MLCanvasActionsPool.h"

#include "MLFunction.h"
#include "MLFunctionsManager.h"

/////////////////////////////////////////////////////////////////////////////

MLEngine::MLEngine( MLCanvas* _canvas ): MLNode(_canvas)
{
//  MLAllocator::allocEngine(this);

  canvas= _canvas;
  MLNode::setName("unknownEngine");

  actions= new MLCanvasActionsPool(canvas);

  _canvas_popup_label= 0;

  pointerAction= 0;
  pointerMode= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLEngine::~MLEngine()
{
  delete pointerAction; pointerAction= 0;
  pointerMode= 0;

  canvas->removeEngine(this);
  delete actions;
  actions= 0;

  canvas= 0;

//  MLAllocator::freeEngine(this);
}

/////////////////////////////////////////////////////////////////////////////

MLNodeType::Type MLEngine::getNodeType() const
{
  return canvas->getModule()->getKoalaNodeTypes()->engine;
}

/////////////////////////////////////////////////////////////////////////////

int MLEngine::getCanvasID() const
{
  return canvas->getID();
}

/////////////////////////////////////////////////////////////////////////////

int MLEngine::getModuleID() const
{
  return canvas->getModuleID();
}

/////////////////////////////////////////////////////////////////////////////

MLModule* MLEngine::getModule() const
{
  return canvas->getModule();
}

/////////////////////////////////////////////////////////////////////////////

void MLEngine::draw()
{}

/////////////////////////////////////////////////////////////////////////////

void MLEngine::enterMode( MLMode* mm )
{
  if( mm == 0 )
    {
    delete pointerAction;
    pointerAction= 0;
    pointerMode= 0;
    return;
    }

  if( mm == pointerMode )
    return;

  delete pointerAction;
  pointerAction= 0;

  // dont forget any mode
  switch( mm->action )
    {
    case MLMode::RECT_ACTION:
      pointerAction= new MLXORRectAction(getGfxWindow());
      break;
    case MLMode::CLICKED_MOTION_ACTION:
      pointerAction= new MLClickedMotionAction;
      break;
    case MLMode::MOTION_ACTION:
      pointerAction= new MLMotionAction;
      break;
    }

  pointerAction->setActionHandler(this);
  pointerMode= mm;
}

/////////////////////////////////////////////////////////////////////////////

void MLEngine::leaveMode( MLMode* mm )
{
  if( mm != 0 )
    {
    delete pointerAction;
    pointerAction= 0;
    pointerMode= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLEngine::engineEventHandler( MLEvent& event )
{
  if( event.type == MLEvent::MOUSE_PRESS ||
      event.type == MLEvent::MOUSE_MOTION ||
      event.type == MLEvent::MOUSE_RELEASE )
    {
    if( pointerAction == 0 )
      return false;

    return pointerAction->handleEvent(event);
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

SystemWindow* MLEngine::getGfxWindow() const
{
  return canvas->gfxWindow;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLEngine::mapped() const
{
  SystemWindow* window= getGfxWindow();
  if( window == 0 )
    return false;

  return canvas->mapped();
}

/////////////////////////////////////////////////////////////////////////////

void MLEngine::postRefresh()
{
  if( canvas == 0 )
    return;
  if( canvas->engine == this )
    canvas->postRefresh();
}

/////////////////////////////////////////////////////////////////////////////

void MLEngine::handleCommand( MLCommand* /*cmd*/ )
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLEngine::handleAction( MLAction* )
{
  return false;
}

/////////////////////////////////////////////////////////////////////////////

MLMode* MLEngine::getMode( int matching_id) const
{
  if( pointerMode == 0 )
    return 0;

  if( matching_id == 0 )
    return pointerMode;

  if( pointerMode->getID() != matching_id )
    return 0;

  return pointerMode;
}

/////////////////////////////////////////////////////////////////////////////

void MLEngine::write( MLWFileBlock& /*block*/ )
{}

/////////////////////////////////////////////////////////////////////////////

void MLEngine::read( MLRFileBlock& /*block*/ )
{}

/////////////////////////////////////////////////////////////////////////////

void MLEngine::resetAll()
{}

/////////////////////////////////////////////////////////////////////////////
