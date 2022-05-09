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
  ModelDrawWire.C

  Stephane Rehel
  September 7 1997
*/

#include "graphics/mlgl.h"

#include "ModelGL.h"
#include "ModelDrawWire.h"
#include "ModelDraw.h"

#include "kmodel/ModelFlags.h"
#include "kmodel/ModelColors.h"
#include "kmodel/ModelModule.h"

#include "scene/MLHashBox.h"
#include "scene/formal/FormalRep.h"
#include "scene/formal/FormalNURBS.h"
#include "scene/formal/FormalNURBSCurve.h"
#include "scene/nurbs/MLNURBS_ControlPoint.h"
#include "scene/nurbs/MLNURBS_Surface.h"
#include "scene/nurbs/MLNURBS_Curve.h"
#include "scene/MeshRep.h"
#include "scene/CurveRep.h"
#include "scene/MLCurveVertex.h"
#include "scene/MLCamera.h"
#include "scene/MLNullObject.h"
#include "scene/MLMesh.h"
#include "scene/MLCurve.h"
#include "scene/lighting/MLLight.h"

/////////////////////////////////////////////////////////////////////////////

ModelDrawWire::ModelDrawWire( ModelDraw* _modelDraw ):
  ModelDrawDriver(_modelDraw)
{}

/////////////////////////////////////////////////////////////////////////////

ModelDrawWire::~ModelDrawWire()
{}

/////////////////////////////////////////////////////////////////////////////

void ModelDrawWire::clear()
{
  glClearColor( ModelColors::wire_background.r(),
                ModelColors::wire_background.g(),
                ModelColors::wire_background.b(),
                1. );
  glClear( GL_COLOR_BUFFER_BIT );

  glDisable( GL_DEPTH_TEST );
//  glDepthMask(GL_FALSE);
  glDisable( GL_LIGHTING );
  glShadeModel(GL_FLAT);

// Note: if i disable glDepthMask, the shaded model isn't drawn correctly
// under micro$oft opengl / Win95
}

/////////////////////////////////////////////////////////////////////////////

void ModelDrawWire::drawGrid()
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  mlColor(ModelColors::grid);

  glLineStipple( 1, 0x5555 );

  for( int i= -5; i <= 5; ++i )
    {
    boolean not_stipple= ( i == -5 || i == 0 || i == 5 );
    if( not_stipple )
      glDisable( GL_LINE_STIPPLE );

    glBegin(GL_LINES);
    glVertex3i( i,-5, 0 );
    glVertex3i( i, 5, 0 );
    glVertex3i( -5, i, 0 );
    glVertex3i(  5, i, 0 );
    glEnd();

    if( not_stipple )
      glEnable( GL_LINE_STIPPLE );
    }

  glDisable( GL_LINE_STIPPLE );
}

/////////////////////////////////////////////////////////////////////////////

void ModelDrawWire::drawCamera( const MLCamera& c )
{
  Point eye= c.location;
  Point at= c.lookat;

  double d= (eye-at).norm();
  double h= d * tan(c.angle*0.5);
  double w= h * double(c.x_res) / double(c.y_res);

  Vector right= c.direction ^ c.sky;
  Vector up= c.right ^ c.direction;
  Vector u= w * right;
  Vector v= h * up;

  Point p11= at + u + v;
  Point p01= at - u + v;
  Point p00= at - u - v;
  Point p10= at + u - v;

  glLoadIdentity();
  glBegin(GL_LINES);
   glColor(ModelColors::camera);
   glPoint(eye); glPoint(p00);
   glPoint(eye); glPoint(p01);
   glPoint(eye); glPoint(p11);
   glPoint(eye); glPoint(p10);
   glPoint(p11); glPoint(p01);
   glPoint(p10); glPoint(p11);
   glColor(ModelColors::X_Axis);
   glPoint(p00); glPoint(p10);
   glColor(ModelColors::Y_Axis);
   glPoint(p01); glPoint(p00);

   glColor(ModelColors::camera);

   int nSubdivisions= 8;
   for( int i= 1; i < nSubdivisions; ++i )
     {
     double x= 2. * double(i) / double(nSubdivisions);
     Point p1, p2;

     // horizontal lines
     p1= p00 + x * v;
     p2= p10 + x * v;
     glPoint(p1);
     glPoint(p2);

     // vertical lines
     p1= p00 + x * u;
     p2= p01 + x * u;
     glPoint(p1);
     glPoint(p2);
     }
  glEnd();
}

/////////////////////////////////////////////////////////////////////////////

