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
  MLIOProgressDialog.C

  Stephane Rehel
  May 7 1997
*/

#include "MLIOProgressDialog.h"

#include "graphics/MLEventManager.h"
#include "graphics/mlgl.h"
#include "kw/KWDialog.h"

#include "MLGlobalDialog.h"
#include "MLModule.h"

/////////////////////////////////////////////////////////////////////////////

MLIOProgressDialog::MLIOProgressDialog( MLModule* _module )
{
  module= _module;
  kwDialog= 0;
  dialog= 0;
  progress= label= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLIOProgressDialog::~MLIOProgressDialog()
{
  close();
}

/////////////////////////////////////////////////////////////////////////////

void MLIOProgressDialog::create( const unsigned char* data,
                                 int _progress, int _label )
{
  progress= _progress;
  label= _label;

  dialog= new MLGlobalDialog(module);

  dialog->create(data);

  kwDialog= dialog->getDialog();
}

/////////////////////////////////////////////////////////////////////////////

void MLIOProgressDialog::close()
{
  delete dialog;
  dialog= 0;
  kwDialog= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLIOProgressDialog::flush()
{
  if( kwDialog == 0 )
    return;

  kwDialog->setValue(progress,d);
  kwDialog->setText(label,msg.get());

  MLEventManager* eventManager= MLEventManager::getEventManager();

  if( eventManager != 0 )
    eventManager->dispatch();

  glFlush();
}

/////////////////////////////////////////////////////////////////////////////
