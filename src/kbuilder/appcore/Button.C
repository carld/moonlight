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
  Button.C

  Stephane Rehel
  August 8 1996
*/

#include <stdlib.h>
#include "graphics/KeyCodes.h"

#include "Canvas.h"

#include "GWidget.h"
#include "kw/KWDialog.h"
#include "ButtonDlg.h"

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editButtonCallback( KWEvent& e )
{
  switch( e.type )
    {
    case KWEvent::KEY:
      if( e.widget == ButtonDlg::linkWidget )
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

boolean Canvas::editButton( GWidget* gw )
{
  if( gw->type != VWidget::BUTTON )
    return false;

  KWDialog* d= new KWDialog;

  d->create( root, ButtonDlg::data, (KWCallbackClass*)this );
  d->setText( ButtonDlg::text, gw->message );
  d->setText( ButtonDlg::name, gw->name );
  d->setText( ButtonDlg::linkWidget, MLString::valueOf(gw->linkWidget));
  d->setChecked( ButtonDlg::available, gw->available );
  d->setText( ButtonDlg::group, MLString("Button ")+MLString::valueOf(gw->id) );
  if( gw->widget->isStaticWidget() )
    d->setAvailable( ButtonDlg::linkWidget, false );
  d->setChecked( ButtonDlg::validate, gw->enterPXM );
  if( ! validateCancel )
    d->setAvailable( ButtonDlg::validate, false );

  d->focus( ButtonDlg::text );

  int code= d->run();

  if( code == KWDialog::VALIDATE )
    {
    gw->message= d->getText(ButtonDlg::text);
    gw->name= d->getText(ButtonDlg::name);
    if( ! gw->widget->isStaticWidget() )
      {
      gw->linkWidget= strtol( d->getText(ButtonDlg::linkWidget).get(),
                              0, 10 );
      gw->linkWidget= max( 0, gw->linkWidget );
      }
    gw->available= d->getChecked( ButtonDlg::available );
    if( validateCancel )
      gw->enterPXM= d->getChecked( ButtonDlg::validate );
    }

  delete d;

  return code == KWDialog::VALIDATE;
}

/////////////////////////////////////////////////////////////////////////////
