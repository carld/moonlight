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
  KWFileDialogManager.C

  Stephane Rehel

  August 11 1997
*/

#include "tools/MLPath.h"
#include "tools/MLFilename.h"
#include "tools/ListFiles.h"

#include "graphics/KeyCodes.h"

#include "KWFileDialogManager.h"
#include "KWDialog.h"

static const boolean hide_dot_files= true;

/////////////////////////////////////////////////////////////////////////////

KWFileDialogManager::KWFileDialogManager()
{
  dialog= 0;
  directories= files= location= selection= ok= cancel= 0;
  size= date= user= 0;

  path= new MLPath;
  listfiles= new ListFiles;

  filename= "";
  pattern= "";
}

/////////////////////////////////////////////////////////////////////////////

KWFileDialogManager::~KWFileDialogManager()
{
  dialog= 0;
  directories= files= location= selection= ok= cancel= 0;

  delete listfiles; listfiles= 0;

  delete path; path= 0;
  pattern= "";
}

/////////////////////////////////////////////////////////////////////////////

void KWFileDialogManager::init( KWDialog* _dialog,
                                unsigned int _directories,
                                unsigned int _files,
                                unsigned int _location,
                                unsigned int _selection,
                                unsigned int _ok,
                                unsigned int _cancel,
                                unsigned int _size /*= 0*/,
                                unsigned int _date /*= 0*/,
                                unsigned int _user /*= 0*/ )
{
  dialog= _dialog;
  directories= _directories;
  files= _files;
  location= _location;
  selection= _selection;
  ok= _ok;
  cancel= _cancel;
  size= _size;
  date= _date;
  user= _user;

  *path= "./";
  pattern= "*";
}

/////////////////////////////////////////////////////////////////////////////

// return true if user has entered a real filename, and return
// the effective full filename

boolean KWFileDialogManager::validate()
{
  filename= "";

  if( dialog == 0 )
    return false;

  MLString fn= dialog->getText(selection);
  if( fn.length() == 0 )
    return false;

  fn= ListFiles::expandFilename(fn.get()).get();

  if( ListFiles::isAccessibleDirectory(fn.get()) )
    {
    if( ! ListFiles::changeToDirectory(fn.get()) )
      return false;
    *path= ListFiles::getCurrentDirectory().get();
    MLString pp= path->get();
    listfiles->list(pp,pattern.get());
    _updateDirectories();
    dialog->setText( selection, pattern );
    dialog->focus(selection);
    return false;
    }

  MLPath p;
  MLString f= "";
  MLFilename::split( fn, p, f );
  if( f.length() == 0 )
    return false;

  if( ! ListFiles::changeToDirectory(p.get().get()) )
    return false;

  if( f.indexOf('*') == -1 && f.indexOf('?') == -1 )
    {
    // user has entered an existing file as
    // '/usr/local/include/toto.h' or 'toto.h'
    *path= p.get();
    dialog->setText( selection, f );
    dialog->focus(selection);
    filename= MLString(path->get().get()) + f;
    return true;
    }

  MLString _p= p.get();
  if( listfiles->list(_p,f.get()) != ListFiles::SUCCESS )
    return false;

  // we assume this is a glob expression
  *path= p.get();
  pattern= f;

  dialog->setText( selection, f );
  dialog->focus(selection);

  _updateDirectories();

  return false;
}

/////////////////////////////////////////////////////////////////////////////

void KWFileDialogManager::updateDirectories()
{
  MLString pp= path->get();
  listfiles->list( pp, pattern.get() );
  _updateDirectories();
}

/////////////////////////////////////////////////////////////////////////////

