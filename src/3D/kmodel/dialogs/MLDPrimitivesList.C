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
  MLDMeshSurfPol.C

  Stephane Rehel

  January 2 1998
*/

#include <stdlib.h>

//#include "tools/IntegerArray.h"

#include "MLDPrimitivesList.h"
#include "PrimitivesListDlg.h"

#include "kw/KWDialog.h"

#include "scene/MLObject.h"
#include "scene/MLChildrenIterator.h"
#include "scene/MLObjectsIterator.h"
#include "scene/MLScene.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelScene.h"
#include "kmodel/ModelCommand.h"
#include "kmodel/ModelCommands.h"

/////////////////////////////////////////////////////////////////////////////

MLDPrimitivesList::MLDPrimitivesList( ModelModule* _model ):
  MLScrollingDialog(_model)
{
  model= _model;
  data= PrimitivesListDlg::data;
  name= "PrimitivesList";

//  list= 0;

  MLScrollingDialog::updateFlags= ModelModule::REFRESH_DIALOGS   |
                                  ModelModule::REFRESH_NAMES     |
                                  ModelModule::REFRESH_SELECTION |
                                  ModelModule::REFRESH_HIER      |
                                  ModelModule::REFRESH_GEOMETRY;
}

/////////////////////////////////////////////////////////////////////////////

