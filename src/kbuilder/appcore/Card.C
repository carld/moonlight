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
  Card.C

  Stephane Rehel
  August 20 1996
*/

#include "Canvas.h"

#include "koala/VCard.h"
#include "kw/KWDialog.h"

#include "GWidget.h"
#include "CardDlg.h"

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editCard()
{
  if( activeCard == 0 )
    return false;
  GWidget* gw= findWidget(activeCard->getWidgetID());
  if( gw == 0 )
    return false;

  KWDialog* d= new KWDialog;

  d->create( root, CardDlg::data, 0 );

  d->setText( CardDlg::text, gw->message );
  d->setText( CardDlg::name, gw->name );
  d->setChecked( CardDlg::available, gw->available );
  d->setText( CardDlg::group, MLString("Card ")+MLString::valueOf(gw->id) );

  d->focus( CardDlg::text );

  int code= d->run();

  if( code == KWDialog::VALIDATE )
    {
    gw->message= d->getText(CardDlg::text);
    if( gw->widget != 0 )
      ((VCard*)(gw->widget))->changeMessage(gw->message);
    gw->name= d->getText(CardDlg::name);
    gw->available= d->getChecked( CardDlg::available );
    }

  delete d;

  return code == KWDialog::VALIDATE;
}

/////////////////////////////////////////////////////////////////////////////
