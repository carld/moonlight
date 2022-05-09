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
  MLExportVRML2.C

  Stephane Rehel

  August 12 1997
*/

#include "MLExportVRML2.h"

#include "tools/Color.h"
#include "tools/MLGammaCorrection.h"

#include "tools/3D/MLPolygon.h"
#include "tools/3D/MLVertex.h"

#include "scene/MLSceneVersion.h"

#include "scene/MeshRep.h"
#include "scene/MLMesh.h"
#include "scene/MLObjectsIterator.h"
#include "scene/lighting/MLMeshLighting.h"
#include "scene/renderer/MLRendererOptions.h"
#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

MLExportVRML2::MLExportVRML2( MLScene* _scene ):
  MLExport(_scene)
{}

/////////////////////////////////////////////////////////////////////////////

MLExportVRML2::~MLExportVRML2()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLExportVRML2::run()
{
  if( file == 0 || scene == 0 )
    return false;

  fprintf( file,
           "#VRML V2.0 utf8\n"
           "\n"
           "WorldInfo {\n"
           "  title \"%s\"\n"
           "  info [ \"Created with Moonlight Creator %s\" ]\n"
           "}\n"
           "\n",
           scene->getName().name.get(),
           MLSceneVersion::version() );

  fprintf( file,
           "Transform {\n"
           "  children [\n"
         );

  MLObjectsIterator oi(scene);
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

    if( mesh->nPolygons == 0 )
      continue;

    if( ! exportMeshVRML2(mlmesh) )
      return false;
    }

  fprintf( file,
           "  ]\n"
           "}\n" );

  close();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLExportVRML2::exportMeshVRML2( MLMesh* mlmesh )
{
  fprintf( file,
           "    # Object: %s\n"
           "    Shape {\n"
           "      geometry IndexedFaceSet {\n",
           mlmesh->getName().name.get()
         );


  const Matrix4x4& o2w= mlmesh->getGlobalXForm().get_o2w();

  MeshRep* mesh= mlmesh->getMeshRep();
  MLVertexArray& vertices= mesh->vertices;
  MLSVertexArray& svertices= mesh->svertices;

  fprintf( file,
           "      coord Coordinate {\n"
           "point [\n" );
  int i;

  for( i= 1; i <= svertices.getSize(); ++i )
    {
    MLSVertex& sv= svertices[i];
    Point p(0,0,0);
    if( sv.allocated() && sv.vertex != 0 )
      {
      MLVertex& v= vertices[ sv.vertex ];
      p= o2w.apply(v.point);
      }

    int outcome=
    fprintf( file,
             "\t%g %g %g%s\n",
             p.x(), p.y(), p.z(),
             (i==svertices.getSize()) ? " ]" : "," );

    if( outcome <= 0 )
      return false;
    }

  fprintf( file,
           "}\n"
           "\n"
           "      color Color {\n"
           "color [\n" );

  // compute it if necessary
  const MLSpectrumArray& lighting= mlmesh->getLighting()->getFinalLighting();

  MLGammaCorrection gc(scene->getRendererOptions()->getGammaCorrection());
  const unsigned char* gamma_table= gc.getTable();

  for( i= 1; i <= svertices.getSize(); ++i )
    {
    MLSVertex& sv= svertices[i];
    Color color(0,0,0);
    if( sv.allocated() && i <= lighting.getSize() )
      {
      const Spectrum& s= lighting.get(i);
      register unsigned char sr, sg, sb;
      register double c;
      c= s.r(); if( c < 0. ) sr= 0; else
                if( c > 1. ) sr= 255; else
                sr= gamma_table[ int(c * 255.) ];
      c= s.g(); if( c < 0. ) sg= 0; else
                if( c > 1. ) sg= 255; else
                sg= gamma_table[ int(c * 255.) ];
      c= s.b(); if( c < 0. ) sb= 0; else
                if( c > 1. ) sb= 255; else
                sb= gamma_table[ int(c * 255.) ];
      color= Color( double(sr)/255., double(sg)/255., double(sb)/255. );
      }

    int outcome=
    fprintf( file,
             "\t%g %g %g%s\n",
             color.r(), color.g(), color.b(),
             (i==svertices.getSize()) ? " ]" : "," );

    if( outcome <= 0 )
      return false;
    }

  fprintf( file,
           "}\n"
           "\n"
           "coordIndex [\n" );

  MLPolygonArray& polygons= mesh->polygons;

  for( i= 1; i <= polygons.getSize(); ++i )
    {
    MLPolygon& p= polygons[i];
    if( ! p.allocated() )
      continue;

    if( (p.flags & MLPolygon::BASE_POLYGON) == 0 )
      continue;

    if( p.nVertices < 3 )
      continue;

    if( i > 1 )
      fprintf( file, ",\n" );

    fprintf( file, "\t" );

    for( int j= 0; j < p.nVertices; ++j )
      fprintf( file, "%d,", p.elements[j].svertex - 1 );

    int outcome=
    fprintf( file,"-1");

    if( outcome <= 0 )
      return false;
    }

  fprintf( file,
           " ]\n"
           "\n"
           "      colorPerVertex TRUE\n"
           "     }\n"
           "   }\n"
         );

  return true;
}

/////////////////////////////////////////////////////////////////////////////

