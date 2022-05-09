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
  MLSaveProject.C

  Stephane Rehel
  April 24 1997
*/

#include <string.h>
#include <errno.h>
#include <stdio.h>

#include "tools/MLFilename.h"
#include "tools/MLPath.h"
#include "tools/ListFiles.h"

#include "graphics/MLEventManager.h"

#include "kw/KWDialog.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelFlags.h"
#include "kmodel/file/ModelModuleIO.h"

#include "scene/file/MLDatabasesList.h"
#include "scene/file/MLDatabase.h"
#include "scene/file/MLAbstractMeshIO.h"
#include "scene/MLScene.h"

#include "MLSaveProject.h"
#include "SaveProjectDlg.h"

/////////////////////////////////////////////////////////////////////////////

static const char* errorString= "*ERROR*";

/////////////////////////////////////////////////////////////////////////////

MLSaveProject::MLSaveProject( ModelModule* _model ):
  MLGlobalDialog(_model)
{
  model= _model;
  dblist= 0;
  files= new ListFiles;
}

/////////////////////////////////////////////////////////////////////////////

MLSaveProject::~MLSaveProject()
{
  delete dblist;
  dblist= 0;

  delete files;
  files= 0;
}

/////////////////////////////////////////////////////////////////////////////

// return the abstract object read
// return 0 if cancelled or error
boolean MLSaveProject::run()
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

  if( ! MLGlobalDialog::create(SaveProjectDlg::data) )
    return false;

  SIListIterator<MLDatabase> li(dblist->get());
  while( ! li.eol() )
    {
    MLDatabase* db= li++;
    kwDialog->appendListText( SaveProjectDlg::databases,
                              db->getBasePath() );
    }
  kwDialog->setInteger( SaveProjectDlg::databases,
                        dblist->getCurrentDatabasePosition());

  MLString dbpath= kwDialog->getText(SaveProjectDlg::databases);
  currentDatabase= dblist->getDatabase(dbpath);

  updateFilesList();

  name= model->getScene()->getName();
  kwDialog->setText( SaveProjectDlg::name, name.name );
  kwDialog->setText( SaveProjectDlg::version,
                     MLDatabase::buildVersionString(name) );
  kwDialog->setChecked(SaveProjectDlg::same_version);
  kwDialog->setChecked( SaveProjectDlg::include_snapshot,
                        model->getFlags()->writeSnapshot );

  kwDialog->focus(SaveProjectDlg::name);

  int outcome= kwDialog->run();

  MLString filename= getFilename();
  MLString versionString= kwDialog->getText(SaveProjectDlg::version);
  MLString nameString= kwDialog->getText(SaveProjectDlg::name);
  boolean includeSnapshot= kwDialog->getChecked(SaveProjectDlg::include_snapshot);

  MLGlobalDialog::close();

  if( filename.length() == 0 ||
      outcome == KWDialog::CANCEL ||
      currentDatabase == 0 )
    return false;

  return save(versionString,nameString,includeSnapshot);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLSaveProject::save( const MLString& versionString,
                           const MLString& nameString,
                           boolean includeSnapshot )
{
  if( currentDatabase == 0 )
    return false;

  MLName& sceneName= model->getScene()->name;

  int version= 0, sublevel= 0;
  if( sscanf( versionString.get(), "%d.%d", &version, &sublevel ) == 2 )
    {
    sceneName.major_version= version;
    sceneName.minor_version= sublevel;
    }

  sceneName.name= nameString;

  model->flags->writeSnapshot= includeSnapshot;

  return MLSaveProject::save( currentDatabase, model );
}

/////////////////////////////////////////////////////////////////////////////

