// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//                 2002       Patrick Mauritz
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
  ModelDraw.C

  Stephane Rehel

  December 8 1996
*/

#include <string.h>

#include "graphics/mlgl.h"
#include "GL/openglu.h"

#include "tools/IntegerArray.h"
#include "tools/Spectrum.h"
#include "tools/MLGammaCorrection.h"

#include "ModelGL.h"
#include "ModelDraw.h"
#include "ModelDrawWire.h"
#include "ModelDrawPreview.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelEngine.h"
#include "kmodel/ModelColors.h"
#include "kmodel/ModelFlags.h"
#include "kmodel/MLLightingProcess.h"

#include "kmodel/MLTagVertexArray.h"

#include "scene/MLCamera.h"
#include "scene/MLNullObject.h"
#include "scene/MLMesh.h"
#include "scene/MLCurve.h"
#include "scene/MLChildrenIterator.h"
#include "scene/MLObjectsIterator.h"
#include "scene/lighting/MLLight.h"
#include "scene/MLScene.h"
#include "scene/renderer/MLRendererOptions.h"

/////////////////////////////////////////////////////////////////////////////

ModelDraw::ModelDraw( ModelEngine* _engine )
{
  engine= _engine;
  model= engine->getModelModule();
  camera= 0;
  selectMode= false;
  displayType= ModelDraw::DISPLAY_WIRE;

  modelDrawWire= new ModelDrawWire(this);
  modelDrawPreview= new ModelDrawPreview(this);
  drawDriver= modelDrawWire;
}

/////////////////////////////////////////////////////////////////////////////

ModelDraw::~ModelDraw()
{
  delete modelDrawPreview;
  modelDrawPreview= 0;

  delete modelDrawWire;
  modelDrawWire= 0;

  drawDriver= 0;

  camera= 0;
}

/////////////////////////////////////////////////////////////////////////////

void ModelDraw::setDisplayType( ModelDraw::DISPLAY_TYPE dt )
{
  switch(dt)
    {
    case DISPLAY_WIRE:       drawDriver= modelDrawWire; break;
    case DISPLAY_WIRE_FRONT: drawDriver= modelDrawWire; break;
    case DISPLAY_PREVIEW:    drawDriver= modelDrawPreview; break;
    default:
      {
      drawDriver= modelDrawWire;
      dt= DISPLAY_WIRE;
      break;
      }
    }

  displayType= dt;
}

/////////////////////////////////////////////////////////////////////////////

void ModelDraw::setCamera( MLCamera& _camera )
{
  camera= &_camera;

  camera->compute();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMultMatrixd( camera->projection.dump() );
}

/////////////////////////////////////////////////////////////////////////////

void ModelDraw::clear()
{
  drawDriver->clear();
}

/////////////////////////////////////////////////////////////////////////////

void ModelDraw::drawGrid()
{
  drawDriver->drawGrid();
}

/////////////////////////////////////////////////////////////////////////////

void ModelDraw::draw( MLScene* scene )
{
  if( scene == 0 || model == 0 )
    return;

  model->getLightingProcess()->sleep();

  draw(scene->getRootObject());

  model->getLightingProcess()->wakeup();
}

/////////////////////////////////////////////////////////////////////////////

void ModelDraw::draw( MLObject* o )
{
  if( o == 0 || model == 0 )
    return;

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  drawDriver->setLighting();

  drawHier(o);

  glDisable(GL_LIGHTING);
}

/////////////////////////////////////////////////////////////////////////////