// protected
void ModelDrawWire::drawMesh( MLMesh* m )
{
  if( m == 0 )
    return;

  boolean highlighted= (m->flags & MLObject::TMP_HIGHLIGHTED) != 0;
  boolean selected= m->selected();

  MLAbstractMesh* abstract= m->getAbstract();

  if( abstract == 0 )
    return;

  if( (m->flags & MLObject::SHOW_BOX) != 0 )
    {
    if( ! m->isBounded() )
      return;

    MeshRep* meshrep= m->getMeshRep();
    if( meshrep == 0 )
      return;

    if( meshrep->dirtyBoundings )
      meshrep->computeBoundings();

    if( highlighted )
      glColor(ModelColors::highlighted);
     else
      {
      if( selected )
        glColor(ModelColors::selected);
       else
        glColor(ModelColors::unselected);
      }

    Box b= abstract->getBBox();
    Point mP= b.getMin();
    Point MP= b.getMax();

    glLineStipple( 2, 0x5555 );
    glEnable( GL_LINE_STIPPLE );
    glBegin(GL_LINES);
      glPoint(Point(mP[0],mP[1],mP[2])); glPoint(Point(mP[0],mP[1],MP[2]));
      glPoint(Point(MP[0],mP[1],mP[2])); glPoint(Point(MP[0],mP[1],MP[2]));
      glPoint(Point(MP[0],MP[1],mP[2])); glPoint(Point(MP[0],MP[1],MP[2]));
      glPoint(Point(mP[0],MP[1],mP[2])); glPoint(Point(mP[0],MP[1],MP[2]));

      glPoint(Point(mP[0],mP[1],mP[2])); glPoint(Point(MP[0],mP[1],mP[2]));
      glPoint(Point(mP[0],mP[1],MP[2])); glPoint(Point(MP[0],mP[1],MP[2]));
      glPoint(Point(mP[0],MP[1],MP[2])); glPoint(Point(MP[0],MP[1],MP[2]));
      glPoint(Point(mP[0],MP[1],mP[2])); glPoint(Point(MP[0],MP[1],mP[2]));

      glPoint(Point(mP[0],mP[1],mP[2])); glPoint(Point(mP[0],MP[1],mP[2]));
      glPoint(Point(mP[0],mP[1],MP[2])); glPoint(Point(mP[0],MP[1],MP[2]));
      glPoint(Point(MP[0],mP[1],MP[2])); glPoint(Point(MP[0],MP[1],MP[2]));
      glPoint(Point(MP[0],mP[1],mP[2])); glPoint(Point(MP[0],MP[1],mP[2]));
    glEnd();
    glDisable( GL_LINE_STIPPLE );
#ifndef NDEBUG
    MLCamera* camera= modelDraw->camera;
    if( camera->ortho )
      {
      float mvm[16];
      glGetFloatv( GL_MODELVIEW_MATRIX, mvm );
      glLoadIdentity();
      const BoundingSphere& bs= m->getObjectBoundingSphere();
//      MeshRep* mesh= m->getMeshRep();
//      const BoundingSphere& bs= mesh->boundingSphere;
      int n= 32;
      Vector X= camera->right * bs.getRadius();
      Vector Y= camera->up * bs.getRadius();

      glBegin(GL_LINE_LOOP);
      for( int i= 0; i < n; ++i )
        {
        double alpha= double(i) / double(n) * M_PI * 2.;
        Point p= bs.getCenter() + cos(alpha) * X + sin(alpha) * Y;
        glPoint(p);
        }
      glEnd();
      glLoadMatrixf(mvm);
      }
#endif
    return;
    }

  MeshRep* mesh= abstract->getMeshRep();
  if( mesh == 0 )
    return;

  // this is for debugging
  if( selected && mesh->hashBox != 0 && model->getFlags()->showHashBox )
    {
    glBegin(GL_LINES);
    glColor3f(0.6,0,0.6); // hard coded color
    Point minP= mesh->hashBox->getMin();
    Point maxP= mesh->hashBox->getMax();
    Vector voxelSize= mesh->hashBox->getVoxelSize();

    int i, j, k;
    double x, y, z;

    x= minP.x();
    for( i= 0; i <= mesh->hashBox->getX(); ++i, x+= voxelSize.x() )
      {
      y= minP.y();
      for( j= 0; j <= mesh->hashBox->getY(); ++j, y+= voxelSize.y() )
        {
        glVertex3f(x,y,minP.z());
        glVertex3f(x,y,maxP.z());
        }
      }

    y= minP.y();
    for( j= 0; j <= mesh->hashBox->getY(); ++j, y+= voxelSize.y() )
      {
      z= minP.z();
      for( k= 0; k <= mesh->hashBox->getZ(); ++k, z+= voxelSize.z() )
        {
        glVertex3f(minP.x(),y,z);
        glVertex3f(maxP.x(),y,z);
        }
      }

    z= minP.z();
    for( k= 0; k <= mesh->hashBox->getZ(); ++k, z+= voxelSize.z() )
      {
      x= minP.x();
      for( i= 0; i <= mesh->hashBox->getX(); ++i, x+= voxelSize.x() )
        {
        glVertex3f(x,minP.y(),z);
        glVertex3f(x,maxP.y(),z);
        }
      }

    glEnd();
    }

  Color* c_highlighted= &ModelColors::highlighted;
  Color* c_normal   = &ModelColors::unselected;
  Color* c_selected = &ModelColors::selected;
  Color* c_border   = &ModelColors::border_edge;
  Color* c_disc     = &ModelColors::disc_edge;
  Color* c_tess     = &ModelColors::tesselation;

  Color* c0;
  if( highlighted )
    {
    selected= false;
    c0= c_highlighted;
    }
   else
    {
    if( selected )
      c0= c_selected;
     else
      c0= c_normal;
    }

  Color* c= c0;
  glColor(*c);

  boolean show_tesselation= model->getFlags()->showTesselation;

  ModelDraw::DISPLAY_TYPE displayType= modelDraw->displayType;

  if( m->isFormal() )
    {
    glLineStipple( 1, 0xfcfc );
    glEnable( GL_LINE_STIPPLE );
    }

  if( displayType == ModelDraw::DISPLAY_WIRE )
    {
    glBegin(GL_LINES);

    MLEdgeArray& edges= mesh->edges;
    MLVertexArray& vertices= mesh->vertices;
    for( int i= 1; i <= edges.getSize(); ++i )
      {
      register MLEdge& e= edges[i];
      if( ! e.allocated() )
        continue;

      if( selected )
        {
        Color* c1= c0;

        if( (e.flags & MLEdge::BASE_EDGE) != 0 )
          {
          if( (e.flags & MLEdge::BORDER) != 0 )
            c1= c_border;
           else
            if( (e.flags & MLEdge::DISC) != 0 )
              c1= c_disc;
          }
         else
          {
          if( (e.flags & MLEdge::TESSELATION) != 0 )
            {
            if( ! show_tesselation )
              continue;
            c1= c_tess;
            }
          }

        if( c1 != c )
          {
          c= c1;
          glColor(*c);
          }
        }
       else
        {
        if( (e.flags & MLEdge::BASE_EDGE) == 0 )
          continue;
        }

      glPoint(vertices[e.v1].point);
      glPoint(vertices[e.v2].point);
      }
    glEnd();
    }

  Point camera_location(0,0,0);
  MLCamera* camera= modelDraw->camera;

  if( displayType == ModelDraw::DISPLAY_WIRE_FRONT )
    {
    camera_location= m->getGlobalXForm().get_w2o().apply(camera->location);

    MLEdgeArray& edges= mesh->edges;
    MLPolygonArray& polygons= mesh->polygons;
    MLVertexArray& vertices= mesh->vertices;

    glBegin(GL_LINES);
    for( int i= polygons.getSize(); i; --i )
      {
      MLPolygon& poly= polygons[i];
      if( ! poly.allocated() )
        continue;
      if( (poly.flags & MLPolygon::BASE_POLYGON) == 0 )
        continue;

      const MLPolygon::Element* poly_elements= poly.elements;
      if( poly_elements == 0 )
        continue;

      register MLVertex& first_vertex= vertices [ poly_elements->vertex ];
      if( ((camera_location-first_vertex.point) | poly.normal) < 0. )
        continue;

      for( int j= 0; j < poly.nVertices; ++j, ++poly_elements )
        {
        if( selected )
          {
          Color* c1= c0;

          register MLEdge& e= edges [ poly_elements->edge ];

          if( (e.flags & MLEdge::BORDER) != 0 )
            c1= c_border;
           else
          if( (e.flags & MLEdge::DISC) != 0 )
            c1= c_disc;

          if( c1 != c )
            {
            c= c1;
            glColor(*c);
            }
          }

        register MLVertex* vertex1= &(vertices[ poly_elements->vertex ]);
        register MLVertex* vertex2;
        if( j+1 == poly.nVertices )
          vertex2= &(vertices[ poly.elements->vertex ]);
         else
          vertex2= &(vertices[ (poly_elements+1)->vertex ]);

        glPoint( vertex1->point );
        glPoint( vertex2->point );
        }
      }
    glEnd(); // GL_LINES
    }

  if( m->isFormal() )
    glDisable( GL_LINE_STIPPLE );

  if( selected && model->getFlags()->showNormals )
    {
    float mvm[16];
    glGetFloatv( GL_MODELVIEW_MATRIX, mvm );
    glLoadIdentity();

    glBegin(GL_LINES);
    glColor(ModelColors::normals);

    const Transform& xform= m->getGlobalXForm();
    double normal_scaling= ( fabs(xform.get_o2w()(0,0)) +
                             fabs(xform.get_o2w()(1,1)) +
                             fabs(xform.get_o2w()(2,2)) ) / 3. * 0.5;

    MLVertexArray& vertices= mesh->vertices;
    MLSVertexArray& svertices= mesh->svertices;
    for( int i= 1; i <= svertices.getSize(); ++i )
      {
      register MLSVertex& sv= svertices[i];
      if( ! sv.allocated() )
        continue;

      if( displayType == ModelDraw::DISPLAY_WIRE_FRONT )
        {
        register MLVertex& v= vertices [ sv.vertex ];
        if( ((camera_location-v.point) | sv.normal) < 0. )
          continue;
        }

      register MLVertex& v= vertices[ sv.vertex ];

      Point p0= xform.o2w(v.point);
      Point p1= p0 + xform.normal_o2w(sv.normal).getNormalized()
                     * normal_scaling;

      glPoint(p0);
      glPoint(p1);
      }
    glEnd();

    glLoadMatrixf(mvm);
    }

  if( selected && model->getFlags()->showObjectAxes )
    {
    static double x1= 0., y1= 0.;
    static boolean initialized= false;
    if( ! initialized )
      {
      const double alpha= DEGtoRAD(20.);
      const double length= 1./6.;
      x1= length * sin(alpha);
      y1= 1. - length * cos(alpha);
      initialized= true;
      }

    glBegin(GL_LINES);
      // X
      glColor(ModelColors::X_Axis);
      glVertex3f(0,0,0);
      glVertex3f(1,0,0);
      glVertex3f(1,0,0);
      glVertex3f(y1,x1,0);
      glVertex3f(1,0,0);
      glVertex3f(y1,-x1,0);

      // Y
      glColor(ModelColors::Y_Axis);
      glVertex3f(0,0,0);
      glVertex3f(0,1,0);
      glVertex3f(0,1,0);
      glVertex3f( x1,y1,0);
      glVertex3f(0,1,0);
      glVertex3f(-x1,y1,0);

      // Z
      glColor(ModelColors::Z_Axis);
      glVertex3f(0,0,0);
      glVertex3f(0,0,1);
      glVertex3f(0,0,1);
      glVertex3f(0,x1,y1);
      glVertex3f(0,0,1);
      glVertex3f(0,-x1,y1);
    glEnd();
    }

  drawNURBSControlNet(m);
}

