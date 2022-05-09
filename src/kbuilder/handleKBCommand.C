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
  handleKBCommand.C

  void KBModule::handleKBCommand( KBCommand* cmd );

  Stephane Rehel

  October 19 1997
*/

#include <stdio.h>

#include "interface/MLPopup.h"

#include "KBCommand.h"
#include "KBCommands.h"
#include "KBStatusBars.h"
#include "KBModule.h"

/////////////////////////////////////////////////////////////////////////////

void KBModule::handleKBCommand( KBCommand* cmd )
{
  if( cmd == 0 )
    return;

  if( cmd->doit(this) )
    return; // implemented into the command

  switch( cmd->getType() )
    {
    case KBCommands::SELECT:
      {
      /* ... */

//      refreshScreen( REFRESH_COLOR );
      return;
      }

    default:
      break;
    }
}

/////////////////////////////////////////////////////////////////////////////
