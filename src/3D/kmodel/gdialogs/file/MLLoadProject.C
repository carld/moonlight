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
  MLLoadProject.C

  Stephane Rehel
  April 22 1997
*/

#include <string.h>
#include <errno.h>

#include "tools/MLPath.h"
#include "tools/ListFiles.h"

#include "graphics/mlgl.h"
#include "graphics/MLPicture.h"

#include "koala/VWidget.h"
#include "kw/KWDialog.h"

#include "MLLoadProject.h"
#include "LoadProjectDlg.h"

#include "kmodel/ModelModule.h"
#include "kmodel/file/ModelModuleIO.h"

#include "scene/file/MLDatabasesList.h"
#include "scene/file/MLDatabase.h"

/////////////////////////////////////////////////////////////////////////////

static boolean view_snapshot= true;
static const char* errorString= "*ERROR*";

/////////////////////////////////////////////////////////////////////////////

MLLoadProject::MLLoadProject( ModelModule* _model ):
  MLGlobalDialog(_model)
{
  model= _model;
  dblist= 0;
  snapshot= 0;
  files= new ListFiles;
}

/////////////////////////////////////////////////////////////////////////////

MLLoadProject::~MLLoadProject()
{
  delete dblist;
  dblist= 0;

  delete files;
  files= 0;

  delete snapshot;
  snapshot= 0;
}

/////////////////////////////////////////////////////////////////////////////

// return the abstract object read
// return 0 if cancelled or error
boolean MLLoadProject::run()
{
  delete dblist;
  dblist= new MLDatabasesList;
  dblist->build();

  delete snapshot;
  snapshot= 0;

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

  if( ! MLGlobalDialog::create(LoadProjectDlg::data) )
    return false;

  SIListIterator<MLDatabase> li(dblist->get());
  while( ! li.eol() )
    {
    MLDatabase* db= li++;
    kwDialog->appendListText( LoadProjectDlg::databases,
                              db->getBasePath() );
    }
  kwDialog->setInteger( LoadProjectDlg::databases,
                        dblist->getCurrentDatabasePosition());

  MLString dbpath= kwDialog->getText(LoadProjectDlg::databases);
  currentDatabase= dblist->getDatabase(dbpath);

  updateFilesList();

  kwDialog->setChecked( LoadProjectDlg::snapshot_switch, view_snapshot );

  kwDialog->focus( LoadProjectDlg::list );

  int outcome= kwDialog->run();

  MLString filename= kwDialog->getText(LoadProjectDlg::filename);
  view_snapshot= kwDialog->getChecked(LoadProjectDlg::snapshot_switch);

  MLGlobalDialog::close();

  if( outcome == KWDialog::CANCEL || currentDatabase == 0 )
    return false;

  filename= currentDatabase->getBasePath() + filename;

  return load(filename);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLLoadProject::load( const MLString& filename )
{
  if( currentDatabase == 0 || filename.length() == 0 )
    return 0;

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

  ModelModuleIO modelIO(model);

  boolean outcome= modelIO.read(filename,currentDatabase);

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

void MLLoadProject::updateFilesList()
{
  if( kwDialog == 0 )
    return;

  kwDialog->clearList(LoadProjectDlg::list);

  MLDatabase* database= currentDatabase;
  if( database == 0 )
    return;

  MLDatabasesList::setCurrent( currentDatabase->getBasePath() );

  MLString path= database->getProjectPath();
  MLString pattern= MLString("*") + MLString(MLDatabase::projectExtension);

  if( files->list(path,pattern,false) != ListFiles::SUCCESS )
    {
    kwDialog->appendListText( LoadProjectDlg::list, errorString );
    kwDialog->setText(LoadProjectDlg::filename,"");
    kwDialog->setText(LoadProjectDlg::size,"");
    kwDialog->setText(LoadProjectDlg::date,"");
    kwDialog->setText(LoadProjectDlg::user,"");
    return;
    }

  IListIterator<ListFiles::FileItem> li(files->getFiles());
  while( ! li.eol() )
    {
    ListFiles::FileItem* item= li.next();
    kwDialog->appendListText( LoadProjectDlg::list, item->name );
    }

  kwDialog->setText(LoadProjectDlg::filename,"");
  kwDialog->setText(LoadProjectDlg::size,"");
  kwDialog->setText(LoadProjectDlg::date,"");
  kwDialog->setText(LoadProjectDlg::user,"");
}

/////////////////////////////////////////////////////////////////////////////

static ListFiles::FileItem* findListItem( IList<ListFiles::FileItem>& list,
                                          const MLString& filename )
{
  IListIterator<ListFiles::FileItem> li(list);
  while( ! li.eol() )
    {
    ListFiles::FileItem* item= li.next();
    if( item->name == filename )
      return item;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLLoadProject::kwCallback( KWEvent& event )
{
  if( event.type == KWEvent::DOUBLE_SELECT &&
      event.widget == LoadProjectDlg::list )
    {
    kwDialog->quit(KWDialog::VALIDATE);
    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == LoadProjectDlg::list )
    {
    MLString filename= kwDialog->getText(LoadProjectDlg::list);
    ListFiles::FileItem* item= findListItem(files->getFiles(),filename);
    if( item == 0 || currentDatabase == 0 )
      {
      kwDialog->setText(LoadProjectDlg::filename,"");
      kwDialog->setText(LoadProjectDlg::size,"");
      kwDialog->setText(LoadProjectDlg::date,"");
      kwDialog->setText(LoadProjectDlg::user,"");
      updatePreview();
      return true;
      }

    filename= (MLString(MLDatabase::PROJECT_DIR_NAME) + MLPath::separator)
              + filename;

    kwDialog->setText( LoadProjectDlg::filename,
                       filename );
    kwDialog->setText( LoadProjectDlg::size,
                       MLString::valueOf(int(item->size)) );
    kwDialog->setText( LoadProjectDlg::date,
                       item->getDateString() );
    kwDialog->setText( LoadProjectDlg::user,
                       item->getUser() );

    updatePreview();

    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == LoadProjectDlg::databases )
    {
    MLString dbpath= kwDialog->getText(LoadProjectDlg::databases);
    currentDatabase= dblist->getDatabase(dbpath);
    updateFilesList();
    updatePreview();
    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == LoadProjectDlg::snapshot_switch )
    {
    updatePreview();
    return true;
    }

  if( event.type == KWEvent::DRAW &&
      event.widget == LoadProjectDlg::snapshot )
    {
    drawPreview();
    return true;
    }

  return MLGlobalDialog::kwCallback(event);
}

/////////////////////////////////////////////////////////////////////////////

void MLLoadProject::updatePreview()
{
  if( kwDialog == 0 )
    return;

  delete snapshot;
  snapshot= 0;

  if( currentDatabase != 0 &&
      kwDialog->getChecked(LoadProjectDlg::snapshot_switch) )
    {
    MLString filename= kwDialog->getText(LoadProjectDlg::filename);
    filename= currentDatabase->getBasePath() + filename;
    snapshot= ModelModuleIO::readSnapshot(filename);
    }

  kwDialog->repaintArea(LoadProjectDlg::snapshot);
}

/////////////////////////////////////////////////////////////////////////////

void MLLoadProject::drawPreview()
{
  if( kwDialog == 0 )
    return;

  VWidget* widget= kwDialog->getWidget(LoadProjectDlg::snapshot);
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

