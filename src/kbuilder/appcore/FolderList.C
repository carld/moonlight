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
  FolderList.C

  Stephane Rehel
  August 12 1996
*/

#include <stdlib.h>
#include "graphics/KeyCodes.h"

#include "Canvas.h"

#include "GWidget.h"
#include "kw/KWDialog.h"
#include "FolderListDlg.h"

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editFolderListCallback( KWEvent& e )
{
  switch( e.type )
    {
    case KWEvent::KEY:
      if( e.widget == FolderListDlg::linkWidget )
        {
        if( e.key.isAscii() )
          return !e.key.isDigit();
        return false;
        }
      break;

    case KWEvent::SELECT:
      {
      if( e.widget == FolderListDlg::textList )
        {
        boolean checked= e.dialog->getChecked( FolderListDlg::textList );
        e.dialog->setAvailable( FolderListDlg::fixedFont, checked );
        break;
        }
      if( e.widget == FolderListDlg::scrollBar )
        {
        boolean checked= e.dialog->getChecked( FolderListDlg::scrollBar );
        e.dialog->setAvailable( FolderListDlg::leftBar, checked );
        e.dialog->setAvailable( FolderListDlg::rightBar, checked );
        }
      break;
      }

    default:
      break;
    }
  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editFolderList( GWidget* gw )
{
  if( gw->type != VWidget::FOLDERLIST )
    return false;

  KWDialog* d= new KWDialog;

  d->create( root, FolderListDlg::data, (KWCallbackClass*)this );
  d->setText( FolderListDlg::name, gw->name );
  d->setText( FolderListDlg::linkWidget, MLString::valueOf(gw->linkWidget));
  d->setChecked( FolderListDlg::available, gw->available );
  d->setChecked( FolderListDlg::textList, gw->textList );
  d->setChecked( FolderListDlg::fixedFont, gw->fixedFont );
  d->setChecked( FolderListDlg::plusminusFolder, gw->folderPlusMinus );
  d->setChecked( FolderListDlg::scrollBar, gw->scrolling );
  d->setChecked( (gw->justify==GWidget::RIGHT) ? FolderListDlg::rightBar
                                               : FolderListDlg::leftBar, true );

  d->setText( FolderListDlg::group, MLString("List ")+MLString::valueOf(gw->id) );
  if( gw->widget->isStaticWidget() )
    d->setAvailable( FolderListDlg::linkWidget, false );
  d->setAvailable( FolderListDlg::fixedFont, gw->textList );

  d->focus( FolderListDlg::name );

  int code= d->run();

  if( code == KWDialog::VALIDATE )
    {
    gw->name= d->getText(FolderListDlg::name);
    if( ! gw->widget->isStaticWidget() )
      {
      gw->linkWidget= strtol( d->getText(FolderListDlg::linkWidget).get(),
                              0, 10 );
      gw->linkWidget= max( 0, gw->linkWidget );
      }
    gw->available= d->getChecked( FolderListDlg::available );
    gw->textList= d->getChecked( FolderListDlg::textList );
    gw->fixedFont= d->getChecked( FolderListDlg::fixedFont );
    gw->folderPlusMinus= d->getChecked( FolderListDlg::plusminusFolder );
    gw->scrolling= d->getChecked( FolderListDlg::scrollBar );
    if( d->getRadioChecked(FolderListDlg::rightBar) == FolderListDlg::rightBar )
      gw->justify= GWidget::RIGHT;
     else
      gw->justify= GWidget::LEFT;
    }

  delete d;

  return code == KWDialog::VALIDATE;
}

/////////////////////////////////////////////////////////////////////////////
