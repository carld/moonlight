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
  OFFLoader.C

  Stephane Rehel
  April 29 1997
*/

#include <stdio.h>
#include <string.h>

#include "OFFLoader.h"

#include "scene/MLScene.h"
#include "scene/MLAbstractMesh.h"
#include "scene/MLMesh.h"

#include "scene/convert/import/MLImporter.h"

/////////////////////////////////////////////////////////////////////////////

OFFLoader::OFFLoader( MLImporter* _importer )
{
  importer= _importer;
  scene= 0;

  file= 0;
  filename= MLString("");
}

/////////////////////////////////////////////////////////////////////////////

OFFLoader::~OFFLoader()
{
  importer->close_file(file);
  file= 0;

  scene= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean OFFLoader::init()
{
  filename= importer->in_filename;

  file= importer->open_file();

  if( file == 0 )
    return false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean OFFLoader::load()
{
  if( file == 0 )
    return false;

  scene= importer->scene;

  char* signature= getline();
  if( signature == 0 )
    {
    fatal_error();
    return false;
    }

  char* data_sizes= 0;
  if( strcmp(signature,"OFF") == 0 )
    data_sizes= getline();

  if( data_sizes == 0 )
    {
    fatal_error();
    return false;
    }

  MLAbstractMesh* ao= new MLAbstractMesh(scene,0);
  ao->generateMesh();

  MLObject* o= new MLMesh( scene->getRootObject(), ao );
  ::ref(o);

  MeshRep* mesh= ao->getMeshRep();

  int nVertices= 0,
      nPolygons= 0,
      nEdges= 0;

  int outcome= sscanf( data_sizes, "%d %d %d",
                       &nVertices, &nPolygons, &nEdges );
  if( outcome != 3 )
    {
    fatal_error();
    return false;
    }

  if( importer->verbose )
    fprintf( stderr, "Reading %d vertices, %d polygons...\n",
             nVertices, nPolygons );

  Point* points= new Point [ nVertices ];
  int i;

  for( i= 0; i < nVertices; ++i )
    {
    char* l= getline();
    if( l == 0 )
      {
      fatal_error();
      return false;
      }

    double x, y, z;
    int outcome= sscanf( l, "%lf %lf %lf", &x, &y, &z );

    if( outcome != 3 )
      {
      fatal_error();
      return false;
      }

    points[i]= Point(x,y,z);
    }

  for( i= 0; i < nPolygons; ++i )
    {
    char* l= getline();
    if( l == 0 )
      {
      fatal_error();
      return false;
      }

    int pn;
    if( ! getint(l,pn) )
      {
      fatal_error();
      return false;
      }

    if( pn < 3 )
      {
      fprintf( stderr, "OFFLoader: got a %d points polygons line %d !?\n",
               pn, lineN );
      return false;
      }

    Point* vertices= new Point [ pn ];
    for( int j= 0; j < pn; ++j )
      {
      int vi;
      if( ! getint(l,vi) )
        {
        fatal_error();
        return false;
        }

      if( vi < 0 || vi >= nVertices )
        {
        fprintf( stderr, "OFFLoader: invalid vertex index %d line %d\n",
                 vi, lineN );
        return false;
        }
      vertices[j]= points[vi];
      }

    importer->addPolygon(mesh,pn,vertices);

    delete vertices;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

char* OFFLoader::getline()
{
  if( file == 0 )
    return 0;

  ++lineN;

  if( feof(file) )
    return 0;

  if( fgets( line, 1023, file ) == 0 )
    return 0;

  char* l= line;
  for(;;)
    {
    if( *l == '\0' )
      break;

    if( ((unsigned char)*l) > 32 )
      break;

    ++l;
    }

  int len= strlen(l);
  while( len > 0 )
    {
    unsigned char ch= l[len - 1];
    if( ch > 32 )
      break;

    l[len-1]= '\0';
    --len;
    }

  if( *l == '\0' )
    return getline(); // an empty line

  return l;
}

/////////////////////////////////////////////////////////////////////////////

int OFFLoader::getint( char*& l, int& i )
{
  if( *l == '\0' )
    return 0;

  int outcome= sscanf( l, "%d", &i );
  if( outcome == 0 )
    return 0;

  char str[20];
  sprintf( str, "%d", i );
  l+= strlen(str);

  while( (*l==' ' || *l=='\t') && (*l != '\0') )
    ++l;

  return 1;
}

/////////////////////////////////////////////////////////////////////////////

void OFFLoader::fatal_error()
{
  fprintf( stderr, "OFFLoader: fatal error reading file line %d\n", lineN );
}

/////////////////////////////////////////////////////////////////////////////

