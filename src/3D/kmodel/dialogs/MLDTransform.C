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
  MLDTransform.C

  Stephane Rehel

  December 18 1996
*/

#include "MLDTransform.h"
#include "TransformDlg.h"

#include "kw/KWDialog.h"

#include "scene/MLObject.h"
#include "scene/MLScene.h"

#include "kmodel/ModelModule.h"
#include "kmodel/commands/MLCSetXForm.h"

/////////////////////////////////////////////////////////////////////////////

MLDTransform::MLDTransform( ModelModule* _model ):
  MLScrollingDialog(_model)
{
  model= _model;
  data= TransformDlg::data;
  name= "Transformation";

  MLScrollingDialog::updateFlags= ModelModule::REFRESH_DIALOGS   |
                                  ModelModule::REFRESH_NAMES     |
                                  ModelModule::REFRESH_SELECTION |
                                  ModelModule::REFRESH_TRANSFORM |
                                  ModelModule::REFRESH_GEOMETRY;
}

/////////////////////////////////////////////////////////////////////////////

MLDTransform::~MLDTransform()
{}

/////////////////////////////////////////////////////////////////////////////

void MLDTransform::update()
{
  MLObject* o= model->getScene()->getObject(model->getSingleSelected());
  if( o == 0 )
    {
    if( kwDialog->getAvailable(TransformDlg::scale_x) )
      {
      kwDialog->setText( TransformDlg::object, "" );
      kwDialog->setValue(TransformDlg::scale_x,0.);
      kwDialog->setValue(TransformDlg::scale_y,0.);
      kwDialog->setValue(TransformDlg::scale_z,0.);
      kwDialog->setValue(TransformDlg::rotate_x,0.);
      kwDialog->setValue(TransformDlg::rotate_y,0.);
      kwDialog->setValue(TransformDlg::rotate_z,0.);
      kwDialog->setValue(TransformDlg::translate_x,0.);
      kwDialog->setValue(TransformDlg::translate_y,0.);
      kwDialog->setValue(TransformDlg::translate_z,0.);

      kwDialog->setAvailable( TransformDlg::scale_x, false );
      kwDialog->setAvailable( TransformDlg::scale_y, false );
      kwDialog->setAvailable( TransformDlg::scale_z, false );
      kwDialog->setAvailable( TransformDlg::rotate_x, false );
      kwDialog->setAvailable( TransformDlg::rotate_y, false );
      kwDialog->setAvailable( TransformDlg::rotate_z, false );
      kwDialog->setAvailable( TransformDlg::translate_x, false );
      kwDialog->setAvailable( TransformDlg::translate_y, false );
      kwDialog->setAvailable( TransformDlg::translate_z, false );
      kwDialog->setAvailable( TransformDlg::apply, false );
      }
    return;
    }

  if( ! kwDialog->getAvailable(TransformDlg::scale_x) )
    {
    kwDialog->setText( TransformDlg::object, "" );
    kwDialog->setAvailable( TransformDlg::scale_x, true );
    kwDialog->setAvailable( TransformDlg::scale_y, true );
    kwDialog->setAvailable( TransformDlg::scale_z, true );
    kwDialog->setAvailable( TransformDlg::rotate_x, true );
    kwDialog->setAvailable( TransformDlg::rotate_y, true );
    kwDialog->setAvailable( TransformDlg::rotate_z, true );
    kwDialog->setAvailable( TransformDlg::translate_x, true );
    kwDialog->setAvailable( TransformDlg::translate_y, true );
    kwDialog->setAvailable( TransformDlg::translate_z, true );
    kwDialog->setAvailable( TransformDlg::apply, true );
    }

  kwDialog->setText(TransformDlg::object,o->getName().name);
  kwDialog->setValue(TransformDlg::scale_x,o->getScale().x());
  kwDialog->setValue(TransformDlg::scale_y,o->getScale().y());
  kwDialog->setValue(TransformDlg::scale_z,o->getScale().z());
  kwDialog->setValue(TransformDlg::rotate_x,RADtoDEG(o->getRotate().x()));
  kwDialog->setValue(TransformDlg::rotate_y,RADtoDEG(o->getRotate().y()));
  kwDialog->setValue(TransformDlg::rotate_z,RADtoDEG(o->getRotate().z()));
  kwDialog->setValue(TransformDlg::translate_x,o->getTranslate().x());
  kwDialog->setValue(TransformDlg::translate_y,o->getTranslate().y());
  kwDialog->setValue(TransformDlg::translate_z,o->getTranslate().z());
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDTransform::kwCallback( KWEvent& event )
{
  switch( event.type )
    {
    case KWEvent::SELECT:
      {
      MLObject* o= model->getScene()->getObject(model->getSingleSelected());
      if( o == 0 )
        return true;

      Vector s= Vector(1,1,1);
      Vector r= Vector(0,0,0);
      Vector t= Vector(0,0,0);

      boolean iscale    = (o->getScale() - s).norm() > 1e-10;
      boolean irotate   = (o->getRotate() - s).norm() > 1e-10;
      boolean itranslate= (o->getTranslate() - s).norm() > 1e-10;

      if( !iscale && !irotate && !itranslate )
        return true;

      MLCSetXForm* cmd= new MLCSetXForm;
      MLCSetXForm* undo= new MLCSetXForm;

      cmd->index= o->getID();

      cmd->createUndo(undo);

      cmd->scale    = s;
      cmd->rotate   = r;
      cmd->translate= t;
      cmd->set_preXForm= true;

      undo->scale    = o->getScale();
      undo->rotate   = o->getRotate();
      undo->translate= o->getTranslate();
      undo->set_preXForm= true;
      undo->preXForm= o->local_xform.preXForm;

      model->post(cmd);

      break;
      }

    case KWEvent::VALIDATE:
      {
      MLObject* o= model->getScene()->getObject(model->getSingleSelected());
      if( o == 0 )
        return true;

      Vector s= Vector( kwDialog->getValue(TransformDlg::scale_x),
                        kwDialog->getValue(TransformDlg::scale_y),
                        kwDialog->getValue(TransformDlg::scale_z) );
      Vector r= Vector( kwDialog->getValue(TransformDlg::rotate_x),
                        kwDialog->getValue(TransformDlg::rotate_y),
                        kwDialog->getValue(TransformDlg::rotate_z) );
      r= Vector( DEGtoRAD(r.x()), DEGtoRAD(r.y()), DEGtoRAD(r.z()) );
      Vector t= Vector( kwDialog->getValue(TransformDlg::translate_x),
                        kwDialog->getValue(TransformDlg::translate_y),
                        kwDialog->getValue(TransformDlg::translate_z) );

      boolean iscale    = (o->getScale() - s).norm() > 1e-10;
      boolean irotate   = (o->getRotate() - s).norm() > 1e-10;
      boolean itranslate= (o->getTranslate() - s).norm() > 1e-10;

      if( !iscale && !irotate && !itranslate )
        return true;

      MLCSetXForm* cmd= new MLCSetXForm;
      MLCSetXForm* undo= new MLCSetXForm;

      cmd->index= o->getID();

      cmd->createUndo(undo);

      cmd->scale    = s;
      cmd->rotate   = r;
      cmd->translate= t;
      cmd->set_preXForm= false;

      undo->scale    = o->getScale();
      undo->rotate   = o->getRotate();
      undo->translate= o->getTranslate();
      undo->set_preXForm= false;

      model->post(cmd);

      break;
      }

    default:
      return MLScrollingDialog::kwCallback(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
