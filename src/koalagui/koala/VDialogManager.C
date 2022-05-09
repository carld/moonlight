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
  VDialogManager.C

  Stephane Rehel

  December 18 1996
*/

#include "tools/IList.h"

#include "graphics/MLEvent.h"

#include "VDialog.h"
#include "VDialogManager.h"

/////////////////////////////////////////////////////////////////////////////

static IList<VDialog> dialogs;

/////////////////////////////////////////////////////////////////////////////

void VDialogManager::addDialog( VDialog* d )
{
  if( d == 0 || dialogs.exist(d) )
    return;

  dialogs.cons(d);
}

/////////////////////////////////////////////////////////////////////////////

void VDialogManager::removeDialog( VDialog* d )
{
  if( d == 0 )
    return;

  dialogs.remove(d);
}

/////////////////////////////////////////////////////////////////////////////

boolean VDialogManager::hasFocus( VDialog* d )
{
  if( d == 0 || dialogs.empty() )
    return false;

  return dialogs.getFirst() == d;
}

/////////////////////////////////////////////////////////////////////////////

void VDialogManager::focus( VDialog* d )
{
  if( d == 0 || dialogs.empty() )
    return;

  if( dialogs.getFirst() == d )
    return;

  if( ! dialogs.exist(d) )
    return;

  dialogs.remove(d);
  dialogs.cons(d);

  dialogs.get(2)->ungetFocus();
  d->getFocus();
}

/////////////////////////////////////////////////////////////////////////////

boolean VDialogManager::handleKey( MLEvent& event )
{
  if( event.type != MLEvent::KEY_PRESS &&
      event.type != MLEvent::KEY_RELEASE )
    return false;

  if( dialogs.empty() )
    return false;

  return dialogs.getFirst()->handleMLKeyboard(event);
}

/////////////////////////////////////////////////////////////////////////////
