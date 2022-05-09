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
  MLERay.C

  Stephane Rehel

  January 18 1997
*/

#include "tools/Chrono.h"
#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"

#include "graphics/mlgl.h"
#include "graphics/SystemWindow.h"
#include "graphics/MLPicture.h"

#include "interface/MLXORRectAction.h"
#include "interface/MLMotionAction.h"
#include "interface/MLCanvasButton.h"
#include "interface/MLCanvasPopup.h"
#include "interface/MLPopup.h"
#include "interface/MLCanvasActionsPool.h"
#include "interface/MLCanvas.h"
#include "interface/MLMode.h"

#include "scene/renderer/MLRayRenderer.h"
#include "scene/renderer/MLRendererOptions.h"
#include "scene/MLScene.h"

#include "kmodel/ModelModes.h"
#include "kmodel/ModelModule.h"

#include "MLERay.h"

/////////////////////////////////////////////////////////////////////////////

MLERay::MLERay( ModelCanvas* _canvas ): MLEPerspective(_canvas)
{
  setName("Ray");
  x1= y1= x2= y2= 0;
  picture= 0;
  window_valid= 0;

  qualityPopup= new MLCanvasPopup(canvas);
  qualityPopup->create("Quality");
  MLPopup* p= qualityPopup->getPopup();
  quality_popup.use_local=           p->addCheck("Use local quality");
  quality_popup.trace_shadows=       p->addCheck("Trace shadows");
  quality_popup.refine_shadows=      p->addCheck("Refine shadows");
  quality_popup.trace_reflections=   p->addCheck("Reflections");
  quality_popup.trace_transmissions= p->addCheck("Transmissions");
  ModelEngine::actions->addRightAction(qualityPopup);
  qualityPopup->setChecked( quality_popup.use_local, false );
  qualityPopup->setChecked( quality_popup.trace_shadows,       true );
  qualityPopup->setChecked( quality_popup.refine_shadows,      true );
  qualityPopup->setChecked( quality_popup.trace_reflections,   true );
  qualityPopup->setChecked( quality_popup.trace_transmissions, true );

  p= MLEngine3D::optionsPopup->getPopup();
  options_popup_select_all= p->add("Select all");

  raytrace_button= new MLCanvasButton(canvas);
  raytrace_button->create("Raytrace!");
  MLEPerspective::actions->addRightAction(raytrace_button);

/*
  select_all_button= new MLCanvasButton(canvas);
  select_all_button->create("SelectAll");
  MLEPerspective::actions->addRightAction(select_all_button);

  main_camera_button= new MLCanvasButton(canvas);
  main_camera_button->create("GetMainCamera");
  MLEPerspective::actions->addRightAction(main_camera_button);
*/

//  updatePopupsFlags();
}

/////////////////////////////////////////////////////////////////////////////

