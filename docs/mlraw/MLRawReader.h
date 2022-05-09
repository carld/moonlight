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
  MLRawReader.h

  Copyright (C) 1997-1998  Stephane Rehel

  December 14 1997
*/

#ifndef __MLRawReader_h
#define __MLRawReader_h

#include <stdio.h>

typedef bool boolean;

/////////////////////////////////////////////////////////////////////////////

class MLRawReader
{
public:
  struct Spectrum
    {
    float r, g, b;
    };

  struct Vertex
    {
    float x, y, z; // location
    float nx, ny, nz; // normal (normalized)
    Spectrum* lighting; // nPools long

    Vertex()
      {
      lighting= 0;
      }
    ~Vertex()
      {
      delete [] lighting;
      lighting= 0;
      }
    };

  struct Polygon
    {
    int nVertices;
    int* vertices;

    Polygon()
      {
      nVertices= 0;
      vertices= 0;
      }
    ~Polygon()
      {
      delete [] vertices;
      vertices= 0;
      }
    };

  struct Object
    {
    char* name;

    int nVertices;
    Vertex* vertices;

    int nPolygons;
    Polygon* polygons;

    Object()
      {
      name= 0;

      nVertices= 0;
      vertices= 0;

      nPolygons= 0;
      polygons= 0;
      }

    ~Object()
      {
      delete [] polygons;
      polygons= 0;

      delete [] vertices;
      vertices= 0;

      delete name;
      name= 0;
      }
    };

protected:
  int nObjects;
  int nPools;
  Object* objects;

  FILE* file;

  boolean verbose;

public:
  MLRawReader( FILE* _file, boolean _verbose = false );

  ~MLRawReader();

  void clear();

protected:
  char* get_line();
  boolean get_integer( int* i );

public:
  boolean read();

protected:
  boolean readObject( int object_index );

public:
  int getNObjects() const
    {
    return nObjects;
    }

  // 0 <= i < nObjects
  Object* getObject( int i ) const;

  int getNPools() const
    {
    return nPools;
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLRawReader_h
