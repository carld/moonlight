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
  MLDQuitInterface.C

  Stephane Rehel

  August 8 1997
*/

#include "kw/KWDialog.h"

#include "QuitInterfaceDlg.h"
#include "MLDQuitInterface.h"

/////////////////////////////////////////////////////////////////////////////

MLDQuitInterface::MLDQuitInterface( Interface* interface ):
  MLGlobalDialog(interface)
{}

/////////////////////////////////////////////////////////////////////////////

MLDQuitInterface::~MLDQuitInterface()
{}

/////////////////////////////////////////////////////////////////////////////

int MLDQuitInterface::run()
{
  if( ! MLGlobalDialog::create(QuitInterfaceDlg::data) )
    return DONT_QUIT;

  kwDialog->focus(QuitInterfaceDlg::cancel);

  state= 0;

  int outcome= kwDialog->run();

  MLGlobalDialog::close();

  if( state == 0 )
    return (outcome==KWDialog::VALIDATE) ? int(REALLY_QUIT)
                                         : int(DONT_QUIT);
   else
    return int(state);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDQuitInterface::kwCallback( KWEvent& event )
{
  if( event.type == KWEvent::SELECT &&
      event.widget == QuitInterfaceDlg::iconify )
    {
    state= int(ICONIFY);
    kwDialog->quit();
    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