MLDPrimitivesList::~MLDPrimitivesList()
{
//  delete list;
//  list= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLDPrimitivesList::update()
{
  if( kwDialog == 0 )
    return;

  updateList();

  MLScene* scene= model->getScene();
  MLObject* o= scene->getObject(model->getSingleSelected());

  if( o == 0 )
    {
    kwDialog->setAvailable( PrimitivesListDlg::list, false );
    kwDialog->setAvailable( PrimitivesListDlg::name, false );
//    kwDialog->setText( PrimitivesListDlg::name, "" );
    kwDialog->setAvailable( PrimitivesListDlg::rename, false );
    return;
    }

  kwDialog->setAvailable( PrimitivesListDlg::list, true );
  kwDialog->setAvailable( PrimitivesListDlg::name, true );
  kwDialog->setAvailable( PrimitivesListDlg::rename, true );

  const MLString& name= o->getName().name;
  kwDialog->setText( PrimitivesListDlg::name, name );

/*
  if( list != 0 )
    {
    int index= 0;
    for( int i= 1; i < list->getSize(); ++i )
      {
      int id= (*list)[i];
      MLObject* oo= scene->getObject(id);
      if( oo == o )
        {
        index= i;
        break;
        }
      }

    if( index != 0 )
      kwDialog->setInteger( PrimitivesListDlg::list, index );
    }
*/

  kwDialog->setInteger( PrimitivesListDlg::list,
                    kwDialog->getListUserDataIndex( PrimitivesListDlg::list,
                                                    (void*) o ) );

  checkNameAvailability();
}

/////////////////////////////////////////////////////////////////////////////

// recursive
void MLDPrimitivesList::_buildList( int folder_id, MLObject* o )
{
  if( o == 0 )
    return;

  MLChildrenIterator ci(o);

  int folderLength= kwDialog->getFolderLength( PrimitivesListDlg::list,
                                               folder_id );
  for( int i= 1; i <= folderLength; ++i )
    {
    MLObject* c= (MLObject*)
                 kwDialog->getFolderUserData( PrimitivesListDlg::list,
                                              folder_id,
                                              i );
    if( c == 0 )
      continue;

    boolean found= false;
    ci.reset();
    while( ! ci.eol() )
      {
      MLObject* child= ci++;
      if( child == c )
        {
        found= true;
        break;
        }
      }

    if( ! found )
      {
      int fid= kwDialog->getFolderID( PrimitivesListDlg::list, folder_id, i );
      kwDialog->deleteFolder( PrimitivesListDlg::list, fid );
      --i;
      --folderLength;
      }
     else
      {
//      int fid= kwDialog->getFolderID( PrimitivesListDlg::list, folder_id, i );
      kwDialog->setFolderData( PrimitivesListDlg::list,
                               folder_id, i,
                               c->getName().name,
                               (void*) c );
      }
    }

  if( o->getNChildren() == 0 )
    return;

  ci.reset();
  while( ! ci.eol() )
    {
    MLObject* child= ci++;
    if( child == 0 )
      continue;

    int f= kwDialog->getFolderUserDataIndex( PrimitivesListDlg::list,
                                             folder_id,
                                             (void*) child );
    if( f == 0 )
      {
      f= kwDialog->appendFolder( PrimitivesListDlg::list,
                                 folder_id,
                                 child->getName().name,
                                 (void*) child );
      }
     else
      f= kwDialog->getFolderID( PrimitivesListDlg::list, folder_id, f );

    if( f == 0 )
      continue;

    _buildList( f, child );
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLDPrimitivesList::buildList()
{
  if( kwDialog == 0 )
    return;

  MLScene* scene= model->getScene();

  _buildList( 0, scene->getRootObject() );
}

/////////////////////////////////////////////////////////////////////////////

void MLDPrimitivesList::updateList()
{
  if( kwDialog == 0 )
    return;

  buildList();

/*
  if( list == 0 )
    list= new IntegerArray;

  list->clear();

  MLScene* scene= model->getScene();

  int nObjects= 0;
  MLObjectsIterator oi(scene);
  while( ! oi.eol() )
    {
    MLObject* o= oi++;
    if( o == 0 )
      continue;
    if( o->isRoot() )
      continue;

    ++nObjects;
    }

  list->alloc(nObjects+1);
  (*list)[0]= -1;

  while( kwDialog->getListLength(PrimitivesListDlg::list) < nObjects )
    kwDialog->appendListText(PrimitivesListDlg::list,"");

  while( kwDialog->getListLength(PrimitivesListDlg::list) > nObjects )
    kwDialog->deleteListText( PrimitivesListDlg::list,
                         kwDialog->getListLength(PrimitivesListDlg::list) );

  int i= 1;
  oi.reset();
  while( ! oi.eol() )
    {
    MLObject* o= oi++;
    if( o == 0 )
      continue;
    if( o->isRoot() )
      continue;

    (*list)[i]= o->getID();

    kwDialog->setListText( PrimitivesListDlg::list, i, o->getName().name );

    ++i;
    }
*/
}

/////////////////////////////////////////////////////////////////////////////

void MLDPrimitivesList::checkNameAvailability()
{
  if( kwDialog == 0 )
    return;

  boolean avail= true;

  int index= kwDialog->getInteger(PrimitivesListDlg::list);
  if( index == 0 )
    {
    avail= false;
    goto next;
    }

  {
  MLString n= kwDialog->getText(PrimitivesListDlg::name);
  if( n.length() == 0 )
    {
    avail= false;
    goto next;
    }

  MLObject* o= (MLObject*)
               kwDialog->getListUserData( PrimitivesListDlg::list, index );
  if( o == 0 )
    {
    avail= false;
    goto next;
    }

  boolean found= true;
  MLScene* scene= model->getScene();
  MLObjectsIterator oi(scene);
  while( ! oi.eol() )
    {
    MLObject* o= oi++;
    if( o->getName().name == n )
      {
      found= true;
      break;
      }
    }

  avail= !found;
  }

next:
  kwDialog->setAvailable(PrimitivesListDlg::rename,avail);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDPrimitivesList::kwCallback( KWEvent& event )
{
  switch( event.type )
    {
    case KWEvent::KEY_PROCESSED:
      {
      checkNameAvailability();
      break;
      }

    case KWEvent::SELECT:
      {
      if( event.widget == PrimitivesListDlg::list )
        {
        int index= kwDialog->getInteger(PrimitivesListDlg::list);
        if( index == 0 )
          break;
        MLObject* o= (MLObject*)
                     kwDialog->getListUserData( PrimitivesListDlg::list,
                                                index );
        if( o == 0 )
          break;

        kwDialog->setText( PrimitivesListDlg::name, o->getName().name );

        checkNameAvailability();

        MLObject* so= model->getScene()->getObject(model->getSingleSelected());

        if( o == so )
          break;

        ModelCommand* cmd= new ModelCommand;
        cmd->type= ModelCommands::SET_SELECTED;
        cmd->indices.alloc(1);
        cmd->indices[0]= o->getID();

        model->post(cmd);
        break;
        }

      break;
      }

    case KWEvent::VALIDATE:
      {
      checkNameAvailability();

      if( ! kwDialog->getAvailable(PrimitivesListDlg::name) )
        break;

//      MLScene* scene= model->getScene();

      int index= kwDialog->getInteger(PrimitivesListDlg::list);
      if( index == 0 )
        break;
      MLObject* o= (MLObject*)
                   kwDialog->getListUserData( PrimitivesListDlg::list,
                                              index );
      if( o == 0 )
        break;

      MLString name= kwDialog->getText(PrimitivesListDlg::name);

      if( o != 0 && name.length() != 0 )
        {
        o->setName(name);

        kwDialog->setListText( PrimitivesListDlg::list, index, name );
        checkNameAvailability();
        }

      break;
      }

    default:
      return MLScrollingDialog::kwCallback(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