void KWFileDialogManager::_updateDirectories()
{
  if( dialog == 0 )
    return;

  dialog->setText( location, path->get().get() );

  dialog->clearList(directories);
  dialog->clearList(files);

  if( ! ListFiles::changeToDirectory(path->get().get()) )
    return;

  IListIterator<ListFiles::FileItem> li(listfiles->getFiles());
  while( ! li.eol() )
    {
    ListFiles::FileItem* fi= li++;
    if( hide_dot_files )
      {
      if( fi->name[0] == '.' && fi->name != ".." )
        continue;
      }

    if( fi->isDirectory )
      {
      if( !(fi->name == ".." && path->get() == "/") )
        dialog->appendListText( directories, fi->name.get() );
      }
     else
      dialog->appendListText( files, fi->name.get() );
    }
}

/////////////////////////////////////////////////////////////////////////////

void KWFileDialogManager::setPattern( const MLString& _pattern )
{
  pattern= _pattern;

  if( dialog != 0 )
    dialog->setText( selection, pattern );
}

/////////////////////////////////////////////////////////////////////////////

void KWFileDialogManager::setPath( const MLString& _path )
{
  *path= MLFilename::expand(_path);
}

/////////////////////////////////////////////////////////////////////////////

static ListFiles::FileItem* findListItem( IList<ListFiles::FileItem>& list,
                                          const MLString& filename )
{
  IListIterator<ListFiles::FileItem> li(list);
  while( ! li.eol() )
    {
    ListFiles::FileItem* item= li.next();
    if( MLString(item->name.get()) == filename )
      return item;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void KWFileDialogManager::updateSizeDateUser()
{
  if( dialog == 0 )
    return;

  MLString path= dialog->getText(location);
  MLString file= dialog->getText(selection);
  MLString filename= path + file;

  ListFiles::FileItem* item= findListItem(listfiles->getFiles(),file);

  if( item == 0 )
    {
    if( size != 0 ) dialog->setText(size,"");
    if( date != 0 ) dialog->setText(date,"");
    if( user != 0 ) dialog->setText(user,"");
    return;
    }

  if( size != 0 )
    dialog->setText( size, MLString::valueOf(int(item->size)) );
  if( date != 0 )
    dialog->setText( date, item->getDateString().get() );
  if( user != 0 )
    dialog->setText( user, item->getUser().get() );
}

/////////////////////////////////////////////////////////////////////////////

boolean KWFileDialogManager::handleKWEvent( KWEvent& event )
{
  if( files != 0 &&
      event.type == KWEvent::SELECT &&
      event.widget == files )
    {
    MLString f= dialog->getText(files);
    dialog->setText( selection, f );
    updateSizeDateUser();
    return true;
    }

  if( files != 0 &&
      event.type == KWEvent::DOUBLE_SELECT &&
      event.widget == files )
    {
    MLString f= dialog->getText(files);
    dialog->setText( selection, f );

    if( validate() )
      dialog->quit(KWDialog::VALIDATE);

    return true;
    }

  if( directories != 0 &&
      event.type == KWEvent::SELECT &&
      event.widget == directories )
    {
    MLString f= dialog->getText(directories);

    dialog->setText( selection, f + MLString("/") );
    updateSizeDateUser();
    return true;
    }

  if( directories != 0 &&
      event.type == KWEvent::DOUBLE_SELECT &&
      event.widget == directories )
    {
    MLString f= dialog->getText(directories);

    dialog->setText( selection, f + MLString("/") );

    validate();

    updateSizeDateUser();

    return true;
    }

  if( (ok != 0 &&
       event.type == KWEvent::SELECT &&
       event.widget == ok) ||
      (event.type == KWEvent::KEY &&
       event.key == KeyCodes::ENTER) )
    {
    if( validate() )
      {
      dialog->quit(KWDialog::VALIDATE);
      }

    return true;
    }

  if( (cancel != 0 &&
       event.type == KWEvent::SELECT &&
       event.widget == cancel) ||
      (event.type == KWEvent::KEY &&
       event.key == KeyCodes::ESC) )
    {
    dialog->quit(KWDialog::CANCEL);
    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

