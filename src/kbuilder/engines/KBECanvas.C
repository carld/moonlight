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
  KBECanvas.C

  Stephane Rehel

  October 24 1997
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

#include "KBECanvas.h"

#include "kbuilder/KBModule.h"
#include "kbuilder/KBModes.h"

#include "kbuilder/modes/KBMSelect.h"
#include "kbuilder/modes/KBMDrag.h"

#include "kbuilder/appcore/Canvas.h"

/////////////////////////////////////////////////////////////////////////////

KBECanvas::KBECanvas( KBCanvas* _canvas ):
  KBEngine(_canvas)
{
  setName("Dialog");

/*
  displayTypeButton= new MLCanvasButton(canvas);
  displayTypeButton->create("Preview");
  KBEngine::actions->addRightAction(displayTypeButton);

  optionsPopup= new MLCanvasPopup(canvas);
  optionsPopup->create("Options");
  MLPopup* p= optionsPopup->getPopup();
  options_popup_wire_front= p->addCheck("Wire front");
  KBEngine::actions->addRightAction(optionsPopup);
*/
}

/////////////////////////////////////////////////////////////////////////////

KBECanvas::~KBECanvas()
{}

/////////////////////////////////////////////////////////////////////////////

void KBECanvas::draw()
{
  if( ! KBEngine::mapped() || kbModule->isIconic() )
    return;

  Canvas* c= kbModule->getBuilderCanvas();
  if( c == 0 )
    return;

  c->draw_now();
}

/////////////////////////////////////////////////////////////////////////////

boolean KBECanvas::handleAction( MLAction* action )
{
/*
  if( action == displayTypeButton )
    {
    // ...

    updatePopupsFlags();

    return true;
    }

  if( action == optionsPopup )
    {
    int label= optionsPopup->getLabel();

    if( label == options_popup_wire_front )
      {
      // ...

      updatePopupsFlags();

      return true;
      }
    }
*/

  if( ! KBEngine::validPointerAction(action) )
    return KBEngine::handleAction(action);

  switch( KBEngine::getMode()->getID() )
    {
    case KBModes::SELECT:
      {
      KBMSelect* s= (KBMSelect*) getMode();

      MLXORRectAction* a= (MLXORRectAction*) action;

      /* ... */
      s->button= a->button;

      kbModule->validateFunction();

      return true;
      }

    case KBModes::DRAG:
      {
      KBMDrag* d= (KBMDrag*) getMode();

      MLMotionAction* a= (MLMotionAction*) action;
      if( a->type != MLMotionAction::MOVING )
        return true;

      d->button= a->button;
      d->screenVector= Vector2( a->delta_size.x(),
                                a->delta_size.y() );

      kbModule->validateFunction();

      return true;
      }

    default:
      break;
    }

  return KBEngine::handleAction(action);
}

/////////////////////////////////////////////////////////////////////////////

void KBECanvas::refreshScreen( unsigned /*what*/ )
{
//  if( (what & KBModule::REFRESH_COLOR) != 0 )
//    KBEngine::postRefresh();
}

/////////////////////////////////////////////////////////////////////////////

void KBECanvas::leaveMode( MLMode* mm )
{
  if( mm != 0 && kbModule != 0 )
    kbModule->postCmdStop(); // close the undo list for non-lonely commands

  KBEngine::leaveMode(mm);
}

/////////////////////////////////////////////////////////////////////////////

void KBECanvas::write( MLWFileBlock& block )
{
  KBEngine::write(block);

//  block << double(r) << double(g) << double(b);

  block << int(0);
}

/////////////////////////////////////////////////////////////////////////////

void KBECanvas::read( MLRFileBlock& block )
{
  KBEngine::read(block);

//  r= block.getDouble();
//  g= block.getDouble();
//  b= block.getDouble();

  if( block.getInteger() == 0 )
    goto end;

end:
  updatePopupsFlags();
}

/////////////////////////////////////////////////////////////////////////////

void KBECanvas::resetAll()
{
//  r= g= b= 0.3;

  KBEngine::resetAll();

//  optionsPopup->setChecked(options_popup_wire_front,false);

  updatePopupsFlags();
}

/////////////////////////////////////////////////////////////////////////////

void KBECanvas::updatePopupsFlags()
{
//  if( optionsPopup == 0 )
//    return;

//  optionsPopup->setAvailable(options_popup_wire_front,false);
//  displayTypeButton->setTitle(" Wire  ");
}

/////////////////////////////////////////////////////////////////////////////