/////////////////////////////////////////////////////////////////////////////

// protected
void ModelDrawWire::drawCurve( MLCurve* c )
{
  if( c == 0 )
    return;

  boolean highlighted= (c->flags & MLObject::TMP_HIGHLIGHTED) != 0;
  boolean selected= c->selected();

  MLAbstractCurve* abstract= c->getAbstract();

  if( abstract == 0 )
    return;

  CurveRep* curve= abstract->getCurveRep();
  if( curve == 0 )
    return;

  Color* c_highlighted= &ModelColors::highlighted;
  Color* c_normal   = &ModelColors::unselected;
  Color* c_selected = &ModelColors::selected;

  Color* c0;
  if( highlighted )
    {
    selected= false;
    c0= c_highlighted;
    }
   else
    {
    if( selected )
      c0= c_selected;
     else
      c0= c_normal;
    }

  Color* _c= c0;
  glColor(*_c);

  if( c->isFormal() )
    {
    glLineStipple( 1, 0xfcfc );
    glEnable( GL_LINE_STIPPLE );
    }

  MLCurveVertex* vertices= curve->vertices;

  if( curve->closed )
    glBegin(GL_LINE_LOOP);
   else
    glBegin(GL_LINE_STRIP);

  for( int i= 1; i <= curve->nVertices; ++i )
    glPoint(vertices[i].point);

  glEnd();

  if( c->isFormal() )
    glDisable( GL_LINE_STIPPLE );

/*
  if( selected && model->getFlags()->showNormals )
    {
    float mvm[16];
    glGetFloatv( GL_MODELVIEW_MATRIX, mvm );
    glLoadIdentity();

    glBegin(GL_LINES);
    glColor(ModelColors::normals);

    const Transform& xform= m->getGlobalXForm();
    double normal_scaling= ( fabs(xform.get_o2w()(0,0)) +
                             fabs(xform.get_o2w()(1,1)) +
                             fabs(xform.get_o2w()(2,2)) ) / 3. * 0.5;

    MLVertexArray& vertices= mesh->vertices;
    MLSVertexArray& svertices= mesh->svertices;
    for( int i= 1; i <= svertices.getSize(); ++i )
      {
      register MLSVertex& sv= svertices[i];
      if( ! sv.allocated() )
        continue;

      if( displayType == ModelDraw::DISPLAY_WIRE_FRONT )
        {
        register MLVertex& v= vertices [ sv.vertex ];
        if( ((camera_location-v.point) | sv.normal) < 0. )
          continue;
        }

      register MLVertex& v= vertices[ sv.vertex ];

      Point p0= xform.o2w(v.point);
      Point p1= p0 + xform.normal_o2w(sv.normal).getNormalized()
                     * normal_scaling;

      glPoint(p0);
      glPoint(p1);
      }
    glEnd();

    glLoadMatrixf(mvm);
    }
*/

  if( selected && model->getFlags()->showObjectAxes )
    {
    static double x1= 0., y1= 0.;
    static boolean initialized= false;
    if( ! initialized )
      {
      const double alpha= DEGtoRAD(20.);
      const double length= 1./6.;
      x1= length * sin(alpha);
      y1= 1. - length * cos(alpha);
      initialized= true;
      }

    glBegin(GL_LINES);
      // X
      glColor(ModelColors::X_Axis);
      glVertex3f(0,0,0);
      glVertex3f(1,0,0);
      glVertex3f(1,0,0);
      glVertex3f(y1,x1,0);
      glVertex3f(1,0,0);
      glVertex3f(y1,-x1,0);

      // Y
      glColor(ModelColors::Y_Axis);
      glVertex3f(0,0,0);
      glVertex3f(0,1,0);
      glVertex3f(0,1,0);
      glVertex3f( x1,y1,0);
      glVertex3f(0,1,0);
      glVertex3f(-x1,y1,0);

      // Z
      glColor(ModelColors::Z_Axis);
      glVertex3f(0,0,0);
      glVertex3f(0,0,1);
      glVertex3f(0,0,1);
      glVertex3f(0,x1,y1);
      glVertex3f(0,0,1);
      glVertex3f(0,-x1,y1);
    glEnd();
    }

  drawNURBSControlNet(c);
}

