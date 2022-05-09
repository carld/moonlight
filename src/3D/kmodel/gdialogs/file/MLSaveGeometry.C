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
  MLSaveGeometry.C

  Stephane Rehel
  April 22 1997
*/

#include <string.h>
#include <errno.h>

#include "tools/MLFilename.h"
#include "tools/ListFiles.h"

#include "kw/KWDialog.h"

#include "interface/MLIOProgressDialog.h"

#include "kmodel/ModelModule.h"

#include "scene/file/MLDatabasesList.h"
#include "scene/file/MLDatabase.h"
#include "scene/file/MLAbstractMeshIO.h"

#include "MLSaveGeometry.h"
#include "SaveGeometryDlg.h"
#include "WritingProgressDlg.h"

/////////////////////////////////////////////////////////////////////////////

static const char* errorString= "*ERROR*";

/////////////////////////////////////////////////////////////////////////////

MLSaveGeometry::MLSaveGeometry( ModelModule* _model ):
  MLGlobalDialog(_model)
{
  model= _model;
  dblist= 0;
  files= new ListFiles;
}

/////////////////////////////////////////////////////////////////////////////

MLSaveGeometry::~MLSaveGeometry()
{
  delete dblist;
  dblist= 0;

  delete files;
  files= 0;
}

/////////////////////////////////////////////////////////////////////////////

// return the abstract object read
// return 0 if cancelled or error
boolean MLSaveGeometry::run( MLAbstractMesh* ao )
{
  if( ao == 0 )
    return false;

  delete dblist;
  dblist= new MLDatabasesList;
  dblist->build();

  if( dblist->get().empty() )
    {
/*
    KWError( "Error",
               MLString("No database available!\nCheck: ")
             + MLDatabasesList::getDatabasesResourceFilename() );
*/
    KWError( "Error",
             MLString("No database available.\nCheck your resource file!") );
    return false;
    }

  if( ! MLGlobalDialog::create(SaveGeometryDlg::data) )
    return false;

  SIListIterator<MLDatabase> li(dblist->get());
  while( ! li.eol() )
    {
    MLDatabase* db= li++;
    kwDialog->appendListText( SaveGeometryDlg::databases,
                              db->getBasePath() );
    }
  kwDialog->setInteger( SaveGeometryDlg::databases,
                        dblist->getCurrentDatabasePosition());

  MLString dbpath= kwDialog->getText(SaveGeometryDlg::databases);
  currentDatabase= dblist->getDatabase(dbpath);

  updateFilesList();

  kwDialog->setText( SaveGeometryDlg::filename,
                     MLAbstractMeshIO::getFilename(ao) );

  kwDialog->focus(SaveGeometryDlg::filename);

  int outcome= kwDialog->run();

  MLString filename= kwDialog->getText(SaveGeometryDlg::filename);
  if( filename == errorString )
    filename= "";

  if( outcome == KWDialog::CANCEL || filename.length() == 0 )
    {
    MLGlobalDialog::close();
    return false;
    }

  boolean ok= save(filename,ao);

  MLGlobalDialog::close();

  return ok;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLSaveGeometry::save( const MLString& _filename,
                            MLAbstractMesh* ao )
{
  if( currentDatabase == 0 || _filename.length() == 0 )
    return false;

  MLString filename= _filename;
  if( MLFilename(filename).getExtension().length() == 0 )
    filename += MLString(MLDatabase::geometryExtension);

  model->stopLightingProcess();

  MLIOProgressDialog* progress= new MLIOProgressDialog(model);
  progress->create( WritingProgressDlg::data,
                    WritingProgressDlg::progress,
                    WritingProgressDlg::label );
  progress->flush();

  MLAbstractMeshIO aoWriter(currentDatabase,model->getScene(),progress);

  model->pushWaitCursor();

  boolean outcome= aoWriter.write(ao,filename);

  delete progress;
  progress= 0;

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

void MLSaveGeometry::updateFilesList()
{
  if( kwDialog == 0 )
    return;

  kwDialog->clearList(SaveGeometryDlg::list);

  MLDatabase* database= currentDatabase;
  if( database == 0 )
    return;

  MLDatabasesList::setCurrent( currentDatabase->getBasePath() );

  MLString path= database->getGeomPath();
  MLString pattern= MLString("*") + MLString(MLDatabase::geometryExtension);

  if( files->list(path,pattern,false) != ListFiles::SUCCESS )
    {
    kwDialog->appendListText( SaveGeometryDlg::list, errorString );
    return;
    }

  IListIterator<ListFiles::FileItem> li(files->getFiles());
  while( ! li.eol() )
    {
    ListFiles::FileItem* item= li.next();
    kwDialog->appendListText( SaveGeometryDlg::list, item->name );
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLSaveGeometry::kwCallback( KWEvent& event )
{
  if( event.type == KWEvent::SELECT &&
      event.widget == SaveGeometryDlg::list )
    {
    MLString filename= kwDialog->getText(SaveGeometryDlg::list);
    if( filename == MLString(errorString) )
      filename= "";

    kwDialog->setText( SaveGeometryDlg::filename, filename );
    kwDialog->focus(SaveGeometryDlg::filename);
    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == SaveGeometryDlg::databases )
    {
    MLString dbpath= kwDialog->getText(SaveGeometryDlg::databases);
    currentDatabase= dblist->getDatabase(dbpath);
    updateFilesList();
    return true;
    }

  return MLGlobalDialog::kwCallback(event);
}

/////////////////////////////////////////////////////////////////////////////

