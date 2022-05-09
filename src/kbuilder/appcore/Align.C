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
  Align.C

  Stephane Rehel
  August 13 1996
*/

#include <stdlib.h>
#include "graphics/KeyCodes.h"

#include "Canvas.h"

#include "GWidget.h"
#include "kw/KWDialog.h"
#include "AlignDlg.h"

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editAlignCallback( KWEvent& e )
{
  switch( e.type )
    {
    case KWEvent::SELECT:
      if( e.widget == AlignDlg::horizAlign )
        {
        boolean ok= e.dialog->getChecked(AlignDlg::horizAlign);
        e.dialog->setAvailable(AlignDlg::left,ok);
        e.dialog->setAvailable(AlignDlg::horizCenter,ok);
        e.dialog->setAvailable(AlignDlg::right,ok);
        break;
        }
      if( e.widget == AlignDlg::vertAlign )
        {
        boolean ok= e.dialog->getChecked(AlignDlg::vertAlign);
        e.dialog->setAvailable(AlignDlg::top,ok);
        e.dialog->setAvailable(AlignDlg::vertCenter,ok);
        e.dialog->setAvailable(AlignDlg::bottom,ok);
        break;
        }
      break;

    default:
      break;
    }
  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editAlign()
{
  if( selectedWidgets.getNElements() <= 1 )
    return true;

  static boolean horizAlignFlag= false;
  static boolean vertAlignFlag= true;
  static KWidget horizAlignMode= AlignDlg::horizCenter;
  static KWidget vertAlignMode= AlignDlg::vertCenter;

  KWDialog* d= new KWDialog;

  d->create( root, AlignDlg::data, (KWCallbackClass*)this );

  d->setChecked( AlignDlg::horizAlign, horizAlignFlag );
  d->setChecked( horizAlignMode, true );
  d->setChecked( AlignDlg::vertAlign, vertAlignFlag );
  d->setChecked( vertAlignMode, true );

  if( ! horizAlignFlag )
    {
    d->setAvailable(AlignDlg::left,false);
    d->setAvailable(AlignDlg::horizCenter,false);
    d->setAvailable(AlignDlg::right,false);
    }
  if( ! vertAlignFlag )
    {
    d->setAvailable(AlignDlg::top,false);
    d->setAvailable(AlignDlg::vertCenter,false);
    d->setAvailable(AlignDlg::bottom,false);
    }

  d->focus( AlignDlg::vertAlign );

  int code= d->run();

  if( code == KWDialog::VALIDATE )
    {
    horizAlignFlag= d->getChecked(AlignDlg::horizAlign);
    horizAlignMode= d->getRadioChecked(AlignDlg::horizCenter);
    vertAlignFlag= d->getChecked(AlignDlg::vertAlign);
    vertAlignMode= d->getRadioChecked(AlignDlg::vertCenter);

    int horiz= 0;
    switch( horizAlignMode )
      {
      case AlignDlg::left: horiz= 1; break;
      case AlignDlg::horizCenter: horiz= 2; break;
      case AlignDlg::right: horiz= 3; break;
      default: horiz= 0;
      }
    if( ! horizAlignFlag )
      horiz= 0;
    int vert= 0;
    switch( vertAlignMode )
      {
      case AlignDlg::top: vert= 1; break;
      case AlignDlg::vertCenter: vert= 2; break;
      case AlignDlg::bottom: vert= 3; break;
      default: vert= 0;
      }
    if( ! vertAlignFlag )
      vert= 0;
    alignWidgets(horiz,vert);
    }

  delete d;

  draw();

  return code == KWDialog::VALIDATE;
}

/////////////////////////////////////////////////////////////////////////////