/////////////////////////////////////////////////////////////////////////////

void ModelDrawWire::drawLight( MLLight* l )
{
  if( l == 0 )
    return;

  boolean highlighted= (l->flags & MLObject::TMP_HIGHLIGHTED) != 0;
  boolean selected= l->selected();

  double f= 0.2;
  double fa= f / 0.70710678;
  if( highlighted )
    glColor(ModelColors::highlighted);
   else
    {
    if( selected )
      {
      int i= (l->directLight ? 0x01 : 0x00)
            +(l->globalLight ? 0x10 : 0x00);
      switch(i)
        {
        case 0x00: glColor(ModelColors::light); break;
        case 0x01: glColor(ModelColors::light_direct); break;
        case 0x10: glColor(ModelColors::light_global); break;
        case 0x11: glColor(ModelColors::light_direct_global); break;
        }
      }
     else
      glColor(ModelColors::unselected);
    }

  if( l->getLightType() != MLLight::SPOT )
    {
    glBegin(GL_LINES);
      glVertex3f(-f,-f,-f); glVertex3f(+f,+f,+f);
      glVertex3f(-f,+f,-f); glVertex3f(+f,-f,+f);
      glVertex3f(+f,-f,-f); glVertex3f(-f,+f,+f);
      glVertex3f(+f,+f,-f); glVertex3f(-f,-f,+f);

      glVertex3f(+fa, 0., 0.); glVertex3f(-fa, 0., 0.);
      glVertex3f( 0.,+fa, 0.); glVertex3f( 0.,-fa, 0.);
      glVertex3f( 0., 0.,+fa); glVertex3f( 0., 0.,-fa);
    glEnd();
    }

  if( l->getLightType() == MLLight::DIRECTIONAL ||
      l->getLightType() == MLLight::SPOT )
    {
    f *= 5.;

    if( l->has_extent )
      f= l->extent;

    const double alpha= DEGtoRAD(20.);
    const double length= f/4.;
    double x1= length * sin(alpha);
    double y1= f - length * cos(alpha);

    glBegin(GL_LINES);
      glVertex3f( 0., 0., 0.);
      glVertex3f( 0., 0.,-f );

      glVertex3f( 0., 0., -f );
      glVertex3f( 0., -x1, -y1 );

      glVertex3f( 0., 0., -f );
      glVertex3f( 0., +x1, -y1 );

      glVertex3f( 0., 0., -f );
      glVertex3f( -x1, 0., -y1 );

      glVertex3f( 0., 0., -f );
      glVertex3f( +x1, 0., -y1 );
    glEnd();

    if( l->getLightType() == MLLight::SPOT )
      {
      float mvm[16];
      glGetFloatv( GL_MODELVIEW_MATRIX, mvm );
      glLoadIdentity();
      double scale= l->local_xform.scale.x();

      double length;
      Vector X, Y;
      l->worldDirection.makeOrthoBase(X,Y);
      X.setNormalized();
      Y.setNormalized();

      if( l->has_extent )
        {
        length= l->extent * cos(l->spot_cutoff);
        X *= l->extent * sin(l->spot_cutoff);
        Y *= l->extent * sin(l->spot_cutoff);
        }
       else
        {
        length= 1.;

        double a= min( l->spot_cutoff, DEGtoRAD(89.99) );
        double ta= tan(a);
        length *= scale;
        X *= length * ta;
        Y *= length * ta;
        }

      Point origin=  l->worldPosition
                   + l->worldDirection * (length );

      int n= 4 * 5; // number of points

      glBegin(GL_LINE_LOOP);
      for( int i= 0; i <= n; ++i )
        {
        double angle= 2. * M_PI * double(i-1) / double(n);
        Point p= origin + X * cos(angle) + Y * sin(angle);
        glPoint(p);
        }
      glEnd(); // GL_LINE_LOOP

      glBegin(GL_LINES);
      for( int i= 0; i <= n; ++i )
        {
        double angle= 2. * M_PI * double(i-1) / double(n);
        Point p= origin + X * cos(angle) + Y * sin(angle);
        glPoint(l->worldPosition);
        glPoint(p);
        }
      glEnd(); // GL_LINEQ

      glLoadMatrixf(mvm);
      }
    }

  if( l->getLightType() != MLLight::DIRECTIONAL &&
      l->has_extent )
    {
    float mvm[16];
    glGetFloatv( GL_MODELVIEW_MATRIX, mvm );
    glLoadIdentity();

    Point origin= l->worldPosition;
    double radius= l->extent;
    int n= 4 * 5; // number of points

    Vector X= modelDraw->camera->right * radius;
    Vector Y= modelDraw->camera->up * radius;

    glBegin(GL_LINE_LOOP);
    for( int i= 0; i <= n; ++i )
      {
      double angle= 2. * M_PI * double(i-1) / double(n);
      Point p= origin + X * cos(angle) + Y * sin(angle);
      glPoint(p);
      }
    glEnd(); // GL_LINE_LOOP

    glLoadMatrixf(mvm);
    }
}

