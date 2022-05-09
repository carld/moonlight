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
  DXFLoader.h

  Creation: SR, August 28th, 1995
  Revisions:
    + SR, January 27th, 1996
      Move it into a separate module
    + SR, March 21st, 1996
      Give up modules, set DXFLoader Action

  Revision:
    Stephane Rehel
    April 25 1997
*/

#ifndef __DXFLoader_h
#define __DXFLoader_h

#include <stdio.h>

#include "tools/Box.h"

#ifndef __DXFExport_h
#include "DXFExport.h"
#endif


class MLImporter;
class MLScene;
class DXFImport;
class MLString;
class MLAbstractMesh;
class MLObject;

/////////////////////////////////////////////////////////////////////////////

class DXFLoader: public DXFExport
{
public:
  boolean layers2Objects;

private:
  MLImporter* importer;

  MLScene* scene;

  DXFImport* dxfImport;
  FILE* file;
  boolean gzip_pipe;
  MLAbstractMesh* object;
  MLString object_name;
  MLAbstractMesh* main_object;
  Point base_point, insertion_point;
  Vector scale;
  double zrotate;

public:
  static int circles_subdivision;

  DXFLoader( MLImporter* _importer );

  virtual ~DXFLoader();

  boolean init();
  boolean load();

  // number of lines in a circle
  // by default: 0, i.e. dont draw circles...
  int nCircleSubdivide() const
    {
    return circles_subdivision;
    }

  void FACE( const Point& p1, const Point& p2, const Point& p3 );
  void FACE( const Point& p1, const Point& p2,
             const Point& p3, const Point& p4 );
  void POLYLINE( int nVertices, const Point* points, boolean closed );
  void addPolygon( int nVertices, const Point* points );

  // note: included blocks don't exist in dxf files
  boolean supportBlocks() const
    {
    return ! layers2Objects;
    }

  virtual void start_BLOCK( const char* name,
                            const Point& basePoint );
  virtual void end_BLOCK();
  virtual void insert_BLOCK( const char* name,
                             const Point& insertionPoint,
                             const Vector& scale,
                             double rotationAngle );

//  void start_exploding_BLOCK( const char* name, int id );
//  void end_exploding_BLOCK( const char* name, int id );

  void start_MAIN();
  void end_MAIN();

  void addObject( MLAbstractMesh* ao, MLObject* father =0 );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __DXFLoader_h
