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

  void OOMModule::handlePopupCommand( int label );

  Stephane Rehel

  OCtober 19 1997
*/

#include "OOMModule.h"

#include "kw/KWDialog.h"

#include "interface/MLInterfaceCommand.h"
#include "interface/MLPopup.h"

#include "model/OOModel.h"
#include "model/OOClass.h"
#include "model/OOPackage.h"

#include "OOMCommand.h"
#include "OOMCommands.h"

#include "OOMPopups.h"

/////////////////////////////////////////////////////////////////////////////

void OOMModule::handlePopupCommand( int label )
{
  if( label == popups->choice1 )
    {
    if( KWWarningOkCancel( "This is really a template module.\n"
                           "Continue?" ) )
      {
//      resetAll();
      }
    return;
    }

  if( label == popups->new_class )
    {
    OOPackage* package= model->getDefaultPackage();
    if( package == 0 )
      return;

    OOClass* c= package->newClass("");
    c->draw.x1= 0;
    c->draw.y1= 0;
    c->draw.x2= 80;
    c->draw.y2= 30;

    refreshScreen(REFRESH_COMPONENTS);
    return;
    }

  if( label == popups->quit )
    {
    MLInterfaceCommand* icmd= new MLInterfaceCommand;
    icmd->type= MLInterfaceCommand::QUIT;
    icmd->post();
    return;
    }

}

/////////////////////////////////////////////////////////////////////////////
