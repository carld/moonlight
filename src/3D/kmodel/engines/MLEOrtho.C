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
  MLEOrtho.C

  Stephane Rehel

  December 1 1996
*/

#include "graphics/mlgl.h"
#include "graphics/MLEvent.h"
#include "graphics/SystemWindow.h"

#include "MLEOrtho.h"

#include "interface/MLMotionAction.h"
#include "interface/MLMode.h"

#include "kmodel/ModelModes.h"

/////////////////////////////////////////////////////////////////////////////

MLEOrtho::MLEOrtho( ModelCanvas* _canvas ): MLEngine3D(_canvas)
{
  setName("Ortho");

  resetCamera();
}

/////////////////////////////////////////////////////////////////////////////

MLEOrtho::~MLEOrtho()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLEOrtho::handleAction( MLAction* action )
{
  if( ! MLEngine::validPointerAction(action) )
    return MLEngine3D::handleAction(action);;

  int modeID= MLEngine::getMode()->getID();
  switch( modeID )
    {
    case ModelModes::CAMERA_ORBIT:
      return true;

    case ModelModes::CAMERA_ZOOM:
    case ModelModes::CAMERA_WALK:
      {
      MLMotionAction* a= (MLMotionAction*) action;
      if( a->type != MLMotionAction::MOVING )
        return true;

      if( a->button == 1 && modeID != ModelModes::CAMERA_WALK )
        {
        Vector translate= getWorldVector(a);
        camera.initByLookat( camera.location - translate,
                             camera.lookat - translate,
                             camera.sky );
        }
       else
        {
        // change angle
        const IVector& size= a->window->getSize();
        double dx= double(a->delta_size.x()) / double(size.x()) * 2.;
        double dy= double(a->delta_size.y()) / double(size.y()) * 2.;

        double speed= 1.;
        if( a->button == 3 )
          speed *= 2.;

        double da= speed * camera.orthoZoom * sqrt(dx*dx+dy*dy) / 2.;
        if( dy < 0. )
          da= -da;
        double newZoom= camera.orthoZoom + da;
        if( newZoom < 1e-10 )
          newZoom= 1e-10;
        if( newZoom > 1e10 )
          newZoom= 1e10;

        camera.orthoZoom= newZoom;
        }

      MLEngine::postRefresh();
      return true;
      }

    default:
      break;
    }

  return MLEngine3D::handleAction(action);;
}

/////////////////////////////////////////////////////////////////////////////

void MLEOrtho::resetCamera()
{
  MLCamera _camera;
  camera= _camera; // set default parameters

  camera.initByLookat( Point(0,0,6), Point(0,0,0), Vector(0,1,0) );
  camera.ortho= true;
  camera.orthoZoom= 6.;
}

/////////////////////////////////////////////////////////////////////////////

void MLEOrtho::resetAll()
{
  MLEngine3D::resetAll();
}

/////////////////////////////////////////////////////////////////////////////

