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
  MLSaveHierarchy.C

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
#include "scene/file/MLObjectIO.h"

#include "MLSaveHierarchy.h"
#include "SaveHierarchyDlg.h"
#include "WritingProgressDlg.h"

/////////////////////////////////////////////////////////////////////////////

static const char* errorString= "*ERROR*";

/////////////////////////////////////////////////////////////////////////////

MLSaveHierarchy::MLSaveHierarchy( ModelModule* _model ):
  MLGlobalDialog(_model)
{
  model= _model;
  dblist= 0;
  files= new ListFiles;
}

/////////////////////////////////////////////////////////////////////////////

MLSaveHierarchy::~MLSaveHierarchy()
{
  delete dblist;
  dblist= 0;

  delete files;
  files= 0;
}

/////////////////////////////////////////////////////////////////////////////

// return the abstract object read
// return 0 if cancelled or error
boolean MLSaveHierarchy::run( MLObject* o )
{
  if( o == 0 )
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

  if( ! MLGlobalDialog::create(SaveHierarchyDlg::data) )
    return false;

  SIListIterator<MLDatabase> li(dblist->get());
  while( ! li.eol() )
    {
    MLDatabase* db= li++;
    kwDialog->appendListText( SaveHierarchyDlg::databases,
                              db->getBasePath() );
    }
  kwDialog->setInteger( SaveHierarchyDlg::databases,
                        dblist->getCurrentDatabasePosition());

  MLString dbpath= kwDialog->getText(SaveHierarchyDlg::databases);
  currentDatabase= dblist->getDatabase(dbpath);

  updateFilesList();

  kwDialog->setText( SaveHierarchyDlg::filename,
                     MLObjectIO::getFilename(o) );

  kwDialog->focus(SaveHierarchyDlg::filename);

  int outcome= kwDialog->run();

  MLString filename= kwDialog->getText(SaveHierarchyDlg::filename);
  if( filename == errorString )
    filename= "";

  if( outcome == KWDialog::CANCEL || filename.length() == 0 )
    {
    MLGlobalDialog::close();
    return false;
    }

  boolean ok= save(filename,o);

  MLGlobalDialog::close();

  return ok;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLSaveHierarchy::save( const MLString& _filename, MLObject* o )
{
  if( currentDatabase == 0 || _filename.length() == 0 )
    return false;

  MLString filename= _filename;
  if( MLFilename(filename).getExtension().length() == 0 )
    filename += MLString(MLDatabase::hierarchyExtension);

  model->stopLightingProcess();

  MLIOProgressDialog* progress= new MLIOProgressDialog(model);
  progress->create( WritingProgressDlg::data,
                    WritingProgressDlg::progress,
                    WritingProgressDlg::label );
  progress->flush();

  model->pushWaitCursor();

  MLObjectIO oWriter(currentDatabase,model->getScene());

  boolean outcome= oWriter.write(o,filename);

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

void MLSaveHierarchy::updateFilesList()
{
  if( kwDialog == 0 )
    return;

  kwDialog->clearList(SaveHierarchyDlg::list);

  MLDatabase* database= currentDatabase;
  if( database == 0 )
    return;

  MLDatabasesList::setCurrent( currentDatabase->getBasePath() );

  MLString path= database->getHierPath();
  MLString pattern= MLString("*") + MLString(MLDatabase::hierarchyExtension);

  if( files->list(path,pattern,false) != ListFiles::SUCCESS )
    {
    kwDialog->appendListText( SaveHierarchyDlg::list, errorString );
    return;
    }

  IListIterator<ListFiles::FileItem> li(files->getFiles());
  while( ! li.eol() )
    {
    ListFiles::FileItem* item= li.next();
    kwDialog->appendListText( SaveHierarchyDlg::list, item->name );
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLSaveHierarchy::kwCallback( KWEvent& event )
{
  if( event.type == KWEvent::SELECT &&
      event.widget == SaveHierarchyDlg::list )
    {
    MLString filename= kwDialog->getText(SaveHierarchyDlg::list);
    if( filename == MLString(errorString) )
      filename= "";

    kwDialog->setText( SaveHierarchyDlg::filename, filename );
    kwDialog->focus(SaveHierarchyDlg::filename);
    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == SaveHierarchyDlg::databases )
    {
    MLString dbpath= kwDialog->getText(SaveHierarchyDlg::databases);
    currentDatabase= dblist->getDatabase(dbpath);
    updateFilesList();
    return true;
    }

  return MLGlobalDialog::kwCallback(event);
}

/////////////////////////////////////////////////////////////////////////////

