// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//                 2002       Patrick Mauritz
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
  KBStatusBars.C

  Stephane Rehel

  October 19 1997
*/

#include <string.h>

#include "interface/MLStatusBars.h"
#include "interface/MLStatusLine.h"
#include "interface/MLStatusLineText.h"
#include "interface/MLStatusLineButton.h"

#include "KBCommand.h"
#include "KBCommands.h"
#include "KBModule.h"

#include "KBStatusBars.h"

/////////////////////////////////////////////////////////////////////////////

KBStatusBars::KBStatusBars( KBModule* _kb,
                            MLStatusBars* _statusBars )
{
  kb= _kb;
  statusBars= _statusBars;

  modeInfo= 0;
  message= 0;
  widgetMessage= 0;
//  single_multiple= 0;
}

/////////////////////////////////////////////////////////////////////////////

KBStatusBars::~KBStatusBars()
{
  modeInfo= 0;
  message= 0;
  widgetMessage= 0;
//  single_multiple= 0;
}

/////////////////////////////////////////////////////////////////////////////

void KBStatusBars::init()
{
  if( kb == 0 || statusBars == 0 )
    return;

  MLStatusLine* line1= statusBars->getLine(0);
  MLStatusLine* line2= statusBars->getLine(1);

  message= new MLStatusLineText( kb,
                                 line1,
                                 0,
                                 MLStatusLineItem::LEFT, true );

//(new MLStatusLineButton( statusBars->getLine1(),
//                         3, MLStatusLineItem::RIGHT, false ))->
//setMessage("ABC");

  modeInfo= new MLStatusLineText( kb,
                                  line2,
                                  15,
                                  MLStatusLineItem::LEFT, false );

  widgetMessage= new MLStatusLineText( kb,
                                  line2,
                                  0,
                                  MLStatusLineItem::LEFT, true );

//  single_multiple= new MLStatusLineButton( kb,
//                                           statusBars->getLine2(),
//                                           3,
//                                           MLStatusLineItem::RIGHT, false);
//  single_multiple->setActionHandler(this);

  line1->finalize();
  line2->finalize();

//  single_multiple->printMessage("SGL");

  update();
}

/////////////////////////////////////////////////////////////////////////////

void KBStatusBars::update()
{
  if( kb == 0 || statusBars == 0 )
    return;

//  if( single_multiple != 0 )
//    single_multiple->setPressed(false);
}

/////////////////////////////////////////////////////////////////////////////

boolean KBStatusBars::handleAction( MLAction* /*action*/ )
{
  if( kb == 0 )
    return false;

//  if( action == single_multiple )
//    {
//    KBCommand* cmd= new KBCommand;
//    cmd->type= KBCommands::PRESS_IT;
//
//    kb->post(cmd);
//
//    return true;
//    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////
