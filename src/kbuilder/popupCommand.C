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
  popupCommand.C

  void KBModule::handlePopupCommand( int label );

  Stephane Rehel

  OCtober 19 1997
*/

#include "KBModule.h"

#include "kw/KWDialog.h"

#include "interface/MLInterfaceCommand.h"
#include "interface/MLPopup.h"

#include "KBCommand.h"
#include "KBCommands.h"

#include "KBPopups.h"

#include "appcore/Canvas.h"

#include "gdialogs/KBDOpen.h"
#include "gdialogs/KBDSaveAs.h"

/////////////////////////////////////////////////////////////////////////////

void KBModule::handlePopupCommand( int label )
{
  Canvas* c= getBuilderCanvas();

  if( label == popups->new_dialog )
    {
    c->newDialog();
    return;
    }

  if( label == popups->open_dialog )
    {
    KBDOpen o(this);
    o.run();
    return;
    }

  if( label == popups->save_dialog )
    {
    c->writeCanvas();
    c->draw();
    return;
    }

  if( label == popups->save_dialog_as )
    {
    KBDSaveAs sa(this);
    sa.run();
    return;
    }

  if( label == popups->edit_dialog )
    {
    c->editDialogAction();
    return;
    }

  if( label == popups->edit_widget )
    {
    c->editWidget();
    return;
    }

  if( label == popups->edit_prefs )
    {
    c->editPrefs();
    return;
    }

//  if( label == popups->test )
//    {
//    return;
//    }

  if( label == popups->quit )
    {
    MLInterfaceCommand* icmd= new MLInterfaceCommand;
    icmd->type= MLInterfaceCommand::QUIT;
    icmd->post();
    return;
    }
}

/////////////////////////////////////////////////////////////////////////////
