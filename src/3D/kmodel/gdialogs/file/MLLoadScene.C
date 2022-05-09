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
  MLLoadScene.C

  Stephane Rehel
  April 22 1997
*/

#include <string.h>
#include <errno.h>

#include "tools/ListFiles.h"

#include "kw/KWDialog.h"

#include "interface/MLIOProgressDialog.h"

#include "kmodel/ModelModule.h"

#include "scene/file/MLDatabasesList.h"
#include "scene/file/MLDatabase.h"
#include "scene/file/MLSceneIO.h"

#include "MLLoadScene.h"
#include "LoadSceneDlg.h"
#include "ReadingProgressDlg.h"

/////////////////////////////////////////////////////////////////////////////

static const char* errorString= "*ERROR*";

/////////////////////////////////////////////////////////////////////////////

MLLoadScene::MLLoadScene( ModelModule* _model ):
  MLGlobalDialog(_model)
{
  model= _model;
  dblist= 0;

  files= new ListFiles;
}

/////////////////////////////////////////////////////////////////////////////

MLLoadScene::~MLLoadScene()
{
  delete dblist;
  dblist= 0;

  delete files;
  files= 0;
}

/////////////////////////////////////////////////////////////////////////////

// return the abstract object read
// return 0 if cancelled or error
boolean MLLoadScene::run()
{
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

  if( ! MLGlobalDialog::create(LoadSceneDlg::data) )
    return false;

  SIListIterator<MLDatabase> li(dblist->get());
  while( ! li.eol() )
    {
    MLDatabase* db= li++;
    kwDialog->appendListText( LoadSceneDlg::databases,
                              db->getBasePath() );
    }
  kwDialog->setInteger( LoadSceneDlg::databases,
                        dblist->getCurrentDatabasePosition());

  MLString dbpath= kwDialog->getText(LoadSceneDlg::databases);
  currentDatabase= dblist->getDatabase(dbpath);

  updateFilesList();

  kwDialog->focus(LoadSceneDlg::filename);

  int outcome= kwDialog->run();

  MLString filename= kwDialog->getText(LoadSceneDlg::filename);
  if( filename == errorString )
    filename= "";

  if( outcome == KWDialog::CANCEL )
    {
    MLGlobalDialog::close();
    return false;
    }

  boolean ok= load(filename);

  MLGlobalDialog::close();

  return ok;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLLoadScene::load( const MLString& filename )
{
  if( currentDatabase == 0 || filename.length() == 0 )
    return false;

  model->stopLightingProcess();

  MLIOProgressDialog* progress= new MLIOProgressDialog(model);
  progress->create( ReadingProgressDlg::data,
                    ReadingProgressDlg::progress,
                    ReadingProgressDlg::label );
  progress->flush();

  model->pushWaitCursor();

  boolean outcome= MLSceneIO::read( currentDatabase,
                                  model->getScene(),
                                  filename,
                                  progress );

  delete progress;
  progress= 0;

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

void MLLoadScene::updateFilesList()
{
  if( kwDialog == 0 )
    return;

  kwDialog->clearList(LoadSceneDlg::list);

  MLDatabase* database= currentDatabase;
  if( database == 0 )
    return;

  MLDatabasesList::setCurrent( currentDatabase->getBasePath() );

  MLString path= database->getScenePath();
  MLString pattern= MLString("*") + MLString(MLDatabase::sceneExtension);

  if( files->list(path,pattern,false) != ListFiles::SUCCESS )
    {
    kwDialog->appendListText( LoadSceneDlg::list, errorString );
    return;
    }

  IListIterator<ListFiles::FileItem> li(files->getFiles());
  while( ! li.eol() )
    {
    ListFiles::FileItem* item= li.next();
    kwDialog->appendListText( LoadSceneDlg::list, item->name );
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLLoadScene::kwCallback( KWEvent& event )
{
  if( event.type == KWEvent::DOUBLE_SELECT &&
      event.widget == LoadSceneDlg::list )
    {
    kwDialog->quit(KWDialog::VALIDATE);
    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == LoadSceneDlg::list )
    {
    MLString filename= kwDialog->getText(LoadSceneDlg::list);
    if( filename == MLString(errorString) )
      filename= "";

    kwDialog->setText( LoadSceneDlg::filename, filename );
    kwDialog->focus(LoadSceneDlg::filename);
    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == LoadSceneDlg::databases )
    {
    MLString dbpath= kwDialog->getText(LoadSceneDlg::databases);
    currentDatabase= dblist->getDatabase(dbpath);
    updateFilesList();
    return true;
    }

  return MLGlobalDialog::kwCallback(event);
}

/////////////////////////////////////////////////////////////////////////////

