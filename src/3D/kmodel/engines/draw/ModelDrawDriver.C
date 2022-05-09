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
  ModelDrawDriver.C

  Stephane Rehel

  October 28 1997

  Abstract class
*/

#include "graphics/mlgl.h"

#include "tools/3D/MLVertex.h"

#include "scene/MLVertexArray.h"
#include "scene/formal/FormalRep.h"
#include "scene/formal/FormalNURBS.h"
#include "scene/formal/FormalNURBSCurve.h"
#include "scene/nurbs/MLNURBS_ControlPoint.h"
#include "scene/nurbs/MLNURBS_Surface.h"
#include "scene/nurbs/MLNURBS_Curve.h"
#include "scene/MeshRep.h"
#include "scene/CurveRep.h"
#include "scene/MLCurveVertex.h"
#include "scene/MLObject.h"
#include "scene/MLMesh.h"
#include "scene/MLCurve.h"
#include "scene/MLCamera.h"

#include "kmodel/ModelColors.h"

#include "ModelGL.h"
#include "ModelDrawDriver.h"
#include "ModelDraw.h"

/////////////////////////////////////////////////////////////////////////////

ModelDrawDriver::ModelDrawDriver( ModelDraw* _modelDraw )
{
  modelDraw= _modelDraw;
  model= modelDraw->getModel();
}

/////////////////////////////////////////////////////////////////////////////

ModelDrawDriver::~ModelDrawDriver()
{
  modelDraw= 0;
  model= 0;
}

/////////////////////////////////////////////////////////////////////////////

void ModelDrawDriver::drawVertices( MLObject* object )
{
  if( object == 0 )
    return;

  if( object->getObjectType() == MLObject::MESH )
    {
    drawMeshVertices( (MLMesh*) object );
    return;
    }

  if( object->getObjectType() == MLObject::CURVE )
    {
    drawCurveVertices( (MLCurve*) object );
    return;
    }
}

/////////////////////////////////////////////////////////////////////////////

void ModelDrawDriver::drawMeshVertices( MLMesh* mlmesh )
{
  if( mlmesh == 0 )
    return;

  MeshRep* mesh= mlmesh->getMeshRep();
  if( mesh == 0 )
    return;

  if( mlmesh->isFormal() )
    return;

  MLVertexArray& vertices= mesh->vertices;
  for( int i= 1; i <= vertices.getSize(); ++i )
    {
    MLVertex& v= vertices[i];
    if( ! v.allocated() )
      continue;

    glLoadName(i);

    glBegin(GL_POINTS);
      glPoint(v.point);
    glEnd();
    }
}

/////////////////////////////////////////////////////////////////////////////

void ModelDrawDriver::drawCurveVertices( MLCurve* mlcurve )
{
  if( mlcurve == 0 )
    return;

  CurveRep* curve= mlcurve->getCurveRep();
  if( curve == 0 )
    return;

  if( mlcurve->isFormal() )
    return;

  MLCurveVertex* vertices= curve->vertices;
  for( int i= 1; i <= curve->nVertices; ++i )
    {
    glLoadName(i);

    glBegin(GL_POINTS);
      glPoint( vertices[i].point );
    glEnd();
    }
}

/////////////////////////////////////////////////////////////////////////////

// for drawing and picking
void ModelDrawDriver::pickNURBSControlPoints( MLObject* object )
{
  if( object == 0 )
    return;

  if( object->getObjectType() == MLObject::MESH )
    {
    pickNURBSMeshControlPoints( (MLMesh*) object );
    return;
    }

  if( object->getObjectType() == MLObject::CURVE )
    {
    pickNURBSCurveControlPoints( (MLCurve*) object );
    return;
    }
}

/////////////////////////////////////////////////////////////////////////////

void ModelDrawDriver::pickNURBSMeshControlPoints( MLMesh* mesh )
{
  if( mesh == 0 )
    return;

  FormalNURBS* fnurbs= mesh->getFormalNURBS();
  if( fnurbs == 0 )
    return;

  const MLNURBS_Surface* nurbs= fnurbs->getSurface();
  if( nurbs == 0 )
    return;

  for( int i= 0; i <= nurbs->getn(); ++i )
    {
    for( int j= 0; j <= nurbs->getm(); ++j )
      {
      const MLNURBS_ControlPoint& cp= nurbs->getConstControlPoint(i,j);

      glLoadName( i + j * (nurbs->getn()+1) + 1 );

      glBegin(GL_POINTS);
        glVertex3f( cp.point.x(), cp.point.y(), cp.point.z() );
      glEnd();
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

void ModelDrawDriver::pickNURBSCurveControlPoints( MLCurve* curve )
{
  if( curve == 0 )
    return;

  FormalNURBSCurve* fnurbs= curve->getFormalNURBS();
  if( fnurbs == 0 )
    return;

  const MLNURBS_Curve* nurbs= fnurbs->getCurve();
  if( nurbs == 0 )
    return;

  for( int i= 0; i <= nurbs->getn(); ++i )
    {
    const MLNURBS_ControlPoint& cp= nurbs->getConstControlPoint(i);

    glLoadName( i + 1 );

    glBegin(GL_POINTS);
      glPoint(cp.point);
    glEnd();
    }
}

/////////////////////////////////////////////////////////////////////////////