/////////////////////////////////////////////////////////////////////////////

void ModelDrawWire::drawNullObject( MLNullObject* no )
{
  if( no == 0 )
    return;

  boolean highlighted= (no->flags & MLObject::TMP_HIGHLIGHTED) != 0;
  boolean selected= no->selected();

  double f= 0.5;

  if( highlighted )
    glColor(ModelColors::highlighted);
   else
    {
    if( selected )
      glColor(ModelColors::null_object);
     else
      glColor(ModelColors::unselected);
    }

  glBegin(GL_LINES);
    glVertex3f(+f , 0., 0.); glVertex3f(-f , 0., 0.);
    glVertex3f( 0.,+f , 0.); glVertex3f( 0.,-f , 0.);
    glVertex3f( 0., 0.,+f ); glVertex3f( 0., 0.,-f );
  glEnd();
}

/////////////////////////////////////////////////////////////////////////////

//#include "graphics/MLSmallSpherePicture.h"
void ModelDrawWire::drawTagVertices( MLObject* object )
{
  if( object == 0 )
    return;

  if( object->getObjectType() != MLObject::MESH )
    return;

  if( ! object->selected() )
    return;

  MLMesh* mlmesh= (MLMesh*) object;
  MeshRep* mesh= mlmesh->getMeshRep();
  if( mesh == 0 )
    return;
  if( mlmesh->isFormal() )
    return;

  const Matrix4x4& o2w= object->global_xform.get_o2w();
  MLCamera* camera= modelDraw->camera;
  Matrix4x4 toViewport= camera->getPixelProjection();
  Matrix4x4 o2vp= toViewport * o2w;

  float mvm[16];
  glGetFloatv( GL_MODELVIEW_MATRIX, mvm );
  float pm[16];
  glGetFloatv( GL_PROJECTION_MATRIX, pm );

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho( 0., GLfloat(camera->x_res), 0., GLfloat(camera->y_res), -1., 1. );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  mlColor(ModelColors::tag_vertex);
  const double margin= 3.; // tags are margin*2 pixels large

  ModelDraw::DISPLAY_TYPE displayType= modelDraw->displayType;

  MLVertexArray& vertices= mesh->vertices;
  MLPolygonArray& polygons= mesh->polygons;
  for( int i= 1; i <= vertices.getSize(); ++i )
    {
    MLVertex& v= vertices[i];

    if( ! v.allocated() || ! v.selected() )
      continue;

    if( displayType == ModelDraw::DISPLAY_WIRE_FRONT )
      {
      boolean found_one= false;
      Vector dir= v.point - camera->location;
      for( int j= 0; j < v.nPolygons; ++j )
        {
        int pi= v.polygons[j];
        MLPolygon& p= polygons[pi];
        if( (p.normal | dir) <= 0. )
          {
          found_one= true;
          break;
          }
        }
      if( ! found_one )
        continue;
      }

    Point p= o2vp.apply(v.point);

    if( p.x()+margin < 0. ||
        p.y()+margin < 0. ||
        p.x()-margin >= double(camera->x_res) ||
        p.y()-margin >= double(camera->y_res) )
      continue;

    glRectf( p.x()-margin, p.y()-margin, p.x()+margin, p.y()+margin );
    }

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(pm);
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(mvm);

/*
float mvm[16];
glGetFloatv( GL_MODELVIEW_MATRIX, mvm );
float pm[16];
glGetFloatv( GL_PROJECTION_MATRIX, pm );

glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho( 0., GLfloat(camera->x_res), 0., GLfloat(camera->y_res), -1., 1. );
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
static MLSmallSpherePicture* sp= 0;
if( sp == 0 )
  {
  sp= new MLSmallSpherePicture(Color(1.2,0,0));
  }

  MLVertexArray& vertices= mesh->vertices;
  for( int i= 1; i <= vertices.getSize(); ++i )
    {
    MLVertex& v= vertices[i];
    if( ! v.allocated() ||  v.selected() )
      return;

Point p= camera->projection.apply(o2w.apply(v.point));

double x= (p.x()+1.) * double(camera->x_res) / 2.;
double y= (p.y()+1.) * double(camera->y_res) / 2.;
//printf( "(%d) %f %f\n",i,
//        x - sp->getWidth()/2,
//        y - sp->getHeight()/2 );
if( x < 0. || y < 0. ||
    x >= double(camera->x_res) ||
    y >= double(camera->y_res) )
  continue;
sp->draw( int(x) - sp->getWidth()/2, int(y) - sp->getHeight()/2 );
//double margin= 3. / double(camera->x_res) *2.;
//double x1= p.x() - margin;
//double y1= p.y() - margin;
//double x2= p.x() + margin;
//double y2= p.y() + margin;
//glColor3f(1,0,0);
//glRectf(x1,y1,x2,y2);
    }

glMatrixMode(GL_PROJECTION);
glLoadMatrixf(pm);
glMatrixMode(GL_MODELVIEW);
glLoadMatrixf(mvm);
*/

}

