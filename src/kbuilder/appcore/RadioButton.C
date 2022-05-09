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
  RadioButton.C

  Stephane Rehel
  August 8 1996
*/

#include "Canvas.h"

#include "GWidget.h"
#include "kw/KWDialog.h"
#include "RadioButtonDlg.h"

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editRadioButton( GWidget* gw )
{
  if( gw->type != VWidget::RADIOBUTTON )
    return false;

  KWDialog* d= new KWDialog;

  d->create( root, RadioButtonDlg::data, (KWCallbackClass*)this );

  d->setText( RadioButtonDlg::text, gw->message );
  d->setText( RadioButtonDlg::name, gw->name );
  d->setChecked( RadioButtonDlg::available, gw->available );
  d->setText( RadioButtonDlg::group,
              MLString("RadioButton ")+MLString::valueOf(gw->id) );
  d->setChecked( (gw->justify==GWidget::RIGHT) ? RadioButtonDlg::right :
                 (gw->justify==GWidget::CENTER) ? RadioButtonDlg::center :
                                                  RadioButtonDlg::left, true );
  d->setValue( RadioButtonDlg::margin, gw->margin );

  d->focus( RadioButtonDlg::text );

  int code= d->run();

  if( code == KWDialog::VALIDATE )
    {
    gw->message= d->getText(RadioButtonDlg::text);
    gw->name= d->getText(RadioButtonDlg::name);
    gw->available= d->getChecked( RadioButtonDlg::available );
    switch( d->getRadioChecked(RadioButtonDlg::left) )
      {
      case RadioButtonDlg::right: gw->justify= GWidget::RIGHT; break;
      case RadioButtonDlg::center: gw->justify= GWidget::CENTER; break;
      case RadioButtonDlg::left:
      default: gw->justify= GWidget::LEFT; break;
      }
    gw->margin= int(d->getValue(RadioButtonDlg::margin));
    }

  delete d;

  return code == KWDialog::VALIDATE;
}

/////////////////////////////////////////////////////////////////////////////
