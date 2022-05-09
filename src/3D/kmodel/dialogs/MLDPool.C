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
  MLDPool.C

  Stephane Rehel

  March 5 1997
*/

#include "tools/Sorts.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelCommand.h"
#include "kmodel/ModelCommands.h"
#include "kmodel/ModelScene.h"

#include "kmodel/commands/MLCEditPool.h"

#include "MLDPool.h"
#include "PoolDlg.h"

#include "interface/MLColorWidget.h"

#include "kw/KWDialog.h"
#include "scene/MLObject.h"
#include "scene/lighting/MLLight.h"
#include "scene/lighting/MLLightPool.h"
#include "scene/lighting/MLPoolTable.h"
#include "scene/lighting/MLLightTable.h"
#include "scene/lighting/MLGlobalLighting.h"
#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

MLDPool::MLDPool( ModelModule* _model ):
  MLScrollingDialog(_model)
{
  model= _model;
  data= PoolDlg::data;
  name= "Light Pool Manager";

  MLScrollingDialog::updateFlags= ModelModule::REFRESH_DIALOGS   |
                                  ModelModule::REFRESH_NAMES     |
                                  ModelModule::REFRESH_SELECTION |
                                  ModelModule::REFRESH_GEOMETRY;
  colorWidget= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLDPool::~MLDPool()
{
  delete colorWidget;
  colorWidget= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLLightPool* MLDPool::getSelectedPool()
{
  return model->getModelScene()->getSelectedPool();

/*
  MLObject* o= model->getScene()->getObject(model->getSingleSelected());

  if( o == 0 )
    return 0;

  if( o->getObjectType() != MLObject::LIGHT )
    return 0;

  return ((MLLight*) o)->getPool();
*/
}

/////////////////////////////////////////////////////////////////////////////

void MLDPool::update()
{
  MLLightPool* pool= getSelectedPool();

  boolean avail= (pool!=0);

  if( ! avail )
    {
    kwDialog->setText(PoolDlg::name,"");
    updateList();
    }

  kwDialog->setAvailable(PoolDlg::colorArea,avail);
  kwDialog->setAvailable(PoolDlg::value_r,avail);
  kwDialog->setAvailable(PoolDlg::value_g,avail);
  kwDialog->setAvailable(PoolDlg::value_b,avail);
  kwDialog->setAvailable(PoolDlg::intensity,avail);
  kwDialog->setAvailable(PoolDlg::radio_rgb,avail);
  kwDialog->setAvailable(PoolDlg::radio_hsv,avail);
  kwDialog->setAvailable(PoolDlg::apply,avail);
  kwDialog->setAvailable(PoolDlg::update,avail);

  if( ! avail )
    return;

  if( pool->getNLights() == 0 )
    kwDialog->setAvailable(PoolDlg::remove,true);
   else
    kwDialog->setAvailable(PoolDlg::remove,false);

  kwDialog->setText( PoolDlg::name, pool->getName() );
  kwDialog->setAvailable(PoolDlg::apply,true);
  kwDialog->setAvailable(PoolDlg::update,true);
  kwDialog->setValue( PoolDlg::intensity, pool->intensity );
  if( colorWidget != 0 )
    colorWidget->set( pool->color );

  updateList();
}

/////////////////////////////////////////////////////////////////////////////

void MLDPool::updateList()
{
  int i;

  // Now, build the pools list
  MLPoolTable* poolTable= model->getScene()->getPoolTable();
  MLString* poolsNames= new MLString [ poolTable->getNPools() + 1 ];
  int j= 0;
  for( i= 1; i <= poolTable->getNPools(); ++i )
    {
    MLLightPool* p= poolTable->getPool(i);
    if( p == 0 )
      continue;
    poolsNames[++j]= p->getName();
    }

  if( j == 0 )
    {
    if( kwDialog->getListLength(PoolDlg::list) > 0 )
      kwDialog->clearList(PoolDlg::list);
    delete [] poolsNames;
    return;
    }

  Sorts<MLString> sort;
  sort.quickSort( poolsNames + 1, j );
  // check if we need to update the list
  boolean update_list= kwDialog->getListLength(PoolDlg::list) != j;

  if( ! update_list )
    {
    for( i= 1; i <= j; ++i )
      {
      if( poolsNames[i] != kwDialog->getListText(PoolDlg::list,i) )
        {
        update_list= true;
        break;
        }
      }
    }

  if( update_list )
    {
    // ok, update pools names list
    kwDialog->clearList(PoolDlg::list);
    for( i= 1; i <= j; ++i )
      kwDialog->appendListText(PoolDlg::list,poolsNames[i]);
    kwDialog->setText(PoolDlg::name_edit,"");
    }

  delete [] poolsNames;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDPool::kwCallback( KWEvent& event )
{
  if( colorWidget != 0 )
    {
    if( colorWidget->colorCallback(event) )
      return true;
    }

  switch( event.type )
    {
    case KWEvent::SELECT:
      {
      if( event.widget == PoolDlg::update )
        {
        MLLightPool* pool= getSelectedPool();

        if( pool != 0 )
          {
          model->stopLightingProcess();
          model->getScene()->getGlobalLighting()->set_dirty_pool(pool);
          model->runLightingProcess();
          }
        return true;
        }

      if( event.widget == PoolDlg::rename )
        {
        MLString name= kwDialog->getText(PoolDlg::name_edit).trim();
        if( name.length() == 0 )
          break;
        const MLString& sname= kwDialog->getText(PoolDlg::list);
        MLLightPool* pool= model->getScene()->getPool(sname);
        if( pool == 0 )
          break;
        if( name == sname )
          break;
        if( model->getScene()->getPool(name) != 0 )
          break; // already exists

        ModelCommand* cmd= new ModelCommand;
        cmd->type= ModelCommands::RENAME_POOL;
        cmd->index= pool->getPoolIndex();
        cmd->string= name;

        model->post(cmd);

        return true;
        }

      if( event.widget == PoolDlg::add )
        {
        MLString name= kwDialog->getText(PoolDlg::name_edit).trim();
        if( model->getScene()->getPool(name) != 0 )
          name= "";

        ModelCommand* cmd= new ModelCommand;
        cmd->type= ModelCommands::ADD_POOL;
        cmd->string= name;

        model->post(cmd);

        return true;
        }

      if( event.widget == PoolDlg::remove )
        {
        MLString name= kwDialog->getText(PoolDlg::name_edit).trim();
        MLLightPool* pool= model->getScene()->getPool(name);

        if( pool == 0 )
          break; // unable to find it !??

        if( pool->getNLights() != 0 )
          break;

        ModelCommand* cmd= new ModelCommand;
        cmd->type= ModelCommands::REMOVE_POOL;
        cmd->string= pool->getName();

        model->post(cmd);

        return true;
        }

      if( event.widget == PoolDlg::list )
        {
        const MLString& name= kwDialog->getText(PoolDlg::list);

        MLLightPool* pool= model->getScene()->getPool(name);
        if( pool == 0 )
          break; // unable to find it !??

        if( pool->getNLights() == 0 )
          kwDialog->setAvailable(PoolDlg::remove,true);
         else
          kwDialog->setAvailable(PoolDlg::remove,false);

        kwDialog->setText(PoolDlg::name_edit,name);
        kwDialog->focus(PoolDlg::name_edit);

        ModelCommand* cmd= new ModelCommand;
        cmd->type= ModelCommands::SET_SELECTED;
        cmd->indices.alloc(pool->getNLights());
        for( int i= 1; i <= pool->getNLights(); ++i )
          cmd->indices[i-1]= pool->getLight(i)->getID();

        model->post(cmd);

        return true;
        }

      break;
      }

    case KWEvent::VALIDATE:
      {
      apply_button();
      break;
      }

    default:
      return MLScrollingDialog::kwCallback(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLDPool::justCreated()
{
  delete colorWidget;
  colorWidget= new MLColorWidget( kwDialog,
                                  PoolDlg::label_r,
                                  PoolDlg::label_g,
                                  PoolDlg::label_b,
                                  PoolDlg::value_r,
                                  PoolDlg::value_g,
                                  PoolDlg::value_b,
                                  PoolDlg::radio_rgb,
                                  PoolDlg::radio_hsv,
                                  PoolDlg::colorArea );
}

/////////////////////////////////////////////////////////////////////////////

// protected
void MLDPool::apply_button()
{
  MLLightPool* pool= getSelectedPool();

  if( pool == 0 )
    return;

  double intensity= kwDialog->getValue(PoolDlg::intensity);
  Color  color    = colorWidget->get();

  if( fabs(intensity - pool->intensity) < 0.001 &&
      fabs((color - pool->color).getGray()) < 0.005 )
     return; // no change

  MLCEditPool* cmd= new MLCEditPool;
  MLCEditPool* undo= new MLCEditPool;

  cmd->createUndo(undo);

  cmd->pool= pool;
  cmd->string= pool->getName();
  cmd->intensity= intensity;
  cmd->color= color;

  undo->pool= pool;
  undo->string= pool->getName();
  undo->intensity= pool->intensity;
  undo->color= pool->color;

  model->post(cmd);
}

/////////////////////////////////////////////////////////////////////////////
