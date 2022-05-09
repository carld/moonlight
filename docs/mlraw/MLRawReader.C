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
  MLRawReader.C

  Copyright (C) 1997-1998  Stephane Rehel

  December 14 1997
*/

#include <string.h>

#include "MLRawReader.h"

/////////////////////////////////////////////////////////////////////////////

MLRawReader::MLRawReader( FILE* _file, boolean _verbose /* = false */ )
{
  file= _file;
  verbose= _verbose;

  objects= 0;
  nObjects= 0;
  nPools= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLRawReader::~MLRawReader()
{
  clear();

  file= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLRawReader::clear()
{
  delete [] objects;
  objects= 0;

  nObjects= 0;
  nPools= 0;
}

/////////////////////////////////////////////////////////////////////////////

char* MLRawReader::get_line()
{
  if( file == 0 )
    return 0;

  static char line[512];

  char* l= 0;
  char* s= 0;

  for(;;)
    {
    l= fgets( line, 511, file );
    if( l == 0 )
      break;

    s= strchr(l,'\n');
    if( s != 0 )
      *s= '\0';

    s= strchr(l,'#');
    if( s != 0 )
      *s= '\0';

    s= l;
    while( *l == ' ' || *l == '\t' )
      ++l;

    if( *l != '\0' )
      break;
    }

  return l;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLRawReader::get_integer( int* i )
{
  if( i == 0 )
    return false;

  char* line= get_line();

  if( line == 0 )
    return 0;

  if( sscanf( line, "%d", i ) != 1 )
    return false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// read a raw format
// return false if error
boolean MLRawReader::read()
{
  clear();

  if( file == 0 )
    return false;

  if( ! get_integer(&nObjects) )
    return false;
  if( ! get_integer(&nPools) )
    return false;

  if( nObjects == 0 )
    return false;

  objects= new Object [ nObjects ];

  for( int i= 0; i < nObjects; ++i )
    if( ! readObject(i) )
      return false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLRawReader::readObject( int object_index )
{
  if( objects == 0 )
    return false;

  if( object_index < 0 || object_index >= nObjects )
    return false;

  Object* o= objects + object_index;

  char* line= get_line();
  if( line == 0 )
    return false;

  // object name is in *line
  if( verbose )
    fprintf( stderr, "reading object %s\n", line );

  o->name= new char [ strlen(line) + 1 ];
  strcpy( o->name, line );

  if( ! get_integer( &(o->nVertices) ) )
    return false;
  if( verbose )
    fprintf( stderr, "  nVertices=%d\n", o->nVertices );

  if( ! get_integer( &(o->nPolygons) ) )
    return false;
  if( verbose )
    fprintf( stderr, "  nPolygons=%d\n", o->nPolygons );

  int i;

  if( o->nVertices == 0 )
    return true;
  o->vertices= new Vertex [ o->nVertices ];
  for( i= 0; i < o->nVertices; ++i )
    {
    Vertex* v= o->vertices + i;

    line= get_line();
    if( line == 0 )
      return false;
    if( sscanf( line, "%f %f %f", &v->x, &v->y, &v->z ) != 3 )
      return false;

    line= get_line();
    if( line == 0 )
      return false;
    if( sscanf( line, "%f %f %f", &v->nx, &v->ny, &v->nz ) != 3 )
      return false;

    if( nPools > 0 )
      {
      v->lighting= new Spectrum [ nPools ];
      for( int j= 0; j < nPools; ++j )
        {
        Spectrum* s= v->lighting+j;

        line= get_line();
        if( line == 0 )
          return false;

        if( sscanf( line, "%f %f %f", &s->r, &s->g, &s->b ) != 3 )
          return false;
        }
      }
    }

  if( o->nPolygons == 0 )
    return true;
  o->polygons= new Polygon [ o->nPolygons ];
  for( i= 0; i < o->nPolygons; ++i )
    {
    Polygon* p= o->polygons + i;

    if( ! get_integer( &(p->nVertices) ) )
      return false;

    if( p->nVertices <= 0 )
      continue;

    p->vertices= new int [ p->nVertices ];
    for( int j= 0; j < p->nVertices; ++j )
      {
      if( ! get_integer( p->vertices + j ) )
        return false;
      }
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// 0 <= i < nObjects
MLRawReader::Object* MLRawReader::getObject( int i ) const
{
  if( objects == 0 )
    return 0;
  if( i < 0 || i >= nObjects )
    return 0;

  return objects + i;
}

/////////////////////////////////////////////////////////////////////////////

