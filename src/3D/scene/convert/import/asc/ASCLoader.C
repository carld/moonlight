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
  ASCLoader.C

  Creation: SR, February 9th, 1996

  Revision:
    Stephane Rehel
    April 27 1997
*/

#include <stdio.h>
#include <string.h>

#include "ASCLoader.h"

#include "scene/MLScene.h"
#include "scene/MLAbstractMesh.h"
#include "scene/MLMesh.h"

#include "scene/convert/import/MLImporter.h"

/////////////////////////////////////////////////////////////////////////////

ASCLoader::ASCLoader( MLImporter* _importer )
{
  importer= _importer;
  scene= 0;

  file= 0;
  filename= MLString("");
}

/////////////////////////////////////////////////////////////////////////////

ASCLoader::~ASCLoader()
{
  importer->close_file(file);
  file= 0;

  scene= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean ASCLoader::init()
{
  filename= importer->in_filename;

  file= importer->open_file();
  if( file == 0 )
    return false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean ASCLoader::load()
{
  if( file == 0 )
    return false;

  lineN= 0;
  scene= importer->scene;

  boolean all_ok= false;

  for(;;)
    {
    MLString object_name("");
    boolean ok= findObject(object_name);
    if( ! ok )
      {
      all_ok= true;
      break; // no more objects
      }

    importer->convertName(object_name);

#ifdef ASC_DEBUG
    if( importer->verbose )
      fprintf( stderr, "ASCLoader: reading object `%s'\n", object_name.get() );
#endif

    int nVertices= 0, nFaces= 0;
    char* line= getNewLine();
    if( line == 0 )
      break;
    int outcome= sscanf( line, "Tri-mesh, Vertices: %d     Faces: %d",
                         &nVertices, &nFaces );
    if( outcome != 2 )
      {
      // this is not a tri-mesh
      continue;
      }
    if( nVertices <= 0 || nFaces <= 0 )
      break;

    vector<Point> vertices;
    vertices.resize(nVertices);
    for(int i=0;i<vertices.size();i++)
      vertices[i]=Point(0,0,0);

#ifdef ASC_DEBUG
    if( importer->verbose )
      fprintf( stderr, "ASCLoader: nVertices= %d  nFaces= %d\n",
               nVertices, nFaces );
#endif

    ok= readVertexList(vertices);
    if( ! ok )
      break;

    ok= readFaceList(object_name,vertices,nFaces);
    if( ! ok )
      break;
    }

  fclose(file);
  file= 0;

#ifdef ASC_DEBUG
  if( ! all_ok )
    fprintf( stderr, "ASCLoader: error parsing line %d\n", lineN );
#endif

  return all_ok;
}

/////////////////////////////////////////////////////////////////////////////

char* ASCLoader::getLine()
{
  if( file == 0 )
    return 0;

  static char* buffer= 0;
  static int maxLineLength= 512;

  if( buffer == 0 )
    buffer= new char [ maxLineLength + 1 ];

  if( feof(file) )
    return 0;

  char* outcome= fgets( buffer, maxLineLength - 1, file );
  if( outcome == 0 )
    return 0;
  ++lineN;

  int length= strlen(buffer);
  while( length > 0 )
    {
    if( int((unsigned char) buffer[length-1]) < int(' ') )
      buffer[--length]= 0;
     else
      break;
    }

  return buffer;
}

/////////////////////////////////////////////////////////////////////////////

char* ASCLoader::getNewLine()
{
  char* line= 0;

  for(;;)
    {
    line= getLine();
    if( line == 0 )
      return 0;
    if( line[0] != ' ' && line[0] != '\t' && line[0] != '\0' )
      break;
    }

  return line;
}

/////////////////////////////////////////////////////////////////////////////

boolean ASCLoader::findObject( MLString& name )
{
  const MLString str("Named object: ");

  for(;;)
    {
    char* line= getNewLine();
    if( line == 0 )
      return false;

    if( (int)strlen(line) < (int)str.length() )
      continue;

    if( MLString(line,0,str.length()) != str )
      continue;

    char* s= line + str.length();
    if( *s != '"' )
      continue;
    name= MLString("");
    ++s;

    while( *s != '"' && *s != '\0' )
      name += *(s++);

    if( *s == '"' )
      {
      // got one object
      break;
      }
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean ASCLoader::readVertexList( vector<Point>& vertices )
{
  for(;;)
    {
    char* line= getNewLine();
    if( line == 0 )
      return false;
    if( strcmp( line, "Vertex list:" ) == 0 )
      break;
    }

  int i;
  for( i= 0; i < vertices.size(); ++i )
    {
    char* line= getNewLine();
    if( line == 0 )
      return false;

    int index;
    double x, y, z;

    int outcome= sscanf( line, "Vertex %d:  X: %lf     Y: %lf     Z: %lf",
                         &index, &x, &y, &z );
    if( outcome != 4 )
      return false;

    if( index < 0 || index >= vertices.size() )
      return false;

    vertices[index]= Point(x,y,z);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean ASCLoader::readFaceList( const MLString& object_name,
                               const vector<Point>& vertices, int nFaces )
{
  for(;;)
    {
    char* line= getNewLine();
    if( line == 0 )
      return false;
    if( strcmp( line, "Face list:" ) == 0 )
      break;
    }

  MLName name;
  name.name= object_name;
  MLMesh* mlmesh= (MLMesh*)scene->getObject(name,MLObject::MESH);
  MLAbstractMesh* ao= 0;
  if( mlmesh != 0 )
    {
    ao= mlmesh->getAbstract();
    }
   else
    {
    ao= new MLAbstractMesh(scene,0);
    ao->setName(object_name);
    ao->generateMesh();

    mlmesh= new MLMesh( scene->getRootObject(), ao );
    ::ref(mlmesh);
    mlmesh->setName(object_name);
    }

  MeshRep* mesh= ao->getMeshRep();

  int i;
  for( i= 0; i < nFaces; ++i )
    {
    char* line= 0;
    for(;;)
      {
      line= getNewLine();
      if( line == 0 )
        return false;
      if( line[0]=='F' &&
          line[1]=='a' &&
          line[2]=='c' &&
          line[3]=='e' &&
          line[4]==' ' )
        break;
      }

    int index;
    int i1, i2, i3;

    int outcome= sscanf( line, "Face %d:    A:%d B:%d C:%d ",
                         &index, &i1, &i2, &i3 );
    if( outcome != 4 )
      return false;

    if( index < 0 || index >= nFaces )
      return false;
    if( i1 < 0 || i1 >= vertices.size() ||
        i2 < 0 || i2 >= vertices.size() ||
        i3 < 0 || i3 >= vertices.size() )
      return false;

    // we've got a face!
    if( scene != 0 )
      {
      Point points[3];
      points[0]= vertices[i1];
      points[1]= vertices[i2];
      points[2]= vertices[i3];

      importer->addPolygon( mesh, 3, points );
      }
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