/////////////////////////////////////////////////////////////////////////////

void ModelDrawWire::drawTagPolygons( MLObject* object )
{
  if( object == 0 )
    return;

  if( object->getObjectType() != MLObject::MESH )
    return;

  if( ! object->selected() )
    return;

  MLMesh* mlmesh= (MLMesh*) object;
  MeshRep* mesh= mlmesh->getMeshRep();
  if( mesh == 0 )
    return;
  if( mlmesh->isFormal() )
    return;

  glBlendFunc( GL_ONE, GL_SRC_ALPHA );
  glEnable(GL_BLEND);
  glColor4f( ModelColors::tag_polygon.r(),
             ModelColors::tag_polygon.g(),
             ModelColors::tag_polygon.b(),
             0.4 );

  ModelDraw::DISPLAY_TYPE displayType= modelDraw->displayType;

  if( displayType == ModelDraw::DISPLAY_WIRE_FRONT )
    {
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    }

  MLPolygonArray& polygons= mesh->polygons;
  MLVertexArray& vertices= mesh->vertices;
  for( int i= 1; i <= polygons.getSize(); ++i )
    {
    MLPolygon& p= polygons[i];

    if( ! p.allocated() || ! p.selected() )
      continue;
    if( (p.flags & MLPolygon::BASE_POLYGON) == 0 )
      continue;

    if( p.nVertices < 3 )
      continue;

    if( p.nVertices == 3 )
      {
      glBegin(GL_TRIANGLES);
        glPoint( vertices[ p.elements[0].vertex ].point );
        glPoint( vertices[ p.elements[1].vertex ].point );
        glPoint( vertices[ p.elements[2].vertex ].point );
      glEnd();
      }
     else
      {
      if( p.tesselation == 0 )
        continue;

      for( int j= 0; j < p.nTessTriangles; ++j )
        {
        MLPolygon& t= polygons[ p.tesselation[j] ];
//printf( "%d %d (%d %d %d)\n",j,p.tesselation[j],
//        t.elements[0].vertex,
//        t.elements[1].vertex,
//        t.elements[2].vertex );
        glBegin(GL_TRIANGLES);
          glPoint( vertices[ t.elements[0].vertex ].point );
          glPoint( vertices[ t.elements[1].vertex ].point );
          glPoint( vertices[ t.elements[2].vertex ].point );
        glEnd();
        }
      }
    }

  glDisable(GL_BLEND);

  if( displayType == ModelDraw::DISPLAY_WIRE_FRONT )
    glDisable(GL_CULL_FACE);
}

