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
  OOMCommand.C

  Stephane Rehel

  October 19 1997
*/

#include "OOMCommand.h"

/////////////////////////////////////////////////////////////////////////////

// static
int OOMCommand::BASE= 0;

/////////////////////////////////////////////////////////////////////////////

// static
void OOMCommand::initOOMCommands()
{
  if( OOMCommand::BASE == 0 )
    BASE= MLCommand::allocBase();
}

/////////////////////////////////////////////////////////////////////////////

OOMCommand* OOMCommand::createUndo( OOMCommand* cmd /* = 0 */ )
{
  if( cmd == 0 )
    cmd= new OOMCommand;

  MLCommand::initUndo(cmd);

  cmd->index= index;
  cmd->v= v;

  cmd->indices= indices;

  if( undo != 0 )
    unref(undo);

  undo= cmd;
  ref(undo);

  undo->undo= this;
  ref(undo->undo);

  return undo;
}

/////////////////////////////////////////////////////////////////////////////

// return true if implemented
boolean OOMCommand::doit( OOMModule* /*oom*/ )
{
  return false;
}

/////////////////////////////////////////////////////////////////////////////
