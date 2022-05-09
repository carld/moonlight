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
  KWWarningOkCancel.C

  Stephane Rehel

  September 23 1997
*/

#include <stdio.h>
#include <stdarg.h>

#include "graphics/MLFont.h"
#include "graphics/MLFonts.h"
#include "koala/VWindow.h"

#include "KWDialog.h"
#include "KWWarningOkCancelDlg.h"

/////////////////////////////////////////////////////////////////////////////

boolean KWWarningOkCancel( const MLString& message )
{
  KWDialog* d= new KWDialog;

  d->create( 0, KWWarningOkCancelDlg::data, 0 );

  MLString** lines= KWDialog::splitString( widgetFont,
                                          message,
                d->getRectangle(KWWarningOkCancelDlg::line1).getSize().x(),
                                          4 );
  d->setText( KWWarningOkCancelDlg::line1, *lines[0] );
  d->setText( KWWarningOkCancelDlg::line2, *lines[1] );
  d->setText( KWWarningOkCancelDlg::line3, *lines[2] );
  d->setText( KWWarningOkCancelDlg::line4, *lines[3] );

  delete lines[0];
  delete lines[1];
  delete lines[2];
  delete lines[3];
  delete lines;

  d->focus( KWWarningOkCancelDlg::cancel );

  int outcome= d->run();

  delete d;

  return outcome == KWDialog::VALIDATE;
}

/////////////////////////////////////////////////////////////////////////////

// title + 4-line-message + cancel button
boolean KWWarningOkCancelf( const char* format, ... )
{
  va_list args;

  va_start( args, format );
  MLString s= MLString::vprintf( format, args );
  va_end( args );

  return KWWarningOkCancel( MLString(s) );
}

/////////////////////////////////////////////////////////////////////////////
