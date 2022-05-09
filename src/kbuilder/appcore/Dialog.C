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
  Dialog.C

  Stephane Rehel
  August 9 1996
*/

#include <stdlib.h>
#include "graphics/KeyCodes.h"

#include "Canvas.h"

#include "GWidget.h"
#include "kw/KWDialog.h"
#include "DialogDlg.h"

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editDialogCallback( KWEvent& e )
{
  KWidget w= e.widget;
  KWDialog* d= e.dialog;

  switch( e.type )
    {
    case KWEvent::KEY:
      if( w == DialogDlg::X ||
          w == DialogDlg::Y ||
          w == DialogDlg::validateID ||
          w == DialogDlg::cancelID )
        {
        if( e.key.isAscii() )
          return !e.key.isDigit();
        return false;
        }
      break;

    case KWEvent::SELECT:
      if( w == DialogDlg::frame )
        {
        boolean checked= d->getChecked(DialogDlg::frame);
        d->setAvailable( DialogDlg::killButton, checked );
        d->setAvailable( DialogDlg::title, checked );
        break;
        }
      if( w == DialogDlg::validateCancel )
        {
        boolean checked= d->getChecked(DialogDlg::validateCancel);
        d->setAvailable( DialogDlg::validateID, checked );
        d->setAvailable( DialogDlg::cancelID, checked );
        break;
        }
      if( w == DialogDlg::C_filename )
        {
        d->setAvailable( DialogDlg::C_edit, d->getChecked(w) );
        if( ! d->getChecked(w) )
          d->setText( DialogDlg::C_edit,
                      d->getText(DialogDlg::name) + MLString(".C") );
        if( d->getChecked(w) )
          d->focus(DialogDlg::C_edit);
        break;
        }
      if( w == DialogDlg::h_filename )
        {
        d->setAvailable( DialogDlg::h_edit, d->getChecked(w) );
        if( ! d->getChecked(w) )
          d->setText( DialogDlg::h_edit,
                      d->getText(DialogDlg::name) + MLString(".h") );
        if( d->getChecked(w) )
          d->focus(DialogDlg::h_edit);
        break;
        }

      break;
    default:
      break;
    }
  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::editDialog()
{
  KWDialog* d= new KWDialog;

  d->create( root, DialogDlg::data, (KWCallbackClass*)this );
  d->setText( DialogDlg::filename, filename );
  d->setText( DialogDlg::name, name );
  d->setAvailable( DialogDlg::C_filename, write_C_file );

  if( !write_C_file )
    {
    d->unmap( DialogDlg::C_filename );
    d->unmap( DialogDlg::C_edit );
    }

  d->setChecked( DialogDlg::C_filename, has_C_filename );
  d->setText( DialogDlg::C_edit, C_filename );
  d->setAvailable( DialogDlg::C_edit, has_C_filename );

  d->setChecked( DialogDlg::h_filename, has_h_filename );
  d->setText( DialogDlg::h_edit, h_filename );
  d->setAvailable( DialogDlg::h_edit, has_h_filename );

  d->setText( DialogDlg::X, MLString::valueOf(dialogSize.x()) );
  d->setText( DialogDlg::Y, MLString::valueOf(dialogSize.y()) );
  d->setText( DialogDlg::title, title );
  d->setChecked( DialogDlg::frame, frame );
  d->setChecked( DialogDlg::killButton, killButton );
  d->setAvailable( DialogDlg::killButton, frame );
  d->setAvailable( DialogDlg::title, frame );

  d->setChecked( DialogDlg::validateCancel, validateCancel );
  d->setText( DialogDlg::validateID, MLString::valueOf(validateID) );
  d->setText( DialogDlg::cancelID, MLString::valueOf(cancelID) );
  d->setAvailable( DialogDlg::validateID, validateCancel );
  d->setAvailable( DialogDlg::cancelID, validateCancel );

  d->focus( DialogDlg::filename );

  int code= d->run();

  if( code == KWDialog::VALIDATE )
    {
    boolean createDialog= false;

    createDialog |= d->getText(DialogDlg::title) != title;
    createDialog |= d->getChecked(DialogDlg::frame) != frame;
    createDialog |= d->getChecked(DialogDlg::killButton) != killButton;

    filename= d->getText(DialogDlg::filename);
    name= d->getText(DialogDlg::name);
    has_C_filename= d->getChecked(DialogDlg::C_filename);
    if( has_C_filename && write_C_file )
      C_filename= d->getText(DialogDlg::C_edit);
     else
      {
      if( name.length() == 0 || !write_C_file )
        C_filename= "";
       else
        C_filename= name + MLString(".C");
      }
    has_h_filename= d->getChecked(DialogDlg::h_filename);
    if( has_h_filename )
      h_filename= d->getText(DialogDlg::h_edit);
     else
      {
      if( name.length() == 0 )
        h_filename= "";
       else
        h_filename= name + MLString(".h");
      }

    title= d->getText(DialogDlg::title);
    frame= d->getChecked(DialogDlg::frame);
    killButton= d->getChecked(DialogDlg::killButton);
    validateCancel= d->getChecked(DialogDlg::validateCancel);

    IRectangle bound= getBounding();
    int x= strtol( d->getText(DialogDlg::X).get(), 0, 10 );
    int y= strtol( d->getText(DialogDlg::Y).get(), 0, 10 );
    x= max( x, bound.p2().x()+1 );
    y= max( y, bound.p2().y()+1 );
    x= max( 20, x );
    y= max( 20, y );

    boolean resizeDialog= (dialogSize != IVector(x,y));

    validateID= strtol( d->getText(DialogDlg::validateID).get(), 0, 10 );
    cancelID= strtol( d->getText(DialogDlg::cancelID).get(), 0, 10 );

    if( createDialog )
      recreate();
     else
      {
      if( resizeDialog )
        changeDialogSize( IVector(x,y) );
      }
    }

  delete d;

  return code == KWDialog::VALIDATE;
}

/////////////////////////////////////////////////////////////////////////////
