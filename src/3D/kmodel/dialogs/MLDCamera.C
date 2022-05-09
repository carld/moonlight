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
  MLDCamera.C

  Stephane Rehel

  December 18 1996
*/

#include "kmodel/ModelModule.h"
#include "MLDCamera.h"
#include "CameraDlg.h"

#include "kw/KWDialog.h"
#include "scene/MLObject.h"
#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

MLDCamera::MLDCamera( ModelModule* _model ): MLScrollingDialog(_model)
{
  model= _model;
  data= CameraDlg::data;
  name= "Camera";

  MLScrollingDialog::updateFlags= ModelModule::REFRESH_DIALOGS    |
                                  ModelModule::REFRESH_MAIN_CAMERA;

}

/////////////////////////////////////////////////////////////////////////////

MLDCamera::~MLDCamera()
{}

/////////////////////////////////////////////////////////////////////////////

void MLDCamera::update()
{
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDCamera::kwCallback( KWEvent& event )
{
  switch( event.type )
    {
    case KWEvent::VALIDATE:
      {
#ifndef NDEBUG
//printf( "validate!!\n");
#endif
      break;
      }

    default:
      return MLScrollingDialog::kwCallback(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
