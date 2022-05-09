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
  createModelDialog.C

  Stephane Rehel
  April 23 1997
*/

#include "interface/MLScrollingDialogs.h"

#include "ModelModule.h"

#include "dialogs/MLDTransform.h"
#include "dialogs/MLDCamera.h"
#include "dialogs/MLDObjectInfo.h"
#include "dialogs/MLDLight.h"
#include "dialogs/MLDPool.h"
#include "dialogs/MLDRadiosity.h"
#include "dialogs/MLDWelcome.h"
#include "dialogs/MLDMeshTesselation.h"
#include "dialogs/MLDCurveTesselation.h"
#include "dialogs/MLDModesList.h"
#include "dialogs/MLDPrimitivesList.h"
#include "dialogs/MLDPlainText.h"

/////////////////////////////////////////////////////////////////////////////

boolean ModelModule::createDialog( const MLString& name )
{
  if( MLModule::dialogs == 0 )
    return false;

  if( MLModule::createDialog(name) )
    return true;

  if( name == "Transformation" )
    {
    MLModule::dialogs->createDialog( new MLDTransform(this) );
    return true;
    }

  if( name == "Camera" )
    {
    MLModule::dialogs->createDialog( new MLDCamera(this) );
    return true;
    }

  if( name == "Object Info" )
    {
    MLModule::dialogs->createDialog( new MLDObjectInfo(this) );
    return true;
    }

  if( name == "Light" )
    {
    MLModule::dialogs->createDialog( new MLDLight(this) );
    return true;
    }

  if( name == "Light Pool Manager" )
    {
    MLModule::dialogs->createDialog( new MLDPool(this) );
    return true;
    }

  if( name == "Radiosity" )
    {
    MLModule::dialogs->createDialog( new MLDRadiosity(this) );
    return true;
    }

  if( name == "Welcome" )
    {
    MLModule::dialogs->createDialog( new MLDWelcome(this) );
    return true;
    }

  if( name == "MeshTesselation" )
    {
    MLModule::dialogs->createDialog( new MLDMeshTesselation(this) );
    return true;
    }

  if( name == "CurveTesselation" )
    {
    MLModule::dialogs->createDialog( new MLDCurveTesselation(this) );
    return true;
    }

  if( name == "ModesList" )
    {
    MLModule::dialogs->createDialog( new MLDModesList(this) );
    return true;
    }

  if( name == "PrimitivesList" )
    {
    MLModule::dialogs->createDialog( new MLDPrimitivesList(this) );
    return true;
    }

  if( name == "PlainText" )
    {
    MLModule::dialogs->createDialog( new MLDPlainText(this) );
    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////
