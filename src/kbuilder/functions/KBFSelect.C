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
  KBFSelect.C

  Stephane Rehel

  October 19 1997
*/

#include <string.h>

#include "KBFSelect.h"

#include "tools/IntegerArray.h"

#include "kbuilder/KBModule.h"
#include "kbuilder/KBModes.h"
#include "kbuilder/KBCommand.h"
#include "kbuilder/KBCommands.h"

#include "kbuilder/modes/KBMSelect.h"

/////////////////////////////////////////////////////////////////////////////

KBFSelect::KBFSelect( MLModule* _module ):
  MLBaseFunction(_module)
{}

/////////////////////////////////////////////////////////////////////////////

KBFSelect::~KBFSelect()
{}

/////////////////////////////////////////////////////////////////////////////

boolean KBFSelect::_call()
{
  module->enterMode(KBModes::SELECT);

  KBModule* tmpl= (KBModule*) module;

  tmpl->pushMessage( "Select function" );

  while( ! exitFunction )
    {
    if( ! interfaceYield() )
      break;

    KBMSelect* s= (KBMSelect*) tmpl->getMode(KBModes::SELECT);
    if( s == 0 )
      break;

    KBCommand* cmd= new KBCommand;
    cmd->type= KBCommands::SELECT;
    cmd->indices= s->objects;

    tmpl->post(cmd);
    }

  tmpl->popMessage();

  module->leaveMode();

  return true;
}

/////////////////////////////////////////////////////////////////////////////
