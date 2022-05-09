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
  MLEngine3D.C

  Stephane Rehel

  December 1 1996
*/

#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"

#include "graphics/mlgl.h"
#include "graphics/MLEvent.h"
#include "graphics/SystemWindow.h"
#include "graphics/MLFont.h"
#include "graphics/MLFonts.h"

#include "interface/MLXORRectAction.h"
#include "interface/MLMotionAction.h"
#include "interface/MLClickedMotionAction.h"
#include "interface/MLMode.h"
#include "interface/MLCanvasPopup.h"
#include "interface/MLPopup.h"
#include "interface/MLCanvasButton.h"
#include "interface/MLCanvasActionsPool.h"

#include "MLEngine3D.h"

#include "draw/ModelDraw.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelModes.h"
#include "kmodel/ModelFlags.h"
#include "kmodel/ModelColors.h"

#include "kmodel/modes/MLMSelect.h"
#include "kmodel/modes/MLMDrag.h"
#include "kmodel/modes/MLMRay.h"
#include "kmodel/modes/MLMRaySelected.h"
#include "kmodel/modes/MLMTagVertex.h"

#include "kmodel/MLTagVertexArray.h"

/////////////////////////////////////////////////////////////////////////////

MLEngine3D::MLEngine3D( ModelCanvas* _canvas ): ModelEngine(_canvas)
{
  setName("Engine3D");

  camera.initByLookat( Point(2*5,0,5), Point(0,0,0), Vector(0,0,1) );
  modelDraw= new ModelDraw(this);

/*
  displayTypePopup= new MLCanvasPopup(canvas);
  displayTypePopup->create("Display");
  MLPopup* p= displayTypePopup->getPopup();
  display_type_popup.wire=       p->add("Wire");
  display_type_popup.wire_front= p->add("Wire front");
  display_type_popup.preview=    p->add("Preview");
  ModelEngine::actions->addRightAction(displayTypePopup);
*/
  displayTypeButton= new MLCanvasButton(canvas);
  displayTypeButton->create("Preview");
  ModelEngine::actions->addRightAction(displayTypeButton);

  optionsPopup= new MLCanvasPopup(canvas);
  optionsPopup->create("Options");
  MLPopup* p= optionsPopup->getPopup();
  options_popup_wire_front= p->addCheck("Wire front");
  ModelEngine::actions->addRightAction(optionsPopup);

//  updatePopupsFlags();
}

/////////////////////////////////////////////////////////////////////////////

MLEngine3D::~MLEngine3D()
{
  delete modelDraw;
  modelDraw= 0;
}

/////////////////////////////////////////////////////////////////////////////

