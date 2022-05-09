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
  KWError.C

  Stephane Rehel
  August 16 1996
*/

#include <stdio.h>
#include <stdarg.h>

#include "graphics/MLFont.h"
#include "graphics/MLFonts.h"
#include "koala/VWindow.h"

#include "KWDialog.h"
#include "KWErrorDlg.h"

/////////////////////////////////////////////////////////////////////////////

void KWError( const MLString& title, const MLString& message )
{
  KWDialog* d= new KWDialog;

  d->create( 0, KWErrorDlg::data, 0 );

  d->setText( KWErrorDlg::title, title );
  MLString** lines= KWDialog::splitString( widgetFont,
                                          message,
                            d->getRectangle(KWErrorDlg::line1).getSize().x(),
                                          4 );
  d->setText( KWErrorDlg::line1, *lines[0] );
  d->setText( KWErrorDlg::line2, *lines[1] );
  d->setText( KWErrorDlg::line3, *lines[2] );
  d->setText( KWErrorDlg::line4, *lines[3] );

  delete lines[0];
  delete lines[1];
  delete lines[2];
  delete lines[3];
  delete lines;

  d->focus( KWErrorDlg::cancel );

  d->run();

  delete d;
}

/////////////////////////////////////////////////////////////////////////////

// title + 4-line-message + cancel button
void KWErrorf( const MLString& title, const char* format, ... )
{
  va_list args;

  va_start( args, format );
  KWError( title, MLString::vprintf(format,args) );
  va_end( args );

}

/////////////////////////////////////////////////////////////////////////////
