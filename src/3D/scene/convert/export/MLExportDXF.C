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
  MLExportDXF.C

  Stephane Rehel

  August 12 1997
*/

#include "MLExportDXF.h"

#include "tools/3D/MLPolygon.h"
#include "tools/3D/MLVertex.h"

#include "scene/MLPolygonsIterator.h"
#include "scene/MeshRep.h"
#include "scene/MLMesh.h"
#include "scene/MLMeshesIterator.h"

/////////////////////////////////////////////////////////////////////////////

MLExportDXF::MLExportDXF( MLScene* _scene ):
  MLExport(_scene)
{}

/////////////////////////////////////////////////////////////////////////////

MLExportDXF::~MLExportDXF()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLExportDXF::run()
{
  if( file == 0 || scene == 0 )
    return false;

  fprintf( file,
           "  0\n"
           "SECTION\n"
           "  2\n"
           "ENTITIES\n" );

  MLMeshesIterator mi(scene);
  while( ! mi.eol() )
    {
    MLMesh* m= mi++;
    if( m == 0 )
      continue;

    if( ! exportMeshDXF(m) )
      return false;
    }

  fprintf( file,
           "  0\n"
           "ENDSEC\n"
           "  0\n"
           "EOF\n" );

  close();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLExportDXF::exportMeshDXF( MLMesh* m )
{
  if( m == 0 || file == 0 )
    return true;

  MeshRep* mesh= m->getMeshRep();

  if( mesh == 0 )
    return true;

  const Matrix4x4& o2w= m->getGlobalXForm().get_o2w();

  MLPolygonArray& polygons= mesh->polygons;

  MLPolygonsIterator pi(m);
  while( ! pi.eol() )
    {
    MLPolygon* p= pi++;

    if( (p->flags & MLPolygon::TESS_TRIANGLE) != 0 ||
        (p->flags & MLPolygon::REFINEMENT) != 0)
      continue;

    if( p->tesselation == 0 )
      {
      // no tesselation
      if( p->nVertices > 3 )
        {
        // tesselation failed for this polygon
        if( p->nVertices == 4 ) // well, this is a quad
          if( ! exportPolygonDXF(o2w,mesh,p) )
            return false;
        }
       else
        {
        if( ! exportPolygonDXF(o2w,mesh,p) )
          return false;
        }
      continue;
      }

    // keep 4-vertices polygons (quads)
    if( p->nVertices == 4 )
      {
      if( ! exportPolygonDXF(o2w,mesh,p) )
        return false;
      continue;
      }

    // export tesselation of this polygon
    for( int i= 0; i < p->nTessTriangles; ++i )
      {
      int ti= p->tesselation[i];
      if( ti < 1 || ti > polygons.getSize() )
        continue;
      MLPolygon& tp= polygons[ti];
      if( ! tp.allocated() )
        continue;
      if( ! exportPolygonDXF(o2w,mesh,&tp) )
        return false;
      }
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLExportDXF::exportPolygonDXF( const Matrix4x4& o2w,
                                       MeshRep* mesh,
                                       MLPolygon* p )
{
  if( p->nVertices != 3 && p->nVertices != 4 )
    return true;

  int vi1= p->elements[0].vertex;
  int vi2= p->elements[1].vertex;
  int vi3= p->elements[2].vertex;
  int vi4;
  if( p->nVertices == 3 )
    vi4= vi3;
   else
    vi4= p->elements[3].vertex;

  const MLVertex& v1= mesh->vertices[vi1];
  const MLVertex& v2= mesh->vertices[vi2];
  const MLVertex& v3= mesh->vertices[vi3];
  const MLVertex& v4= mesh->vertices[vi4];
  const Point& _p1= v1.point;
  const Point& _p2= v2.point;
  const Point& _p3= v3.point;
  const Point& _p4= v4.point;

  Point p1= o2w.apply(_p1);
  Point p2= o2w.apply(_p2);
  Point p3= o2w.apply(_p3);
  Point p4= o2w.apply(_p4);

  int outcome=
  fprintf( file,
           "  0\n"
           "3DFACE\n"
           "  8\n"
           "0\n"
           "  6\n"
           "CONTINUOUS\n"
           " 62\n"
           "     4\n"
           " 10\n%g\n"
           " 20\n%g\n"
           " 30\n%g\n"
           " 11\n%g\n"
           " 21\n%g\n"
           " 31\n%g\n"
           " 12\n%g\n"
           " 22\n%g\n"
           " 32\n%g\n"
           " 13\n%g\n"
           " 23\n%g\n"
           " 33\n%g\n",
           p1.x(), p1.y(), p1.z(),
           p2.x(), p2.y(), p2.z(),
           p3.x(), p3.y(), p3.z(),
           p4.x(), p4.y(), p4.z() );

  if( outcome <= 0 )
    return false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////
