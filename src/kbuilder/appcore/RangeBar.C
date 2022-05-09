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
  RangeBar.C

  Stephane Rehel
  August 8 1996
*/

#include <stdlib.h>
#include "graphics/KeyCodes.h"

#include "Canvas.h"

#include "GWidget.h"
#include "kw/KWDialog.h"
#include "RangeBarDlg.h"

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editRangeBar( GWidget* gw )
{
  if( gw->type != VWidget::RANGEBAR )
    return false;

  KWDialog* d= new KWDialog;

  d->create( root, RangeBarDlg::data, (KWCallbackClass*)this );
  d->setText( RangeBarDlg::name, gw->name );
  d->setChecked( RangeBarDlg::available, gw->available );
  d->setText( RangeBarDlg::group, MLString("RangeBar ")+MLString::valueOf(gw->id) );
  d->setValue( RangeBarDlg::minValue, gw->minValue );
  d->setValue( RangeBarDlg::maxValue, gw->maxValue );

  d->focus( RangeBarDlg::name );

  int code= d->run();

  if( code == KWDialog::VALIDATE )
    {
    gw->name= d->getText(RangeBarDlg::name);
    gw->available= d->getChecked( RangeBarDlg::available );
    gw->minValue= d->getValue(RangeBarDlg::minValue);
    gw->maxValue= d->getValue(RangeBarDlg::maxValue);
    }

  delete d;

  return code == KWDialog::VALIDATE;
}

/////////////////////////////////////////////////////////////////////////////
