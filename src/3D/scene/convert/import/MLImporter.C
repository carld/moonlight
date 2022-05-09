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
  MLImporter.C

  Stephane Rehel

  April 27 1997
*/

#include <stdio.h>

#include "tools/MLString.h"
#include "tools/MLCommandLine.h"
#include "tools/MLFileEntry.h"
#include "tools/MLPath.h"
#include "tools/MLFilename.h"

#include "tools/3D/MLPolygon.h"
#include "tools/3D/MLVertex.h"

#include "MLImporter.h"

#include "scene/MeshRep.h"
#include "scene/MLScene.h"
#include "scene/MLAbstractMesh.h"
#include "scene/MLAbstractMeshesIterator.h"

#include "scene/file/MLDatabase.h"
#include "scene/file/MLDatabasesList.h"

/////////////////////////////////////////////////////////////////////////////

MLImporter::MLImporter()
{
  compressed= false;
  verbose= false;
  reverseFaces= false;
  in_filename= "";
  filename_ext= "";
  scene_name= "";
  scene= 0;
  layers2objects= false;
  no_write= false;
}

/////////////////////////////////////////////////////////////////////////////

MLImporter::~MLImporter()
{}

/////////////////////////////////////////////////////////////////////////////

void MLImporter::addPolygon( MeshRep* mesh,
                             int nVertices, const Point* points )
{
  int i1= mesh->vertices.getSize() + 1;

  MLPolygon& polygon= mesh->addPolygon();
  polygon.alloc(nVertices);
  polygon.flags |= MLPolygon::ALLOCATED | MLPolygon::BASE_POLYGON;
  for( int i= 0; i < nVertices; ++i )
    {
    MLVertex& vertex= mesh->vertices.append();
    vertex.flags |= MLVertex::ALLOCATED | MLVertex::BASE_VERTEX;
    vertex.point= points[i];

    if( reverseFaces )
      polygon.elements[i].vertex= i1 + nVertices - 1 - i;
     else
      polygon.elements[i].vertex= i1 + i;
    }

  mesh->nVertices += nVertices;
  mesh->nPolygons++;
}

/////////////////////////////////////////////////////////////////////////////

void MLImporter::finalize()
{
  scene->getRootObject()->updateTransformations();

  MLAbstractMeshesIterator aoi(scene);
  while( ! aoi.eol() )
    {
    MLAbstractMesh* ao= aoi++;

    MeshRep* mesh= ao->getMeshRep();
    if( mesh == 0 )
      continue;

    fprintf( stderr, "finalizing mesh '%s' (%d vertices, %d polygons)\n",
             ao->getName().name.get(),
             mesh->nVertices, mesh->nPolygons );

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

    ao->generateMesh(); // update bbox

    fprintf( stderr, "finalized '%s': %d vertices, %d polygons\n",
             ao->getName().name.get(),
             mesh->nVertices, mesh->nPolygons );
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLImporter::convertName( MLString& name )
{
  for( int i= 0; i < int(name.length()); ++i )
    {
    if( name[i] == '.' || name[i] == ',' || name[i] == '/' )
      name[i]= '-';
    }

  name= name.trim();
}

/////////////////////////////////////////////////////////////////////////////

// static
int MLImporter::parseOptions( MLCommandLine& cmd )
{
  verbose= false;
  if( cmd.findOption("-v") ||
      cmd.findOption("-verbose") )
    verbose= true;

  MLString databasePath= cmd.getStringAfter("-db");
  if( databasePath.length() == 0 )
    {
    MLDatabasesList dblist;
    dblist.build();
    if( ! dblist.get().empty() )
      {
      fprintf( stderr, "Getting default database location\n" );
      databasePath= dblist.get().get(1)->getBasePath();
      }
    }

  if( databasePath.length() == 0 )
    {
    fprintf( stderr, "Error: Can't find any database.\n" );
    return -1;
    }

  if( verbose )
    fprintf( stderr, "Database location=%s\n",databasePath.get());

  MLFileEntry databasePath_fe(databasePath);
  if( ! databasePath_fe.isDirectory() ||
      ! databasePath_fe.canRead() ||
      ! databasePath_fe.canExecute() )
    {
    fprintf( stderr, "Error: Unable to access directory '%s'\n",
             databasePath.get() );
    return 1;
    }

  database= new MLDatabase(MLPath(databasePath));

  reverseFaces= false;
  if( cmd.findOption("-rv") )
    reverseFaces= true;

  layers2objects= false;
  if( cmd.findOption("-dxfl2o") )
    layers2objects= true;

  no_write= false;
  if( cmd.findOption("-nw") )
    no_write= true;

  scene_name= cmd.getStringAfter("-outname");

  const char* tmp_strg= cmd.getFirstUnusedOption();
  if( tmp_strg != 0 )
    {
    fprintf( stderr, "Error: Unknown option '%s'\n", tmp_strg );
    return -1;
    }

  in_filename= cmd.getFirst();
  if( in_filename.length() == 0 )
    {
    fprintf( stderr, "Error: no filename specified\n" );
    return -1;
    }

  MLFilename in_f= MLFilename(in_filename);
  filename_ext= in_f.getExtension();

  filename_ext= filename_ext.toLowerCase();

  if( filename_ext == ".gz" || filename_ext == ".z" )
    {
    compressed= true;
    MLString f= in_filename.substring( 0, in_filename.length()
                                          -filename_ext.length() - 1 );
    filename_ext= MLFilename(f).getExtension();
    }

  if( scene_name.length() == 0 )
    scene_name= in_f.getFilename();

  int i;
  for( i= 0; i < scene_name.length(); ++i )
    if( scene_name[i] == '.' ||
        scene_name[i] == ',' ||
        scene_name[i] == '/' )
      scene_name[i]= '-';

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

FILE* MLImporter::open_file() const
{
  if( ! compressed )
    return fopen( in_filename.get(), "r" );

  return popen( (MLString("gzip -cd ") + in_filename).get(), "r" );
}

/////////////////////////////////////////////////////////////////////////////

void MLImporter::close_file( FILE* file ) const
{
  if( file == 0 || file == stdin )
    return;

  if( compressed )
    pclose(file);
   else
    fclose(file);
}

/////////////////////////////////////////////////////////////////////////////
