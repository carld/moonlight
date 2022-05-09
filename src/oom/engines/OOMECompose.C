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
  OOMECompose.C

  Stephane Rehel

  December 2 1997
*/

#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"

#include "graphics/mlgl.h"
#include "graphics/MLEvent.h"
#include "graphics/SystemWindow.h"

#include "interface/MLXORRectAction.h"
#include "interface/MLMotionAction.h"
#include "interface/MLClickedMotionAction.h"
#include "interface/MLMode.h"
#include "interface/MLCanvasPopup.h"
#include "interface/MLPopup.h"
#include "interface/MLCanvasButton.h"
#include "interface/MLCanvasActionsPool.h"

#include "OOMECompose.h"
#include "OOMCamera.h"
#include "OOMDraw.h"

#include "oom/OOMModule.h"
#include "oom/OOMModes.h"

#include "oom/modes/OOMMSelect.h"
#include "oom/modes/OOMMDrag.h"
#include "oom/modes/OOMMCameraZoom.h"

/////////////////////////////////////////////////////////////////////////////

OOMECompose::OOMECompose( OOMCanvas* _canvas ):
  OOMEngine(_canvas)
{
  setName("Compose");

  camera= new OOMCamera;
  oomDraw= new OOMDraw(this);

  displayTypeButton= new MLCanvasButton(canvas);
  displayTypeButton->create("Preview");
  OOMEngine::actions->addRightAction(displayTypeButton);

  optionsPopup= new MLCanvasPopup(canvas);
  optionsPopup->create("Options");
  MLPopup* p= optionsPopup->getPopup();
  options_popup_wire_front= p->addCheck("Wire front");
  OOMEngine::actions->addRightAction(optionsPopup);
}

/////////////////////////////////////////////////////////////////////////////

OOMECompose::~OOMECompose()
{
  delete camera;
  camera= 0;

  delete oomDraw;
  oomDraw= 0;
}

/////////////////////////////////////////////////////////////////////////////

// protected
// return false if window is unmapped, etc.
boolean OOMECompose::_back_draw()
{
  if( ! MLEngine::mapped() || oomModule->isIconic() )
    return false;

  SystemWindow* window= MLEngine::getGfxWindow();
  if( window == 0 )
    return false;

  oomDraw->draw();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void OOMECompose::draw()
{
  if( ! _back_draw() )
    return;

  SystemWindow* window= MLEngine::getGfxWindow();

  if( window != 0 )
    window->swapBuffers();
}

/////////////////////////////////////////////////////////////////////////////

boolean OOMECompose::handleAction( MLAction* action )
{
  if( action == displayTypeButton )
    {
    /* ... */

    updatePopupsFlags();

    return true;
    }

  if( action == optionsPopup )
    {
    int label= optionsPopup->getLabel();

    if( label == options_popup_wire_front )
      {
      /* ... */

      updatePopupsFlags();

      return true;
      }
    }

  if( ! MLEngine::validPointerAction(action) )
    return OOMEngine::handleAction(action);

  switch( MLEngine::getMode()->getID() )
    {
    case OOMModes::SELECT:
      {
      OOMMSelect* s= (OOMMSelect*) getMode();

      MLXORRectAction* a= (MLXORRectAction*) action;

      /* ... */
      s->button= a->button;

      oomModule->validateFunction();

      return true;
      }

    case OOMModes::DRAG:
      {
      OOMMDrag* d= (OOMMDrag*) getMode();

      MLMotionAction* a= (MLMotionAction*) action;
      if( a->type != MLMotionAction::MOVING )
        return true;

      d->button= a->button;
      d->screenVector= Vector2( a->delta_size.x(),
                                a->delta_size.y() );

      oomModule->validateFunction();

      return true;
      }

    case OOMModes::CAMERA_ZOOM:
      {
      MLMotionAction* a= (MLMotionAction*) action;
      if( a->type != MLMotionAction::MOVING )
        return true;

      if( a->button == 1 )
        {
        camera->x-= a->delta_size.x();
        camera->y-= a->delta_size.y();
        }
       else
        {
        double speed= 1.2;
        if( a->button == 3 )
          speed *= 2.;
        IVector size= a->window->getSize();
        double dx= double(a->delta_size.x()) / double(size.x());
        double dy= double(a->delta_size.y()) / double(size.y());
        double da= speed * sqrt( dx*dx + dy*dy );
        if( da < 1e-20 )
          return true;
        if( dy < 0. )
          da= -da;
        camera->zoom+= da;
        if( camera->zoom < 1e-3 )
          camera->zoom= 1e-3;
        if( camera->zoom > 1e+3 )
          camera->zoom= 1e+3;
        }

      refreshScreen(OOMModule::REFRESH_COMPONENTS);
      return true;
      }

    default:
      break;
    }

  return OOMEngine::handleAction(action);
}

/////////////////////////////////////////////////////////////////////////////

void OOMECompose::refreshScreen( unsigned what )
{
  if( (what & OOMModule::REFRESH_COMPONENTS) != 0 )
    MLEngine::postRefresh();
}

/////////////////////////////////////////////////////////////////////////////

void OOMECompose::leaveMode( MLMode* mm )
{
  if( mm != 0 && oomModule != 0 )
    oomModule->postCmdStop(); // close the undo list for non-lonely commands

  OOMEngine::leaveMode(mm);
}

/////////////////////////////////////////////////////////////////////////////

void OOMECompose::write( MLWFileBlock& block )
{
  OOMEngine::write(block);

//  block << double(r) << double(g) << double(b);

  block << int(0);
}

/////////////////////////////////////////////////////////////////////////////

void OOMECompose::read( MLRFileBlock& block )
{
  OOMEngine::read(block);

//  r= block.getDouble();
//  g= block.getDouble();
//  b= block.getDouble();

  if( block.getInteger() == 0 )
    goto end;

end:
  updatePopupsFlags();
}

/////////////////////////////////////////////////////////////////////////////

void OOMECompose::resetAll()
{
//  r= g= b= 0.3;

  OOMEngine::resetAll();

  optionsPopup->setChecked(options_popup_wire_front,false);

  updatePopupsFlags();
}

/////////////////////////////////////////////////////////////////////////////

void OOMECompose::updatePopupsFlags()
{
  if( optionsPopup == 0 )
    return;

  optionsPopup->setAvailable(options_popup_wire_front,false);

  displayTypeButton->setTitle(" Wire  ");
}

/////////////////////////////////////////////////////////////////////////////
