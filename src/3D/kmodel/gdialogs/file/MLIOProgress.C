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
  MLIOProgress.C

  Stephane Rehel
  May 5 1997
*/

#include "MLIOProgress.h"

#include "interface/MLGlobalDialog.h"
#include "kmodel/ModelModule.h"

#include "kw/KWDialog.h"

/////////////////////////////////////////////////////////////////////////////

MLIOProgress::MLIOProgress( ModelModule* _model )
{
  model= _model;
  kwDialog= 0;
  dialog= 0;
  progress= label= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLIOProgress::~MLIOProgress()
{
  close();
}

/////////////////////////////////////////////////////////////////////////////

void MLIOProgress::create( const unsigned char* data,
                           int _progress, int _label )
{
  progress= _progress;
  label= _label;

  dialog= new MLGlobalDialog(model);

  dialog->create(data);

  kwDialog= dialog->getDialog();
}

/////////////////////////////////////////////////////////////////////////////

void MLIOProgress::close()
{
  delete dialog;
  dialog= 0;
  kwDialog= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLIOProgress::empty()
{
  set(0.);
}

/////////////////////////////////////////////////////////////////////////////

void MLIOProgress::full()
{
  set(1.);
}

/////////////////////////////////////////////////////////////////////////////

void MLIOProgress::set( double d ) // d=0..1
{
  if( kwDialog == 0 )
    return;

  kwDialog->setValue(progress,d);
}

/////////////////////////////////////////////////////////////////////////////

void MLIOProgress::setMessage( const MLString& msg )
{
  if( kwDialog == 0 )
    return;

  kwDialog->setText(label,msg);
}

/////////////////////////////////////////////////////////////////////////////

void MLIOProgress::flush()
{
}

/////////////////////////////////////////////////////////////////////////////

