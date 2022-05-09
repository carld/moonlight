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
  ValueBar.C

  Stephane Rehel
  August 8 1996
*/

#include <stdlib.h>
#include "graphics/KeyCodes.h"

#include "Canvas.h"

#include "GWidget.h"
#include "kw/KWDialog.h"
#include "ValueBarDlg.h"

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editValueBarCallback( KWEvent& e )
{
  switch( e.type )
    {
    case KWEvent::KEY:
      if( e.widget == ValueBarDlg::linkWidget )
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

boolean Canvas::editValueBar( GWidget* gw )
{
  if( gw->type != VWidget::VALUEBAR )
    return false;

  KWDialog* d= new KWDialog;

  d->create( root, ValueBarDlg::data, (KWCallbackClass*)this );
  d->setText( ValueBarDlg::name, gw->name );
  d->setText( ValueBarDlg::linkWidget, MLString::valueOf(gw->linkWidget));
  d->setChecked( ValueBarDlg::available, gw->available );
  d->setText( ValueBarDlg::group, MLString("ValueBar ")+MLString::valueOf(gw->id) );
  if( gw->widget->isStaticWidget() )
    d->setAvailable( ValueBarDlg::linkWidget, false );
  d->setChecked( ValueBarDlg::fixedFont, gw->fixedFont );
  d->setValue( ValueBarDlg::minValue, gw->minValue );
  d->setValue( ValueBarDlg::maxValue, gw->maxValue );
  d->setValue( ValueBarDlg::nDigits, gw->nDigits );
  d->setValue( ValueBarDlg::minBar, gw->minValueBar );
  d->setValue( ValueBarDlg::maxBar, gw->maxValueBar );
  d->setValue( ValueBarDlg::editWidth, gw->editWidth );

  d->focus( ValueBarDlg::name );

  int code= d->run();

  if( code == KWDialog::VALIDATE )
    {
    gw->name= d->getText(ValueBarDlg::name);
    if( ! gw->widget->isStaticWidget() )
      {
      gw->linkWidget= strtol( d->getText(ValueBarDlg::linkWidget).get(),
                              0, 10 );
      gw->linkWidget= max( 0, gw->linkWidget );
      }
    gw->available= d->getChecked( ValueBarDlg::available );
    gw->fixedFont= d->getChecked( ValueBarDlg::fixedFont );
    gw->minValue= d->getValue(ValueBarDlg::minValue);
    gw->maxValue= d->getValue(ValueBarDlg::maxValue);
    gw->nDigits= (int)d->getValue(ValueBarDlg::nDigits);
    gw->minValueBar= d->getValue(ValueBarDlg::minBar);
    gw->maxValueBar= d->getValue(ValueBarDlg::maxBar);
    gw->editWidth= (int)d->getValue(ValueBarDlg::editWidth);
    gw->editWidth= min( gw->size.x()-10, gw->editWidth );
    }

  delete d;

  return code == KWDialog::VALIDATE;
}

/////////////////////////////////////////////////////////////////////////////
