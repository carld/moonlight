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
  Md2Loader.C

  From Stephane Rehel's ASCLoader.C

  phooky, 2/6/98

*/

#include <stdio.h>
#include <string.h>

#include "Md2Loader.h"

#include "scene/MLScene.h"
#include "scene/MLAbstractMesh.h"
#include "scene/MLMesh.h"

#include "scene/convert/import/MLImporter.h"

/////////////////////////////////////////////////////////////////////////////

Md2Loader::Md2Loader( MLImporter* _importer )
{
  importer= _importer;
  scene= 0;

  file= 0;
  filename= MLString("");
}

/////////////////////////////////////////////////////////////////////////////

Md2Loader::~Md2Loader()
{
  importer->close_file(file);
  file= 0;

  scene= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean Md2Loader::init()
{
  filename= importer->in_filename;

  file= importer->open_file();
  if( file == 0 )
    return false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean Md2Loader::load()
        {
  if( file == 0 )
    return false;

  scene= importer->scene;

        // Read header and do basic error checks
        if ( fread( &header, sizeof( dmdl_t ), 1, file ) < 1 )
                {
                fprintf( stderr, "Md2Loader: Couldn't read `%s'\n", filename.get() );
                return false;
                }
        if ( header.ident != IDALIASHEADER )
                {
                fprintf( stderr, "Md2Loader: Bad header on `%s' (not a .md2 file?)\n", filename.get() );
                return false;
                }

  for( int i = 0; i < 1 /* header.num_frames */; i++ )
    {
                // load frame header
                fseek( file, header.ofs_frames + header.framesize * i, SEEK_SET );
                daliasframe_t frameHeader;
                fread( &frameHeader, (int) &( (daliasframe_t*) 0 )->verts, 1, file );
                dtrivertx_t coords[ header.num_xyz ];
                fread( coords, sizeof( dtrivertx_t ) * header.num_xyz, 1, file );

                // convert data
                MLString frame_name( "" );
                for ( int j = 0; j < 16; j++ )
                        {
                        if ( !frameHeader.name[j] ) break;
                        frame_name += frameHeader.name[j];
                        }

                Point parr[ header.num_xyz ];
                //parr.alloc( header.num_xyz );

                for ( int j = 0; j < header.num_xyz; j++ )
                        {
                        parr[ j ] = Point(
                                (coords[ j ].v[0] * frameHeader.scale[0] + frameHeader.translate[0]),
                                (coords[ j ].v[1] * frameHeader.scale[1] + frameHeader.translate[1]),
                                (coords[ j ].v[2] * frameHeader.scale[2] + frameHeader.translate[2]) );
                        }

    importer->convertName( frame_name );

                // prepare mesh
                MLName name;
                name.name = frame_name;
                MLMesh* mlmesh = (MLMesh*)scene->getObject( name, MLObject::MESH );
                MLAbstractMesh* absmesh;
                if ( mlmesh )
                        {
                        absmesh = mlmesh->getAbstract();
                        }
                else
                        {
                        absmesh = new MLAbstractMesh( scene, 0 );
                        absmesh->setName( frame_name );
                        absmesh->generateMesh();
                        mlmesh = new MLMesh( scene->getRootObject(), absmesh );
                        ::ref(mlmesh);
                        mlmesh->setName( frame_name );
                        }
                MeshRep* mesh = absmesh->getMeshRep();

                fseek( file, header.ofs_tris, SEEK_SET );

                for ( int j = 0; j < header.num_tris; j++ )
                        {
                        dtriangle_t tri;
                        fread( &tri, sizeof( dtriangle_t ), 1, file );
                        Point points[3];

                        // The .md2 triangles are RHR; apparently moonlight is LHR...
                        points[0] = parr[ tri.index_xyz[2] ];
                        points[1] = parr[ tri.index_xyz[1] ];
                        points[2] = parr[ tri.index_xyz[0] ];
                        importer->addPolygon( mesh, 3, points );
                        }
#ifdef Md2_DEBUG
    if( importer->verbose )
      fprintf( stderr, "Md2Loader: reading object `%s'\n", frame_name.get() );
#endif
                }

  fclose(file);
  file= 0;
  return true;
        }

