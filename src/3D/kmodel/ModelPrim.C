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
  ModelPrim.C

  Stephane Rehel

  December 8 1996
*/

#include <assert.h>

#include "ModelPrim.h"

#include "scene/MLAbstractMesh.h"
#include "scene/MLAbstractCurve.h"
#include "scene/MLObject.h"
#include "scene/MeshRep.h"
#include "scene/formal/FormalRep.h"
#include "scene/formal/FormalCurveRep.h"

#include "scene/formal/FormalPolyhedron.h"

/////////////////////////////////////////////////////////////////////////////

MLAbstractMesh* ModelPrim::getFormal( MLScene* scene, int type )
{
  FormalRep* fr= FormalRep::newFormal(scene,FormalRep::TYPE(type));
  assert( fr != 0 );

  return new MLAbstractMesh(scene,fr);
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractCurve* ModelPrim::getFormalCurve( MLScene* scene, int type )
{
  FormalCurveRep* fr= FormalCurveRep::newFormal( scene,
                                                 FormalCurveRep::TYPE(type));
  assert( fr != 0 );

  return new MLAbstractCurve(scene,fr);
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractMesh* ModelPrim::getPolyhedron( MLScene* scene,
                                            int polyhedron_type )
{
  FormalRep* fr= FormalRep::newFormal(scene,FormalRep::POLYHEDRON);
  assert( fr != 0 );

  ((FormalPolyhedron*)fr)->setPolyhedronType
                      ( FormalPolyhedron::PolyhedronType(polyhedron_type) );

  return new MLAbstractMesh(scene,fr);
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractMesh* ModelPrim::getPointLightAbstract( MLScene* /*scene*/ )
{
  static MLAbstractMesh* a= 0;

  return a;

/*
  if( a == 0 )
    {
    // ok, build it
    a= new MLAbstractMesh(scene,0);
    a->generateMesh();

    MeshRep* mesh= a->getMeshRep();
assert(mesh!=0);
    mesh->clear();

    float f= 0.1;

    Point p000(-f,-f,-f); Point& p1= p000;
    Point p100( f,-f,-f); Point& p2= p100;
    Point p010(-f, f,-f); Point& p3= p010;
    Point p110( f, f,-f); Point& p4= p110;
    Point p001(-f,-f, f); Point& p5= p001;
    Point p101( f,-f, f); Point& p6= p101;
    Point p011(-f, f, f); Point& p7= p011;
    Point p111( f, f, f); Point& p8= p111;

    mesh->alloc( 8, 12, 6 );

    // Set vertices with empty svertices[] and edges[]
    mesh->vertices[1].point= p1;
    mesh->vertices[2].point= p2;
    mesh->vertices[3].point= p3;
    mesh->vertices[4].point= p4;
    mesh->vertices[5].point= p5;
    mesh->vertices[6].point= p6;
    mesh->vertices[7].point= p7;
    mesh->vertices[8].point= p8;

    // Set edges with empty polygons
    mesh->edges[ 1].v1= 1; mesh->edges[ 1].v2= 2;
    mesh->edges[ 2].v1= 3; mesh->edges[ 2].v2= 4;
    mesh->edges[ 3].v1= 1; mesh->edges[ 3].v2= 3;
    mesh->edges[ 4].v1= 2; mesh->edges[ 4].v2= 4;
    mesh->edges[ 5].v1= 5; mesh->edges[ 5].v2= 6;
    mesh->edges[ 6].v1= 7; mesh->edges[ 6].v2= 8;
    mesh->edges[ 7].v1= 5; mesh->edges[ 7].v2= 7;
    mesh->edges[ 8].v1= 6; mesh->edges[ 8].v2= 8;
    mesh->edges[ 9].v1= 1; mesh->edges[ 9].v2= 5;
    mesh->edges[10].v1= 2; mesh->edges[10].v2= 6;
    mesh->edges[11].v1= 3; mesh->edges[11].v2= 7;
    mesh->edges[12].v1= 4; mesh->edges[12].v2= 8;

    // Set polygons vertices (instead of svertices)
    mesh->polygons[1].allocQuad(1,3,4,2);
    mesh->polygons[2].allocQuad(5,6,8,7);
    mesh->polygons[3].allocQuad(2,4,8,6);
    mesh->polygons[4].allocQuad(3,7,8,4);
    mesh->polygons[5].allocQuad(1,5,7,3);
    mesh->polygons[6].allocQuad(1,2,6,5);

    mesh->finalizeRep();

    ref(a); // prevent this abstract to be deleted
    }

  return a;
*/
}

/////////////////////////////////////////////////////////////////////////////
