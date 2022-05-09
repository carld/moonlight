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
  MLDPlainText.C

  Stephane Rehel

  March 5 1998
*/

#include <stdlib.h>

#include "MLDPlainText.h"
#include "PlainTextDlg.h"

#include "tools/MLFilename.h"

#include "kw/KWDialog.h"

#include "scene/MeshRep.h"

#include "scene/MLObject.h"
#include "scene/MLMesh.h"
#include "scene/MLScene.h"

#include "scene/formal/FormalRep.h"
#include "scene/formal/FormalText.h"

#include "kmodel/ModelModule.h"

#include "kmodel/commands/MLCSetPlainText.h"

#include "kmodel/gdialogs/MLDSelectTTFont.h"

/////////////////////////////////////////////////////////////////////////////

MLDPlainText::MLDPlainText( ModelModule* _model ):
  MLScrollingDialog(_model)
{
  model= _model;
  data= PlainTextDlg::data;
  name= "PlainText";

  MLScrollingDialog::updateFlags= ModelModule::REFRESH_DIALOGS   |
                                  ModelModule::REFRESH_NAMES     |
                                  ModelModule::REFRESH_SELECTION |
                                  ModelModule::REFRESH_TRANSFORM |
                                  ModelModule::REFRESH_GEOMETRY;
}

/////////////////////////////////////////////////////////////////////////////

MLDPlainText::~MLDPlainText()
{}

/////////////////////////////////////////////////////////////////////////////

MLMesh* MLDPlainText::getValidFormal()
{
  if( kwDialog == 0 )
    return 0;

  MLObject* o= model->getScene()->getObject(model->getSingleSelected());

  if( o == 0 )
    return 0;

  if( o->getObjectType() != MLObject::MESH )
    return 0;

  MLMesh* mesh= (MLMesh*) o;
  FormalRep* formalRep= mesh->getFormalRep();
  if( formalRep == 0 )
    return 0;

  if( formalRep->getType() != FormalRep::TEXT )
    return 0;

  return mesh;
}

/////////////////////////////////////////////////////////////////////////////

void MLDPlainText::update()
{
  MLObject* o= model->getScene()->getObject(model->getSingleSelected());

  MLMesh* mesh= getValidFormal();

  boolean ok= (mesh != 0);

  FormalText* ft= 0;

  if( ok )
    ft= (FormalText*) mesh->getFormalRep();

  kwDialog->setAvailable( PlainTextDlg::object, ok );
  kwDialog->setAvailable( PlainTextDlg::total_polygons, ok );
  kwDialog->setAvailable( PlainTextDlg::font, ok );
  kwDialog->setAvailable( PlainTextDlg::set_font, ok );
  kwDialog->setAvailable( PlainTextDlg::precision, ok );
  kwDialog->setAvailable( PlainTextDlg::front, ok );
  kwDialog->setAvailable( PlainTextDlg::side, ok );
  kwDialog->setAvailable( PlainTextDlg::extrude_height, ok );
  kwDialog->setAvailable( PlainTextDlg::back, ok );
  kwDialog->setAvailable( PlainTextDlg::reversed_sides, ok );
// wowow!!! there is a bug in meshrep::finalize() ...
// one face sharing an edge with a reversed one, and pouf.
kwDialog->setAvailable( PlainTextDlg::reversed_sides,0);
  kwDialog->setAvailable( PlainTextDlg::apply, ok );

  if( !ok )
    {
    kwDialog->setText( PlainTextDlg::object, "" );
    kwDialog->setText( PlainTextDlg::total_polygons, "" );
    return;
    }

  kwDialog->setText( PlainTextDlg::object, o->getName().name );
  kwDialog->setText( PlainTextDlg::total_polygons,
                     MLString::valueOf(mesh->getMeshRep()->nPolygons) );
  kwDialog->setText( PlainTextDlg::font, ft->ttf_name );
  kwDialog->setText( PlainTextDlg::text, ft->text );

  kwDialog->setValueEditRanges( PlainTextDlg::precision,
                                0.01, 200,
                                2 // ndigits
                              );
  kwDialog->setValue( PlainTextDlg::precision, ft->precision );

  kwDialog->setChecked( PlainTextDlg::front, ft->front );
  kwDialog->setChecked( PlainTextDlg::side, ft->side );
  kwDialog->setChecked( PlainTextDlg::back, ft->back );
  kwDialog->setChecked( PlainTextDlg::reversed_sides, ft->reversed_sides );

  kwDialog->setValueEditRanges( PlainTextDlg::extrude_height,
                                0., 1e10,
                                4 // ndigits
                              );
  kwDialog->setValue( PlainTextDlg::extrude_height, ft->extrude_height );

  kwDialog->setAvailable( PlainTextDlg::extrude_height, ft->side );
  kwDialog->setAvailable( PlainTextDlg::reversed_sides, false );

// wowow!!! there is a bug in meshrep::finalize() ...
// one face sharing an edge with a reversed one, and pouf.
kwDialog->setAvailable( PlainTextDlg::reversed_sides,0);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDPlainText::kwCallback( KWEvent& event )
{
  switch( event.type )
    {
    case KWEvent::SELECT:
      {
      if( event.widget == PlainTextDlg::side )
        {
        boolean ok= kwDialog->getChecked( PlainTextDlg::side );
        kwDialog->setAvailable( PlainTextDlg::extrude_height, ok );
        kwDialog->setAvailable( PlainTextDlg::reversed_sides, ok );
        break;
        }

      if( event.widget == PlainTextDlg::set_font )
        {
        MLDSelectTTFont sf(model);
        MLString name= "";
        if( sf.run(name) )
          {
          MLFilename f(name);
          kwDialog->setText( PlainTextDlg::font,
                             f.getFilename() + f.getExtension() );
          }

        return true;
        }

      break;
      }

    case KWEvent::VALIDATE:
      {
      MLMesh* mesh= getValidFormal();
      if( mesh == 0 )
        return true;
      FormalText* ft= (FormalText*) mesh->getFormalRep();

      MLCSetPlainText* cmd= new MLCSetPlainText;
      MLCSetPlainText* undo= new MLCSetPlainText;

      cmd->index= mesh->getID();

      cmd->createUndo(undo);

      cmd->text= kwDialog->getText(PlainTextDlg::text);
      cmd->ttf_name= kwDialog->getText(PlainTextDlg::font);
      cmd->precision= kwDialog->getValue(PlainTextDlg::precision);
      cmd->extrude_height= kwDialog->getValue(PlainTextDlg::extrude_height);
      cmd->front= kwDialog->getChecked(PlainTextDlg::front);
      cmd->side= kwDialog->getChecked(PlainTextDlg::side);
      cmd->back= kwDialog->getChecked(PlainTextDlg::back);
      cmd->reversed_sides= kwDialog->getChecked(PlainTextDlg::reversed_sides);

      undo->text= ft->text;
      undo->ttf_name= ft->ttf_name;
      undo->precision= ft->precision;
      undo->extrude_height= ft->extrude_height;
      undo->front= ft->front;
      undo->side= ft->side;
      undo->back= ft->back;
      undo->reversed_sides= ft->reversed_sides;

      model->post(cmd);

      break;
      }

    default:
      return MLScrollingDialog::kwCallback(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
