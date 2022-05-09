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
  MLExportMLRaw.h

  Stephane Rehel

  December 14 1997
*/

#include "MLExportMLRaw.h"

#include "tools/Color.h"
#include "tools/MLGammaCorrection.h"

#include "tools/3D/MLPolygon.h"
#include "tools/3D/MLVertex.h"

#include "scene/MLSceneVersion.h"

#include "scene/MeshRep.h"
#include "scene/MLMesh.h"
#include "scene/MLObjectsIterator.h"
#include "scene/lighting/MLMeshLighting.h"
#include "scene/lighting/MLPoolTable.h"
#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

MLExportMLRaw::MLExportMLRaw( MLScene* _scene ):
  MLExport(_scene)
{
  only_triangles= false;
  only_one_pool= false;
}

/////////////////////////////////////////////////////////////////////////////

MLExportMLRaw::~MLExportMLRaw()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLExportMLRaw::run()
{
  if( file == 0 || scene == 0 )
    return false;

  MLObjectsIterator oi(scene);

  int nObjects= 0;
  while( ! oi.eol() )
    {
    MLObject* o= oi++;
    if( o == 0 )
      continue;

    if( o->getObjectType() != MLObject::MESH )
      continue;

    MLMesh* mlmesh= (MLMesh*) o;

    MeshRep* mesh= mlmesh->getMeshRep();
    if( mesh == 0 )
      continue;

    if( mesh->nPolygons == 0 || mesh->nVertices == 0 )
      continue;

    ++nObjects;
    }

  int nPools= only_one_pool ? 1 : scene->getPoolTable()->getNPools();

  fprintf( file,
           "# Exported from Moonlight Creator %s\n"
           "# Scene name: %s\n"
           "%d # number of objects\n"
           "%d # number of pools\n",
           scene->getName().name.get(),
           MLSceneVersion::version(),
           nObjects,
           nPools );

  int object_index= 1;
  oi.reset();
  while( ! oi.eol() )
    {
    MLObject* o= oi++;
    if( o == 0 )
      continue;

    if( o->getObjectType() != MLObject::MESH )
      continue;

    MLMesh* mlmesh= (MLMesh*) o;

    MeshRep* mesh= mlmesh->getMeshRep();
    if( mesh == 0 )
      continue;

    if( mesh->nPolygons == 0 || mesh->nVertices == 0 )
      continue;

    if( ! exportMeshMLRaw(object_index,nObjects,mlmesh) )
      return false;

    ++object_index;
    }

  close();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLExportMLRaw::exportMeshMLRaw( int object_index, int n_objects,
                                      MLMesh* mlmesh )
{
  const Matrix4x4& o2w= mlmesh->getGlobalXForm().get_o2w();

  MeshRep* mesh= mlmesh->getMeshRep();
  MLVertexArray& vertices= mesh->vertices;
  MLSVertexArray& svertices= mesh->svertices;
  MLPolygonArray& polygons= mesh->polygons;

  int i;

  int nPolygons= 0;
  for( i= 1; i <= polygons.getSize(); ++i )
    {
    MLPolygon& p= polygons[i];
    if( ! export_this_polygon(p) )
     continue;
    ++nPolygons;
    }

  fprintf( file,
           "%s # object %d/%d\n"
           "%d # nVertices\n"
           "%d # nPolygons\n",
           mlmesh->getName().name.get(),
           object_index, n_objects,
           svertices.getSize(),
           nPolygons
         );

  MLPoolTable* poolTable= scene->getPoolTable();

  int nPools= poolTable->getNPools();

  MLMeshLighting* lighting= mlmesh->getLighting();

  for( i= 1; i <= svertices.getSize(); ++i )
    {
    MLSVertex& sv= svertices[i];
    Point p(0,0,0);
    Vector normal(0,0,0);
    if( sv.allocated() && sv.vertex != 0 )
      {
      MLVertex& v= vertices[ sv.vertex ];
      p= o2w.apply(v.point);
      normal= mlmesh->getGlobalXForm().normal_o2w(sv.normal);
      double norm= normal.norm();
      if( norm > 1e-20 )
        normal /= norm;
      }

    int outcome;

    outcome= fprintf( file, "%g %g %g\n", p.x(), p.y(), p.z() );
    if( outcome <= 0 )
      return false;

    outcome= fprintf( file, "%g %g %g\n",
                      normal.x(), normal.y(), normal.z() );
    if( outcome <= 0 )
      return false;

    if( only_one_pool )
      {
      Spectrum s(0,0,0);

      for( int j= 1; j <= nPools; ++j )
        {
        MLLightPool* pool= poolTable->getPool(j);
        s+= lighting->getSVertexFinalLighting(pool,i);
        }
      fprintf( file, "%g %g %g\n", s.r(), s.g(), s.b() );
      }
     else
      {
      for( int j= 1; j <= nPools; ++j )
        {
        MLLightPool* pool= poolTable->getPool(j);
        Spectrum s= lighting->getSVertexFinalLighting(pool,i);
        fprintf( file, "%g %g %g\n", s.r(), s.g(), s.b() );
        }
      }
    }

  for( i= 1; i <= polygons.getSize(); ++i )
    {
    MLPolygon& p= polygons[i];
    if( ! export_this_polygon(p) )
     continue;

    fprintf( file, "%d\n", p.nVertices );

    for( int j= 0; j < p.nVertices; ++j )
      {
      int outcome= fprintf( file, "%d\n", p.elements[j].svertex-1 );
      if( outcome <= 0 )
        return false;
      }
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLExportMLRaw::export_this_polygon( const MLPolygon& p ) const
{
  if( ! p.allocated() )
    return false;

  if( only_triangles )
    {
    return p.nVertices == 3;
    }
   else
    {
    if( (p.flags & MLPolygon::BASE_POLYGON) != 0 )
      return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////
