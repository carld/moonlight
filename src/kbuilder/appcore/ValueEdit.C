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
  ValueEdit.C

  Stephane Rehel
  August 8 1996
*/

#include <stdlib.h>
#include "graphics/KeyCodes.h"

#include "Canvas.h"

#include "GWidget.h"
#include "kw/KWDialog.h"
#include "ValueEditDlg.h"

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editValueEditCallback( KWEvent& e )
{
  switch( e.type )
    {
    case KWEvent::KEY:
      if( e.widget == ValueEditDlg::linkWidget )
        {
        if( e.key.isAscii() )
          return !e.key.isDigit();
        return false;
        }
      break;

    default:
      break;
    }
  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editValueEdit( GWidget* gw )
{
  if( gw->type != VWidget::VALUEEDIT )
    return false;

  KWDialog* d= new KWDialog;

  d->create( root, ValueEditDlg::data, (KWCallbackClass*)this );
  d->setText( ValueEditDlg::name, gw->name );
  d->setText( ValueEditDlg::linkWidget, MLString::valueOf(gw->linkWidget));
  d->setChecked( ValueEditDlg::available, gw->available );
  d->setText( ValueEditDlg::group, MLString("ValueEdit ")+MLString::valueOf(gw->id) );
  if( gw->widget->isStaticWidget() )
    d->setAvailable( ValueEditDlg::linkWidget, false );
  d->setChecked( ValueEditDlg::fixedFont, gw->fixedFont );
  d->setValue( ValueEditDlg::minValue, gw->minValue );
  d->setValue( ValueEditDlg::maxValue, gw->maxValue );
  d->setValue( ValueEditDlg::nDigits, gw->nDigits );

  d->focus( ValueEditDlg::name );

  int code= d->run();

  if( code == KWDialog::VALIDATE )
    {
    gw->name= d->getText(ValueEditDlg::name);
    if( ! gw->widget->isStaticWidget() )
      {
      gw->linkWidget= strtol( d->getText(ValueEditDlg::linkWidget).get(),
                              0, 10 );
      gw->linkWidget= max( 0, gw->linkWidget );
      }
    gw->available= d->getChecked( ValueEditDlg::available );
    gw->fixedFont= d->getChecked( ValueEditDlg::fixedFont );
    gw->minValue= d->getValue(ValueEditDlg::minValue);
    gw->maxValue= d->getValue(ValueEditDlg::maxValue);
    gw->nDigits= (int)d->getValue(ValueEditDlg::nDigits);
    }

  delete d;

  return code == KWDialog::VALIDATE;
}

/////////////////////////////////////////////////////////////////////////////
