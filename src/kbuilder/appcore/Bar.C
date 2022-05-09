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
  Bar.C

  Stephane Rehel
  August 8 1996
*/

#include "Canvas.h"

#include "GWidget.h"
#include "kw/KWDialog.h"
#include "BarDlg.h"

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editBar( GWidget* gw )
{
  if( gw->type != VWidget::BAR &&
      gw->type != VWidget::SCROLLBAR )
    return false;

  KWDialog* d= new KWDialog;

  d->create( root, BarDlg::data, (KWCallbackClass*)this );

  d->setText( BarDlg::name, gw->name );
  d->setText( BarDlg::group, MLString("Bar ")+MLString::valueOf(gw->id) );
  d->setChecked( BarDlg::available, gw->available );
  d->setChecked( BarDlg::arrowButtons, gw->type == VWidget::SCROLLBAR );

  d->focus( BarDlg::name );

  int code= d->run();

  if( code == KWDialog::VALIDATE )
    {
    gw->name= d->getText(BarDlg::name);
    gw->available= d->getChecked(BarDlg::available);
    gw->type= d->getChecked(BarDlg::arrowButtons) ? VWidget::SCROLLBAR
                                                  : VWidget::BAR;
    }

  delete d;

  return code == KWDialog::VALIDATE;
}

/////////////////////////////////////////////////////////////////////////////