/////////////////////////////////////////////////////////////////////////////

void ModelDrawWire::drawNURBSControlNet( MLMesh* mesh )
{
  if( mesh == 0 )
    return;

  FormalNURBS* fnurbs= mesh->getFormalNURBS();
  if( fnurbs == 0 )
    return;

  const MLNURBS_Surface* nurbs= fnurbs->getSurface();
  if( nurbs == 0 )
    return;

  boolean highlighted= (mesh->flags & MLObject::TMP_HIGHLIGHTED) != 0;
  boolean selected= mesh->selected();

  if( highlighted )
    glColor(ModelColors::highlighted);
   else
    {
    if( selected )
      glColor(ModelColors::nurbs_control_net);
     else
      glColor(ModelColors::unselected);
    }

  int i, j;

  glLineStipple( 1, 0x5555 );
  glEnable(GL_LINE_STIPPLE);
  for( i= 0; i <= nurbs->getn(); ++i )
    {
    glBegin(GL_LINE_STRIP);
    for( j= 0; j <= nurbs->getm(); ++j )
      {
      const MLNURBS_ControlPoint& cp= nurbs->getConstControlPoint(i,j);
      glPoint(cp.point);
      }
    glEnd();
    }
  for( j= 0; j <= nurbs->getm(); ++j )
    {
    glBegin(GL_LINE_STRIP);
    for( i= 0; i <= nurbs->getn(); ++i )
      {
      const MLNURBS_ControlPoint& cp= nurbs->getConstControlPoint(i,j);
      glPoint(cp.point);
      }
    glEnd();
    }
  glDisable(GL_LINE_STIPPLE);

  const Matrix4x4& o2w= mesh->global_xform.get_o2w();
  MLCamera* camera= modelDraw->camera;
  Matrix4x4 toViewport= camera->getPixelProjection();
  Matrix4x4 o2vp= toViewport * o2w;

  float mvm[16];
  glGetFloatv( GL_MODELVIEW_MATRIX, mvm );
  float pm[16];
  glGetFloatv( GL_PROJECTION_MATRIX, pm );

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho( 0., GLfloat(camera->x_res), 0., GLfloat(camera->y_res), -1., 1. );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  const double margin= 3.; // control points are margin*2 pixels large

  Color selected_cp, unselected_cp;
  if( highlighted )
    {
    selected_cp=   ModelColors::highlighted;
    unselected_cp= ModelColors::highlighted;
    }
   else
    {
    if( selected )
      {
      selected_cp=   ModelColors::selected_nurbs_control_point;
      unselected_cp= ModelColors::unselected_nurbs_control_point;
      }
     else
      {
      selected_cp=   ModelColors::unselected;
      unselected_cp= ModelColors::unselected;
      }
    }

  int sel= 0;
  mlColor(unselected_cp);

  for( i= 0; i <= nurbs->getn(); ++i )
    {
    for( j= 0; j <= nurbs->getm(); ++j )
      {
      const MLNURBS_ControlPoint& cp= nurbs->getConstControlPoint(i,j);

      int _sel= cp.selected() ? 1 : 0;
      if( _sel != sel )
        {
        sel= _sel;
        mlColor( sel ? selected_cp : unselected_cp );
        }
      Point p= o2vp.apply(cp.point);

      register double x1= p.x()-margin;
      register double y1= p.y()-margin;
      register double x2= p.x()+margin;
      register double y2= p.y()+margin;
      if( x2 < 0. || y2 < 0. ||
          x1 >= double(camera->x_res) || y1 >= double(camera->y_res) )
        continue;

      glBegin(GL_LINE_LOOP);
        glVertex2f(x1,y1);
        glVertex2f(x2,y1);
        glVertex2f(x2,y2);
        glVertex2f(x1,y2);
      glEnd();
      }
    }

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(pm);
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(mvm);
}

