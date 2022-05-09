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
  MLExportMd2.C

        phooky <phooky@gyral.com>

  based on MLExportVRML1.C by Stephane Rehel

  March 11 1998
    - Removed the values.h for portability. /Kepler
*/

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "MLExportMd2.h"
#include "../formats/md2types.h"

#include "tools/3D/MLVertex.h"
#include "tools/3D/MLPolygon.h"

#include "scene/MLPolygonsIterator.h"

#include "scene/MeshRep.h"
#include "scene/MLMesh.h"

#include "scene/MLObject.h"
#include "scene/MLObjectsIterator.h"

#include "scene/MLScene.h"

#ifndef MAXDOUBLE
#define MAXDOUBLE   1.79769313486231470e+308
#endif

#ifndef MINDOUBLE
#define MINDOUBLE   4.94065645841246544e-324
#endif

#ifndef MAXFLOAT
#define MAXFLOAT    3.40282347e+38F
#endif

#ifndef MINFLOAT
#define MINFLOAT    1.40129846e-45F
#endif

/////////////////////////////////////////////////////////////////////////////

#define MD2_SCALE 0.1

/////////////////////////////////////////////////////////////////////////////

MLExportMd2::MLExportMd2( MLScene* _scene ):
  MLExport(_scene)
{}

/////////////////////////////////////////////////////////////////////////////

MLExportMd2::~MLExportMd2()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLExportMd2::run()
{
  if( file == 0 || scene == 0 )
    return false;

  MLVertexArray vArray;
  MLPolygonArray pArray;

  // BBox won't give us a box containing nothing nowhere. :(
  Point bmin( MAXDOUBLE, MAXDOUBLE, MAXDOUBLE );
  Point bmax( MINDOUBLE, MINDOUBLE, MINDOUBLE );

  MLObjectsIterator oi(scene);
  while( ! oi.eol() )
    {
    MLObject* o= oi++;

    if( o == 0 || o->getObjectType() != MLObject::MESH )
      continue;

    MLMesh* mlmesh= (MLMesh*) o;

    MeshRep* mesh= mlmesh->getMeshRep();

    // an assumption is made that all values have been properly computed--
    // is this valid?
    if( mesh == 0 || mesh->nPolygons == 0 )
      continue;

    // we remove holes of the vertices array
    mesh->reindexVertices();

    int firstVertex= vArray.getSize();

    const Matrix4x4& o2w= mlmesh->getGlobalXForm().get_o2w();

    MLVertexArray& vertices= mesh->vertices;
    for( int i= 1; i <= vertices.getSize(); ++i )
      {
      MLVertex& v= vertices[i];

      MLVertex& newv= vArray.append();

      if( ! v.allocated() )
        {
        // mmh, this is a bug. this should not happen.
        // let's be safe
        newv.point= Point(0,0,0);
        }
       else
        newv.point= o2w.apply(v.point);

      bmin= min( bmin, newv.point );
      bmax= max( bmax, newv.point );
      }

    MLPolygonsIterator pi(mlmesh);
    while ( ! pi.eol() )
      {
      MLPolygon* p= pi++;

      if ( p->nVertices != 3 || p->tesselation != 0 )
        continue;

      MLPolygon &newp = pArray.append();

      newp.allocTriangle( p->elements[0].vertex + firstVertex,
                          p->elements[1].vertex + firstVertex,
                          p->elements[2].vertex + firstVertex );
      }
    }

  Vector scale = (bmax - bmin) / 255.;
  Vector translate = bmin;

  // convert the vertices and polygons into the appropriate .md2 constructs

  dtriangle_t* tris = new dtriangle_t [ pArray.getSize() ];
  // sets index_sts to 0
  memset( (void*) tris, 0, sizeof(dtriangle_t) * pArray.getSize() );

  int i;

  for( i = 0; i < pArray.getSize(); i++ )
    {
    MLPolygon& p= pArray[i+1];

    // here we'll use 0-based indexing
    // reverse the chirality again
    tris[i].index_xyz[ 0 ] = p.elements[2].vertex - 1;
    tris[i].index_xyz[ 1 ] = p.elements[1].vertex - 1;
    tris[i].index_xyz[ 2 ] = p.elements[0].vertex - 1;
    }

#ifdef DEBUG
  printf(" SCALE: \t%f\t%f\t%f\n", scale.vect[0], scale.vect[1], scale.vect[2] );
  printf(" TRANSLATE: \t%f\t%f\t%f\n", translate.vect[0], translate.vect[1], translate.vect[2] );
#endif

  dtrivertx_t* verts = new dtrivertx_t [ vArray.getSize() ];
  memset( (void*) verts, 0, sizeof(dtrivertx_t) * vArray.getSize() );

  for ( i = 0; i < vArray.getSize(); i++ )
    {
    MLVertex& v= vArray[i+1];
    Point& p= v.point;

    verts[i].v[0] = (unsigned char) ( (p[0] - translate[0]) / scale[0] );
    verts[i].v[1] = (unsigned char) ( (p[1] - translate[1]) / scale[1] );
    verts[i].v[2] = (unsigned char) ( (p[2] - translate[2]) / scale[2] );
    verts[i].lightnormalindex = 0; // FIXME, and how.
    }

  dmdl_t header;
  memset( (void*) &header, 0, sizeof( header ) );

  // Set magic numbers
  header.ident = IDALIASHEADER;
  header.version = ALIAS_VERSION;
  header.framesize = sizeof( daliasframe_t ) +
                     sizeof( dtrivertx_t ) * (vArray.getSize() - 1);
  header.num_skins = 1;
  header.num_xyz = vArray.getSize();
  header.num_st = 1;
  header.num_tris = pArray.getSize();
  header.num_frames = 1;
  header.skinwidth = header.skinheight = 50;

  // write the actual data

  fseek( file, sizeof( dmdl_t ), SEEK_SET );

  // write skins-- null skin for now
  header.ofs_skins = ftell( file );
  char skinname[ MAX_SKINNAME ];
  memset( (void*) skinname, 0, MAX_SKINNAME );
  sprintf( skinname, "null skin" );
  fwrite( skinname, MAX_SKINNAME, 1, file );

  // write sts
  header.ofs_st = ftell( file );
  dstvert_t stvert;
  stvert.s = stvert.t = 0;
  fwrite( &stvert, sizeof( dstvert_t ), 1, file );

  // write tris
  header.ofs_tris = ftell( file );
  fwrite( tris, sizeof( dtriangle_t ), pArray.getSize(), file );

  // write frame header...
  header.ofs_frames = ftell( file );

  daliasframe_t frame;
  memset( (void*) &frame, 0, sizeof(daliasframe_t) );

  frame.scale[ 0 ] = scale[0];
  frame.scale[ 1 ] = scale[1];
  frame.scale[ 2 ] = scale[2];
  frame.translate[ 0 ] = translate[0];
  frame.translate[ 1 ] = translate[1];
  frame.translate[ 2 ] = translate[2];
  sprintf( frame.name, "base frame" );
  fwrite( &frame, (int) ((daliasframe_t *)0)->verts, 1, file ); // ugly!
  fwrite( verts, sizeof( dtrivertx_t ), vArray.getSize(), file );

  // write GL commands
  header.ofs_glcmds = ftell( file );
  header.num_glcmds = 0; // we'll hash this out later
  header.ofs_end = ftell( file );

  // finally, write the header
  fseek( file, 0, SEEK_SET );
  fwrite( &header, sizeof( dmdl_t ), 1, file );

  close();

  delete tris;
  tris= 0;

  delete verts;
  verts= 0;

  return true;
}

/////////////////////////////////////////////////////////////////////////////