// returns a objectIDs array
void ModelDraw::pickObjects( MLScene* scene,
                             const IPoint& _p1, const IPoint& _p2,
                             IntegerArray& objects )
{
  objects.clear();

  if( scene == 0 || camera == 0 || model == 0 )
    return;

  int totalObjects= scene->getNObjects();
  if( totalObjects == 0 )
    return;

  IPoint p1= min(_p1,_p2);
  IPoint p2= max(_p1,_p2);
  IVector size= p2 - p1 + IVector(1,1);
  int min_size= 4;
  if( size.x() < min_size && size.y() < min_size )
    {
    while( size[0] < min_size )
      {
      --p1[0];
      ++size[0];
      if( size[0] < min_size )
        {
        ++p2[0];
        ++size[0];
        }
      }
    while( size[1] < min_size )
      {
      --p1[1];
      ++size[1];
      if( size[1] < min_size )
        {
        ++p2[1];
        ++size[1];
        }
      }
    }

  GLint viewport[4];
  glGetIntegerv( GL_VIEWPORT, viewport );
  camera->compute();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPickMatrix( double(p1.x()+p2.x())/2., double(p1.y()+p2.y())/2.,
                 double(size.x()), double(size.y()),
                 viewport );
  glMultMatrixd( camera->projection.dump() );

  int bufSize= totalObjects * 4;
  if( bufSize > 65536 )
    bufSize= 65536; // about 16000 objects are selectable in the same
                    // picking

  GLuint* selectBuffer= new GLuint [ bufSize ];
  glSelectBuffer( bufSize, selectBuffer );
  glRenderMode( GL_SELECT );
  selectMode= true;

  glInitNames();
  glPushName(GLuint(-1));

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  draw(scene->getRootObject());

  int hits= glRenderMode(GL_RENDER);

  selectMode= false;
  glSelectBuffer( bufSize, 0 );

  if( hits <= 0 )
    {
    delete selectBuffer;
    selectBuffer= 0;
    return;
    }

  if( hits > bufSize )
    hits= bufSize;

  for( int pass= 0; pass <= 1; ++pass )
    {
    GLuint* ptr= selectBuffer;
    int n= 0;
    for( int i= 0; i < hits; ++i )
      {
      //  for each hit
      int names= int(*ptr);
      ptr+= 3;
      for( int j= 0; j < names; ++j, ++ptr ) // for each name
        {
        if( *ptr > 0 )
          {
          if( pass == 0 )
            ++n;
           else
            {
            objects[n]= *ptr;
            ++n;
            }
          }
        }
      }

    if( pass == 0 )
      {
      if( n == 0 )
        break;
      objects.alloc(n);
      }
    }

  delete selectBuffer;
  selectBuffer= 0;
}

/////////////////////////////////////////////////////////////////////////////

// returns a (objectID,vertexID) array
void ModelDraw::pickVertices( MLScene* scene,
                              const IPoint& _p1, const IPoint& _p2,
                              MLTagVertexArray& vertices )
{
  vertices.clear();

  if( scene == 0 || camera == 0 || model == 0 )
    return;

//  int totalObjects= scene->getNObjects();
//  if( totalObjects == 0 )
//    return;

  IPoint p1= min(_p1,_p2);
  IPoint p2= max(_p1,_p2);
  IVector size= p2 - p1 + IVector(1,1);
  int min_size= 4;
  if( size.x() < min_size && size.y() < min_size )
    {
    while( size[0] < min_size )
      {
      --p1[0];
      ++size[0];
      if( size[0] < min_size )
        {
        ++p2[0];
        ++size[0];
        }
      }
    while( size[1] < min_size )
      {
      --p1[1];
      ++size[1];
      if( size[1] < min_size )
        {
        ++p2[1];
        ++size[1];
        }
      }
    }

  GLint viewport[4];
  glGetIntegerv( GL_VIEWPORT, viewport );
  camera->compute();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPickMatrix( double(p1.x()+p2.x())/2., double(p1.y()+p2.y())/2.,
                 double(size.x()), double(size.y()),
                 viewport );
  glMultMatrixd( camera->projection.dump() );

  int bufSize= 32000;//totalObjects * 4;
  if( bufSize > 65536 )
    bufSize= 65536; // about 16000 points are selectable in the same
                    // picking

  GLuint* selectBuffer= new GLuint [ bufSize ];

  MLObjectsIterator oi(scene);
  while( ! oi.eol() )
    {
    MLObject* object= oi++;

    if( object == 0 )
      continue;

    if( ! object->selected() )
      continue;

    glSelectBuffer( bufSize, selectBuffer );
    glRenderMode( GL_SELECT );
    selectMode= true;

    glInitNames();
    glPushName(GLuint(-1));

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

#ifdef DBL_IS_FLOAT
    glLoadMatrixf( object->global_xform.dump_o2w() );
#else
    glLoadMatrixd( object->global_xform.dump_o2w() );
#endif

    drawDriver->drawVertices(object);

    // in case it's a nurbs:
    drawDriver->pickNURBSControlPoints(object);

    int hits= glRenderMode(GL_RENDER);
    glSelectBuffer( bufSize, 0 );
    selectMode= false;

    if( hits <= 0 )
      continue;

    if( hits > bufSize )
      hits= bufSize;

    int k= vertices.getSize();
    for( int pass= 0; pass <= 1; ++pass )
      {
      GLuint* ptr= selectBuffer;
      int n= 0;
      for( int i= 0; i < hits; ++i )
        {
        //  for each hit
        int names= int(*ptr);
        ptr+= 3;
        for( int j= 0; j < names; ++j, ++ptr ) // for each name
          {
          if( *ptr > 0 )
            {
            if( pass == 0 )
              ++n;
             else
              {
              vertices[k++]= object->getID();
              vertices[k++]= *ptr;
              }
            }
          }
        }
      if( n == 0 )
        break;

      if( pass == 0 )
        vertices.realloc( vertices.getSize() + n*2 );
      }
    }

  delete selectBuffer;
  selectBuffer= 0;
}

