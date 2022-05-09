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
  MLDLoadMaterial.C

  Stephane Rehel

  September 23 1997
*/

#include <string.h>
#include <errno.h>

#include "tools/ListFiles.h"

#include "kw/KWDialog.h"

#include "interface/MLIOProgressDialog.h"

#include "kmodel/ModelModule.h"

#include "scene/file/MLDatabasesList.h"
#include "scene/file/MLDatabase.h"
#include "scene/file/MLMaterialIO.h"

#include "MLDLoadMaterial.h"
#include "LoadMaterialDlg.h"
#include "ReadingProgressDlg.h"

/////////////////////////////////////////////////////////////////////////////

static const char* errorString= "*ERROR*";

/////////////////////////////////////////////////////////////////////////////

MLDLoadMaterial::MLDLoadMaterial( ModelModule* _model ):
  MLGlobalDialog(_model)
{
  model= _model;
  dblist= 0;
  files= new ListFiles;
}

/////////////////////////////////////////////////////////////////////////////

MLDLoadMaterial::~MLDLoadMaterial()
{
  delete dblist;
  dblist= 0;

  delete files;
  files= 0;
}

/////////////////////////////////////////////////////////////////////////////

// return the material read. This material is *not* registered!
// return 0 if cancelled or error
MLMaterial* MLDLoadMaterial::run()
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
    return 0;
    }


  if( ! MLGlobalDialog::create(LoadMaterialDlg::data) )
    return 0;

  SIListIterator<MLDatabase> li(dblist->get());
  while( ! li.eol() )
    {
    MLDatabase* db= li++;
    kwDialog->appendListText( LoadMaterialDlg::databases,
                              db->getBasePath() );
    }
  kwDialog->setInteger( LoadMaterialDlg::databases,
                        dblist->getCurrentDatabasePosition());

  MLString dbpath= kwDialog->getText(LoadMaterialDlg::databases);
  currentDatabase= dblist->getDatabase(dbpath);

  updateFilesList();

  kwDialog->focus(LoadMaterialDlg::filename);

  int outcome= kwDialog->run();

  MLString filename= kwDialog->getText(LoadMaterialDlg::filename);
  if( filename == errorString )
    filename= "";

  if( outcome == KWDialog::CANCEL )
    {
    MLGlobalDialog::close();
    return 0;
    }

  MLMaterial* m= load(filename);

  MLGlobalDialog::close();

  return m;
}

/////////////////////////////////////////////////////////////////////////////

MLMaterial* MLDLoadMaterial::load( const MLString& filename )
{
  if( currentDatabase == 0 || filename.length() == 0 )
    return 0;

  model->stopLightingProcess();

  MLIOProgressDialog* progress= new MLIOProgressDialog(model);
  progress->create( ReadingProgressDlg::data,
                    ReadingProgressDlg::progress,
                    ReadingProgressDlg::label );
  progress->flush();

  model->pushWaitCursor();

  MLMaterial* material= MLMaterialIO::read( currentDatabase,
                                            model->getScene(),
                                            filename,
                                            progress );

  delete progress;
  progress= 0;

  model->popWaitCursor();

  if( material == 0 )
    {
    KWError( "Error",
               MLString("Unable to load:\n")
             + filename + MLString("\n")
             + MLString(strerror(errno)) );
    return 0;
    }

  return material;
}

/////////////////////////////////////////////////////////////////////////////

void MLDLoadMaterial::updateFilesList()
{
  if( kwDialog == 0 )
    return;

  kwDialog->clearList(LoadMaterialDlg::list);

  MLDatabase* database= currentDatabase;
  if( database == 0 )
    return;

  MLDatabasesList::setCurrent( currentDatabase->getBasePath() );

  MLString path= database->getMaterialPath();
  MLString pattern= MLString("*") + MLString(MLDatabase::materialExtension);

  if( files->list(path,pattern,false) != ListFiles::SUCCESS )
    {
    kwDialog->appendListText( LoadMaterialDlg::list, errorString );
    return;
    }

  IListIterator<ListFiles::FileItem> li(files->getFiles());
  while( ! li.eol() )
    {
    ListFiles::FileItem* item= li.next();
    kwDialog->appendListText( LoadMaterialDlg::list, item->name );
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDLoadMaterial::kwCallback( KWEvent& event )
{
  if( event.type == KWEvent::DOUBLE_SELECT &&
      event.widget == LoadMaterialDlg::list )
    {
    kwDialog->quit(KWDialog::VALIDATE);
    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == LoadMaterialDlg::list )
    {
    MLString filename= kwDialog->getText(LoadMaterialDlg::list);
    if( filename == MLString(errorString) )
      filename= "";

    kwDialog->setText( LoadMaterialDlg::filename, filename );
    kwDialog->focus(LoadMaterialDlg::filename);
    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == LoadMaterialDlg::databases )
    {
    MLString dbpath= kwDialog->getText(LoadMaterialDlg::databases);
    currentDatabase= dblist->getDatabase(dbpath);
    updateFilesList();
    return true;
    }

  return MLGlobalDialog::kwCallback(event);
}

/////////////////////////////////////////////////////////////////////////////
