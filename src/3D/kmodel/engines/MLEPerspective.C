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
  MLEPerspective.h

  Stephane Rehel

  December 1 1996
*/

#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"

#include "graphics/mlgl.h"
#include "graphics/MLEvent.h"
#include "graphics/SystemWindow.h"

#include "interface/MLMotionAction.h"
#include "interface/MLMode.h"
#include "interface/MLCanvas.h"
#include "interface/MLCanvasPopup.h"
#include "interface/MLPopup.h"

#include "kmodel/ModelModes.h"
#include "kmodel/ModelModule.h"
#include "kmodel/ModelFlags.h"

#include "MLEPerspective.h"

/////////////////////////////////////////////////////////////////////////////

MLEPerspective::MLEPerspective( ModelCanvas* _canvas ): MLEngine3D(_canvas)
{
  setName("Perspective");

  mainCameraEngine= false;
  mainCamera= false;

  if( optionsPopup != 0 )
    {
    MLPopup* p= optionsPopup->getPopup();
    options_popup_main_camera= p->addCheck("Main camera");
    }
   else
    options_popup_main_camera= 0;

  resetCamera();

//  updatePopupsFlags();
}

/////////////////////////////////////////////////////////////////////////////

MLEPerspective::~MLEPerspective()
{}

/////////////////////////////////////////////////////////////////////////////