// protected
// return false if window is unmapped, etc.
boolean MLEngine3D::_back_draw()
{
  if( ! MLEngine::mapped() || model->isIconic() )
    return false;

  SystemWindow* window= MLEngine::getGfxWindow();
  if( window == 0 )
    return false;

  window->current();

/*
mlFront();
glClearColor(1,1,1,0);
glClear( GL_COLOR_BUFFER_BIT );
glFlush();
sleep(1);
*/
//printf( "ENGINE(%p) ",this);fflush(stdout);

  mlBack();

  camera.x_res= window->getSize().x();
  camera.y_res= window->getSize().y();
  modelDraw->setCamera(camera);

  modelDraw->clear();
  modelDraw->drawGrid();

  modelDraw->draw(scene);

  if( model->getFlags()->showCamera && !followMainCamera() )
    modelDraw->drawCamera( model->getMainCamera() );

  if( model->getFlags()->showViewAxes &&
      (modelDraw->getDisplayType() == ModelDraw::DISPLAY_WIRE ||
       modelDraw->getDisplayType() == ModelDraw::DISPLAY_WIRE_FRONT) )
    drawViewAxes();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLEngine3D::draw()
{
  if( ! _back_draw() )
    return;

  SystemWindow* window= MLEngine::getGfxWindow();

  if( window != 0 )
    window->swapBuffers();
}

/////////////////////////////////////////////////////////////////////////////

void MLEngine3D::pickObjects( MLXORRectAction* a, IntegerArray& objects )
{
  MLEngine::getGfxWindow()->current();

  modelDraw->setCamera(camera);

  modelDraw->pickObjects(scene,a->p1,a->p2,objects);
}

/////////////////////////////////////////////////////////////////////////////

void MLEngine3D::pickVertices( MLXORRectAction* a,
                               MLTagVertexArray& vertices )
{
  MLEngine::getGfxWindow()->current();

  modelDraw->setCamera(camera);

  modelDraw->pickVertices(scene,a->p1,a->p2,vertices);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLEngine3D::handleAction( MLAction* action )
{
/*
  if( action == displayTypePopup )
    {
    int label= displayTypePopup->getLabel();
    ModelDraw::DISPLAY_TYPE newType= ModelDraw::DISPLAY_NONE;

    if( label == display_type_popup.wire )
      newType= ModelDraw::DISPLAY_WIRE;
     else
    if( label == display_type_popup.wire_front )
      newType= ModelDraw::DISPLAY_WIRE_FRONT;
     else
    if( label == display_type_popup.preview )
      newType= ModelDraw::DISPLAY_PREVIEW;

    if( newType != ModelDraw::DISPLAY_NONE &&
        newType != modelDraw->displayType )
      {
      modelDraw->displayType= newType;
      refreshScreen(ModelModule::REFRESH_DISPLAY_TYPE);
      }
    return true;
    }
*/

  if( action == displayTypeButton )
    {
    ModelDraw::DISPLAY_TYPE old_type= modelDraw->getDisplayType();
    ModelDraw::DISPLAY_TYPE new_type= ModelDraw::DISPLAY_NONE;

    if( old_type == ModelDraw::DISPLAY_PREVIEW )
      {
      // go to wire
      boolean front= optionsPopup->getChecked(options_popup_wire_front);
      if( front )
        new_type= ModelDraw::DISPLAY_WIRE_FRONT;
       else
        new_type= ModelDraw::DISPLAY_WIRE;
      }
     else
      {
      new_type= ModelDraw::DISPLAY_PREVIEW;
      }

    modelDraw->setDisplayType(new_type);

    refreshScreen(ModelModule::REFRESH_DISPLAY_TYPE);
    updatePopupsFlags();

    return true;
    }

  if( action == optionsPopup )
    {
    int label= optionsPopup->getLabel();

    if( label == options_popup_wire_front )
      {
      boolean checked= !optionsPopup->getChecked(options_popup_wire_front);
      optionsPopup->setChecked(options_popup_wire_front,checked);

      ModelDraw::DISPLAY_TYPE old_type= modelDraw->getDisplayType();
      ModelDraw::DISPLAY_TYPE new_type= old_type;

      if( checked && old_type == ModelDraw::DISPLAY_WIRE )
        new_type= ModelDraw::DISPLAY_WIRE_FRONT;
      if( !checked && old_type == ModelDraw::DISPLAY_WIRE_FRONT )
        new_type= ModelDraw::DISPLAY_WIRE;

      if( new_type != old_type )
        {
        modelDraw->setDisplayType(new_type);

        refreshScreen(ModelModule::REFRESH_DISPLAY_TYPE);
        updatePopupsFlags();
        }

      return true;
      }
    }

  if( ! MLEngine::validPointerAction(action) )
    return ModelEngine::handleAction(action);

  switch( MLEngine::getMode()->getID() )
    {
    case ModelModes::SELECT:
      {
      MLMSelect* s= (MLMSelect*) getMode();

      MLXORRectAction* a= (MLXORRectAction*) action;

      pickObjects( a, s->objects );
      s->button= a->button;

      model->validateFunction();

      return true;
      }

    case ModelModes::TAG_VERTEX:
      {
      MLMTagVertex* tv= (MLMTagVertex*) getMode();

      MLXORRectAction* a= (MLXORRectAction*) action;

      pickVertices( a, *(tv->vertices) );
      tv->button= a->button;

      model->validateFunction();

      return true;
      }

    case ModelModes::DRAG:
      {
      MLMDrag* d= (MLMDrag*) getMode();

      MLMotionAction* a= (MLMotionAction*) action;
      if( a->type != MLMotionAction::MOVING )
        return true;

      d->v= getWorldVector(a);
      d->button= a->button;
      d->screenVector= getScreenVector(a);
      d->position1_x= a->position1.x();
      d->position1_y= a->position1.y();
      d->position2_x= a->position2.x();
      d->position2_y= a->position2.y();
      d->growing= a->delta_size.y() > 0;
      d->view_vector= camera.direction;
      d->ortho_camera= camera.ortho;

      model->validateFunction();

      return true;
      }

    case ModelModes::RAY_SELECTED:
      {
      MLMRaySelected* r= (MLMRaySelected*) getMode();

      MLClickedMotionAction* a= (MLClickedMotionAction*) action;
      if( a->type != MLClickedMotionAction::START )
        return true;

      if( r->select( a->origin.x(), a->origin.y(), camera ) )
        {
        r->button= a->button;

        model->validateFunction();
        }

      return true;
      }

    case ModelModes::RAY:
      {
      MLMRay* r= (MLMRay*) getMode();

      MLClickedMotionAction* a= (MLClickedMotionAction*) action;
      if( a->type != MLClickedMotionAction::START )
        return true;

      if( r->select( a->origin.x(), a->origin.y(), camera ) )
        {
        r->button= a->button;

        model->validateFunction();
        }

      return true;
      }

    default:
      break;
    }

  return ModelEngine::handleAction(action);
}

/////////////////////////////////////////////////////////////////////////////

Vector MLEngine3D::getWorldVector( MLMotionAction* a ) const
{
  double dx= double(a->delta_size.x()) / double(camera.x_res) * 2.;
  double dy= double(a->delta_size.y()) / double(camera.y_res) * 2.;
  double d_right, d_up;
  const Point& fp= camera.lookat;

  if( camera.ortho )
    {
    DBL sx, sy;
    camera.getOrthoSize(sx,sy);
    d_right= dx * sx;
    d_up=    dy * sy;
    }
   else
    {
    double linear_speed= (fp - camera.location).norm()
                                          * tan(camera.angle*0.5);
    d_right= dx * linear_speed;
    d_up=    dy * linear_speed;
    }

  return (camera.right * d_right) + (camera.up * d_up);
}

/////////////////////////////////////////////////////////////////////////////

Vector2 MLEngine3D::getScreenVector( MLMotionAction* a ) const
{
  double dx= double(a->delta_size.x()) / double(camera.x_res) * 2.;
  double dy= double(a->delta_size.y()) / double(camera.y_res) * 2.;
  return Vector2(dx,dy);
}

/////////////////////////////////////////////////////////////////////////////

void MLEngine3D::refreshScreen( unsigned what )
{
  if( (what & ModelModule::REFRESH_GEOMETRY) != 0 ||
      (what & ModelModule::REFRESH_TRANSFORM) != 0 ||
      (what & ModelModule::REFRESH_CAMERA) != 0 ||
      (what & ModelModule::REFRESH_DISPLAY_TYPE) != 0 )
    goto post;

  if( modelDraw->getDisplayType() == ModelDraw::DISPLAY_PREVIEW )
    {
    if( (what & ModelModule::REFRESH_ILLUM) != 0 ||
        (what & ModelModule::REFRESH_MATERIALS) != 0 ||
        (what & ModelModule::REFRESH_PREVIEW) != 0 )
      goto post;

    return;
    }

  if( modelDraw->getDisplayType() == ModelDraw::DISPLAY_WIRE ||
      modelDraw->getDisplayType() == ModelDraw::DISPLAY_WIRE_FRONT )
    {
    if( (what & ModelModule::REFRESH_SELECTION) != 0 ||
        (what & ModelModule::REFRESH_MAIN_CAMERA) != 0 ||
        (what & ModelModule::REFRESH_WIRE) != 0 )
      goto post;

    if( (what & ModelModule::REFRESH_HIER) != 0 &&
        model->getFlags()->showHierStruct )
      goto post;

    return;
    }

  return;

post:
  MLEngine::postRefresh();
}

/////////////////////////////////////////////////////////////////////////////

void MLEngine3D::refreshMainCamera( const MLCamera& )
{}

/////////////////////////////////////////////////////////////////////////////

void MLEngine3D::leaveMode( MLMode* mm )
{
  if( mm != 0 && model != 0 )
    model->postCmdStop(); // close the undo list for non-lonely commands

  ModelEngine::leaveMode(mm);
}

/////////////////////////////////////////////////////////////////////////////

void MLEngine3D::write( MLWFileBlock& block )
{
  ModelEngine::write(block);

  block << camera;
  block << int(modelDraw->getDisplayType());

  block << int(0);
}

/////////////////////////////////////////////////////////////////////////////

void MLEngine3D::read( MLRFileBlock& block )
{
  ModelEngine::read(block);

  block >> camera;

  ModelDraw::DISPLAY_TYPE dt= ModelDraw::DISPLAY_TYPE(block.getInteger());
  modelDraw->setDisplayType(dt);

  if( block.getInteger() == 0 )
    goto end;

end:
  updatePopupsFlags();
}

/////////////////////////////////////////////////////////////////////////////

void MLEngine3D::resetAll()
{
  modelDraw->resetAll();

  ModelEngine::resetAll();

  optionsPopup->setChecked(options_popup_wire_front,false);

  updatePopupsFlags();
}

/////////////////////////////////////////////////////////////////////////////

void MLEngine3D::updatePopupsFlags()
{
  if( optionsPopup == 0 )
    return;

  if( modelDraw->getDisplayType() == ModelDraw::DISPLAY_PREVIEW )
    {
    optionsPopup->setAvailable(options_popup_wire_front,false);

    displayTypeButton->setTitle(" Wire  ");
    }
   else
    {
    optionsPopup->setAvailable(options_popup_wire_front,true);

    if( modelDraw->getDisplayType() == ModelDraw::DISPLAY_WIRE )
      optionsPopup->setChecked(options_popup_wire_front,false);
    if( modelDraw->getDisplayType() == ModelDraw::DISPLAY_WIRE_FRONT )
      optionsPopup->setChecked(options_popup_wire_front,true);

    displayTypeButton->setTitle("Preview");
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLEngine3D::drawViewAxes()
{
  // go to pixel mode
  SystemWindow* window= MLEngine::getGfxWindow();
  if( window == 0 )
    return;

  window->pixelMode();

  double size= 40.; // pixels
  double margin= 10; // pixels

  Point2 x, y, z, O;

  if( camera.ortho )
    {
    Vector _x= camera.projection.apply(Vector(1,0,0));
    Vector _y= camera.projection.apply(Vector(0,1,0));
    Vector _z= camera.projection.apply(Vector(0,0,1));
    x= Point2(_x.x(),_x.y());
    y= Point2(_y.x(),_y.y());
    z= Point2(_z.x(),_z.y());
    O= Point2(0,0);

    Point2 minP= min( x, min( y, z ) );
    Point2 maxP= max( x, max( y, z ) );
    x -= minP;
    y -= minP;
    z -= minP;
    O -= minP;
    double s= (maxP - minP).norm();
    x *= size / s;
    y *= size / s;
    z *= size / s;
    O *= size / s;
    }
   else
    {
    Vector _x= camera.projection.apply(Vector(1,0,0));
    Vector _y= camera.projection.apply(Vector(0,1,0));
    Vector _z= camera.projection.apply(Vector(0,0,1));
    x= Point2(_x.x(),_x.y());
    y= Point2(_y.x(),_y.y());
    z= Point2(_z.x(),_z.y());
    O= Point2(0,0);

    size /= 1.3;
    double s= tan(camera.angle/2.);
    x *= size * s;
    y *= size * s;
    z *= size * s;
    O *= size * s;
    x += Point2(size,size);
    y += Point2(size,size);
    z += Point2(size,size);
    O += Point2(size,size);
    }

  x += Point2(margin,margin);
  y += Point2(margin,margin);
  z += Point2(margin,margin);
  O += Point2(margin,margin);

  double angle= DEGtoRAD(20.);
  const double length= 1. / 4.;
  double sin_a= sin(angle) * length;
  double cos_a= cos(angle) * length;
  int ok[3];

#define draw_arrow(i,dir) \
    {                                                               \
    Vector2 d= dir - O;                                             \
    double l= d.norm();                                             \
    if( l > 1e-5 )                                                  \
      {                                                             \
      ok[i]= 1;                                                     \
      Vector2 n( -d.y(), d.x() );                                   \
      Vector2 nx( d[0]*cos_a, d[1]*cos_a );                         \
      Vector2 ny( n[0]*sin_a, n[1]*sin_a );                         \
      glVertex2f( O[0], O[1] );                                     \
      glVertex2f( dir[0], dir[1] );                                 \
      glVertex2f( dir[0], dir[1] );                                 \
      glVertex2f( dir[0] - nx[0] - ny[0], dir[1] - nx[1] - ny[1] ); \
      glVertex2f( dir[0], dir[1] );                                 \
      glVertex2f( dir[0] - nx[0] + ny[0], dir[1] - nx[1] + ny[1] ); \
      dir += d / l * 5.;                                            \
      }                                                             \
     else                                                           \
      ok[i]= 0;                                                     \
    }

  glBegin(GL_LINES);
    mlColor(ModelColors::X_Axis); draw_arrow(0,x);
    mlColor(ModelColors::Y_Axis); draw_arrow(1,y);
    mlColor(ModelColors::Z_Axis); draw_arrow(2,z);
  glEnd();

#undef draw_arrow

  MLFont* font= smallFont;
  int dx= -(font->getCharWidth('X') / 2);
  int dy= -(-font->getDescent() + 1 + font->getHeight() / 2 - 1);
  if( ok[0] )
    {
    mlColor(ModelColors::X_Axis);
    font->print( int(x[0])+dx, int(x[1])+dy, "X", 1 );
    }

  if( ok[1] )
    {
    mlColor(ModelColors::Y_Axis);
    font->print( int(y[0])+dx, int(y[1])+dy, "Y", 1 );
    }

  if( ok[2] )
    {
    mlColor(ModelColors::Z_Axis);
    font->print( int(z[0])+dx, int(z[1])+dy, "Z", 1 );
    }

}

/////////////////////////////////////////////////////////////////////////////
