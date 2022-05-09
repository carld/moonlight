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
  MLScrollingDialog.C

  Stephane Rehel

  December 18 1996
*/

#include "MLScrollingDialog.h"

#include "MLAllocator.h"

#include "MLModule.h"
#include "MLInterfaceCommand.h"

#include "kw/kw.h"

/////////////////////////////////////////////////////////////////////////////

MLScrollingDialog::MLScrollingDialog( MLModule* _mlmodule )
{
  MLAllocator::allocScrollingDialog(this);

  name= "";
  data= 0;
  kwDialog= 0;
  mlmodule= _mlmodule;
  updateFlags= ~ (unsigned int) 0;
}

/////////////////////////////////////////////////////////////////////////////

MLScrollingDialog::~MLScrollingDialog()
{
  data= 0;
  kwDialog= 0;

  MLAllocator::freeScrollingDialog(this);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLScrollingDialog::kwCallback( KWEvent& event )
{
  if( event.type == KWEvent::KILL_DIALOG )
    {
    MLScrollingDialog::remove();
    return true;
    }

  return KWCallbackClass::kwCallback(event);
}

/////////////////////////////////////////////////////////////////////////////

void MLScrollingDialog::remove()
{
  if( mlmodule == 0 )
    return;

  MLInterfaceCommand* icmd= new MLInterfaceCommand;
  icmd->type= MLInterfaceCommand::REMOVE_DIALOG;
  icmd->string= name;
  mlmodule->post(icmd);
}

/////////////////////////////////////////////////////////////////////////////

int MLScrollingDialog::getModuleID() const
{
  if( mlmodule == 0 )
    return 0;

  return mlmodule->getID();
}

/////////////////////////////////////////////////////////////////////////////
