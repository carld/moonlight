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
  DXFImport.h

  DXF file reader and converter:
     DXF file -->  DXFImport  -->  DXFExport --> any output (file, screen...)

  Revision:
    Stephane Rehel
    April 25 1997
*/

#ifndef __DXFImport_h
#define __DXFImport_h

#include <stdio.h>

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

#ifndef __DXFExport_h
#include "DXFExport.h"
#endif

#ifndef __DXFReader_h
#include "DXFReader.h"
#endif

/////////////////////////////////////////////////////////////////////////////

// DXFImport: read dxf file, write blocks into raw files, and then
//            send objects to *dxfExport given in argument to init()
class DXFImport: public DXFExport
{
public:
  struct BLOCK
         {
         long pos;
         char* name;
         int id;
         boolean empty;
         BLOCK* next;
         Point base;
         boolean anonymous;
         };
  static const char* MAIN_BLOCK_NAME;

private:

  char tmp_filename[L_tmpnam+1];
  DXFReader dxfReader;
  int nBlocks;
  FILE* file;
  BLOCK* blocks;
  BLOCK* curBlock;
  FILE* fstderr;
  boolean explodeBlocks;
  struct TRANSFORM
         {
         Vector scale;
         double sina, cosa;
         Vector translate;
         Vector base;
         TRANSFORM* next;
         } *transform;

  DXFExport* dxfExport;
  Point curColor;
  int curLayer;

  enum {
       POINT_ID,
       CIRCLE_ID,
       CYLINDER_ID,
       FACE3_ID,
       FACE4_ID,
       POLYLINE_ID,
       MESHSURFACE_ID,
       INSERT_ID,

       SET_COLOR_ID,
       BLOCK_END_ID
       };

  Point minPoint, maxPoint, isoPoint;

public:
  DXFImport();
  ~DXFImport()
    {
    destroy();
    }
  void destroy();

  void init( FILE* dxfFile, DXFExport* _dxfExport, FILE* fstderr= 0 );

  int getNBlocks() const
    {
    return nBlocks;
    }

  BLOCK* getBlock( int i );
  BLOCK* findBlock( const char* name );
  BLOCK* lastBlock();
  BLOCK* newBlock( const char* name );
  BLOCK* appendBlock( const char* name );
  boolean blockIsBefore( BLOCK* blk, BLOCK* before_this );
  BLOCK* previousBlock( BLOCK* blk );
  void insertBeforeBlock( BLOCK* blk, BLOCK* before_this );
  BLOCK* newBeforeBlock( BLOCK* before_this, const char* name );
  void detachBlock( BLOCK* b );
  void dependsOnBlock( BLOCK* b, const char* name );

  void writeInteger( int i )
    {
    int I= i;
    assert( file != 0 );
    fwrite( (const void*) &I, sizeof(I), 1, file );
    }
  void writeDouble( const DBL d )
    {
    double D= d;
    assert( file != 0 );
    fwrite( (const void*) &D, sizeof(D), 1, file );
    }
  void writePoint( const Point& p );
  void writeVector( const Vector& v );
  int readInteger()
    {
    assert( file != 0 );
    int i;
    fread( (void*) &i, sizeof(i), 1, file );
    return i;
    }
  DBL readDouble()
    {
    assert( file != 0 );
    DBL d;
    fread( (void*) &d, sizeof(d), 1, file );
    return d;
    }
  void readPoint( Point& p );
  void readVector( Vector& v );

  void test_SET_COLOR();
  int nCircleSubdivide() const
    {
    assert( dxfExport != 0 );
    return dxfExport->nCircleSubdivide();
    }
  void POINT( const Point& p );
  void CIRCLE( const Point& center, double radius );
  void CYLINDER( const Point& center, double radius, const Vector& length );
  void FACE( const Point& p1, const Point& p2, const Point& p3 );
  void FACE( const Point& p1, const Point& p2,
             const Point& p3, const Point& p4 );
  void POLYLINE( int nVertices, const Point* points, boolean closed );
  boolean supportMeshSurface() const
    { return true; }
  void MESHSURFACE( const Point* vertices,
                    int M, int N,
                    boolean closed_M, boolean closed_N );

  boolean supportBlocks() const
    {
    return true;
    }
  void insert_BLOCK( const char* name,
                     const Point& insertionPoint,
                     const Vector& scale,
                     double rotationAngle );

  // note: included blocks don't exist in dxf files
  void start_BLOCK( const char* name,
                    const Point& basePoint );
  void end_BLOCK();

  // when we've read all the blocks, we start reading main object
  void start_MAIN();
  void end_MAIN();

  const Point getColor() const
    {
    return curColor;
    }
  DXFReader::LAYER* getLayer()
    {
    return dxfReader.getLayer( curLayer );
    }
  int getNLayers()
    {
    return dxfReader.getNLayers();
    }

  DXFReader::LAYER* getLayer( int i )
    {
    return dxfReader.getLayer( i );
    }

  void transformPoint( Point& p1 );
  void read();
  void exportBlock( BLOCK* b );
  void go_exporting();
  void exportAll();

  void computeBoundings();

  Point getMinPoint() const;
  Point getMaxPoint() const;
  Point getIsoPoint() const;
  void setEmptyBlock( const char* name );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __DXFImport_h
