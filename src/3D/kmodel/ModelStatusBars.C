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
  ModelStatusBars.C

  Stephane Rehel

  August 3 1997
*/

#include "interface/MLStatusBars.h"
#include "interface/MLStatusLine.h"

#include "interface/MLStatusLineItem.h"
#include "interface/MLStatusLineText.h"
#include "interface/MLStatusLineEntry.h"
#include "interface/MLStatusLineButton.h"

#include "MLStatusLightingSwitch.h"
#include "ModelStatusLineEntry.h"
#include "ModelCommands.h"
#include "ModelModule.h"
#include "ModelFlags.h"

#include "commands/MLCSetVariable.h"

#include "ModelStatusBars.h"

/////////////////////////////////////////////////////////////////////////////

ModelStatusBars::ModelStatusBars( ModelModule* _model,
                                  MLStatusBars* _statusBars )
{
  model= _model;
  statusBars= _statusBars;

  modeInfo= 0;
  message= 0;
  dlt= 0;
  ilt= 0;
  single_multiple= 0;

  component.obj= 0;
  component.vtx= 0;
  component.pol= 0;
}

/////////////////////////////////////////////////////////////////////////////

ModelStatusBars::~ModelStatusBars()
{
  modeInfo= 0;
  message= 0;
  dlt= 0;
  ilt= 0;
  single_multiple= 0;
  component.obj= 0;
  component.vtx= 0;
  component.pol= 0;
}

/////////////////////////////////////////////////////////////////////////////

void ModelStatusBars::init()
{
  if( model == 0 || statusBars == 0 )
    return;

    ////////////////
   // FIRST LINE //
  ////////////////

  MLStatusLine* line1= statusBars->getLine(0);
  logs= new ModelStatusLineEntry( model,
                                  line1,
                                  0,
                                  MLStatusLineItem::LEFT, true );

  dlt= new MLStatusLightingSwitch( model,
                                   line1,
                                   3,
                                   MLStatusLineItem::RIGHT, false );
  dlt->setActionHandler(this);

  single_multiple= new MLStatusLineButton( model,
                                           line1,
                                           3,
                                           MLStatusLineItem::RIGHT, false);
  single_multiple->setActionHandler(this);

    /////////////////
   // SECOND LINE //
  /////////////////

  MLStatusLine* line2= statusBars->getLine(1);
  modeInfo= new MLStatusLineText( model,
                                  line2,
                                  12,
                                  MLStatusLineItem::LEFT, false );

  message= new MLStatusLineText( model,
                                 line2,
                                 0,
                                 MLStatusLineItem::LEFT, true );

  ilt= new MLStatusLightingSwitch( model,
                                   line2,
                                   3,
                                   MLStatusLineItem::RIGHT, false);
  ilt->setActionHandler(this);

//  new MLStatusLineText( model,
//                        statusBars->getLine2(),
//                        0,
//                        MLStatusLineItem::RIGHT,
//                        false );

  component.pol= new MLStatusLineButton( model,
                                         line2,
                                         3,
                                         MLStatusLineItem::RIGHT, false );
  component.pol->one_time_press= true;
  component.pol->setActionHandler(this);
  component.pol->printMessage("POL");

  component.vtx= new MLStatusLineButton( model,
                                         line2,
                                         3,
                                         MLStatusLineItem::RIGHT, false );
  component.vtx->one_time_press= true;
  component.vtx->setActionHandler(this);
  component.vtx->printMessage("VTX");

  component.obj= new MLStatusLineButton( model,
                                         line2,
                                         3,
                                         MLStatusLineItem::RIGHT, false );
  component.obj->one_time_press= true;
  component.obj->setActionHandler(this);
  component.obj->printMessage("OBJ");

  // END

  line1->finalize();
  line2->finalize();

  dlt->printMessage("DLT",1);
  ilt->printMessage("ILT",1);

  single_multiple->printMessage("SGL");

  update();
}

/////////////////////////////////////////////////////////////////////////////

void ModelStatusBars::update()
{
  if( model == 0 || statusBars == 0 )
    return;

  const ModelFlags* flags= model->getFlags();

  if( dlt != 0 )
    dlt->setPressed( flags->computeDirectLighting );

  if( ilt != 0 )
    ilt->setPressed( flags->computeIndirectLighting );

  if( single_multiple != 0 )
    single_multiple->setPressed( flags->singleSelection );

  if( component.obj != 0 )
    component.obj->setPressed(
                     flags->modellingComponent==ModelFlags::COMPONENT_OBJ );
  if( component.vtx != 0 )
    component.vtx->setPressed(
                     flags->modellingComponent==ModelFlags::COMPONENT_VTX );
  if( component.pol != 0 )
    component.pol->setPressed(
                     flags->modellingComponent==ModelFlags::COMPONENT_POL );
}

/////////////////////////////////////////////////////////////////////////////

boolean ModelStatusBars::handleAction( MLAction* action )
{
  if( model == 0 )
    return false;

  const ModelFlags* flags= model->getFlags();

  if( action == dlt )
    {
    boolean yes= ! flags->computeDirectLighting;
    dlt->setPressed(yes);

    model->post( new MLCSetVariable( ModelFlags::COMPUTE_DIRECT_LIGHTING,
                                     yes ) );

    return true;
    }

  if( action == ilt )
    {
    boolean yes= ! flags->computeIndirectLighting;
    ilt->setPressed(yes);

    model->post( new MLCSetVariable( ModelFlags::COMPUTE_INDIRECT_LIGHTING,
                                     yes ) );

    return true;
    }

  if( action == single_multiple )
    {
    model->post( new MLCSetVariable( ModelFlags::SINGLE_SELECTION,
                                     !flags->singleSelection ) );
    return true;
    }

  if( action == component.obj )
    {
    if( !component.obj->getPressed() )
      return true;
    model->post( new MLCSetVariable( ModelFlags::MODELLING_COMPONENT,
                                     int(ModelFlags::COMPONENT_OBJ) ) );
    return true;
    }

  if( action == component.vtx )
    {
    if( !component.vtx->getPressed() )
      return true;
    model->post( new MLCSetVariable( ModelFlags::MODELLING_COMPONENT,
                                     int(ModelFlags::COMPONENT_VTX) ) );
    return true;
    }

  if( action == component.pol )
    {
    if( !component.pol->getPressed() )
      return true;
    model->post( new MLCSetVariable( ModelFlags::MODELLING_COMPONENT,
                                     int(ModelFlags::COMPONENT_POL) ) );
    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////
