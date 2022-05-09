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
  Label.C

  Stephane Rehel
  August 8 1996
*/

#include "Canvas.h"

#include "GWidget.h"
#include "kw/KWDialog.h"
#include "LabelDlg.h"

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editLabel( GWidget* gw )
{
  if( gw->type != VWidget::LABEL )
    return false;

  KWDialog* d= new KWDialog;

  d->create( root, LabelDlg::data, (KWCallbackClass*)this );

  d->setText( LabelDlg::text, gw->message );
  d->setText( LabelDlg::name, gw->name );
  d->setChecked( LabelDlg::available, gw->available );
  d->setValue( LabelDlg::labelHeight, gw->size.y() );
  d->setText( LabelDlg::group, MLString("Label ")+MLString::valueOf(gw->id) );

  d->setChecked( (gw->justify==GWidget::RIGHT) ? LabelDlg::right :
                 (gw->justify==GWidget::CENTER) ? LabelDlg::center :
                                                  LabelDlg::left, true );
  d->setValue( LabelDlg::margin, gw->margin );

  d->setChecked( gw->fixedFont ? LabelDlg::fixedFont :
                   gw->bigFont ? LabelDlg::bigFont :
                                 LabelDlg::normalFont, true );

  d->focus( LabelDlg::text );

  int code= d->run();

  if( code == KWDialog::VALIDATE )
    {
    gw->message= d->getText(LabelDlg::text);
    gw->name= d->getText(LabelDlg::name);
    gw->available= d->getChecked( LabelDlg::available );
    gw->size[1]= int(d->getValue(LabelDlg::labelHeight));
    switch( d->getRadioChecked(LabelDlg::normalFont) )
      {
      case LabelDlg::fixedFont:
        gw->fixedFont= true;
        gw->bigFont= false;
        break;
      case LabelDlg::bigFont:
        gw->fixedFont= false;
        gw->bigFont= true;
        break;
      default:
        gw->fixedFont= false;
        gw->bigFont= false;
        break;
      }
    switch( d->getRadioChecked(LabelDlg::left) )
      {
      case LabelDlg::right: gw->justify= GWidget::RIGHT; break;
      case LabelDlg::center: gw->justify= GWidget::CENTER; break;
      case LabelDlg::left:
      default: gw->justify= GWidget::LEFT; break;
      }
    gw->margin= int(d->getValue(LabelDlg::margin));
    }

  delete d;

  return code == KWDialog::VALIDATE;
}

/////////////////////////////////////////////////////////////////////////////
