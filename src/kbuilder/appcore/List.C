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
  List.C

  Stephane Rehel
  August 12 1996
*/

#include <stdlib.h>
#include "graphics/KeyCodes.h"

#include "Canvas.h"

#include "GWidget.h"
#include "kw/KWDialog.h"
#include "ListDlg.h"

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editListCallback( KWEvent& e )
{
  switch( e.type )
    {
    case KWEvent::KEY:
      if( e.widget == ListDlg::linkWidget )
        {
        if( e.key.isAscii() )
          return !e.key.isDigit();
        return false;
        }
      break;

    case KWEvent::SELECT:
      {
      if( e.widget == ListDlg::textList )
        {
        boolean checked= e.dialog->getChecked( ListDlg::textList );
        e.dialog->setAvailable( ListDlg::fixedFont, checked );
        break;
        }
      if( e.widget == ListDlg::scrollBar )
        {
        boolean checked= e.dialog->getChecked( ListDlg::scrollBar );
        e.dialog->setAvailable( ListDlg::leftBar, checked );
        e.dialog->setAvailable( ListDlg::rightBar, checked );
        }
      break;
      }

    default:
      break;
    }
  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editList( GWidget* gw )
{
  if( gw->type != VWidget::LIST &&
      gw->type != VWidget::TEXTLIST )
    return false;

  KWDialog* d= new KWDialog;

  d->create( root, ListDlg::data, (KWCallbackClass*)this );
  d->setText( ListDlg::name, gw->name );
  d->setText( ListDlg::linkWidget, MLString::valueOf(gw->linkWidget));
  d->setChecked( ListDlg::available, gw->available );
  d->setChecked( ListDlg::textList, gw->textList );
  d->setChecked( ListDlg::fixedFont, gw->fixedFont );
  d->setChecked( ListDlg::scrollBar, gw->scrolling );
  d->setChecked( (gw->justify==GWidget::RIGHT) ? ListDlg::rightBar
                                               : ListDlg::leftBar, true );

  d->setText( ListDlg::group, MLString("List ")+MLString::valueOf(gw->id) );
  if( gw->widget->isStaticWidget() )
    d->setAvailable( ListDlg::linkWidget, false );
  d->setAvailable( ListDlg::fixedFont, gw->textList );

  d->focus( ListDlg::name );

  int code= d->run();

  if( code == KWDialog::VALIDATE )
    {
    gw->name= d->getText(ListDlg::name);
    if( ! gw->widget->isStaticWidget() )
      {
      gw->linkWidget= strtol( d->getText(ListDlg::linkWidget).get(),
                              0, 10 );
      gw->linkWidget= max( 0, gw->linkWidget );
      }
    gw->available= d->getChecked( ListDlg::available );
    gw->textList= d->getChecked( ListDlg::textList );
    gw->fixedFont= d->getChecked( ListDlg::fixedFont );
    gw->scrolling= d->getChecked( ListDlg::scrollBar );
    if( d->getRadioChecked(ListDlg::rightBar) == ListDlg::rightBar )
      gw->justify= GWidget::RIGHT;
     else
      gw->justify= GWidget::LEFT;
    }

  delete d;

  return code == KWDialog::VALIDATE;
}

/////////////////////////////////////////////////////////////////////////////
