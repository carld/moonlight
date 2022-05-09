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
  OOMStatusBars.C

  Stephane Rehel

  October 19 1997
*/

#include "interface/MLStatusBars.h"
#include "interface/MLStatusLine.h"
#include "interface/MLStatusLineText.h"
#include "interface/MLStatusLineButton.h"

#include "OOMCommand.h"
#include "OOMCommands.h"
#include "OOMModule.h"

#include "OOMStatusBars.h"

/////////////////////////////////////////////////////////////////////////////

OOMStatusBars::OOMStatusBars( OOMModule* _oom,
                                MLStatusBars* _statusBars )
{
  oom= _oom;
  statusBars= _statusBars;

  modeInfo= 0;
  message= 0;
  single_multiple= 0;
}

/////////////////////////////////////////////////////////////////////////////

OOMStatusBars::~OOMStatusBars()
{
  modeInfo= 0;
  message= 0;
  single_multiple= 0;
}

/////////////////////////////////////////////////////////////////////////////

void OOMStatusBars::init()
{
  if( oom == 0 || statusBars == 0 )
    return;

  message= new MLStatusLineText( oom,
                                 statusBars->getLine(0),
                                 0,
                                 MLStatusLineItem::LEFT, true );

  modeInfo= new MLStatusLineText( oom,
                                  statusBars->getLine(1),
                                  0,
                                  MLStatusLineItem::LEFT, true );

  single_multiple= new MLStatusLineButton( oom,
                                           statusBars->getLine(1),
                                           3,
                                           MLStatusLineItem::RIGHT, false);
  single_multiple->setActionHandler(this);

  statusBars->getLine(0)->finalize();
  statusBars->getLine(1)->finalize();

  single_multiple->printMessage("SGL");

  update();
}

/////////////////////////////////////////////////////////////////////////////

void OOMStatusBars::update()
{
  if( oom == 0 || statusBars == 0 )
    return;

  if( single_multiple != 0 )
    single_multiple->setPressed(false);
}

/////////////////////////////////////////////////////////////////////////////

boolean OOMStatusBars::handleAction( MLAction* action )
{
  if( oom == 0 )
    return false;

  if( action == single_multiple )
    {
    OOMCommand* cmd= new OOMCommand;
    cmd->type= OOMCommands::PRESS_IT;

    oom->post(cmd);

    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////