// static
boolean MLSaveProject::save( MLDatabase* database, ModelModule* model )
{
  if( database == 0 )
    return false;

  model->stopLightingProcess();

  if( model->getFlags()->writeSnapshot )
    {
    // be sure we've closed any dialog before snapshot
    MLEventManager* eventManager= MLEventManager::getEventManager();

    eventManager->get();
    eventManager->dispatch();
    }

  ModelModuleIO modelIO(model);

  boolean outcome= modelIO.write(database);

  if( ! outcome )
    {
    KWError( "Error",
               MLString("Unable to save project:\n")
             + MLString(strerror(errno)) );
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLSaveProject::updateFilesList()
{
  if( kwDialog == 0 )
    return;

  kwDialog->clearList(SaveProjectDlg::list);

  MLDatabase* database= currentDatabase;
  if( database == 0 )
    {
    kwDialog->setText(SaveProjectDlg::name,"");
    kwDialog->setText(SaveProjectDlg::version,"");
    kwDialog->setChecked(SaveProjectDlg::same_version);
    return;
    }

  MLDatabasesList::setCurrent( currentDatabase->getBasePath() );

  MLString path= database->getProjectPath();
  MLString pattern= MLString("*") + MLString(MLDatabase::projectExtension);

  if( files->list(path,pattern,false) != ListFiles::SUCCESS )
    {
    kwDialog->appendListText( SaveProjectDlg::list, errorString );
    kwDialog->setText(SaveProjectDlg::name,"");
    kwDialog->setText(SaveProjectDlg::version,"");
    kwDialog->setChecked(SaveProjectDlg::same_version);
    return;
    }

  IListIterator<ListFiles::FileItem> li(files->getFiles());
  while( ! li.eol() )
    {
    ListFiles::FileItem* item= li.next();
    kwDialog->appendListText( SaveProjectDlg::list, item->name );
    }

//  kwDialog->setText(SaveProjectDlg::name,"");
//  kwDialog->setText(SaveProjectDlg::version,"");
  kwDialog->setChecked(SaveProjectDlg::same_version);
}

/////////////////////////////////////////////////////////////////////////////

MLString MLSaveProject::getFilename()
{
  if( kwDialog == 0 )
    return "";

  MLString name= kwDialog->getText(SaveProjectDlg::name);
  if( name.length() == 0 )
    return "";

  MLString version= kwDialog->getText(SaveProjectDlg::version);
  if( version.length() == 0 )
    return "";

  return ( name + MLDatabase::sepChar )
         + version
         + MLString(MLDatabase::projectExtension);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLSaveProject::kwCallback( KWEvent& event )
{
  if( event.type == KWEvent::KEY &&
      event.widget == SaveProjectDlg::name )
    {
    if( event.key == Key('.') ||
        event.key == Key(MLDatabase::sepChar) ||
        event.key == Key(MLPath::separator) )
      return true;
    return false;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == SaveProjectDlg::list )
    {
    MLString filename= kwDialog->getText(SaveProjectDlg::list);
    if( filename == MLString(errorString) )
      filename= "";

    int index= filename.indexOf(MLDatabase::sepChar);
    if( index != -1 )
      {
      MLString n= filename.substring(0,index-1);
      int version= 0, sublevel= 0;

      if( sscanf( filename.substring(index+1,filename.length()-1).get(),
                  "%d.%d", &version, &sublevel ) != 2 )
        filename= "";
      if( version < 0 ) version= 0;
      if( sublevel < 0 ) sublevel= 0;
      name.name= n;
      name.major_version= version;
      name.minor_version= sublevel;
      }
     else
      filename= "";

    if( filename.length() == 0 )
      {
      kwDialog->setText( SaveProjectDlg::name, "" );
      kwDialog->setText( SaveProjectDlg::version, "" );
      }
     else
      {
      kwDialog->setText( SaveProjectDlg::name, name.name );
      kwDialog->setText( SaveProjectDlg::version,
                         MLDatabase::buildVersionString(name) );
      kwDialog->focus(SaveProjectDlg::name);
      }

    kwDialog->setChecked(SaveProjectDlg::same_version);
    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == SaveProjectDlg::same_version )
    {
    kwDialog->setText( SaveProjectDlg::version,
                       MLDatabase::buildVersionString(name) );
    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == SaveProjectDlg::new_version )
    {
    MLName n;
    n.major_version= name.major_version + 1;
    n.minor_version= 0;
    kwDialog->setText( SaveProjectDlg::version,
                       MLDatabase::buildVersionString(n) );
    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == SaveProjectDlg::new_sublevel )
    {
    MLName n;
    n.major_version= name.major_version;
    n.minor_version= name.minor_version + 1;
    kwDialog->setText( SaveProjectDlg::version,
                       MLDatabase::buildVersionString(n) );
    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == SaveProjectDlg::databases )
    {
    MLString dbpath= kwDialog->getText(SaveProjectDlg::databases);
    currentDatabase= dblist->getDatabase(dbpath);
    updateFilesList();
    return true;
    }

  return MLGlobalDialog::kwCallback(event);
}

/////////////////////////////////////////////////////////////////////////////

