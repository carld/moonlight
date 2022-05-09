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
  MLAbstractMesh_1_1.C

  Stephane Rehel
  September 12 1997

  MLAbstractMesh* MLAbstractMeshIO::read_1_1( MLRFileBlock* block );
*/

#include <stdio.h>

#include "MLAbstractMeshIO.h"
#include "MLAbstractMeshIDs.h"

#include "tools/file/MLRFile.h"
#include "tools/file/MLRFileBlock.h"
#include "tools/MLIOProgress.h"

#include "scene/MLAbstractMesh.h"

#include "scene/formal/FormalRep.h"
#include "scene/MeshRep.h"
#include "scene/MLHashBox.h"
#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

static
MeshRep* readMesh( MLAbstractMesh* ao,
                   MLRFileBlock& rb, MLIOProgress* progress )
{
  MeshRep* mesh= new MeshRep;

  int _nVertices, _nEdges, _nSVertices, _nPolygons;

  rb.go(MLAbstractMeshID::MESH_PARAMS);
    {
    // for quick reading
    rb >> _nVertices;
    rb >> _nEdges;
    rb >> mesh->bbox;

    rb >> _nSVertices;
    rb >> _nPolygons;

    rb >> mesh->nVertices;
    rb >> mesh->nEdges;
    rb >> mesh->nSVertices;
    rb >> mesh->nPolygons;
    mesh->discAngle= rb.getDouble();
    mesh->cos_discAngle= rb.getDouble();
    mesh->smallDistance= rb.getDouble();

    Point bs_center;
    double bs_radius;

    rb >> bs_center;
    bs_radius= rb.getDouble();
    mesh->boundingSphere= BoundingSphere( bs_center, bs_radius );
    }

  int i;

  mesh->vertices.realloc (_nVertices);
  mesh->edges.realloc    (_nEdges);
  mesh->svertices.realloc(_nSVertices);
  mesh->polygons.realloc (_nPolygons);

  // approx. sizes
  int  vertices_size= _nVertices * sizeof(MLVertex);
  int     edges_size= _nEdges * sizeof(MLEdge);
  int svertices_size= _nSVertices * sizeof(MLSVertex);
  int  polygons_size= _nPolygons * sizeof(MLPolygon);
  int total_size= vertices_size
                 +edges_size
                 +svertices_size
                 +polygons_size;

  progress->setMessage( MLString("Reading geometry ")+ao->getName().name );
  progress->setRange( 0., double(total_size) );
  progress->empty();
  progress->flush();

  unsigned int flag;

  rb.go(MLAbstractMeshID::MESH_VERTICES);
    {
    for( i= 1; i <= _nVertices; ++i )
      {
      register MLVertex& v= mesh->vertices[i];
      rb >> v.point;
      }
    for( i= 1; i <= _nVertices; ++i )
      {
      MLVertex& v= mesh->vertices[i];
      rb >> flag;
      v.flags= MLVertex::Flag(flag);
      if( ! v.allocated() )
        continue;
      rb >> v.nEdges;
      rb >> v.nPolygons;
      rb >> v.nSVertices;

      if( v.nEdges > 0 )     v.edges= new int [ v.nEdges ];
      if( v.nPolygons > 0 )  v.polygons= new int [ v.nPolygons ];
      if( v.nSVertices > 0 ) v.svertices= new int [ v.nSVertices ];

      int j;
      for( j= 0; j < v.nEdges; ++j )
        rb >> v.edges[j];
      for( j= 0; j < v.nPolygons; ++j )
        rb >> v.polygons[j];
      for( j= 0; j < v.nSVertices; ++j )
        rb >> v.svertices[j];

      if( (i&63) == 0 )
        progress->set( (i-1) * sizeof(MLVertex) );
      }
    }

  rb.go(MLAbstractMeshID::MESH_EDGES);
    {
    for( i= 1; i <= _nEdges; ++i )
      {
      MLEdge& e= mesh->edges[i];
      rb >> flag;
      e.flags= MLEdge::Flag(flag);
      if( ! e.allocated() )
        continue;

      rb >> e.v1;
      rb >> e.v2;
      rb >> e.polygon1;
      rb >> e.polygon2;

      if( (i&63) == 0 )
        progress->set( vertices_size + (i-1) * sizeof(MLEdge) );
      }
    }

  rb.go(MLAbstractMeshID::MESH_SVERTICES);
    {
    for( i= 1; i <= _nSVertices; ++i )
      {
      MLSVertex& v= mesh->svertices[i];
      rb >> flag;
      v.flags= MLSVertex::Flag(flag);
      if( ! v.allocated() )
        continue;

      rb >> v.vertex;
      rb >> v.normal;
      rb >> v.material;
      rb >> v.txtCoord;

      if( (i&63) == 0 )
        progress->set( vertices_size + edges_size
                      + (i-1) * sizeof(MLSVertex) );
      }
    }

  rb.go(MLAbstractMeshID::MESH_POLYGONS);
    {
    for( i= 1; i <= _nPolygons; ++i )
      {
      MLPolygon& p= mesh->polygons[i];
      p.index= i;
//      p.mesh= mesh;

      rb >> flag;
      p.flags= MLPolygon::Flag(flag);
      if( ! p.allocated() )
        continue;

      rb >> p.nVertices;
      rb >> p.material;

      rb >> p.nTessTriangles;
      rb >> p.fatherPolygon;

      if( p.nTessTriangles > 0 )
        {
        p.tesselation= new int [ p.nTessTriangles ];

        for( int j= 0; j < p.nTessTriangles; ++j )
          rb >> p.tesselation[j];
        }

      if( p.nVertices > 0 )
        p.elements= new MLPolygon::Element [ p.nVertices ];

      for( int j= 0; j < p.nVertices; ++j )
        {
        rb >> p.elements[j].vertex;
        rb >> p.elements[j].edge;
        rb >> p.elements[j].svertex;
        }

      if( (i&63) == 0 )
        progress->set( vertices_size + edges_size + svertices_size
                      + (i-1) * sizeof(MLPolygon) );
      }
    }

  for( i= 1; i <= _nPolygons; ++i )
    {
    MLPolygon& p= mesh->polygons[i];
    if( ! p.allocated() )
      continue;
    p.updateNormalArea( mesh->vertices.getArray(),
                        mesh->polygons.getArray() );
    }

  mesh->computeBoundings();

  progress->full();
  progress->flush();
/*
mesh->svertices.clear();
mesh->nSVertices= 0;
for( i= 1; i <= mesh->polygons.getSize(); ++i )
  {
  MLPolygon& p= mesh->polygons[i];
  if( ! p.allocated() )
    continue;
  if( (p.flags & MLPolygon::TESS_TRIANGLE) != 0 )
    {
    p.unallocate();
    mesh->nPolygons--;
    }
  }
boolean verbose= 1;
if( verbose ) fprintf( stderr, "  Edges pass 1/2\n" );
mesh->findEdges();
if( verbose ) fprintf( stderr, "  Vertices pass\n" );
mesh->mergeVertices();
if( verbose ) fprintf( stderr, "  Edges pass 2/2\n" );
mesh->findEdges();
if( verbose ) fprintf( stderr, "  Global pass\n" );
mesh->finalizeRep();
if( verbose ) fprintf( stderr, "  Reindexing vertices 1/2\n" );
mesh->reindexVertices();
if( verbose ) fprintf( stderr, "  Reindexing edges\n" );
mesh->reindexEdges();
if( verbose ) fprintf( stderr, "  Reindexing vertices 2/2\n" );
mesh->reindexSVertices();
if( verbose ) fprintf( stderr, "  Reindexing polygons\n" );
mesh->reindexPolygons();
*/
  rb.go(MLAbstractMeshID::MESH_HASH_BOX);
    {
    int hasHashBox;
    rb >> hasHashBox;

    if( ! hasHashBox )
      {
      mesh->buildNewHashBox();
      }
     else
      {
      MLHashBox* hb= new MLHashBox(mesh->bbox);

      int nx, ny, nz;
      rb >> nx >> ny >> nz;

      hb->create(nx,ny,nz);

      for( int z= 0; z < hb->getZ(); ++z )
        for( int y= 0; y < hb->getY(); ++y )
          {
          int x= 0;
          while( x < hb->getX() )
            {
            int nElements;
            rb >> nElements;
            if( nElements < 0 )
              {
              x+= -nElements;
              continue;
              }

            MLHashBox::Voxel& voxel= (*hb)(x,y,z);
            voxel.alloc(nElements);

            for( int j= 0; j < nElements; ++j )
              rb >> voxel.polygons[j];
            ++x;
            }
          }

      mesh->hashBox= hb;
      }
    }

  return mesh;
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractMesh* MLAbstractMeshIO::read_1_1( MLRFileBlock& rb )
{
  if( ! rb.find(MLAbstractMeshID::ABSTRACT) ||
      ! rb.find(MLAbstractMeshID::FORMAL_REP) ||
      ! rb.find(MLAbstractMeshID::MESH_PARAMS) )
    return 0;

  MLName name;

  rb.go(MLAbstractMeshID::NAME);
    {
    rb >> name;
    }

  FormalRep* formalRep= 0;
  rb.go(MLAbstractMeshID::FORMAL_REP);
    {
    int hasFormal;
    rb >> hasFormal;
    if( hasFormal )
      formalRep= FormalRep::read(scene,&rb);
    }

  MLAbstractMesh* ao= 0;
  rb.go(MLAbstractMeshID::ABSTRACT);
    {
    ao= new MLAbstractMesh(scene,formalRep);

    ao->name= name;

    unsigned int flags;
    rb >> flags;
    ao->flags= MLAbstractMesh::Flag(flags);
    }

  MeshRep* mesh= readMesh(ao,rb,progress);

  if( mesh == 0 )
    {
    delete ao;
    return 0;
    }

  ao->meshRep= mesh;

  return ao;
}

/////////////////////////////////////////////////////////////////////////////