MLERay::~MLERay()
{
  delete picture;
  picture= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLERay::moveRaytracedWindow( int dx, int dy )
{
  if( ! window_valid )
    return;

  IVector size= MLEngine::getGfxWindow()->getSize();

  if( dx < 0 )
    { if( x1+dx < 0 ) dx= -x1; }
   else
    { if( x2+dx >= size.x() ) dx= size.x()-1-x2; }
  if( dy < 0 )
    { if( y1+dy < 0 ) dy= -y1; }
   else
    { if( y2+dy >= size.y() ) dy= size.y()-1-y2; }
  if( dx == 0 && dy == 0 )
    return;
  x1+= dx;
  x2+= dx;
  y1+= dy;
  y2+= dy;

  delete picture; picture= 0;

  refreshScreen( ModelModule::REFRESH_CAMERA );
}

/////////////////////////////////////////////////////////////////////////////

void MLERay::resizeRaytracedWindow( int dx, int dy )
{
  if( ! window_valid )
    return;

  IVector size= MLEngine::getGfxWindow()->getSize();

  int new_x2= x2 + dx;
  if( new_x2 >= size.x() )
    new_x2= size.x()-1;
  if( new_x2 - x1 + 1 < 8 )
    new_x2= 8 + x1 - 1;

  int new_y2= y2 + dy;
  if( new_y2 >= size.y() )
    new_y2= size.y()-1;
  if( new_y2 - y1 + 1 < 8 )
    new_y2= 8 + y1 - 1;

  if( new_x2 == x2 && new_y2 == y2 )
    return;
  x2= new_x2;
  y2= new_y2;

  delete picture; picture= 0;

  refreshScreen( ModelModule::REFRESH_CAMERA );
}

/////////////////////////////////////////////////////////////////////////////

boolean MLERay::handleAction( MLAction* action )
{
  if( action == raytrace_button )
    {
    raytrace();
    return true;
    }

  if( action == qualityPopup )
    {
    int label= qualityPopup->getLabel();
    boolean checked= qualityPopup->getChecked(label);
    qualityPopup->setChecked( label, !checked );

    updatePopupsFlags();

    return true;
    }

  if( action == MLEngine3D::optionsPopup )
    {
    int label= MLEngine3D::optionsPopup->getLabel();
    if( label == options_popup_select_all )
      {
      delete picture; picture= 0;

      IVector size= MLEngine::getGfxWindow()->getSize();
      x1= 0;
      y1= 0;
      x2= size.x() - 1;
      y2= size.y() - 1;
      creation_width= size.x();
      creation_height= size.y();
      window_valid= true;
      refreshScreen( ModelModule::REFRESH_CAMERA );

      return true;
      }
    }

/*
  if( action == main_camera_button )
    {
    camera= model->getMainCamera();
    refreshScreen( ModelModule::REFRESH_CAMERA );
    }
*/

  if( ! MLEngine::validPointerAction(action) )
    return MLEPerspective::handleAction(action);

  int modeID= MLEngine::getMode()->getID();

  if( modeID == ModelModes::SELECT )
    {
    MLXORRectAction* a= (MLXORRectAction*) action;

    x1= min( a->p1.x(), a->p2.x() );
    y1= min( a->p1.y(), a->p2.y() );
    x2= max( a->p1.x(), a->p2.x() );
    y2= max( a->p1.y(), a->p2.y() );
    IVector size= MLEngine::getGfxWindow()->getSize();

    if( x1 < 0 ) x1= 0;
    if( x1 >= size.x() ) x2= size.x() - 1;
    if( y1 < 0 ) y1= 0;
    if( y2 >= size.y() ) y2= size.y() - 1;

    int width= x2 - x1 + 1;
    int height= y2 - y1 + 1;
    if( width < 8 || height < 8 )
      {
      if( window_valid )
        {
        // remove previous window
        window_valid= false;
        delete picture; picture= 0;
        refreshScreen( ModelModule::REFRESH_CAMERA );
        }
      return true;
      }

    window_valid= true;
    delete picture; picture= 0;
    creation_width= size.x();
    creation_height= size.y();
    refreshScreen( ModelModule::REFRESH_CAMERA );

    return true;
    }

/*
  if( modeID == ModelModes::TRANSLATE ||
      modeID == ModelModes::MOVE )
    {
    MLMotionAction* a= (MLMotionAction*) action;
    if( a->type != MLMotionAction::MOVING )
      return true;

    int dx= a->delta_size.x();
    int dy= a->delta_size.y();

    moveRaytracedWindow( dx, dy );

    return true;
    }

  if( modeID == ModelModes::TRANSLATE_XYZ )
    {
    MLMotionAction* a= (MLMotionAction*) action;
    if( a->type != MLMotionAction::MOVING )
      return true;

    int dx= a->delta_size.x();
    int dy= a->delta_size.y();

    switch( a->button )
      {
      case 1: dy= 0; break;
      case 2: dx= 0; break;
      default: dx= dy= 0; break;
      }

    moveRaytracedWindow( dx, dy );

    return true;
    }

  if( modeID == ModelModes::SCALE )
    {
    MLMotionAction* a= (MLMotionAction*) action;
    if( a->type != MLMotionAction::MOVING )
      return true;

    int dx= a->delta_size.x();
    int dy= a->delta_size.y();

    resizeRaytracedWindow( dx, dy );

    return true;
    }

  if( modeID == ModelModes::SCALE_XYZ )
    {
    MLMotionAction* a= (MLMotionAction*) action;
    if( a->type != MLMotionAction::MOVING )
      return true;

    int dx= a->delta_size.x();
    int dy= a->delta_size.y();

    switch( a->button )
      {
      case 1: dy= 0; break;
      case 2: dx= 0; break;
      default: dx= dy= 0; break;
      }

    resizeRaytracedWindow( dx, dy );

    return true;
    }

  if( //MLEngine::pointerMode == ModelModes::ROTATE ||
      modeID == ModelModes::ROTATE_XYZ )
    return true;
*/

  return MLEPerspective::handleAction(action);
}

/////////////////////////////////////////////////////////////////////////////

void MLERay::refreshScreen( unsigned what )
{
  if( (what & ModelModule::REFRESH_GEOMETRY) != 0 ||
      (what & ModelModule::REFRESH_TRANSFORM) != 0 ||
      (what & ModelModule::REFRESH_CAMERA) != 0 ||
      (what & ModelModule::REFRESH_ILLUM) != 0 ||
      (what & ModelModule::REFRESH_MATERIALS) != 0 )
    {
    // picture should be raytraced again
    delete picture;
    picture= 0;
    MLEngine::postRefresh();
    return;
    }

  MLEPerspective::refreshScreen(what);
}

/////////////////////////////////////////////////////////////////////////////

void MLERay::draw()
{
  if( ! window_valid )
    {
    MLEPerspective::draw();
    return;
    }
  SystemWindow* window= MLEngine::getGfxWindow();
  if( window == 0 )
    return;

  IVector size= window->getSize();
  if( creation_width != size.x() || creation_height != size.y() )
    {
    // window has been resized
    window_valid= false;
    delete picture; picture= 0;
    MLEPerspective::draw();
    return;
    }

  if( ! MLEngine3D::_back_draw() )
    return;

  window->currentPixel();

  glDrawBuffer(GL_BACK);
  if( picture == 0 )
    {
    glDisable(GL_DEPTH_TEST);
    mlColor3f(0.,0.,1.);
    glLineStipple( 3, 0x7777 );
    glEnable( GL_LINE_STIPPLE );

    glBegin(GL_LINE_LOOP);
    glVertex2i(x1,y1);
    glVertex2i(x2,y1);
    glVertex2i(x2,y2);
    glVertex2i(x1,y2);
    glEnd();

    glDisable( GL_LINE_STIPPLE );
    }
   else
    {
    picture->draw( x1, y1 );
    }

  window->swapBuffers();
}

/////////////////////////////////////////////////////////////////////////////

void MLERay::raytrace()
{
  if( ! window_valid )
    {
    model->printMessage( "Please Select a raytraced area first.", 2 );
    return;
    }
  int width= x2 - x1 + 1;
  int height= y2 - y1 + 1;
  delete picture;
  picture= 0;
  if( width < 8 || height < 8 )
    return;
  picture= new MLPicture;
  picture->createRGB( width, height );

  model->stopLightingProcess();

  model->pushWaitCursor();
  glFlush();

  MLRendererOptions* options= scene->getRendererOptions();
  boolean save_trace_shadows= options->trace_shadows;
  boolean save_refine_shadows= options->trace_global_shadows;
  boolean save_trace_reflections= options->trace_reflections;
  boolean save_trace_transmissions= options->trace_transmissions;

  options->trace_shadows       =
                 qualityPopup->getChecked(quality_popup.trace_shadows);
  options->trace_global_shadows=
                 qualityPopup->getChecked(quality_popup.refine_shadows);
  options->trace_reflections   =
                 qualityPopup->getChecked(quality_popup.trace_reflections);
  options->trace_transmissions =
                 qualityPopup->getChecked(quality_popup.trace_transmissions);

  MLRayRenderer* renderer= new MLRayRenderer(scene,camera,true);
  renderer->setRenderedArea(x1,y1,x2,y2);
  renderer->allocBuffer(picture->getData());
  renderer->init();

  Chrono chrono;
  chrono.start();
  renderer->trace();
  chrono.stop();

  delete renderer;
  renderer= 0;

  options->trace_shadows       = save_trace_shadows;
  options->trace_global_shadows= save_refine_shadows;
  options->trace_reflections   = save_trace_reflections;
  options->trace_transmissions = save_trace_transmissions;

  model->popWaitCursor();

  MLString msg= MLString::printf( "%.2f seconds, %dx%d, %.0f us/pixel",
                 chrono.elapsed(), width, height,
                 chrono.elapsed()*1e6 / (double(width) * double(height)) );
  model->printMessage(msg);

  MLEngine::postRefresh();
}

/////////////////////////////////////////////////////////////////////////////

void MLERay::write( MLWFileBlock& block )
{
  MLEPerspective::write(block);

  block << x1 << y1 << x2 << y2;
  block << creation_width << creation_height;
  block << int(window_valid);
  block << int(1);

  block << int(qualityPopup->getChecked(quality_popup.use_local));
  block << int(qualityPopup->getChecked(quality_popup.trace_shadows));
  block << int(qualityPopup->getChecked(quality_popup.refine_shadows));
  block << int(qualityPopup->getChecked(quality_popup.trace_reflections));
  block << int(qualityPopup->getChecked(quality_popup.trace_transmissions));

  block << int(0);
}

/////////////////////////////////////////////////////////////////////////////

void MLERay::read( MLRFileBlock& block )
{
  MLEPerspective::read(block);

  block >> x1 >> y1 >> x2 >> y2;
  block >> creation_width >> creation_height;
  window_valid= block.getInteger();

  delete picture;
  picture= 0;

  if( block.getInteger() == 0 )
    goto end;

  qualityPopup->setChecked( quality_popup.use_local,
                            block.getInteger() != 0 );
  qualityPopup->setChecked( quality_popup.trace_shadows,
                            block.getInteger() != 0 );
  qualityPopup->setChecked( quality_popup.refine_shadows,
                            block.getInteger() != 0 );
  qualityPopup->setChecked( quality_popup.trace_reflections,
                            block.getInteger() != 0 );
  qualityPopup->setChecked( quality_popup.trace_transmissions,
                            block.getInteger() != 0 );

  if( block.getInteger() == 0 )
    goto end;

end:
  updatePopupsFlags();
}

/////////////////////////////////////////////////////////////////////////////

void MLERay::resetAll()
{
  x1= y1= x2= y2= 0;
  delete picture;
  picture= 0;
  window_valid= 0;

  if( qualityPopup != 0 )
    {
    qualityPopup->setChecked(quality_popup.use_local, false );
    qualityPopup->setChecked(quality_popup.trace_shadows,       true );
    qualityPopup->setChecked(quality_popup.refine_shadows,      true );
    qualityPopup->setChecked(quality_popup.trace_reflections,   true );
    qualityPopup->setChecked(quality_popup.trace_transmissions, true );
    }

  MLEPerspective::resetAll(); // will call updatePopupsFlags();
}

/////////////////////////////////////////////////////////////////////////////

void MLERay::updatePopupsFlags()
{
  if( qualityPopup == 0 )
    return;

  boolean ok= qualityPopup->getChecked(quality_popup.use_local);
  qualityPopup->setAvailable(quality_popup.trace_shadows,       ok );
  qualityPopup->setAvailable(quality_popup.refine_shadows,      ok );
  qualityPopup->setAvailable(quality_popup.trace_reflections,   ok );
  qualityPopup->setAvailable(quality_popup.trace_transmissions, ok );

  if( !qualityPopup->getChecked(quality_popup.trace_shadows) )
    qualityPopup->setAvailable(quality_popup.refine_shadows, false );

  MLEPerspective::updatePopupsFlags();
}

/////////////////////////////////////////////////////////////////////////////
