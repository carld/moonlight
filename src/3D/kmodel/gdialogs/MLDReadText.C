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
  MLDReadText.C

  Stephane Rehel

  August 9 1997
*/

#include <string.h>

#include "graphics/KeyCodes.h"

#include "kw/KWDialog.h"

#include "kmodel/ModelModule.h"

#include "MLDReadText.h"
#include "ReadTextDlg.h"

/////////////////////////////////////////////////////////////////////////////

MLDReadText::MLDReadText( ModelModule* _model, const char** _text ):
  MLGlobalDialog(_model)
{
  text= _text;
}

/////////////////////////////////////////////////////////////////////////////

MLDReadText::~MLDReadText()
{}

/////////////////////////////////////////////////////////////////////////////

void MLDReadText::run()
{
  if( ! MLGlobalDialog::create(ReadTextDlg::data) )
    return;

  const char** s= text;
  while( *s != 0 )
    {
    const char* line= *s;
    kwDialog->appendListText( ReadTextDlg::text, line );
    ++s;
    }

  kwDialog->focus(ReadTextDlg::text);

  kwDialog->run();

  MLGlobalDialog::close();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDReadText::kwCallback( KWEvent& event )
{
  if( event.type == KWEvent::KEY &&
      event.key == KeyCodes::ESC )
    {
    kwDialog->quit();
    return true;
    }

  return MLGlobalDialog::kwCallback(event);
}

/////////////////////////////////////////////////////////////////////////////