void MLEPerspective::refreshMainCamera( const MLCamera& c )
{
  if( mainCamera && &c != &camera )
    {
    camera= c;
    if( canvas != 0 )
      {
      if( canvas->getEngine() == this && canvas->mapped() )
        refreshScreen(ModelModule::REFRESH_CAMERA);
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLEPerspective::handleAction( MLAction* action )
{
  if( action == optionsPopup )
    {
    int label= optionsPopup->getLabel();

    if( label == options_popup_main_camera && !mainCameraEngine )
      {
      if( mainCamera )
        mainCamera= false;
       else
        {
        const MLCamera& mc= model->getMainCamera();
        mainCamera= true;
        model->refreshMainCamera(mc);
        }

      optionsPopup->setChecked(options_popup_main_camera,mainCamera);

      return true;
      }
    }

  if( ! MLEngine::validPointerAction(action) )
    return MLEngine3D::handleAction(action);

  switch( MLEngine::getMode()->getID() )
    {
    case ModelModes::CAMERA_ORBIT:
      {
      MLMotionAction* a= (MLMotionAction*) action;
      if( a->type != MLMotionAction::MOVING )
        return true;

      const IVector& size= a->window->getSize();
      double   dphi= double(a->delta_size.x()) / double(size.x()) * -M_PI/2.;
      double dtheta= double(a->delta_size.y()) / double(size.y()) * -M_PI/2.;
      double phi= camera.phi;
      double theta= camera.theta;
      if( a->button == 1 || a->button == 2 )
        phi+= dphi;
      if( a->button == 1 || a->button == 3 )
        {
        theta+= dtheta;
        if( theta > DEGtoRAD(89.99) )
          theta= DEGtoRAD(89.99);
        if( theta < DEGtoRAD(-89.99) )
          theta= DEGtoRAD(-89.99);
        }
      double radius= (camera.location - camera.lookat).norm();
      camera.initSpheric( phi, theta, radius );

if( mainCamera )
  model->refreshMainCamera(camera);
if( model->getFlags()->showCamera && mainCamera )
model->refreshScreen(ModelModule::REFRESH_MAIN_CAMERA);
      refreshScreen(ModelModule::REFRESH_CAMERA);
      return true;
      }

    case ModelModes::CAMERA_ZOOM:
      {
      MLMotionAction* a= (MLMotionAction*) action;
      if( a->type != MLMotionAction::MOVING )
        return true;

      if( a->button == 1 )
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

        double speed= DEGtoRAD(30.);
        if( a->button == 3 )
          speed *= 2.;
        double da= speed * sqrt(dx*dx+dy*dy) / 2.;
        if( dy < 0. )
          da= -da;
        double newAngle= camera.angle + da;
        if( newAngle < DEGtoRAD(2.) )
          newAngle= DEGtoRAD(2.);
        if( newAngle > DEGtoRAD(179.) )
          newAngle= DEGtoRAD(179.);

        camera.angle= newAngle;
        }

if( mainCamera )
  model->refreshMainCamera(camera);
if( model->getFlags()->showCamera && mainCamera )
model->refreshScreen(ModelModule::REFRESH_MAIN_CAMERA);
      refreshScreen(ModelModule::REFRESH_CAMERA);
      return true;
      }

    case ModelModes::CAMERA_WALK:
      {
      MLMotionAction* a= (MLMotionAction*) action;
      if( a->type != MLMotionAction::MOVING )
        return true;

      const IVector& size= a->window->getSize();
      double dx= double(a->delta_size.x()) / double(size.x());
      double dy= double(a->delta_size.y()) / double(size.y());

      // change distance
      double speed= (a->button==3) ? 2.0 :
                    (a->button==1) ? 1.5 : 1.;

      double r= (camera.lookat - camera.location).norm();
      double dr= (speed * sqrt(dx*dx+dy*dy)) * r;
      if( dy < 0. ) r-= dr; else r+= dr;
      if( r < 1e-6 ) r= 1e-6;
      if( r > 1e6 )  r= 1e6;

      camera.init( camera.lookat - camera.direction * r,
                   camera.direction,
                   camera.sky );

if( mainCamera )
  model->refreshMainCamera(camera);
if( model->getFlags()->showCamera && mainCamera )
model->refreshScreen(ModelModule::REFRESH_MAIN_CAMERA);
      refreshScreen(ModelModule::REFRESH_CAMERA);
      return true;
      }

    default:
      break;
    }

  return MLEngine3D::handleAction(action);;
}

/////////////////////////////////////////////////////////////////////////////

void MLEPerspective::write( MLWFileBlock& block )
{
  block << int(1);
  block << int(mainCamera);

  block << int(0);

  MLEngine3D::write(block);
}

/////////////////////////////////////////////////////////////////////////////

void MLEPerspective::read( MLRFileBlock& block )
{
  if( block.getInteger() == 0 )
    goto end;

  mainCamera= (block.getInteger() != 0);

  if( block.getInteger() == 0 )
    goto end;

end:
  MLEngine3D::read(block);
}

/////////////////////////////////////////////////////////////////////////////

void MLEPerspective::resetCamera()
{
  MLCamera _camera;
  camera= _camera; // set default parameters

  camera.initByLookat( Point(2*5,0,5), Point(0,0,0), Vector(0,0,1) );

  // well,i have to call this function, there's a small bug in initByLookat()
  camera.initSpheric( camera.phi,camera.theta,
                      (camera.lookat-camera.location).norm());
}

/////////////////////////////////////////////////////////////////////////////

void MLEPerspective::resetAll()
{
  resetCamera();

  mainCamera= mainCameraEngine;

  MLEngine3D::resetAll();
}

/////////////////////////////////////////////////////////////////////////////

void MLEPerspective::setMainCameraEngine()
{
  mainCameraEngine= true;

  updatePopupsFlags();
}

/////////////////////////////////////////////////////////////////////////////

void MLEPerspective::updatePopupsFlags()
{
  if( optionsPopup != 0 )
    {
    if( mainCameraEngine )
      {
      optionsPopup->setAvailable( options_popup_main_camera, false );
      optionsPopup->setChecked( options_popup_main_camera, true );
      mainCamera= true;
      }
     else
      {
      optionsPopup->setAvailable( options_popup_main_camera, true );
      optionsPopup->setChecked( options_popup_main_camera, mainCamera );
      }
    }

  MLEngine3D::updatePopupsFlags();
}

/////////////////////////////////////////////////////////////////////////////
