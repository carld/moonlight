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
  ModelScene.C

  Stephane Rehel

  December 15 1996
*/

#include <math.h>
#include <string.h>

#include "ModelScene.h"

#include "tools/IntegerArray.h"
#include "tools/Transform.h"

#include "scene/MLScene.h"
#include "scene/MLObject.h"
#include "scene/MeshRep.h"
#include "scene/CurveRep.h"
#include "scene/MLCurveVertex.h"
#include "scene/MLMesh.h"
#include "scene/MLCurve.h"

#include "scene/formal/FormalNURBS.h"
#include "scene/formal/FormalNURBSCurve.h"
#include "scene/nurbs/MLNURBS_Surface.h"
#include "scene/nurbs/MLNURBS_Curve.h"

#include "scene/lighting/MLLight.h"
#include "scene/lighting/MLGlobalLighting.h"

#include "scene/MLChildrenIterator.h"
#include "scene/MLObjectsIterator.h"
#include "scene/MLMeshesIterator.h"

#include "scene/renderer/MLRendererOptions.h"

#include "ModelCommand.h"

#include "ModelModule.h"

/////////////////////////////////////////////////////////////////////////////

ModelScene::ModelScene( ModelModule* _model, MLScene* _scene )
{
  model= _model;
  scene= _scene;
}

/////////////////////////////////////////////////////////////////////////////

ModelScene::~ModelScene()
{
  scene= 0;
}

/////////////////////////////////////////////////////////////////////////////

