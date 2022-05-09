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
  TextBox.C

  Stephane Rehel
  August 8 1996
*/

#include "Canvas.h"

#include "GWidget.h"
#include "kw/KWDialog.h"
#include "TextBoxDlg.h"

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editTextBox( GWidget* gw )
{
  if( gw->type != VWidget::TEXTBOX )
    return false;

  KWDialog* d= new KWDialog;

  d->create( root, TextBoxDlg::data, (KWCallbackClass*)this );

  d->setText( TextBoxDlg::text, gw->message );
  d->setText( TextBoxDlg::name, gw->name );
  d->setChecked( TextBoxDlg::available, gw->available );
  d->setChecked( TextBoxDlg::fixedFont, gw->fixedFont );
  d->setText( TextBoxDlg::group, MLString("TextBox ")+MLString::valueOf(gw->id) );

  d->setChecked( (gw->justify==GWidget::RIGHT) ? TextBoxDlg::right :
                 (gw->justify==GWidget::LEFT)  ? TextBoxDlg::left :
                                                 TextBoxDlg::center, true );

  d->focus( TextBoxDlg::text );

  int code= d->run();

  if( code == KWDialog::VALIDATE )
    {
    gw->message= d->getText(TextBoxDlg::text);
    gw->name= d->getText(TextBoxDlg::name);
    gw->available= d->getChecked( TextBoxDlg::available );
    gw->fixedFont= d->getChecked( TextBoxDlg::fixedFont );

    switch( d->getRadioChecked(TextBoxDlg::left) )
      {
      case TextBoxDlg::right: gw->justify= GWidget::RIGHT; break;
      case TextBoxDlg::left: gw->justify= GWidget::LEFT; break;
      case TextBoxDlg::center:
      default: gw->justify= GWidget::CENTER; break;
      }
    }

  delete d;

  return code == KWDialog::VALIDATE;
}

/////////////////////////////////////////////////////////////////////////////
