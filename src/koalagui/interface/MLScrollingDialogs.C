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
  MLScrollingDialogs.C

  Stephane Rehel

  January 1st 1998
          ^^^
          woo!
*/

#include "koala/VScrollingDialogs.h"
#include "kw/KWDialog.h"

#include "MLScrollingDialogs.h"
#include "MLScrollingDialog.h"
#include "MLModule.h"
#include "Interface.h"

/////////////////////////////////////////////////////////////////////////////

static int max_scrolling_dialogs= 64;

/////////////////////////////////////////////////////////////////////////////

MLScrollingDialogs::MLScrollingDialogs( MLModule* _module )
{
  module= _module;

  dialogs= new MLScrollingDialog* [ max_scrolling_dialogs + 1 ];
  nScrollingDialogs= 0;

  for( int i= 0; i <= max_scrolling_dialogs; ++i )
    dialogs[i]= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLScrollingDialogs::~MLScrollingDialogs()
{
  removeAllDialogs();
}

/////////////////////////////////////////////////////////////////////////////

MLScrollingDialog* MLScrollingDialogs::getScrollingDialog( int i ) const
{
  if( i < 1 || i > nScrollingDialogs )
    return 0;
  if( dialogs == 0 )
    return 0;

  return dialogs[i];
}

/////////////////////////////////////////////////////////////////////////////

void MLScrollingDialogs::postRefreshDialog( int dialogID )
{
  module->getInterface()->postRefreshDialog(dialogID);
}

/////////////////////////////////////////////////////////////////////////////

void MLScrollingDialogs::postRefreshDialog( const MLString& name )
{
  MLScrollingDialog* sd= getDialog(name);

  if( sd == 0 )
    return;

  postRefreshDialog(sd->getID());
}

/////////////////////////////////////////////////////////////////////////////

void MLScrollingDialogs::refreshScreen( unsigned int what )
{
  for( int i= 1; i <= nScrollingDialogs; ++i )
    {
    MLScrollingDialog* sd= dialogs[i];
    if( sd == 0 )
      continue;

    if( (sd->getUpdateFlags() & what) != 0 )
      postRefreshDialog(sd->getID());
    }
}

/////////////////////////////////////////////////////////////////////////////

MLScrollingDialog* MLScrollingDialogs::getDialog( const MLString& name ) const
{
  for( int i= 1; i <= nScrollingDialogs; ++i )
    {
    MLScrollingDialog* sd= dialogs[i];
    if( sd->name == name )
      return sd;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLScrollingDialogs::createDialog( MLScrollingDialog* dialog )
{
  if( dialog == 0 )
    return;
  if( dialog->data == 0 )
    return;
  if( nScrollingDialogs >= max_scrolling_dialogs )
    return;

  if( getDialog(dialog->getName()) != 0 )
    {
    delete dialog;
    return;
    }

  VScrollingDialogs* scrollingDialogs= module->scrollingDialogs;
  if( scrollingDialogs == 0 )
    return;

  KWDialog* kwDialog= new KWDialog;
  kwDialog->setScrollingCartouche();
  if( ! kwDialog->create( scrollingDialogs, 0, 0, dialog->data, dialog ) )
    {
    delete kwDialog;
    return;
    }
  dialog->kwDialog= kwDialog;
//  dialog->name= kwDialog->getName();

  scrollingDialogs->append(kwDialog);

  ++nScrollingDialogs;
  dialogs[ nScrollingDialogs ]= dialog;

  dialog->justCreated();
  dialog->update();
}

/////////////////////////////////////////////////////////////////////////////

void MLScrollingDialogs::removeDialog( const MLString& name )
{
  VScrollingDialogs* scrollingDialogs= module->scrollingDialogs;
  if( scrollingDialogs == 0 )
    return;

  int i;
  for( i= 1; i <= nScrollingDialogs; ++i )
    if( dialogs[i]->name == name )
      break;

  if( i > nScrollingDialogs )
    return;

  scrollingDialogs->remove( dialogs[i]->kwDialog->getName() );

  while( i+1 <= nScrollingDialogs )
    {
    dialogs[i]= dialogs[i+1];
    ++i;
    }

  --nScrollingDialogs;
}

/////////////////////////////////////////////////////////////////////////////

void MLScrollingDialogs::removeAllDialogs()
{
  while( nScrollingDialogs > 0 )
    {
    if( dialogs[1] == 0 )
      break; // mmh?
    removeDialog( dialogs[1]->name );
    }

  nScrollingDialogs= 0;
}

/////////////////////////////////////////////////////////////////////////////