/////////////////////////////////////////////////////////////////////////////

void ModelDrawWire::drawNURBSControlNet( MLCurve* curve )
{
  if( curve == 0 )
    return;

  FormalNURBSCurve* fnurbs= curve->getFormalNURBS();
  if( fnurbs == 0 )
    return;

  const MLNURBS_Curve* nurbs= fnurbs->getCurve();
  if( nurbs == 0 )
    return;

  boolean highlighted= (curve->flags & MLObject::TMP_HIGHLIGHTED) != 0;
  boolean selected= curve->selected();

  if( highlighted )
    glColor(ModelColors::highlighted);
   else
    {
    if( selected )
      glColor(ModelColors::nurbs_control_net);
     else
      glColor(ModelColors::unselected);
    }

  int i;

  glLineStipple( 1, 0x5555 );
  glEnable(GL_LINE_STIPPLE);
  glBegin(GL_LINE_STRIP);
  for( i= 0; i <= nurbs->getn(); ++i )
    {
    const MLNURBS_ControlPoint& cp= nurbs->getConstControlPoint(i);
    glPoint(cp.point);
    }
  glEnd();
  glDisable(GL_LINE_STIPPLE);

  const Matrix4x4& o2w= curve->global_xform.get_o2w();
  MLCamera* camera= modelDraw->camera;
  Matrix4x4 toViewport= camera->getPixelProjection();
  Matrix4x4 o2vp= toViewport * o2w;

  float mvm[16];
  glGetFloatv( GL_MODELVIEW_MATRIX, mvm );
  float pm[16];
  glGetFloatv( GL_PROJECTION_MATRIX, pm );

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho( 0., GLfloat(camera->x_res), 0., GLfloat(camera->y_res), -1., 1. );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  const double margin= 3.; // control points are margin*2 pixels large

  Color selected_cp, unselected_cp;
  if( highlighted )
    {
    selected_cp=   ModelColors::highlighted;
    unselected_cp= ModelColors::highlighted;
    }
   else
    {
    if( selected )
      {
      selected_cp=   ModelColors::selected_nurbs_control_point;
      unselected_cp= ModelColors::unselected_nurbs_control_point;
      }
     else
      {
      selected_cp=   ModelColors::unselected;
      unselected_cp= ModelColors::unselected;
      }
    }

  int sel= 0;
  mlColor(unselected_cp);

  for( i= 0; i <= nurbs->getn(); ++i )
    {
    const MLNURBS_ControlPoint& cp= nurbs->getConstControlPoint(i);

    int _sel= cp.selected() ? 1 : 0;
    if( _sel != sel )
      {
      sel= _sel;
      mlColor( sel ? selected_cp : unselected_cp );
      }
    Point p= o2vp.apply(cp.point);

    register double x1= p.x()-margin;
    register double y1= p.y()-margin;
    register double x2= p.x()+margin;
    register double y2= p.y()+margin;
    if( x2 < 0. || y2 < 0. ||
        x1 >= double(camera->x_res) || y1 >= double(camera->y_res) )
      continue;

    glBegin(GL_LINE_LOOP);
      glVertex2f(x1,y1);
      glVertex2f(x2,y1);
      glVertex2f(x2,y2);
      glVertex2f(x1,y2);
    glEnd();
    }

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(pm);
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(mvm);
}

/////////////////////////////////////////////////////////////////////////////
