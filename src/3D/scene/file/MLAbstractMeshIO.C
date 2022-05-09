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
  MLAbstractMeshIO.C

  Stephane Rehel
  April 6 1997
*/

#include <stdio.h>

#include "MLAbstractMeshIO.h"
#include "MLAbstractMeshIDs.h"

#include "tools/MLFileEntry.h"
#include "tools/MLIOProgress.h"

#include "tools/file/MLWFile.h"
#include "tools/file/MLWFileBlock.h"

#include "tools/file/MLRFile.h"
#include "tools/file/MLRFileBlock.h"

#include "tools/file/MLFileInfo.h"

#include "scene/MLAbstractMesh.h"
#include "scene/MLAbstractMeshAllocator.h"
#include "scene/formal/FormalRep.h"
#include "scene/MeshRep.h"
#include "scene/MLHashBox.h"
#include "scene/MLScene.h"
#include "scene/MLSceneVersion.h"

#include "MLDatabase.h"

/////////////////////////////////////////////////////////////////////////////

static const int major_version= 1;
static const int minor_version= 1;

/////////////////////////////////////////////////////////////////////////////

MLAbstractMeshIO::MLAbstractMeshIO( MLDatabase* _database,
                                        MLScene* _scene,
                                        MLIOProgress* _progress /* = 0 */ ):
  MLIO(_progress)
{
  database= _database;
  scene= _scene;
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractMeshIO::~MLAbstractMeshIO()
{
  database= 0;
  scene= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLAbstractMeshIO::write( MLAbstractMesh* ao, MLWFileBlock& wb )
{
  if( progress != 0 )
    progress->setMessage( MLString("Writing geometry ") + ao->name.name );

  ao->name.filename= getAbstractObjectFilename( database,
                                                scene->getName(),
                                                ao->name );

  wb.begin(MLAbstractMeshID::DRIVER_VERSION);
    wb << major_version;
    wb << minor_version;
  wb.end();

  wb.begin(MLAbstractMeshID::INFO);
    MLFileInfo::write( MLSceneVersion::version(),
                       MLSceneVersion::build_cpu(),
                       MLSceneVersion::build_vendor(),
                       MLSceneVersion::build_os(),
                       wb );
  wb.end();

  wb.begin(MLAbstractMeshID::NAME);
    wb << ao->name;
  wb.end();

  wb.begin(MLAbstractMeshID::ABSTRACT);
    wb << (unsigned int)ao->flags;
  wb.end();

  wb.begin(MLAbstractMeshID::FORMAL_REP);
    {
    FormalRep* fr= ao->formalRep;
    if( fr == 0 )
      wb << int(0);
     else
      {
      wb << int(1);
      FormalRep::write(&wb,fr);
      }
    }
  wb.end();

  writeMesh(ao->meshRep,wb);
}

/////////////////////////////////////////////////////////////////////////////

void MLAbstractMeshIO::writeMesh( MeshRep* mesh, MLWFileBlock& wb )
{
  assert( mesh != 0 );

  if( mesh->dirtyBoundings )
    mesh->computeBoundings(); // be sure to save an up-to-date bounding box

  wb.begin(MLAbstractMeshID::MESH_PARAMS);
    {
    // for quick reading
    wb << mesh->vertices.getSize();
    wb << mesh->edges.getSize();
    wb << mesh->bbox;

    wb << mesh->svertices.getSize();
    wb << mesh->polygons.getSize();

    wb << mesh->nVertices;
    wb << mesh->nEdges;
    wb << mesh->nSVertices;
    wb << mesh->nPolygons;

    wb << double(mesh->discAngle);
    wb << double(mesh->cos_discAngle);
    wb << double(mesh->smallDistance);

    wb << mesh->boundingSphere.getCenter();
    wb << double(mesh->boundingSphere.getRadius());
    }
  wb.end();

  int i;

  // approx. sizes
  int  vertices_size= mesh->vertices.getSize() * sizeof(MLVertex);
  int     edges_size= mesh->edges.getSize() * sizeof(MLEdge);
  int svertices_size= mesh->vertices.getSize() * sizeof(MLSVertex);
  int  polygons_size= mesh->polygons.getSize() * sizeof(MLPolygon);
  int   hashbox_size= 1;
  if( mesh->hashBox != 0 )
    {
    MLHashBox* hb= mesh->hashBox;
    hashbox_size= hb->getX() * hb->getY() * hb->getZ()
                 *sizeof(MLHashBox::Voxel);
    }
  int total_size= vertices_size
                 +edges_size
                 +svertices_size
                 +polygons_size
                 +hashbox_size;

  progress->setRange( 0., double(total_size) );
  progress->empty();
  progress->flush();

  wb.begin(MLAbstractMeshID::MESH_VERTICES);
    {
    // moved on top for quick loading
    for( i= 1; i <= mesh->vertices.getSize(); ++i )
      {
      MLVertex& v= mesh->vertices[i];
      if( ! v.allocated() )
        wb << Point(0,0,0);
       else
        wb << v.point;
      }
    for( i= 1; i <= mesh->vertices.getSize(); ++i )
      {
      MLVertex& v= mesh->vertices[i];
      wb << (unsigned int) v.flags;
      if( ! v.allocated() )
        continue;
      wb << v.nEdges;
      wb << v.nPolygons;
      wb << v.nSVertices;

      int j;
      for( j= 0; j < v.nEdges; ++j )
        wb << v.edges[j];
      for( j= 0; j < v.nPolygons; ++j )
        wb << v.polygons[j];
      for( j= 0; j < v.nSVertices; ++j )
        wb << v.svertices[j];

      if( (i&63) == 0 )
        progress->set( (i-1) * sizeof(MLVertex) );
      }
    }
  wb.end();

  wb.begin(MLAbstractMeshID::MESH_EDGES);
    {
    for( i= 1; i <= mesh->edges.getSize(); ++i )
      {
      MLEdge& e= mesh->edges[i];
      wb << (unsigned int) e.flags;
      if( ! e.allocated() )
        continue;

      wb << e.v1;
      wb << e.v2;
      wb << e.polygon1;
      wb << e.polygon2;

      if( (i&63) == 0 )
        progress->set( vertices_size
                      + (i-1) * sizeof(MLEdge) );
      }
    }
  wb.end();

  wb.begin(MLAbstractMeshID::MESH_SVERTICES);
    {
    for( i= 1; i <= mesh->svertices.getSize(); ++i )
      {
      MLSVertex& v= mesh->svertices[i];
      wb << (unsigned int) v.flags;
      if( ! v.allocated() )
        continue;

      wb << v.vertex;
      wb << v.normal;
      wb << v.material;
      wb << v.txtCoord;

      if( (i&63) == 0 )
        progress->set( vertices_size + edges_size
                      + (i-1) * sizeof(MLSVertex) );
      }
    }
  wb.end();

  wb.begin(MLAbstractMeshID::MESH_POLYGONS);
    {
    for( i= 1; i <= mesh->polygons.getSize(); ++i )
      {
      MLPolygon& p= mesh->polygons[i];
      wb << (unsigned int) p.flags;
      if( ! p.allocated() )
        continue;

      wb << p.nVertices;
      wb << p.material;

      wb << p.nTessTriangles;
      wb << p.fatherPolygon;
      int j;
      for( j= 0; j < p.nTessTriangles; ++j )
        wb << p.tesselation[j];

      for( j= 0; j < p.nVertices; ++j )
        {
        wb << p.elements[j].vertex;
        wb << p.elements[j].edge;
        wb << p.elements[j].svertex;
        }

      if( (i&63) == 0 )
        progress->set( vertices_size + edges_size + svertices_size
                      +(i-1) * sizeof(MLPolygon) );
      }
    }
  wb.end();

  wb.begin(MLAbstractMeshID::MESH_HASH_BOX);
    {
    MLHashBox* hb= mesh->hashBox;
    boolean ok= (hb!=0);
    if( ok ) ok= hb->allocated();

    wb << int(ok ? 1 : 0);

    if( hb != 0 )
      {
      wb << hb->getX()
         << hb->getY()
         << hb->getZ();
      for( int z= 0; z < hb->getZ(); ++z )
        {
        for( int y= 0; y < hb->getY(); ++y )
          {
          int x= 0;
          while( x < hb->getX() )
            {
            int x2= x;
            while( x2 < hb->getX() )
              {
              if( hb->get(x2,y,z)->nElements != 0 )
                break;
              ++x2;
              }
            if( x2 > x )
              {
              wb << int( -(x2-x) );
              }
            x= x2;
            if( x >= hb->getX() )
              break;
            while( x < hb->getX() )
              {
              const MLHashBox::Voxel* voxel= hb->get(x,y,z);
              if( voxel->nElements == 0 )
                break;
              wb << int(voxel->nElements);
              for( int j= 0; j < voxel->nElements; ++j )
                wb << voxel->polygons[j];
              ++x;
              }
            } // x
          } // y

        progress->set( vertices_size + edges_size + svertices_size
                      +polygons_size
                      +z * hb->getX() * hb->getY()
                         * sizeof(MLHashBox::Voxel) );
        } // z
      }
    }
  wb.end(); // MESH_HASH_BOX

  progress->full();
  progress->flush();
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractMesh* MLAbstractMeshIO::read( MLRFileBlock& block )
{
  if( ! block.go(MLAbstractMeshID::DRIVER_VERSION) )
    return 0;

  int major, minor;
  block >> major;
  block >> minor;

  int version= major * 100 + minor;

  MLAbstractMesh* ao= 0;

  switch( version )
    {
    case 100: ao= read_1_0(block); break;
    case 101: ao= read_1_1(block); break;
    default: break;
    }

  return ao;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLAbstractMesh* MLAbstractMeshIO::read( MLDatabase* database,
                                            MLScene* scene,
                                            MLRFileBlock& block,
                                            MLIOProgress* progress /*= 0*/ )
{
  MLAbstractMeshIO aoReader(database,scene,progress);

  MLAbstractMesh* ao= aoReader.read(block);

  if( ao != 0 )
    {
//    MLRFile* file= block.getFile();
    ao->name.filename= MLAbstractMeshIO::getFilename(ao);

    // adjust any other abstract name that poinst to this ao->name
    // this prevents other abstracts to have the same filename
    // when we save next time
    scene->getAbstractAllocator()->adjustNameIndices(ao);
    }

  return ao;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLAbstractMesh* MLAbstractMeshIO::read( MLDatabase* database,
                                            MLScene* scene,
                                            const MLString& filename,
                                            MLIOProgress* progress /*= 0*/ )
{
  MLAbstractMesh* ao= scene->getAbstractByFilename(filename);

  if( ao != 0 )
    return ao; // already loaded

  if( database == 0 )
    return 0;

  MLRFile file;

  if( ! file.open(filename) )
    return 0;

  if( ! file.init(MLAbstractMeshID::signature) )
    {
    file.close();
    return 0;
    }

  MLRFileBlock& rb= file.getMainBlock();

  ao= MLAbstractMeshIO::read( database, scene, rb, progress );

  file.close();

  return ao;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLAbstractMesh* MLAbstractMeshIO::read( MLDatabase* database,
                                            MLScene* scene,
                                            const MLName& name,
                                            MLIOProgress* progress /*= 0*/ )
{
  MLString filename= getAbstractObjectFilename( database,
                                               scene->getName(),
                                               name );

  return read( database, scene, filename, progress );
}

/////////////////////////////////////////////////////////////////////////////

boolean MLAbstractMeshIO::write( MLAbstractMesh* ao,
                                 const MLString& filename )
{
  MLWFile file;

  if( ! file.create(filename) )
    return false;

  if( ! file.init(MLAbstractMeshID::signature) )
    {
    file.close();
    MLFileEntry(filename).remove();
    return false;
    }

  MLWFileBlock& wb= file.getMainBlock();

  write(ao,wb);

  if( ! wb.close() )
    {
    file.close();
    MLFileEntry(filename).remove();
    return false;
    }

  file.close();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// static
boolean MLAbstractMeshIO::write( MLDatabase* database,
                                 MLAbstractMesh* ao,
                                 MLIOProgress* progress /*= 0*/ )
{
  if( database == 0 )
    return true;

  MLString filename= getAbstractObjectFilename(database,ao);

  MLAbstractMeshIO aoWriter(database,ao->getScene(),progress);

  return aoWriter.write(ao,filename);
}

/////////////////////////////////////////////////////////////////////////////

// static
// e.g. my_scene,1.2,geom-1.mlg
MLString MLAbstractMeshIO::getFilename( MLAbstractMesh* ao )
{
  return getFilename( ao->getScene()->getName(), ao->getName() );
}

/////////////////////////////////////////////////////////////////////////////

// static
// e.g. my_scene,1.2,geom-1.mlg
MLString MLAbstractMeshIO::getFilename( const MLName& sceneName,
                                         const MLName& name )
{
  MLString n= sceneName.name;
  n += MLDatabase::sepChar;
  n += MLDatabase::buildVersionString(sceneName);
  n += MLDatabase::sepChar;
  n += name.name;
  if( name.name_index != MLName::default_name_index )
    {
    // in case if we have two objects named 'geom-1' for example
    // e.g. my_scene,1.2,geom-1,2.mlg
    n += MLDatabase::sepChar;
    n += MLString::valueOf( name.name_index );
    }

  n += MLDatabase::geometryExtension;

  return n;
}

/////////////////////////////////////////////////////////////////////////////

// static
// e.g. /usr/ml/database/GEOM/my_scene,1.2,geom-1.mlg
MLString MLAbstractMeshIO::getAbstractObjectFilename( MLDatabase* database,
                                                       MLAbstractMesh* ao )
{
  return getAbstractObjectFilename( database,
                                    ao->getScene()->getName(),
                                    ao->getName() );
}

/////////////////////////////////////////////////////////////////////////////

// static
// e.g. /usr/ml/database/GEOM/my_scene,1.2,geom-1.mlg
MLString MLAbstractMeshIO::getAbstractObjectFilename( MLDatabase* database,
                                                       const MLName& sceneName,
                                                       const MLName& name )
{
  return   ((database==0) ? MLString("") : database->getGeomPath())
         + getFilename(sceneName,name);
}

/////////////////////////////////////////////////////////////////////////////