/////////////////////////////////////////////////////////////////////////////

void ModelDraw::drawCamera( const MLCamera& c )
{
  drawDriver->drawCamera(c);
}

/////////////////////////////////////////////////////////////////////////////

// protected
void ModelDraw::drawHier( MLObject* o )
{
  if( o == 0 )
    return;

#ifdef DBL_IS_FLOAT
  glLoadMatrixf( o->global_xform.dump_o2w() );
#else
  glLoadMatrixd( o->global_xform.dump_o2w() );
#endif

  if( selectMode )
    glLoadName( GLuint(o->getID()) );

  const ModelFlags* flags= model->getFlags();

  if( flags->modellingComponent == ModelFlags::COMPONENT_POL &&
      !selectMode &&
      o->selected() )
    drawDriver->drawTagPolygons(o);

  drawObject(o);

  if( flags->modellingComponent == ModelFlags::COMPONENT_VTX &&
      !selectMode &&
      o->selected() )
    drawDriver->drawTagVertices(o);

  // draw non selected, then selected objects
  MLChildrenIterator oi(o);
  for( int what= 0; what <= 1; ++what )
    {
    oi.reset();
    while( ! oi.eol() )
      {
      MLObject* child= oi++;

      if( (child->selected()?1:0) != what )
        continue;
      if( model->getFlags()->showHierStruct &&
          o->getFather() != 0 &&
          (displayType == DISPLAY_WIRE ||
           displayType == DISPLAY_WIRE_FRONT) &&
          !selectMode )
        {
        glLoadIdentity();

        Point o1= o->global_xform.o2w(Point(0,0,0));
        Point o2= child->global_xform.o2w(Point(0,0,0));
        Point middle= (o1+o2) * 0.5;
        glColor(ModelColors::hierarchy_link);

        GLfloat lineWidth;
        glGetFloatv( GL_LINE_WIDTH, &lineWidth );

        glLineWidth(3.);
        glBegin(GL_LINES);
          glPoint(o1);
          glPoint(middle);
        glEnd();
        glLineWidth(lineWidth);
        glBegin(GL_LINES);
          glPoint(middle);
          glPoint(o2);
        glEnd();
        }

      drawHier(child);
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

// protected
void ModelDraw::drawObject( MLObject* o )
{
  if( o == 0 )
    return;
  if( o->isRoot() )
    return;

  if( o->getObjectType() == MLObject::MESH )
    {
    MLMesh* mesh= (MLMesh*) o;
    drawDriver->drawMesh(mesh);
    return;
    }

  if( o->getObjectType() == MLObject::CURVE )
    {
    MLCurve* curve= (MLCurve*) o;
    drawDriver->drawCurve(curve);
    return;
    }

  if( o->getObjectType() == MLObject::LIGHT )
    {
    MLLight* light= (MLLight*) o;
    drawDriver->drawLight(light);
    return;
    }

  if( o->getObjectType() == MLObject::NULL_OBJECT )
    {
    MLNullObject* no= (MLNullObject*) o;
    drawDriver->drawNullObject(no);
    return;
    }
}

/////////////////////////////////////////////////////////////////////////////

void ModelDraw::resetAll()
{
  setDisplayType(ModelDraw::DISPLAY_WIRE);
}

/////////////////////////////////////////////////////////////////////////////
