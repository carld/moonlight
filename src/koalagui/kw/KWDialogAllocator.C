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
  KWDialogAllocator.C

  The Koala Widget API (kWAPI)

  Stephane Rehel
  August 23 1996
*/

#include "tools/IList.h"
#include "KWDialog.h"
#include "KWDialogAllocator.h"

/////////////////////////////////////////////////////////////////////////////

static IList<KWDialog> dialogs;
static unsigned int dindex= 1;

/////////////////////////////////////////////////////////////////////////////

void KWDialogAllocator::addDialog( KWDialog* d )
{
  if( d == 0 || dialogs.exist(d) )
    return;

  dialogs.cons(d);

  while( findDialog( KWDialogID(dindex<<16) ) != 0 || dindex==0 )
    ++dindex;

  d->dialogID= KWDialogID(dindex<<16);
}

/////////////////////////////////////////////////////////////////////////////

void KWDialogAllocator::removeDialog( KWDialog* d )
{
  if( d == 0 )
    return;

  dialogs.remove(d);
}

/////////////////////////////////////////////////////////////////////////////

KWDialog* KWDialogAllocator::findDialog( register KWDialogID id )
{
  if( id == 0 )
    return 0;

  IListIterator<KWDialog> li(dialogs);
  while(li)
    {
    register KWDialog* d= li.next();
    if( d->dialogID == id )
      return d;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