// Check for boundings/hashboxes to update
void ModelScene::updateBoundingHashBox()
{
  scene->updateBoundingHashBox();
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::translate( ModelCommand* cmd )
{
  Vector v= cmd->v;
  if( v.norm() < 1e-20 )
    return;

  _translate(scene->getRootObject(),v,false);

/*
  IntegerArray& objects= cmd->indices;
  int nObjects= objects.getSize();

  if( nObjects == 0 )
    return;

  for( int i= 0; i < nObjects; ++i )
    {
    MLObject* o= scene->getObject(objects[i]);

    if( o == 0 || o == scene->getRootObject() )
      {
      objects[i]= -1;
      continue; // never mind
      }

    Vector local_v= ( o->local_xform.preXForm.get_w2o()
                     *o->father->global_xform.get_w2o())
                   .apply(v);
    o->translate(o->getTranslate()+local_v);

    scene->getGlobalLighting()->object_is_transformed(o);
    }
*/

  scene->updateTransformations();
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::_translate( MLObject* o, const Vector& v, boolean done )
{
  if( o == 0 )
    return;

/*
  if( o->father != 0 )
    {
    if( o->selected() )
      {
      if( ! done )
        {
        Vector local_v= ( o->local_xform.preXForm.get_w2o()
                         *o->father->global_xform.get_w2o())
                       .apply(v);
        o->translate(o->getTranslate()+local_v);
        done= true;
        }
      }
     else
      {
      if( done )
        {
        Vector local_v= ( o->local_xform.preXForm.get_w2o()
                         *o->father->global_xform.get_w2o())
                        .apply(v);
        o->translate(o->getTranslate()-local_v);
        done= false;
        }
      }
    }
*/

  if( o->selected() && !done )
    {
    Vector local_v= ( o->local_xform.preXForm.get_w2o()
                     *o->father->global_xform.get_w2o())
                   .apply(v);

    // update shadow before
    if( o->getObjectType() == MLObject::MESH )
      scene->getGlobalLighting()->mesh_shadow_has_changed((MLMesh*)o);

    o->translate(o->getTranslate()+local_v);

    done= true;
    }

  // update shadow after
  if( done )
    scene->getGlobalLighting()->object_is_transformed(o);

  MLChildrenIterator oi(o);
  while( ! oi.eol() )
    _translate( oi++, v, done );
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::translateVertices( ModelCommand* cmd )
{
  Vector v= cmd->v;
  if( v.norm() < 1e-20 )
    return;

  IntegerArray& objects= cmd->indices;
  int nObjects= objects.getSize();

  if( nObjects == 0 )
    return;

  for( int k= 0; k < nObjects; ++k )
    {
    MLObject* o= scene->getObject(objects[k]);

    if( o == 0 || o == scene->getRootObject() )
      {
      objects[k]= -1;
      continue; // never mind
      }

    if( o->getObjectType() == MLObject::MESH )
      {
      MLMesh* mesh= (MLMesh*) o;

      MeshRep* meshrep= mesh->getMeshRep();
      if( meshrep == 0 )
        continue;

      Vector local_v= o->global_xform.w2o(v);

      if( mesh->isNURBS() )
        {
        boolean found= false;

        FormalNURBS* nurbs= mesh->getFormalNURBS();
        MLNURBS_Surface* surface= nurbs->getSurface();
        if( surface == 0 )
          continue;

        for( int i= 0; i <= surface->getn(); ++i )
          {
          for( int j= 0; j <= surface->getm(); ++j )
             {
             MLNURBS_ControlPoint& cp= surface->getControlPoint(i,j);
             if( ! cp.selected() )
               continue;

             cp.setPoint( cp.point + local_v );
             found= true;
             }
          }

        if( found )
          {
          nurbs->regenerateVertices(meshrep);

          meshrep->invalidateBoundings();

          scene->getGlobalLighting()->object_is_transformed(o);
          }

        continue;
        }

      if( mesh->isFormal() )
        continue;

      boolean found= false;
      boolean invalidateBoundings= false;
      Box internal_bbox= Box(Point(0,0,0),Point(1,1,1));
      if( meshrep->dirtyBoundings )
        invalidateBoundings= true;
       else
        {
        Box& b= meshrep->bbox;
        Vector size= b.getDiag() * 0.05;
        internal_bbox= Box( b.getMin() + size, b.getMax() - size );
        }

      MLVertexArray& vertices= meshrep->vertices;
      int j;
      for( j= 1; j <= vertices.getSize(); ++j )
        {
        MLVertex& vtx= vertices[j];
        if( !vtx.allocated() || !vtx.selected() )
          continue;

        if( ! invalidateBoundings )
          if( ! internal_bbox.into(vtx.point) )
            invalidateBoundings= true;

        vtx.point+= local_v;

        if( ! invalidateBoundings )
          if( ! internal_bbox.into(vtx.point) )
            invalidateBoundings= true;

        found= true;
        }

      if( found )
        {
        if( invalidateBoundings )
          meshrep->invalidateBoundings();
         else
          meshrep->invalidateHashBox();

        scene->getGlobalLighting()->object_is_transformed(o);

        finalizeVerticesTransformation(meshrep);
        }
      }

    if( o->getObjectType() == MLObject::CURVE )
      {
      MLCurve* curve= (MLCurve*) o;

      CurveRep* curverep= curve->getCurveRep();
      if( curverep == 0 )
        continue;

      Vector local_v= o->global_xform.w2o(v);

      if( curve->isNURBS() )
        {
        boolean found= false;

        FormalNURBSCurve* nurbs= curve->getFormalNURBS();
        MLNURBS_Curve* ncurve= nurbs->getCurve();
        if( ncurve == 0 )
          continue;

        for( int i= 0; i <= ncurve->getn(); ++i )
          {
          MLNURBS_ControlPoint& cp= ncurve->getControlPoint(i);
          if( ! cp.selected() )
            continue;

          cp.setPoint( cp.point + local_v );
          found= true;
          }

        if( found )
          {
          nurbs->regenerateVertices(curverep);

//          curverep->invalidateBoundings();
          }
        continue;
        }

      if( curve->isFormal() )
        continue;

      boolean found= false;
//      boolean invalidateBoundings= false;
//      Box internal_bbox= Box(Point(0,0,0),Point(1,1,1));
//      if( meshrep->dirtyBoundings )
//        invalidateBoundings= true;
//       else
//        {
//        Box& b= meshrep->bbox;
//        Vector size= b.getDiag() * 0.05;
//        internal_bbox= Box( b.getMin() + size, b.getMax() - size );
//        }

      MLCurveVertex* vertices= curverep->vertices;
      int j;
      for( j= 1; j <= curverep->nVertices; ++j )
        {
        MLCurveVertex& vtx= vertices[j];
        if( !vtx.allocated() || !vtx.selected() )
          continue;
        vtx.point+= local_v;
        found= true;
        }

      if( found )
        {
//        if( invalidateBoundings )
//          meshrep->invalidateBoundings();
//         else
//          meshrep->invalidateHashBox();

//        scene->getGlobalLighting()->object_is_transformed(o);

//        finalizeVerticesTransformation(meshrep);
        }
      }

    } // end for(objects)
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::finalizeVerticesTransformation( MeshRep* meshrep )
{
  if( meshrep == 0 )
    return;

  MLVertexArray& vertices= meshrep->vertices;
  MLSVertexArray& svertices= meshrep->svertices;
  MLPolygonArray& polygons= meshrep->polygons;

  for( int j= 1; j <= vertices.getSize(); ++j )
    {
    MLVertex& vtx= vertices[j];
    if( !vtx.allocated() || !vtx.selected() )
      continue;

    int k;
    for( k= 0; k < vtx.nPolygons; ++k )
      {
      int pi= vtx.polygons[k];
      if( pi == 0 )
        continue;

      MLPolygon& p= polygons[ pi ];
      if( ! p.allocated() )
        continue;
      p.updateNormalArea( meshrep->vertices.getArray(),
                          meshrep->polygons.getArray(),
                          true); // recursively

      for( int l= 0; l < p.nVertices; ++l )
        {
        int svi= p.elements[l].svertex;
        if( svi == 0 )
          continue;
        MLSVertex& sv= svertices[svi];

        // normal of this vertex is to be updated
        sv.flags |= MLSVertex::TMP_FLAG;
        }
      }

    for( k= 0; k < vtx.nPolygons; ++k )
      {
      int pi= vtx.polygons[k];
      if( pi == 0 )
        continue;
      MLPolygon& p= polygons[ pi ];
      if( ! p.allocated() )
        continue;

      for( int l= 0; l < p.nVertices; ++l )
        {
        int svi= p.elements[l].svertex;
        if( svi == 0 )
          continue;
        MLSVertex& sv= svertices[svi];
        if( (sv.flags & MLSVertex::TMP_FLAG) == 0 )
          continue;

        sv.flags &= ~MLSVertex::TMP_FLAG;

        sv.updateNormal( meshrep->vertices.getArray(),
                         meshrep->polygons.getArray() );
        }
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::translatePolygons( ModelCommand* cmd )
{
  Vector v= cmd->v;
  if( v.norm() < 1e-20 )
    return;

  IntegerArray& objects= cmd->indices;
  int nObjects= objects.getSize();

  if( nObjects == 0 )
    return;

  for( int i= 0; i < nObjects; ++i )
    {
    MLObject* o= scene->getObject(objects[i]);

    if( o == 0 || o == scene->getRootObject() )
      {
      objects[i]= -1;
      continue; // never mind
      }

    if( o->getObjectType() != MLObject::MESH )
      continue;
    MLMesh* mesh= (MLMesh*) o;

    if( mesh->isFormal() )
      continue;

    MeshRep* meshrep= mesh->getMeshRep();
    if( meshrep == 0 )
      continue;

    Vector local_v= o->global_xform.w2o(v);

    boolean found= false;
    boolean invalidateBoundings= false;
    Box internal_bbox= Box(Point(0,0,0),Point(1,1,1));
    if( meshrep->dirtyBoundings )
      invalidateBoundings= true;
     else
      {
      Box& b= meshrep->bbox;
      Vector size= b.getDiag() * 0.05;
      internal_bbox= Box( b.getMin() + size, b.getMax() - size );
      }

    MLPolygonArray& polygons= meshrep->polygons;
    MLVertexArray& vertices= meshrep->vertices;
    int j;
    for( j= 1; j <= polygons.getSize(); ++j )
      {
      MLPolygon& pol= polygons[j];
      if( !pol.allocated() || !pol.selected() )
        continue;

      for( int k= 0; k < pol.nVertices; ++k )
        {
        MLVertex& vtx= vertices[ pol.elements[k].vertex ];
        if( !vtx.allocated() || (vtx.flags & MLVertex::TMP_FLAG) != 0 )
          continue;

        if( ! invalidateBoundings )
          if( ! internal_bbox.into(vtx.point) )
            invalidateBoundings= true;

        vtx.point+= local_v;
        vtx.flags |= MLVertex::TMP_FLAG;

        if( ! invalidateBoundings )
          if( ! internal_bbox.into(vtx.point) )
            invalidateBoundings= true;

        found= true;
        }
      }

    if( found )
      {
      if( invalidateBoundings )
        meshrep->invalidateBoundings();
       else
        meshrep->invalidateHashBox();

      for( int k= 1; k <= vertices.getSize(); ++k )
        {
        MLVertex& vtx= vertices[k];
        vtx.flags &= ~MLVertex::TMP_FLAG;
        }

      scene->getGlobalLighting()->object_is_transformed(o);

      finalizePolygonsTransformation(meshrep);
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::finalizePolygonsTransformation( MeshRep* meshrep )
{
  if( meshrep == 0 )
    return;

  MLSVertexArray& svertices= meshrep->svertices;
  MLVertexArray& vertices= meshrep->vertices;
  MLPolygonArray& polygons= meshrep->polygons;

  int j;
  for( j= 1; j <= polygons.getSize(); ++j )
    {
    MLPolygon& pol= polygons[j];
    if( !pol.allocated() || !pol.selected() )
      continue;

    for( int k= 0; k < pol.nVertices; ++k )
      {
      int vi= pol.elements[k].vertex;
      if( vi == 0 )
        continue;
      MLVertex& v= vertices[vi];

      // normal of this vertex is to be updated
      v.flags |= MLVertex::TMP_FLAG;
      }
    }

  for( j= 1; j <= vertices.getSize(); ++j )
    {
    MLVertex& vtx= vertices[j];
    if( !vtx.allocated() || (vtx.flags & MLVertex::TMP_FLAG) == 0 )
      continue;

    vtx.flags &= ~MLVertex::TMP_FLAG;

    int k;
    for( k= 0; k < vtx.nPolygons; ++k )
      {
      int pi= vtx.polygons[k];
      if( pi == 0 )
        continue;

      MLPolygon& p= polygons[ pi ];
      if( ! p.allocated() )
        continue;
      p.updateNormalArea( meshrep->vertices.getArray(),
                          meshrep->polygons.getArray(),
                          true ); // recursively
      for( int l= 0; l < p.nVertices; ++l )
        {
        int svi= p.elements[l].svertex;
        if( svi == 0 )
          continue;
        MLSVertex& sv= svertices[svi];

        // normal of this vertex is to be updated
        sv.flags |= MLSVertex::TMP_FLAG;
        }
      }

    for( k= 0; k < vtx.nPolygons; ++k )
      {
      int pi= vtx.polygons[k];
      if( pi == 0 )
        continue;
      MLPolygon& p= polygons[ pi ];
      if( ! p.allocated() )
        continue;

      for( int l= 0; l < p.nVertices; ++l )
        {
        int svi= p.elements[l].svertex;
        if( svi == 0 )
          continue;
        MLSVertex& sv= svertices[svi];
        if( (sv.flags & MLSVertex::TMP_FLAG) == 0 )
          continue;

        sv.flags &= ~MLSVertex::TMP_FLAG;

        sv.updateNormal(  meshrep->vertices.getArray(),
                          meshrep->polygons.getArray() );
        }
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean ModelScene::scale( const Vector& s )
{
  if( (s - Vector(1,1,1)).norm() < 1e-20 )
    return false;

  _scale(scene->getRootObject(),s,false);
  scene->updateTransformations();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::_scale( MLObject* o, const Vector& s, boolean done )
{
  if( o == 0 )
    return;

  if( o->selected() && !done )
    {
    // update shadow before
    if( o->getObjectType() == MLObject::MESH )
      scene->getGlobalLighting()->mesh_shadow_has_changed((MLMesh*)o);

    o->scale( o->getScale() & s );

    done= true;
    }

  // update shadow after
  if( done )
    scene->getGlobalLighting()->object_is_transformed(o);

  MLChildrenIterator oi(o);
  while( ! oi.eol() )
    _scale( oi++, s, done );
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::scaleVertices( ModelCommand* cmd )
{
  Vector s= cmd->v;
  if( (s - Vector(1,1,1)).norm() < 1e-20 )
    return;

  IntegerArray& objects= cmd->indices;
  int nObjects= objects.getSize();

  if( nObjects == 0 )
    return;

  for( int k= 0; k < nObjects; ++k )
    {
    MLObject* o= scene->getObject(objects[k]);

    if( o == 0 || o == scene->getRootObject() )
      {
      objects[k]= -1;
      continue; // never mind
      }

    if( o->getObjectType() != MLObject::MESH )
      continue;
    MLMesh* mesh= (MLMesh*) o;

    MeshRep* meshrep= mesh->getMeshRep();
    if( meshrep == 0 )
      continue;

    if( mesh->isNURBS() )
      {
      boolean found= false;

      FormalNURBS* nurbs= mesh->getFormalNURBS();
      MLNURBS_Surface* surface= nurbs->getSurface();
      if( surface == 0 )
        continue;

      for( int i= 0; i <= surface->getn(); ++i )
        {
        for( int j= 0; j <= surface->getm(); ++j )
           {
           MLNURBS_ControlPoint& cp= surface->getControlPoint(i,j);
           if( ! cp.selected() )
             continue;

           cp.setPoint( Point( cp.point.x() * s.x(),
                               cp.point.y() * s.y(),
                               cp.point.z() * s.z() ) );
           found= true;
           }
        }

      if( found )
        {
        nurbs->regenerateVertices(meshrep);

        meshrep->invalidateBoundings();

        scene->getGlobalLighting()->object_is_transformed(o);
        }

      continue;
      }

    if( mesh->isFormal() )
      continue;

    boolean found= false;
    boolean invalidateBoundings= false;
    Box internal_bbox= Box(Point(0,0,0),Point(1,1,1));
    if( meshrep->dirtyBoundings )
      invalidateBoundings= true;
     else
      {
      Box& b= meshrep->bbox;
      Vector size= b.getDiag() * 0.05;
      internal_bbox= Box( b.getMin() + size, b.getMax() - size );
      }

    MLVertexArray& vertices= meshrep->vertices;
    int j;
    for( j= 1; j <= vertices.getSize(); ++j )
      {
      MLVertex& vtx= vertices[j];
      if( !vtx.allocated() || !vtx.selected() )
        continue;

      if( ! invalidateBoundings )
        if( ! internal_bbox.into(vtx.point) )
          invalidateBoundings= true;

      vtx.point[0] *= s.x();
      vtx.point[1] *= s.y();
      vtx.point[2] *= s.z();

      if( ! invalidateBoundings )
        if( ! internal_bbox.into(vtx.point) )
          invalidateBoundings= true;

      found= true;
      }

    if( found )
      {
      if( invalidateBoundings )
        meshrep->invalidateBoundings();
       else
        meshrep->invalidateHashBox();

      scene->getGlobalLighting()->object_is_transformed(o);

      finalizeVerticesTransformation(meshrep);
      }

    }
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::scalePolygons( ModelCommand* cmd )
{
  Vector s= cmd->v;
  if( (s - Vector(1,1,1)).norm() < 1e-20 )
    return;

  IntegerArray& objects= cmd->indices;
  int nObjects= objects.getSize();

  if( nObjects == 0 )
    return;

  for( int i= 0; i < nObjects; ++i )
    {
    MLObject* o= scene->getObject(objects[i]);

    if( o == 0 || o == scene->getRootObject() )
      {
      objects[i]= -1;
      continue; // never mind
      }

    if( o->getObjectType() != MLObject::MESH )
      continue;
    MLMesh* mesh= (MLMesh*) o;

    if( mesh->isFormal() )
      continue;

    MeshRep* meshrep= mesh->getMeshRep();
    if( meshrep == 0 )
      continue;

    boolean found= false;
    boolean invalidateBoundings= false;
    Box internal_bbox= Box(Point(0,0,0),Point(1,1,1));
    if( meshrep->dirtyBoundings )
      invalidateBoundings= true;
     else
      {
      Box& b= meshrep->bbox;
      Vector size= b.getDiag() * 0.05;
      internal_bbox= Box( b.getMin() + size, b.getMax() - size );
      }

    MLPolygonArray& polygons= meshrep->polygons;
    MLVertexArray& vertices= meshrep->vertices;
    int j;
    for( j= 1; j <= polygons.getSize(); ++j )
      {
      MLPolygon& pol= polygons[j];
      if( !pol.allocated() || !pol.selected() )
        continue;

//      Point center= pol.getCenter();

      for( int k= 0; k < pol.nVertices; ++k )
        {
        MLVertex& vtx= vertices[ pol.elements[k].vertex ];
        if( !vtx.allocated() || (vtx.flags & MLVertex::TMP_FLAG) != 0 )
          continue;

        if( ! invalidateBoundings )
          if( ! internal_bbox.into(vtx.point) )
            invalidateBoundings= true;

        vtx.point[0] *= s.x();
        vtx.point[1] *= s.y();
        vtx.point[2] *= s.z();
        vtx.flags |= MLVertex::TMP_FLAG;

        if( ! invalidateBoundings )
          if( ! internal_bbox.into(vtx.point) )
            invalidateBoundings= true;

        found= true;
        }
      }

    if( found )
      {
      if( invalidateBoundings )
        meshrep->invalidateBoundings();
       else
        meshrep->invalidateHashBox();

      for( int k= 1; k <= vertices.getSize(); ++k )
        {
        MLVertex& vtx= vertices[k];
        vtx.flags &= ~MLVertex::TMP_FLAG;
        }

      scene->getGlobalLighting()->object_is_transformed(o);

      finalizePolygonsTransformation(meshrep);
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean ModelScene::rotate( const Vector& a )
{
  if( a.norm() < 1e-20 )
    return false;

  _rotate(scene->getRootObject(),a,false);
  scene->updateTransformations();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::_rotate( MLObject* o, const Vector& a, boolean done )
{
  if( o == 0 )
    return;

  if( o->selected() && !done )
    {
    // update shadow before
    if( o->getObjectType() == MLObject::MESH )
      scene->getGlobalLighting()->mesh_shadow_has_changed((MLMesh*)o);

/*
    Matrix4x4 rx, ry, rz;

    rx.setRotationX(o->getRotate().x());
    ry.setRotationY(o->getRotate().y());
    rz.setRotationZ(o->getRotate().z());
    Matrix4x4 r1= rz * ry * rx;

    rx.setRotationX(a.x());
    ry.setRotationY(a.y());
    rz.setRotationZ(a.z());
    Matrix4x4 r2= rz * ry * rx;

    Matrix4x4 r3 = r2 * r1;
    Vector ts, tr, tv;
    r3.getTransformation( ts, tr, tv );
    o->rotate(tr);
*/
    o->rotate( o->getRotate() + a );
    done= true;
    }

  // update shadow after
  if( done )
    scene->getGlobalLighting()->object_is_transformed(o);

  MLChildrenIterator oi(o);
  while( ! oi.eol() )
    _rotate( oi++, a, done );
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::rotateVertices( ModelCommand* cmd )
{
  Vector a= cmd->v;
  if( a.norm() < 1e-20 )
    return;

  IntegerArray& objects= cmd->indices;
  int nObjects= objects.getSize();

  if( nObjects == 0 )
    return;

  Matrix4x4 rotationMatrix;
  rotationMatrix.setTransformation( Vector(1,1,1), // scale
                                    a, // rotate
                                    Vector(0,0,0) // translate
                                  );

  for( int k= 0; k < nObjects; ++k )
    {
    MLObject* o= scene->getObject(objects[k]);

    if( o == 0 || o == scene->getRootObject() )
      {
      objects[k]= -1;
      continue; // never mind
      }

    if( o->getObjectType() != MLObject::MESH )
      continue;
    MLMesh* mesh= (MLMesh*) o;

    MeshRep* meshrep= mesh->getMeshRep();
    if( meshrep == 0 )
      continue;

    if( mesh->isNURBS() )
      {
      boolean found= false;

      FormalNURBS* nurbs= mesh->getFormalNURBS();
      MLNURBS_Surface* surface= nurbs->getSurface();
      if( surface == 0 )
        continue;

      for( int i= 0; i <= surface->getn(); ++i )
        {
        for( int j= 0; j <= surface->getm(); ++j )
           {
           MLNURBS_ControlPoint& cp= surface->getControlPoint(i,j);
           if( ! cp.selected() )
             continue;

           cp.setPoint( rotationMatrix.apply(cp.point) );

           found= true;
           }
        }

      if( found )
        {
        nurbs->regenerateVertices(meshrep);

        meshrep->invalidateBoundings();

        scene->getGlobalLighting()->object_is_transformed(o);
        }

      continue;
      }

    if( mesh->isFormal() )
      continue;

    boolean found= false;
    boolean invalidateBoundings= false;
    Box internal_bbox= Box(Point(0,0,0),Point(1,1,1));
    if( meshrep->dirtyBoundings )
      invalidateBoundings= true;
     else
      {
      Box& b= meshrep->bbox;
      Vector size= b.getDiag() * 0.05;
      internal_bbox= Box( b.getMin() + size, b.getMax() - size );
      }

    MLVertexArray& vertices= meshrep->vertices;
    int j;
    for( j= 1; j <= vertices.getSize(); ++j )
      {
      MLVertex& vtx= vertices[j];
      if( !vtx.allocated() || !vtx.selected() )
        continue;

      if( ! invalidateBoundings )
        if( ! internal_bbox.into(vtx.point) )
          invalidateBoundings= true;

      vtx.point= rotationMatrix.apply(vtx.point);

      if( ! invalidateBoundings )
        if( ! internal_bbox.into(vtx.point) )
          invalidateBoundings= true;

      found= true;
      }

    if( found )
      {
      if( invalidateBoundings )
        meshrep->invalidateBoundings();
       else
        meshrep->invalidateHashBox();

      scene->getGlobalLighting()->object_is_transformed(o);

      finalizeVerticesTransformation(meshrep);
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean ModelScene::world_rotate( const Vector& axis, double angle )
{
  if( axis.norm() < 1e-20 || fabs(angle) < 1e-30 )
    return false;

  _world_rotate(scene->getRootObject(),axis,angle,false);

  scene->updateTransformations();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::_world_rotate( MLObject* o,
                                const Vector& axis, double angle,
                                boolean done )
{
  if( o == 0 )
    return;

  if( o->selected() && !done )
    {
    // update shadow before
    if( o->getObjectType() == MLObject::MESH )
      scene->getGlobalLighting()->mesh_shadow_has_changed((MLMesh*)o);

    Vector o_axis= o->getGlobalXForm().w2o(axis);
    o_axis.setNormalized();

    Matrix4x4 r1;
    r1.setRotation( axis, angle );
    Vector t_s, t_r, t_t;
    r1.getTransformation( t_s, t_r, t_t );
    o->rotate( o->getRotate() + t_r );

//    Matrix4x4 rx, ry, rz;
//
//    rx.setRotationX(o->getRotate().x());
//    ry.setRotationY(o->getRotate().y());
//    rz.setRotationZ(o->getRotate().z());
//    Matrix4x4 r1= rz * ry * rx;
//
//    Matrix4x4 r2;
//    r2.setRotation( axis, angle );
//    Matrix4x4 r3 = r2 * r1;
//    Vector ts, tr, tv;
//r1.getTransformation( ts, tr, tv );
//printf( "x: %f (%f)\n",tr.x(),o->getRotate().x());
//printf( "y: %f (%f)\n",tr.y(),o->getRotate().y());
//printf( "z: %f (%f)\n",tr.z(),o->getRotate().z());
//
//    r3.getTransformation( ts, tr, tv );
//printf( "tr : %f %f %f\n",tr.x(),tr.y(),tr.z());
//
//    o->rotate(-tr);

    done= true;
    }

  // update shadow after
  if( done )
    scene->getGlobalLighting()->object_is_transformed(o);

  MLChildrenIterator oi(o);
  while( ! oi.eol() )
    _world_rotate( oi++, axis, angle, done );
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::world_rotateVertices( ModelCommand* cmd,
                                       const Vector& axis, double angle )
{
  if( fabs(angle) < 1e-20 || axis.norm() < 1e-20 )
    return;

  IntegerArray& objects= cmd->indices;
  int nObjects= objects.getSize();

  if( nObjects == 0 )
    return;

  for( int i= 0; i < nObjects; ++i )
    {
    MLObject* o= scene->getObject(objects[i]);

    if( o == 0 || o == scene->getRootObject() )
      {
      objects[i]= -1;
      continue; // never mind
      }

    if( o->getObjectType() != MLObject::MESH )
      continue;
    MLMesh* mesh= (MLMesh*) o;
    MeshRep* meshrep= mesh->getMeshRep();
    if( meshrep == 0 )
      continue;

    Vector o_axis= o->getGlobalXForm().w2o(axis);
    o_axis.setNormalized();
//    Matrix4x4 r1;
//    r1.setRotation( axis, angle );
//    Vector t_s, t_r, t_t;
//    r1.getTransformation( t_s, t_r, t_t );
    Matrix4x4 rotationMatrix;
    rotationMatrix.setRotation( o_axis, angle );

    boolean found= false;
    boolean invalidateBoundings= false;
    Box internal_bbox= Box(Point(0,0,0),Point(1,1,1));
    if( meshrep->dirtyBoundings )
      invalidateBoundings= true;
     else
      {
      Box& b= meshrep->bbox;
      Vector size= b.getDiag() * 0.05;
      internal_bbox= Box( b.getMin() + size, b.getMax() - size );
      }

    MLVertexArray& vertices= meshrep->vertices;
    int j;
    for( j= 1; j <= vertices.getSize(); ++j )
      {
      MLVertex& vtx= vertices[j];
      if( !vtx.allocated() || !vtx.selected() )
        continue;

      if( ! invalidateBoundings )
        if( ! internal_bbox.into(vtx.point) )
          invalidateBoundings= true;

      vtx.point= rotationMatrix.apply(vtx.point);

      if( ! invalidateBoundings )
        if( ! internal_bbox.into(vtx.point) )
          invalidateBoundings= true;

      found= true;
      }

    if( found )
      {
      if( invalidateBoundings )
        meshrep->invalidateBoundings();
       else
        meshrep->invalidateHashBox();

      scene->getGlobalLighting()->object_is_transformed(o);
      }

    finalizeVerticesTransformation(meshrep);
    }
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::selectAll( IToggle toggle )
{
  MLObjectsIterator oi(scene);
  switch(toggle)
    {
    case ISET:
      while( ! oi.eol() )
        (oi++)->flags |= MLObject::SELECTED;
      break;

    case IUNSET:
      while( ! oi.eol() )
        (oi++)->flags &= ~MLObject::SELECTED;
      break;

    case ITOGGLE:
      while( ! oi.eol() )
        (oi++)->flags ^= MLObject::SELECTED;
      break;
    }
}

/////////////////////////////////////////////////////////////////////////////

// return 0 if none selected or more than a single one
int ModelScene::getSingleSelected()
{
  MLObjectsIterator oi(scene);

  int id= -1;
  while( ! oi.eol() )
    {
    MLObject* o= oi++;
    if( o == 0 )
      continue;
    if( o->getFather() == 0 )
      continue; // this is roto object of scene
    if( o->selected() )
      {
      if( id != -1 )
        return 0; // more than one single object selected
      id= o->getID();
      }
    }

  return id;
}

/////////////////////////////////////////////////////////////////////////////

// return nSelected
int ModelScene::getSelectedObjects( ModelCommand* cmd )
{
  int n= 0;

  MLObjectsIterator oi(scene);

  while( ! oi.eol() )
    if( (oi++)->selected() )
      ++n;

  if( n == 0 )
    {
    cmd->indices.clear();
    return 0;
    }

  cmd->indices.alloc(n);

  int i= 0;
  oi.reset();
  while( ! oi.eol() )
    {
    MLObject* o= oi++;
    if( o->selected() )
      cmd->indices[i++]= o->getID();
    }

  return n;
}

/////////////////////////////////////////////////////////////////////////////

// return nObjects
int ModelScene::getAllObjects( ModelCommand* cmd )
{
  int n= 0;

  MLObjectsIterator oi(scene);

  while( ! oi.eol() )
    {
    MLObject* o= oi++;
    if( o == 0 || o == scene->getRootObject() )
      continue;
    ++n;
    }

  if( n == 0 )
    {
    cmd->indices.clear();
    return 0;
    }

  cmd->indices.alloc(n);

  int i= 0;
  oi.reset();
  while( ! oi.eol() )
    {
    MLObject* o= oi++;
    if( o == 0 || o == scene->getRootObject() )
      continue;
    cmd->indices[i++]= o->getID();
    }

  return n;
}

/////////////////////////////////////////////////////////////////////////////

// return nLights
int ModelScene::getAllLights( ModelCommand* cmd )
{
  int n= 0;

  MLObjectsIterator oi(scene);

  while( ! oi.eol() )
    {
    MLObject* o= oi++;
    if( o == 0 || o == scene->getRootObject() )
      continue;

    if( o->getObjectType() != MLObject::LIGHT )
      continue;

    ++n;
    }

  if( n == 0 )
    {
    cmd->indices.clear();
    return 0;
    }

  cmd->indices.alloc(n);

  int i= 0;
  oi.reset();
  while( ! oi.eol() )
    {
    MLObject* o= oi++;
    if( o == 0 || o == scene->getRootObject() )
      continue;

    if( o->getObjectType() != MLObject::LIGHT )
      continue;

    cmd->indices[i++]= o->getID();
    }

  return n;
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::selectObjects( ModelCommand* cmd, IToggle toggle )
{
  IntegerArray& objects= cmd->indices;
  int nObjects= objects.getSize();

  if( nObjects == 0 )
    return;

  for( int i= 0; i < nObjects; ++i )
    {
    int id= objects[i];
    MLObject* o= scene->getObject(id);
    if( o == 0 )
      continue;

    switch(toggle)
      {
      case ISET:    o->flags |= MLObject::SELECTED; break;
      case IUNSET:  o->flags &=~MLObject::SELECTED; break;
      case ITOGGLE: o->flags ^= MLObject::SELECTED; break;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

// we reparent this child so that its global transformation doesn't change
boolean ModelScene::reparent( MLObject* child, MLObject* new_parent )
{
  if( child == 0 || new_parent == 0 || child == new_parent )
    return false;
  if( child == scene->getRootObject() )
    return false;

  child->setFather(new_parent);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::duplicateObjects( ModelCommand* cmd )
{
  IntegerArray& objects= cmd->indices;
  int nObjects= objects.getSize();

  if( nObjects == 0 )
    return;

  for( int i= 0; i < nObjects; ++i )
    {
    MLObject* o= scene->getObject(objects[i]);
    if( o == 0 || o == scene->getRootObject() )
      {
      objects[i]= -1;
      continue; // nerver mind
      }

    MLObject* duplicated= o->duplicate();

    if( duplicated == 0 )
      continue;

    scene->getGlobalLighting()->object_is_transformed(duplicated);

    duplicated->flags |= MLObject::SELECTED;

    o->flags &= ~MLObject::SELECTED;

    duplicated->updateHierBBox();
    }
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::instantiateObjects( ModelCommand* cmd )
{
  IntegerArray& objects= cmd->indices;
  int nObjects= objects.getSize();

  if( nObjects == 0 )
    return;

  for( int i= 0; i < nObjects; ++i )
    {
    MLObject* o= scene->getObject(objects[i]);
    if( o == 0 || o == scene->getRootObject() )
      {
      objects[i]= -1;
      continue; // nerver mind
      }

    MLObject* instantiated= 0;
    switch( o->getObjectType() )
      {
      case MLObject::FIRST_OBJECT_TYPE: //?
      case MLObject::LAST_OBJECT_TYPE: //?
        break;
      case MLObject::MESH:
        {
        MLMesh* mesh= (MLMesh*) o;
        instantiated= new MLMesh( mesh->getFather(), mesh->getAbstract() );
        mesh->copy( (MLMesh*) instantiated );
        break;
        }
      default:
        {
        instantiated= o->duplicate();
        break;
        }
      }

    if( instantiated == 0 )
      continue;

    scene->getGlobalLighting()->object_is_transformed(instantiated);

    instantiated->flags |= MLObject::SELECTED;

    o->flags &= ~MLObject::SELECTED;

    instantiated->updateHierBBox();
    }
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::deleteObjects( ModelCommand* cmd )
{
  IntegerArray& objects= cmd->indices;
  int nObjects= objects.getSize();

  if( nObjects == 0 )
    return;

  for( int i= 0; i < nObjects; ++i )
    {
    MLObject* o= scene->getObject(objects[i]);
    if( o == 0 || o == scene->getRootObject() )
      {
      objects[i]= -1;
      continue; // never mind
      }

    if( o->getObjectType() == MLObject::MESH )
      scene->getGlobalLighting()->mesh_shadow_has_changed((MLMesh*)o);
    o->detach();
    }
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::wakeupObjects( ModelCommand* cmd )
{
  IntegerArray& objects= cmd->indices;
  int nObjects= objects.getSize();

  if( nObjects == 0 )
    return;

  for( int i= 0; i < nObjects; ++i )
    {
    MLObject* o= scene->getDetachedObject(objects[i]);
    if( o == 0 || o == scene->getRootObject() )
      {
      objects[i]= -1;
      continue; // never mind
      }

    o->attach(); // Cross your fingers here.
    scene->getGlobalLighting()->object_is_transformed(o);
    }
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::hideObjects( ModelCommand* cmd, IToggle toggle )
{
  IntegerArray& objects= cmd->indices;
  int nObjects= objects.getSize();

  if( nObjects == 0 )
    return;

  for( int i= 0; i < nObjects; ++i )
    {
    MLObject* o= scene->getObject(objects[i]);

    if( o == 0 || o == scene->getRootObject() )
      {
      objects[i]= -1;
      continue; // never mind
      }

    switch(toggle)
      {
      case ISET:    o->flags |= MLObject::SHOW_BOX; break;
      case IUNSET:  o->flags &=~MLObject::SHOW_BOX; break;
      case ITOGGLE: o->flags ^= MLObject::SHOW_BOX; break;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::resetAll()
{
  scene->clear();

  MLRendererOptions* options= scene->getRendererOptions();
  options->current_system_gamma_correction= model->getGammaCorrectionValue();
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::resetObjectsRadiosity()
{
  MLGlobalLighting* gl= scene->getGlobalLighting();

  MLMeshesIterator mi(scene);
  while( ! mi.eol() )
    {
    MLMesh* m= mi++;

    if( ! m->selected() )
      continue;

    gl->reset_mesh_lighting(m);
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean ModelScene::removeAllHighlighting()
{
  boolean found= false;

  MLObjectsIterator oi(scene);
  while( ! oi.eol() )
    {
    MLObject* o= oi++;
    if( o == 0 )
      continue;
    if( (o->flags & MLObject::TMP_HIGHLIGHTED) != 0 )
      found= true;
    o->flags &= ~MLObject::TMP_HIGHLIGHTED;
    }

  return found;
}

/////////////////////////////////////////////////////////////////////////////

void ModelScene::setHighlighted( IntegerArray& objects )
{
  for( int i= 0; i < objects.getSize(); ++i )
    {
    MLObject* o= scene->getObject( objects[i] );
    if( o == 0 )
      continue;
    o->flags |= MLObject::TMP_HIGHLIGHTED;
    }
}

/////////////////////////////////////////////////////////////////////////////

MLLightPool* ModelScene::getSelectedPool()
{
  MLLightPool* pool= 0;

  MLObjectsIterator oi(scene);
  while( ! oi.eol() )
    {
    MLObject* o= oi++;
    if( o == 0 )
      continue;
    if( ! o->selected() )
      continue;
    if( o->getObjectType() != MLObject::LIGHT )
      return 0;

    MLLight* light= (MLLight*) o;
    if( pool == 0 )
      pool= light->getPool();
     else
      {
      if( light->getPool() != pool )
        return 0;
      }
    }

  return pool;
}

/////////////////////////////////////////////////////////////////////////////
