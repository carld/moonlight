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
  MLDSavePackaged.C

  Stephane Rehel

  December 29 1997
*/

#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "tools/MLPath.h"
#include "tools/MLFilename.h"

#include "graphics/mlgl.h"
#include "graphics/MLPicture.h"

#include "koala/VWidget.h"
#include "kw/KWDialog.h"
#include "kw/KWFileDialogManager.h"

#include "MLDSavePackaged.h"
#include "SavePackagedDlg.h"

#include "scene/file/MLDatabase.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelFlags.h"
#include "kmodel/file/ModelModuleIO.h"
#include "kmodel/file/MLPackagedIO.h"

/////////////////////////////////////////////////////////////////////////////

static char* current_path= 0;
//static const char* errorString= "*ERROR*";

/////////////////////////////////////////////////////////////////////////////

MLDSavePackaged::MLDSavePackaged( ModelModule* _model ):
  MLGlobalDialog(_model)
{
  model= _model;
  snapshot= 0;
  manager= new KWFileDialogManager;
}

/////////////////////////////////////////////////////////////////////////////

MLDSavePackaged::~MLDSavePackaged()
{
  delete manager;
  manager= 0;

  delete snapshot;
  snapshot= 0;
}

/////////////////////////////////////////////////////////////////////////////

// return the abstract object read
// return 0 if cancelled or error
boolean MLDSavePackaged::run()
{
  delete snapshot;
  snapshot= 0;

  if( ! MLGlobalDialog::create(SavePackagedDlg::data) )
    return false;

  manager->init( kwDialog,
                 SavePackagedDlg::directories,
                 SavePackagedDlg::files,
                 SavePackagedDlg::location,
                 SavePackagedDlg::selection,
                 SavePackagedDlg::ok,
                 SavePackagedDlg::cancel );

  manager->setPattern( MLString("*") + MLDatabase::packagedExtension );
  if( current_path == 0 )
    current_path= strdup("~");
  manager->setPath(current_path);
  manager->updateDirectories();

  kwDialog->setChecked( SavePackagedDlg::snapshot,
                        model->getFlags()->writeSnapshot );

  kwDialog->focus( SavePackagedDlg::selection );

  int outcome= kwDialog->run();

  MLString filename= manager->getFilename();
  boolean include_snapshot= kwDialog->getChecked(SavePackagedDlg::snapshot);

  free( (void*) current_path );
  current_path= strdup( kwDialog->getText(SavePackagedDlg::location).get() );

  MLGlobalDialog::close();

  if( outcome == KWDialog::CANCEL )
    return false;

  if( filename.length() == 0 )
    return false;

  return save(filename,include_snapshot);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDSavePackaged::save( const MLString& _filename,
                             boolean include_snapshot )
{
  if( _filename.length() == 0 )
    return false;

  MLString filename= _filename;
  if( MLFilename(filename).getExtension().length() == 0 )
    filename += MLString(MLDatabase::packagedExtension);

  model->stopLightingProcess();

  model->pushWaitCursor();

  model->flags->writeSnapshot= include_snapshot;

  MLPackagedIO p_io(model);

  boolean outcome= p_io.write(filename);

  model->popWaitCursor();

  if( ! outcome )
    {
    KWError( "Error",
               MLString("Unable to save:\n")
             + filename + MLString("\n")
             + MLString(strerror(errno)) );
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDSavePackaged::kwCallback( KWEvent& event )
{
  if( manager->handleKWEvent(event) )
    return true;

  return MLGlobalDialog::kwCallback(event);
}

/////////////////////////////////////////////////////////////////////////////
