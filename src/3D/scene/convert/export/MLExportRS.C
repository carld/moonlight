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
  MLExportRS.C

  Stephane Rehel

  August 12 1997
*/

#include "tools/3D/MLPolygon.h"
#include "tools/3D/MLVertex.h"

#include "MLExportRS.h"

#include "scene/MLSceneVersion.h"

#include "scene/MLPolygonsIterator.h"
#include "scene/MeshRep.h"
#include "scene/MLMesh.h"
#include "scene/MLMeshesIterator.h"
#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

MLExportRS::MLExportRS( MLScene* _scene ):
  MLExport(_scene)
{}

/////////////////////////////////////////////////////////////////////////////

MLExportRS::~MLExportRS()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLExportRS::run()
{
  if( file == 0 || scene == 0 )
    return false;

  fprintf( file, "/* Exported from Moonlight Creator %s */\n",
           MLSceneVersion::version() );
  fprintf( file, "/* Scene: %s */\n"
                 "\n", scene->getName().name.get() );

  MLMeshesIterator mi(scene);
  while( ! mi.eol() )
    {
    MLMesh* m= mi++;
    if( m == 0 )
      continue;

    if( ! exportMeshRS(m) )
      return false;
    }

  close();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLExportRS::exportMeshRS( MLMesh* m )
{
  if( m == 0 || file == 0 )
    return true;

  MeshRep* mesh= m->getMeshRep();

  if( mesh == 0 )
    return true;
  if( mesh->nPolygons == 0 )
    return true;

  fprintf( file, "/* Object: %s */\n", m->getName().name.get() );

  fprintf( file, "list\n" );

  const Matrix4x4& o2w= m->getGlobalXForm().get_o2w();

  MLPolygonsIterator pi(m);
  while( ! pi.eol() )
    {
    MLPolygon* p= pi++;

    if( p->nVertices < 3 )
      continue;

    if( p->tesselation != 0 )
      continue;

    if( p->nVertices != 3 )
      continue; // failed to tesselate

    int vi1= p->elements[0].vertex;
    int vi2= p->elements[1].vertex;
    int vi3= p->elements[2].vertex;
    int svi1= p->elements[0].svertex;
    int svi2= p->elements[1].svertex;
    int svi3= p->elements[2].svertex;

    if( svi1 == 0 || svi2 == 0 || svi3 == 0 )
      continue;

    const MLVertex& v1= mesh->vertices[vi1];
    const MLVertex& v2= mesh->vertices[vi2];
    const MLVertex& v3= mesh->vertices[vi3];
    const MLSVertex& sv1= mesh->svertices[svi1];
    const MLSVertex& sv2= mesh->svertices[svi2];
    const MLSVertex& sv3= mesh->svertices[svi3];

    Point p1= o2w.apply(v1.point);
    Point p2= o2w.apply(v2.point);
    Point p3= o2w.apply(v3.point);

    Vector n1= o2w.apply(sv1.normal).getNormalized();
    Vector n2= o2w.apply(sv2.normal).getNormalized();
    Vector n3= o2w.apply(sv3.normal).getNormalized();

    int outcome=
    fprintf( file, " triangle\n"
                   "\t%g %g %g \t%g %g %g\n"
                   "\t%g %g %g \t%g %g %g\n"
                   "\t%g %g %g \t%g %g %g\n",
                   p1.x(), p1.y(), p1.z(), n1.x(), n1.y(), n1.z(),
                   p2.x(), p2.y(), p2.z(), n2.x(), n2.y(), n2.z(),
                   p3.x(), p3.y(), p3.z(), n3.x(), n3.y(), n3.z() );

    if( outcome <= 0 )
      return false;
    }

  fprintf( file, "end\n" );
  fprintf( file, "/* of Object: %s */\n", m->getName().name.get() );
  fprintf( file, "\n" );

  return true;
}

/////////////////////////////////////////////////////////////////////////////

