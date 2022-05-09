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
  KBDSaveAs.C

  Stephane Rehel

  October 25 1997
*/

#include <string.h>
#include <stdlib.h>

#include "kw/KWFileDialogManager.h"
#include "kw/KWDialog.h"

#include "kbuilder/KBModule.h"

#include "KBDSaveAs.h"
#include "KBSaveAsDlg.h"

#include "kbuilder/appcore/Canvas.h"

/////////////////////////////////////////////////////////////////////////////

KBDSaveAs::KBDSaveAs( KBModule* _kb ):
  MLGlobalDialog(_kb)
{
  kb= _kb;

  manager= new KWFileDialogManager;
}

/////////////////////////////////////////////////////////////////////////////

KBDSaveAs::~KBDSaveAs()
{
  kb= 0;

  delete manager; manager= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean KBDSaveAs::run()
{
  if( ! MLGlobalDialog::create(KBSaveAsDlg::data) )
    return false;

  manager->init( kwDialog,
                 KBSaveAsDlg::directories,
                 KBSaveAsDlg::files,
                 KBSaveAsDlg::location,
                 KBSaveAsDlg::selection,
                 KBSaveAsDlg::ok,
                 KBSaveAsDlg::cancel );

  static char* currentPath= 0;

  if( currentPath == 0 )
    currentPath= strdup("~");

  manager->setPattern(Canvas::canvasExtension);

  manager->setPath(currentPath);

  manager->updateDirectories();

  Canvas* canvas= kb->getBuilderCanvas();
  kwDialog->setText( KBSaveAsDlg::selection,
                     (canvas->getFilename()
                       ==
                      (MLString("noname")+Canvas::canvasExtension))
                     ? MLString("") : canvas->getFilename() );

  kwDialog->focus(KBSaveAsDlg::selection);

  int outcome= kwDialog->run();

  free( (void*) currentPath );
  currentPath= strdup( kwDialog->getText(KBSaveAsDlg::location).get() );

  MLGlobalDialog::close();

  if( outcome == KWDialog::VALIDATE && manager != 0 )
    {
    MLString filename= manager->getFilename();

    canvas->writeCanvas(filename);

    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean KBDSaveAs::kwCallback( KWEvent& event )
{
  if( manager != 0 )
    if( manager->handleKWEvent(event) )
      return true;

  return MLGlobalDialog::kwCallback(event);
}

/////////////////////////////////////////////////////////////////////////////
