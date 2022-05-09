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
  CanvasKey.C

  Stephane Rehel

  October 24 1997

  from Canvas.C

  *** CAREFUL: This is disgusting code ***
  ***          Sorry.                  ***
*/

#include "graphics/KeyCodes.h"
#include "graphics/MLEvent.h"
#include "graphics/MLEventManager.h"

#include "koala/VDialog.h"

#include "Canvas.h"
#include "CanvasXinfo.h"

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::handleMLKeyboard( MLEvent& event )
{
  if( dragging )
    return false;

  static boolean testing= false;
  Key key= event.key;

  MLEventManager* eventManager= MLEventManager::getEventManager();

  if( testing )
    {
    if( key == KeyCodes::ALT_T && event.type == MLEvent::KEY_PRESS )
      goto end_testing;
    return dialog->handleMLKeyboard(event);
    }

  if( event.type != MLEvent::KEY_PRESS )
    return false;

  if( key == KeyCodes::ALT_T )
    {
    if( testing )
      goto end_testing;

    // go testing
    XUnmapWindow( xinfo->display, xinfo->windowID );
    testing= true;

    for(;;)
      {
      eventManager->wait();
      eventManager->dispatch();
      }
end_testing:
    testing= false;
    XMapWindow( xinfo->display, xinfo->windowID );
    setActiveCard(dialog->getActiveCard());
    draw();
    }

  key= key.upperCase();

  if( key == KeyCodes::DELETE )
    {
    deleteWidgets();
    return true;
    }

  if( key == KeyCodes::B )
    {
    newButton();
    return true;
    }

  if( key == KeyCodes::L )
    {
    newLabel();
    return true;
    }

  if( key == KeyCodes::C )
    {
    newCheckBox();
    return true;
    }

  if( key == KeyCodes::E )
    {
    newEdit();
    return true;
    }

  if( key == KeyCodes::V )
    {
    newVertBar();
    return true;
    }

  if( key == KeyCodes::Z )
    {
    newHorizBar();
    return true;
    }

  if( key == KeyCodes::T )
    {
    newTextBox();
    return true;
    }

  if( key == KeyCodes::I )
    {
    newList();
    return true;
    }

  if( key == KeyCodes::N )
    {
    newHorizLine();
    return true;
    }

  if( key == KeyCodes::G )
    {
    newGroup();
    return true;
    }

  if( key == KeyCodes::A )
    {
    newArea();
    return true;
    }

  if( key == KeyCodes::R )
    {
    newRadioButton();
    return true;
    }

  if( key == KeyCodes::CTRL_D )
    {
    duplicateWidgets();
    return true;
    }

  if( key == KeyCodes::CTRL_A )
    {
    availableWidgets();
    return true;
    }

  if( key == KeyCodes::ALT_F2 || key == KeyCodes::CTRL_S )
    {
    writeCanvas(filename);
    draw();
    return true;
    }
  if( key == KeyCodes::F2 )
    {
    writeCanvas();
    drawInfos();
    return true;
    }
  if( key == KeyCodes::F3 )
    {
    readCanvas();
    drawInfos();
    return true;
    }

  if( key == KeyCodes::CANGLE )
    {
    expandCanvas();
    return true;
    }

  if( key == KeyCodes::OANGLE )
    {
    shrinkCanvas();
    return true;
    }

  if( key == KeyCodes::CTRL_C )
    {
    centerAllWidgets();
    return true;
    }

  if( key == KeyCodes::UP )
    {
    moveWidgets( IVector(0,-granularity) );
    return true;
    }
  if( key == KeyCodes::DOWN )
    {
    moveWidgets( IVector(0,granularity) );
    return true;
    }
  if( key == KeyCodes::LEFT )
    {
    moveWidgets( IVector(-granularity,0) );
    return true;
    }
  if( key == KeyCodes::RIGHT )
    {
    moveWidgets( IVector(granularity,0) );
    return true;
    }

  if( key == KeyCodes::SPACE && !selectedWidgets.empty() )
    {
    editWidget();
    return true;
    }

  if( key == KeyCodes::ALT_SPACE )
    {
    editDialogAction();
    return true;
    }

  if( key == KeyCodes::INSERT.shift() )
    {
    copy_from_clipboard();
    return true;
    }

  if( key == KeyCodes::INSERT.control() )
    {
    copy_to_clipboard();
    return true;
    }

  if( key == KeyCodes::DELETE.shift() )
    {
    copy_to_clipboard();
    deleteWidgets();
    return true;
    }


  return false;
}

/////////////////////////////////////////////////////////////////////////////
