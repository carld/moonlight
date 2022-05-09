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
  Chooser.C

  Stephane Rehel
  August 18 1996
*/

#include <stdlib.h>
#include "graphics/KeyCodes.h"

#include "Canvas.h"

#include "GWidget.h"
#include "kw/KWDialog.h"
#include "ChooserDlg.h"

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editChooserCallback( KWEvent& e )
{
  switch( e.type )
    {
    case KWEvent::KEY:
      if( e.widget == ChooserDlg::linkWidget )
        {
        if( e.key.isAscii() )
          return !e.key.isDigit();
        return false;
        }
      if( e.widget == ChooserDlg::itemsList && e.key == KeyCodes::DELETE )
        {
        int i= e.dialog->getInteger(ChooserDlg::itemsList);
        e.dialog->deleteListText( ChooserDlg::itemsList, i );
        return true;
        }
      if( e.widget == ChooserDlg::item && e.key == KeyCodes::ENTER )
        {
        const MLString& text= e.dialog->getText(ChooserDlg::item);
        int i= e.dialog->getInteger(ChooserDlg::itemsList);
        if( e.dialog->getListTextIndex(ChooserDlg::itemsList,text) == 0 )
          {
          if( i == 0 )
            e.dialog->appendListText( ChooserDlg::itemsList, text );
           else
            e.dialog->insertListText( ChooserDlg::itemsList, i, text );
          }
        e.dialog->focus(ChooserDlg::item);
        return true;
        }
      break;

    case KWEvent::SELECT:
      {
      if( e.widget == ChooserDlg::appendItem )
        {
        const MLString& text= e.dialog->getText(ChooserDlg::item);
        if( e.dialog->getListTextIndex(ChooserDlg::itemsList,text) == 0 )
          e.dialog->appendListText( ChooserDlg::itemsList, text );
        break;
        }
      if( e.widget == ChooserDlg::insertItem )
        {
        const MLString& text= e.dialog->getText(ChooserDlg::item);
        int i= e.dialog->getInteger(ChooserDlg::itemsList);
        if( e.dialog->getListTextIndex(ChooserDlg::itemsList,text) == 0 )
          {
          if( i == 0 )
            e.dialog->appendListText( ChooserDlg::itemsList, text );
           else
            e.dialog->insertListText( ChooserDlg::itemsList, i, text );
          }
        break;
        }
      if( e.widget == ChooserDlg::deleteItem )
        {
        int i= e.dialog->getInteger(ChooserDlg::itemsList);
        if( i != 0 )
          e.dialog->deleteListText( ChooserDlg::itemsList, i );
        break;
        }
      if( e.widget == ChooserDlg::itemsList )
        {
        int i= e.dialog->getInteger(ChooserDlg::itemsList);
        if( i == 0 )
          break;
        const MLString& text= e.dialog->getListText(ChooserDlg::itemsList,i);
        e.dialog->setText( ChooserDlg::item, text );
        e.dialog->focus(ChooserDlg::item);
        }
      break;
      }

    default:
      break;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editChooser( GWidget* gw )
{
  if( gw->type != VWidget::CHOOSER )
    return false;

  KWDialog* d= new KWDialog;

  d->create( root, ChooserDlg::data, (KWCallbackClass*)this );
  d->setText( ChooserDlg::name, gw->name );
  d->setText( ChooserDlg::linkWidget, MLString::valueOf(gw->linkWidget));
  d->setChecked( ChooserDlg::available, gw->available );
  d->setText( ChooserDlg::group, MLString("Chooser ")+MLString::valueOf(gw->id) );
  if( gw->widget->isStaticWidget() )
    d->setAvailable( ChooserDlg::linkWidget, false );

  SIListIterator<MLString> li(gw->list);
  while( ! li.eol() )
    d->appendListText( ChooserDlg::itemsList, *li.next() );
  d->setInteger( ChooserDlg::itemsList, 1 );

  d->focus( ChooserDlg::name );

  int code= d->run();

  if( code == KWDialog::VALIDATE )
    {
    gw->name= d->getText(ChooserDlg::name);
    if( ! gw->widget->isStaticWidget() )
      {
      gw->linkWidget= strtol( d->getText(ChooserDlg::linkWidget).get(),
                              0, 10 );
      gw->linkWidget= max( 0, gw->linkWidget );
      }
    gw->available= d->getChecked( ChooserDlg::available );

    while( ! gw->list.empty() )
      {
      delete gw->list.getFirst();
      gw->list.removeFirst();
      }
    for( int i= 1; i <= d->getListLength(ChooserDlg::itemsList); ++i )
      {
      const MLString& string= d->getListText(ChooserDlg::itemsList,i);
      gw->list.append( new MLString(string) );
      }
    }

  delete d;

  return code == KWDialog::VALIDATE;
}

/////////////////////////////////////////////////////////////////////////////
