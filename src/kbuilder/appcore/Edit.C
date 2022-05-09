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
  Edit.C

  Stephane Rehel
  August 8 1996
*/

#include <stdlib.h>
#include "graphics/KeyCodes.h"

#include "Canvas.h"

#include "GWidget.h"
#include "kw/KWDialog.h"
#include "EditDlg.h"

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editEditCallback( KWEvent& e )
{
  switch( e.type )
    {
    case KWEvent::KEY:
      if( e.widget == EditDlg::linkWidget )
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

boolean Canvas::editEdit( GWidget* gw )
{
  if( gw->type != VWidget::EDIT )
    return false;

  KWDialog* d= new KWDialog;

  d->create( root, EditDlg::data, (KWCallbackClass*)this );
  d->setText( EditDlg::name, gw->name );
  d->setText( EditDlg::linkWidget, MLString::valueOf(gw->linkWidget));
  d->setChecked( EditDlg::available, gw->available );
  d->setText( EditDlg::group, MLString("Edit ")+MLString::valueOf(gw->id) );
  if( gw->widget->isStaticWidget() )
    d->setAvailable( EditDlg::linkWidget, false );
  d->setChecked( EditDlg::fixedFont, gw->fixedFont );

  d->focus( EditDlg::name );

  int code= d->run();

  if( code == KWDialog::VALIDATE )
    {
    gw->name= d->getText(EditDlg::name);
    if( ! gw->widget->isStaticWidget() )
      {
      gw->linkWidget= strtol( d->getText(EditDlg::linkWidget).get(),
                              0, 10 );
      gw->linkWidget= max( 0, gw->linkWidget );
      }
    gw->available= d->getChecked( EditDlg::available );
    gw->fixedFont= d->getChecked( EditDlg::fixedFont );
    }

  delete d;

  return code == KWDialog::VALIDATE;
}

/////////////////////////////////////////////////////////////////////////////
