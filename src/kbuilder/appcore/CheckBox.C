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
  CheckBox.C

  Stephane Rehel
  August 8 1996
*/

#include "Canvas.h"

#include "GWidget.h"
#include "kw/KWDialog.h"
#include "CheckBoxDlg.h"

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editCheckBox( GWidget* gw )
{
  if( gw->type != VWidget::CHECKBOX )
    return false;

  KWDialog* d= new KWDialog;

  d->create( root, CheckBoxDlg::data, 0 );

  d->setText( CheckBoxDlg::text, gw->message );
  d->setText( CheckBoxDlg::name, gw->name );
  d->setChecked( CheckBoxDlg::available, gw->available );
  d->setText( CheckBoxDlg::group, MLString("CheckBox ")+MLString::valueOf(gw->id) );
  d->setChecked( (gw->justify==GWidget::RIGHT) ? CheckBoxDlg::right :
                 (gw->justify==GWidget::CENTER) ? CheckBoxDlg::center :
                                                  CheckBoxDlg::left, true );
  d->setValue( CheckBoxDlg::margin, gw->margin );

  d->focus( CheckBoxDlg::text );

  int code= d->run();

  if( code == KWDialog::VALIDATE )
    {
    gw->message= d->getText(CheckBoxDlg::text);
    gw->name= d->getText(CheckBoxDlg::name);
    gw->available= d->getChecked( CheckBoxDlg::available );
    switch( d->getRadioChecked(CheckBoxDlg::left) )
      {
      case CheckBoxDlg::right: gw->justify= GWidget::RIGHT; break;
      case CheckBoxDlg::center: gw->justify= GWidget::CENTER; break;
      case CheckBoxDlg::left:
      default: gw->justify= GWidget::LEFT; break;
      }
    gw->margin= int(d->getValue(CheckBoxDlg::margin));
    }

  delete d;

  return code == KWDialog::VALIDATE;
}

/////////////////////////////////////////////////////////////////////////////
