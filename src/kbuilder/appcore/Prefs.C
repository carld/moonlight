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
  Prefs.C

  Stephane Rehel
  August 15 1996
*/

#include "Canvas.h"

#include "GWidget.h"
#include "kw/KWDialog.h"
#include "PrefsDlg.h"

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editPrefs()
{
  KWDialog* d= new KWDialog;

  d->create( root, PrefsDlg::data, (KWCallbackClass*)0 );

  d->setValue( PrefsDlg::granul, defaultGranularity );
  d->setChecked( PrefsDlg::drawGroups, drawGroups );
  d->setValue( PrefsDlg::top, top_margin );
  d->setValue( PrefsDlg::bottom, bottom_margin );
  d->setValue( PrefsDlg::right, right_margin );
  d->setValue( PrefsDlg::left, left_margin );

  d->focus( PrefsDlg::granul );

  int code= d->run();

  if( code == KWDialog::VALIDATE )
    {
    defaultGranularity= int(d->getValue(PrefsDlg::granul));
    defaultGranularity= max( 2, defaultGranularity );
    if( granularity != 1 )
      granularity= defaultGranularity;

    drawGroups= d->getChecked(PrefsDlg::drawGroups);
    top_margin= (int) d->getValue(PrefsDlg::top);
    bottom_margin= (int) d->getValue(PrefsDlg::bottom);
    right_margin= (int) d->getValue(PrefsDlg::right);
    left_margin= (int) d->getValue(PrefsDlg::left);
    }

  delete d;

  draw();

  return code == KWDialog::VALIDATE;
}

/////////////////////////////////////////////////////////////////////////////
