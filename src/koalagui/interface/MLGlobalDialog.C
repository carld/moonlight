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
  MLGlobalDialog.C

  Stephane Rehel
  April 20 1997
*/

#include <stdio.h>

#include "interface/Interface.h"

#include "MLGlobalDialog.h"
#include "MLModule.h"

#include "kw/KWDialog.h"

/////////////////////////////////////////////////////////////////////////////

struct RegisteredDialog
  {
  const unsigned char* data;
  KWDialog* dialog; // null if not open
  KWidget activeCard;
  };

static const int nMaxDialogs= 100;
static RegisteredDialog dialogs[nMaxDialogs+1];
static int nDialogs= 0;

/////////////////////////////////////////////////////////////////////////////

static KWidget register_dialog( const unsigned char* data, KWDialog* dialog )
{
  if( data == 0 )
    return KWidget(0);

  for( int i= 1; i <= nDialogs; ++i )
    if( dialogs[i].data == data )
      {
      dialogs[i].dialog= dialog;
      return dialogs[i].activeCard;
      }

  if( nDialogs >= nMaxDialogs )
    {
    fprintf( stderr, "MLGlobalDialog: nMaxDialogs reached!\n" );
    return KWidget(0);
    }

  ++nDialogs;

  dialogs[nDialogs].data= data;
  dialogs[nDialogs].dialog= dialog;
  dialogs[nDialogs].activeCard= KWidget(0);

  return KWidget(0);
}

/////////////////////////////////////////////////////////////////////////////

MLGlobalDialog::MLGlobalDialog( Interface* _interface )
{
  interface= _interface;
  module= 0;
  kwDialog= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLGlobalDialog::MLGlobalDialog( MLModule* _module )
{
  module= _module;
  interface= module->interface;
  kwDialog= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLGlobalDialog::~MLGlobalDialog()
{
  close();
}

/////////////////////////////////////////////////////////////////////////////

void MLGlobalDialog::close()
{
  if( kwDialog != 0 )
    {
    KWidget activeCard= kwDialog->getActiveCard();
    if( activeCard != KWidget(0) )
      {
      for( int i= 1; i <= ::nDialogs; ++i )
        if( dialogs[i].dialog == kwDialog )
          {
          dialogs[i].activeCard= kwDialog->getActiveCard();
          dialogs[i].dialog= 0;
          }
      }
    }

  delete kwDialog;
  kwDialog= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLGlobalDialog::create( int x, int y, const unsigned char* dialogMap )
{
  if( kwDialog != 0 )
    {
    delete kwDialog;
    kwDialog= 0;
    }

  interface->pushWaitCursor();

  boolean ok= true;

  kwDialog= new KWDialog;

  if( ! kwDialog->create( 0, x, y, dialogMap, this ) )
    {
    delete kwDialog;
    kwDialog= 0;
    ok= false;
    }

  KWidget activeCard= register_dialog(dialogMap,kwDialog);
  if( activeCard != KWidget(0) )
    kwDialog->activateCard(activeCard);

  interface->popWaitCursor();

  return ok;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLGlobalDialog::create( const unsigned char* dialogMap )
{
  if( kwDialog != 0 )
    {
    delete kwDialog;
    kwDialog= 0;
    }

  interface->pushWaitCursor();

  boolean ok= true;

  kwDialog= new KWDialog;

  if( ! kwDialog->create( 0, dialogMap, this ) )
    {
    delete kwDialog;
    kwDialog= 0;
    ok= false;
    }

  KWidget active_card= register_dialog(dialogMap,kwDialog);

  if( active_card != KWidget(0) )
    kwDialog->activateCard(active_card);

  interface->popWaitCursor();

  return ok;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLGlobalDialog::kwCallback( KWEvent& event )
{
  return KWCallbackClass::kwCallback(event);
}

/////////////////////////////////////////////////////////////////////////////

// static
void MLGlobalDialog::resetAll()
{
  nDialogs= 0;
}

/////////////////////////////////////////////////////////////////////////////
