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
  MLExportOFF.C

  Stephane Rehel

  August 12 1997
*/

#include "MLExportOFF.h"

#include "tools/3D/MLPolygon.h"
#include "tools/3D/MLVertex.h"

#include "scene/MLPolygonsIterator.h"
#include "scene/MeshRep.h"
#include "scene/MLMesh.h"
#include "scene/MLMeshesIterator.h"

/////////////////////////////////////////////////////////////////////////////

MLExportOFF::MLExportOFF( MLScene* _scene ):
  MLExport(_scene)
{}

/////////////////////////////////////////////////////////////////////////////

MLExportOFF::~MLExportOFF()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLExportOFF::run()
{
  if( file == 0 || scene == 0 )
    return false;

  fprintf( file, "OFF\n" );

  int nVertices= 0;
  int nPolygons= 0;

  MLMeshesIterator mi(scene);
  while( ! mi.eol() )
    {
    MLMesh* mlmesh= mi++;
    if( mlmesh == 0 )
      continue;

    MeshRep* mesh= mlmesh->getMeshRep();
    if( mesh == 0 )
      continue;

    nVertices+= mesh->vertices.getSize();

    MLPolygonsIterator pi(mlmesh);
    while( ! pi.eol() )
      {
      MLPolygon* p= pi++;
      if( (p->flags & MLPolygon::BASE_POLYGON) == 0 )
        continue;
      if( p->nVertices < 3 )
        continue;
      ++nPolygons;
      }
    }

  fprintf( file, "%d %d %d\n", nVertices, nPolygons, 0 );

  mi.reset();
  while( ! mi.eol() )
    {
    MLMesh* mlmesh= mi++;
    if( ! exportVerticesOFF(mlmesh) )
      return false;
    }

  vertex_base= 0;

  mi.reset();
  while( ! mi.eol() )
    {
    MLMesh* mlmesh= mi++;
    if( ! exportPolygonsOFF(mlmesh) )
      return false;
    }

  close();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLExportOFF::exportVerticesOFF( MLMesh* mlmesh )
{
  if( mlmesh == 0 || file == 0 )
    return true;

  MeshRep* mesh= mlmesh->getMeshRep();

  if( mesh == 0 )
    return true;

  const Matrix4x4& o2w= mlmesh->getGlobalXForm().get_o2w();

  for( int i= 1; i <= mesh->vertices.getSize(); ++i )
    {
    MLVertex& v= mesh->vertices[i];
    int outcome;
    if( v.allocated() )
      {
      Point p= o2w.apply(v.point);
      outcome= fprintf( file, "%g %g %g\n", p.x(), p.y(), p.z() );
      }
     else
      outcome= fprintf( file, "0 0 0\n" );

    if( outcome <= 0 )
      return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLExportOFF::exportPolygonsOFF( MLMesh* mlmesh )
{
  if( mlmesh == 0 || file == 0 )
    return true;

  MeshRep* mesh= mlmesh->getMeshRep();

  if( mesh == 0 )
    return true;

  MLPolygonsIterator pi(mlmesh);
  while( ! pi.eol() )
    {
    MLPolygon* p= pi++;

    if( (p->flags & MLPolygon::BASE_POLYGON) == 0 )
      continue;

    if( p->nVertices < 3 )
      continue;

    fprintf( file, "%d", p->nVertices );
    for( int i= 0; i < p->nVertices; ++i )
      fprintf( file, " %d", vertex_base + p->elements[i].vertex - 1 );

    int outcome= fprintf( file, "\n" );
    if( outcome <= 0 )
      return false;
    }

  vertex_base += mesh->vertices.getSize();

  return true;
}

/////////////////////////////////////////////////////////////////////////////
