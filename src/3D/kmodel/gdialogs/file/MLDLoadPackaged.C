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
  MLDLoadPackaged.C

  Stephane Rehel

  December 29 1997
*/

#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "graphics/mlgl.h"
#include "graphics/MLPicture.h"

#include "koala/VWidget.h"
#include "kw/KWDialog.h"
#include "kw/KWFileDialogManager.h"

#include "MLDLoadPackaged.h"
#include "LoadPackagedDlg.h"

#include "scene/file/MLDatabase.h"

#include "kmodel/ModelModule.h"
#include "kmodel/file/ModelModuleIO.h"
#include "kmodel/file/MLPackagedIO.h"

/////////////////////////////////////////////////////////////////////////////

static boolean view_snapshot= true;
static char* current_path= 0;
//static const char* errorString= "*ERROR*";

/////////////////////////////////////////////////////////////////////////////

MLDLoadPackaged::MLDLoadPackaged( ModelModule* _model ):
  MLGlobalDialog(_model)
{
  model= _model;
  snapshot= 0;
  manager= new KWFileDialogManager;
}

/////////////////////////////////////////////////////////////////////////////

MLDLoadPackaged::~MLDLoadPackaged()
{
  delete manager;
  manager= 0;

  delete snapshot;
  snapshot= 0;
}

/////////////////////////////////////////////////////////////////////////////

// return the abstract object read
// return 0 if cancelled or error
boolean MLDLoadPackaged::run()
{
  delete snapshot;
  snapshot= 0;

  if( ! MLGlobalDialog::create(LoadPackagedDlg::data) )
    return false;

  manager->init( kwDialog,
                 LoadPackagedDlg::directories,
                 LoadPackagedDlg::files,
                 LoadPackagedDlg::location,
                 LoadPackagedDlg::selection,
                 LoadPackagedDlg::ok,
                 LoadPackagedDlg::cancel,
                 LoadPackagedDlg::size,
                 LoadPackagedDlg::date,
                 LoadPackagedDlg::user );

  manager->setPattern( MLString("*") + MLDatabase::packagedExtension );
  if( current_path == 0 )
    current_path= strdup("~");
  manager->setPath(current_path);
  manager->updateDirectories();

  kwDialog->setChecked( LoadPackagedDlg::snapshot_switch, view_snapshot );

  kwDialog->focus( LoadPackagedDlg::selection );

  int outcome= kwDialog->run();

  MLString filename= manager->getFilename();

  view_snapshot= kwDialog->getChecked(LoadPackagedDlg::snapshot_switch);

  free( (void*) current_path );
  current_path= strdup( kwDialog->getText(LoadPackagedDlg::location).get() );

  MLGlobalDialog::close();

  if( outcome == KWDialog::CANCEL )
    return false;

  if( filename.length() == 0 )
    return false;

  return load(filename);
}

/////////////////////////////////////////////////////////////////////////////

MLString MLDLoadPackaged::getFilename()
{
  if( kwDialog == 0 )
    return "";

  MLString file= kwDialog->getText(LoadPackagedDlg::files);
  MLString path= kwDialog->getText(LoadPackagedDlg::location);

  MLString filename= path + file;

  return filename;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDLoadPackaged::load( const MLString& filename )
{
  if( filename.length() == 0 )
    return false;

  model->stopLightingProcess();

  if( ! model->isProjectEmpty() )
    {
    if( ! KWWarningOkCancel( "The current project is going to be cleared.\n"
                             "Continue?" ) )
    return false;
    }

  model->stopLightingProcess();

  model->resetAll();

  model->pushWaitCursor();

  MLPackagedIO p_io(model);

  boolean outcome= p_io.read(filename);

  model->popWaitCursor();

  if( ! outcome )
    {
    KWError( "Error",
               MLString("Unable to load:\n")
             + filename + MLString("\n")
             + MLString(strerror(errno)) );
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDLoadPackaged::kwCallback( KWEvent& event )
{
  if( event.type == KWEvent::SELECT &&
      event.widget == LoadPackagedDlg::files )
    updatePreview();

  if( manager->handleKWEvent(event) )
    return true;

  if( event.type == KWEvent::SELECT &&
      event.widget == LoadPackagedDlg::snapshot_switch )
    {
    updatePreview();
    return true;
    }

  if( event.type == KWEvent::DRAW &&
      event.widget == LoadPackagedDlg::snapshot )
    {
    drawPreview();
    return true;
    }

  return MLGlobalDialog::kwCallback(event);
}

/////////////////////////////////////////////////////////////////////////////

void MLDLoadPackaged::updatePreview()
{
  if( kwDialog == 0 )
    return;

  delete snapshot;
  snapshot= 0;

  if( kwDialog->getChecked(LoadPackagedDlg::snapshot_switch) )
    {
    MLString filename= getFilename();
    snapshot= ModelModuleIO::readSnapshot(filename);
    }

  kwDialog->repaintArea(LoadPackagedDlg::snapshot);
}

/////////////////////////////////////////////////////////////////////////////

void MLDLoadPackaged::drawPreview()
{
  if( kwDialog == 0 )
    return;

  VWidget* widget= kwDialog->getWidget(LoadPackagedDlg::snapshot);
  if( widget == 0 )
    return;

  if( ! widget->beginDraw() )
    return;

  if( snapshot == 0 )
    {
    IVector size= widget->getSize();
    mlLightGray();
    mlRect( 0, 0, size.x()-1, size.y()-1 );
    }
   else
    {
    snapshot->draw(0,0);
    }

  widget->endDraw();
}

/////////////////////////////////////////////////////////////////////////////
