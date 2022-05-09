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
  MLCommand.C

  Stephane Rehel

  November 28 1996
*/

#include "tools/IList.h"

#include "MLCommand.h"

#include "Interface.h"

/////////////////////////////////////////////////////////////////////////////

void MLCommand::post()
{
  interface->post(this);
}

/////////////////////////////////////////////////////////////////////////////

// static protected
int MLCommand::allocBase()
{
  static int base_counter= 0;

  ++base_counter;

  return base_counter << 16;
}

/////////////////////////////////////////////////////////////////////////////

// init an undo command for the current command
void MLCommand::initUndo( MLCommand* undo ) const
{
  if( undo == 0 )
    return;

  undo->isUndo= ! isUndo;
  undo->isUserCommand= 0;
  undo->lonelyCommand= lonelyCommand;
  undo->canUndo= canUndo;

  undo->module= module;
  undo->canvas= canvas;
  undo->engine= engine;

  undo->base= base;
  undo->type= type;
}

/////////////////////////////////////////////////////////////////////////////
