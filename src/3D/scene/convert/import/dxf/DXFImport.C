// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//                 2002       Patrick Mauritz
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
  DXFImport.C

  Last version: May 1995

  Revision:
  Stephane Rehel
  April 25 1997
*/

#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include "DXFImport.h"

#include "DXFBound.h"

/////////////////////////////////////////////////////////////////////////////

const char* DXFImport::MAIN_BLOCK_NAME= "<MAIN>";

/////////////////////////////////////////////////////////////////////////////

DXFImport::DXFImport()
{
  file= 0;
  blocks= 0;
  nBlocks= 0;
  curBlock= 0;
  dxfExport= 0;
  transform= 0;
  curColor= Point( -1., -1., -1. );
  tmp_filename[0]= 0;
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::init( FILE* dxfFile,
                      DXFExport* _dxfExport,
                      FILE* _fstderr /*= 0*/ )
{
  fstderr= _fstderr;
  dxfReader.init( dxfFile, this, _fstderr );
  dxfExport= _dxfExport;

  file= 0;
  blocks= 0;
  nBlocks= 0;
  curBlock= 0;
  transform= 0;
  curColor= Point( -1., -1., -1. );
  curLayer= 0; // i.e. default Layer

  maxPoint= Point(1e30,1e30,1e30);
  minPoint= Point(-1e30,-1e30,-1e30);
  isoPoint= Point(0,0,0);
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::destroy()
{
  if( file != 0 )
    {
    fclose( file );
    file= 0;
    }
  while( blocks != 0 )
    {
    BLOCK* next= blocks->next;
    if( blocks->name != 0 )
      {
      free( blocks->name );
      blocks->name= 0;
      }
    delete blocks;
    blocks= next;
    }
  nBlocks= 0;
  curBlock= 0;

  if( tmp_filename[0] != 0 )
    unlink( tmp_filename );
  tmp_filename[0]= 0;
}

/////////////////////////////////////////////////////////////////////////////

DXFImport::BLOCK* DXFImport::getBlock( int i )
{
  BLOCK* b= blocks;
  assert( i < nBlocks );
  while( i-- != 0 )
    b= b->next;
  return b;
}

/////////////////////////////////////////////////////////////////////////////

DXFImport::BLOCK* DXFImport::findBlock( const char* name )
{
  BLOCK* b= blocks;
  while( b != 0 )
    {
    if( strcmp( b->name, name ) == 0 )
      break;
    b= b->next;
    }
  return b;
}

/////////////////////////////////////////////////////////////////////////////

DXFImport::BLOCK* DXFImport::lastBlock()
{
  assert( blocks != 0 );

  BLOCK* b= blocks;
  while( b->next != 0 )
    b= b->next;
  return b;
}

/////////////////////////////////////////////////////////////////////////////

DXFImport::BLOCK* DXFImport::newBlock( const char* name )
{
  assert( findBlock( name ) == 0 );

  BLOCK* b= new BLOCK;
  assert( b != 0 );

  b->pos= -1;
  b->name= strdup( name );
  b->next= 0;
  b->id= nBlocks+1;
  b->empty= false;
  b->anonymous= false;

  ++nBlocks;
  return b;
}

/////////////////////////////////////////////////////////////////////////////

DXFImport::BLOCK* DXFImport::appendBlock( const char* name )
{
  BLOCK* b= newBlock( name );

  if( blocks == 0 )
    blocks= b;
   else
    lastBlock()->next= b;
  return b;
}

/////////////////////////////////////////////////////////////////////////////

// test is block blk is before block 'before_this'
boolean DXFImport::blockIsBefore( BLOCK* blk, BLOCK* before_this )
{
  assert( blocks != 0 );

  BLOCK* b= blocks;
  while( b != before_this )
    {
    assert( b != 0 );
    if( b == blk )
      return true;
    b= b->next;
    }
  return false;
}

/////////////////////////////////////////////////////////////////////////////

DXFImport::BLOCK* DXFImport::previousBlock( BLOCK* blk )
{
  assert( blk != blocks );

  BLOCK* b= blocks;
  while( b->next != blk )
    {
    b= b->next;
    assert( b != 0 );
    }
  return b;
}

/////////////////////////////////////////////////////////////////////////////

// insert block blk before block 'before_this'
void DXFImport::insertBeforeBlock( BLOCK* blk, BLOCK* before_this )
{
  if( before_this == blocks )
    {
    blocks= blk;
    blk->next= before_this;
    }
   else
    {
    previousBlock( before_this )->next= blk;
    blk->next= before_this;
    }
}

/////////////////////////////////////////////////////////////////////////////

// new block 'name' before existing block 'before_this'
DXFImport::BLOCK* DXFImport::newBeforeBlock( BLOCK* before_this, const char* name )
{
  BLOCK* newb= newBlock( name );
  insertBeforeBlock( newb, before_this );
  return newb;
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::detachBlock( BLOCK* b )
{
  if( b == blocks )
    blocks= b->next;
   else
    previousBlock( b )->next= b->next;
  b->next= 0;
}

/////////////////////////////////////////////////////////////////////////////

//
// block b depends on block 'name'
// then: + creates block name before b is it dones'nt exist
//   or: + detach existing block 'name' and reinsert it before 'b'
//
void DXFImport::dependsOnBlock( BLOCK* b, const char* name )
{
  BLOCK* sub= findBlock( name );
  if( sub == 0 )
    newBeforeBlock( b, name );
   else
    {
    assert( blocks != 0 );
    if( blockIsBefore( sub, b ) ) // already before
      return;
    detachBlock( sub );
    insertBeforeBlock( sub, b );
    }
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::writePoint( const Point& p )
{
  writeDouble( p.x() );
  writeDouble( p.y() );
  writeDouble( p.z() );
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::writeVector( const Vector& v )
{
  writeDouble( v.x() );
  writeDouble( v.y() );
  writeDouble( v.z() );
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::readPoint( Point& p )
{
  p.setx( readDouble() );
  p.sety( readDouble() );
  p.setz( readDouble() );
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::readVector( Vector& v )
{
  v.setx( readDouble() );
  v.sety( readDouble() );
  v.setz( readDouble() );
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::test_SET_COLOR()
{
  if( curColor != dxfReader.getColor() )
    {
    curColor= dxfReader.getColor();
    writeInteger( SET_COLOR_ID );
    writePoint( curColor );
    }
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::POINT( const Point& p )
{
  test_SET_COLOR();
  writeInteger( POINT_ID );
  writeInteger( dxfReader.getLayer()->index );
  writePoint( p );
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::CIRCLE( const Point& center, double radius )
{
  test_SET_COLOR();
  writeInteger( CIRCLE_ID );
  writeInteger( dxfReader.getLayer()->index );
  writePoint( center );
  writeDouble( radius );
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::CYLINDER( const Point& center, double radius,
                          const Vector& length )
{
  test_SET_COLOR();
  writeInteger( CYLINDER_ID );
  writeInteger( dxfReader.getLayer()->index );
  writePoint( center );
  writeDouble( radius );
  writePoint( center + length );
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::FACE( const Point& p1, const Point& p2, const Point& p3 )
{
  test_SET_COLOR();
  writeInteger( FACE3_ID );
  writeInteger( dxfReader.getLayer()->index );
  writePoint( p1 );
  writePoint( p2 );
  writePoint( p3 );
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::FACE( const Point& p1, const Point& p2,
                      const Point& p3, const Point& p4 )
{
  test_SET_COLOR();
  writeInteger( FACE4_ID );
  writeInteger( dxfReader.getLayer()->index );
  writePoint( p1 );
  writePoint( p2 );
  writePoint( p3 );
  writePoint( p4 );
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::POLYLINE( int nVertices, const Point* points,
                          boolean closed )
{
  test_SET_COLOR();
  writeInteger( POLYLINE_ID );
  writeInteger( dxfReader.getLayer()->index );
  writeInteger( nVertices );
  writeInteger( (closed) ? 1 : 0 );
  for( int i= 0; i< nVertices; ++i )
    writePoint( points[i] );
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::MESHSURFACE( const Point* vertices,
                             int M, int N,
                             boolean closed_M, boolean closed_N )
{
  test_SET_COLOR();
  writeInteger( MESHSURFACE_ID );
  writeInteger( dxfReader.getLayer()->index );
  writeInteger( M );
  writeInteger( (closed_M) ? 1 : 0 );
  writeInteger( N );
  writeInteger( (closed_N) ? 1 : 0 );
  for( int n= 0; n< N; ++n )
    for( int m= 0; m< M; ++m )
      writePoint( vertices[m+n*M] );
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::insert_BLOCK( const char* name,
                              const Point& insertionPoint,
                              const Vector& scale,
                              double rotationAngle )
{
  test_SET_COLOR();
  writeInteger( INSERT_ID );
  writeInteger( dxfReader.getLayer()->index );
  writePoint( insertionPoint );
  writeVector( scale );
  writeDouble( rotationAngle );
  writeInteger( strlen( name )+1 );
  fwrite( (const void*) name, strlen(name)+1, 1, file );

  // now, say: current block depends on block 'name'
  assert( curBlock != 0 );
  dependsOnBlock( curBlock, name );
  curColor= Point( -1., -1., -1. );
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::start_BLOCK( const char* name, const Point& basePoint )
{
  assert( curBlock == 0 );

  BLOCK* b= findBlock( name );
  if( b == 0 )
    b= appendBlock( name );
  curBlock= b;
  assert( b->pos == -1 );
  b->base= basePoint;
  b->anonymous= (dxfReader.group70() & 1) != 0;

  if( fstderr != 0 )
    fprintf( fstderr, "reading block %s\n", name );
  b->pos= ftell( file );
  curColor= Point( -1., -1., -1. );
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::end_BLOCK()
{
  assert( curBlock != 0 );

  writeInteger( BLOCK_END_ID );

  curBlock= 0;
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::start_MAIN()
{
  start_BLOCK( MAIN_BLOCK_NAME, Point(0.,0.,0.) );
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::end_MAIN()
{
  end_BLOCK();
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::transformPoint( Point& p1 )
{
  if( ! explodeBlocks )
    return;

  TRANSFORM* t= transform;
  while( t != 0 )
    {
    p1-= t->base;
    p1= Point( p1.x() * t->scale.x(),
               p1.y() * t->scale.y(),
               p1.z() * t->scale.z() );
    p1= Point( t->cosa * p1.x() - t->sina * p1.y(),
               t->sina * p1.x() + t->cosa * p1.y(),
               p1.z() );
    p1+= t->translate;
    t= t->next;
    }
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::read()
{
  assert( file == 0 );

// this is a buggy version with 'tmpfile()':
//  it seems that 'file' is not always open in binary mode...
// file= tmpfile();

  if( tmp_filename[0] != 0 )
    unlink( tmp_filename );

  tmpnam( tmp_filename );
  file= fopen( tmp_filename, "wb" );

  if( file == 0 )
    {
    fprintf( stderr, "unable to open file '%s'\n'", tmp_filename );
    fflush(stderr);
    assert(false);
    }

  minPoint= Point( 1e30, 1e30, 1e30 );
  maxPoint= Point(-1e30,-1e30,-1e30 );
  isoPoint= Point(0,0,0);

  assert( curBlock == 0 );
  dxfReader.read();
  assert( curBlock == 0 );

  fclose( file );
  file= 0;
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::exportBlock( BLOCK* b )
{
  boolean mainBlock= strcmp( b->name, MAIN_BLOCK_NAME ) == 0;

  if( b->pos == -1 || b->empty ) // oooh, an empty block!
    return;

  fseek( file, b->pos, SEEK_SET );

  if( mainBlock )
    dxfExport->start_MAIN();
   else
    if( ! explodeBlocks )
      dxfExport->start_BLOCK( b->name, b->base );

  for(;;)
    {
    int id= readInteger();
    if( id == BLOCK_END_ID )
      break;
    switch( id )
      {
      case POINT_ID:
        {
        Point p;
        curLayer= readInteger();
        readPoint( p );
        transformPoint(p);
        dxfExport->POINT(p);
        break;
        }
      case CIRCLE_ID:
        {
        Point center;
        DBL radius;
        curLayer= readInteger();
        readPoint( center );
        radius= readDouble();
        // note: if blocks are exploded, transformation is not correct
        transformPoint(center);
        dxfExport->CIRCLE( center, radius );
        break;
        }
      case CYLINDER_ID:
        {
        Point center;
        DBL radius;
        Point p2;
        curLayer= readInteger();
        readPoint( center );
        radius= readDouble();
        readPoint( p2 );
        // note: if blocks are exploded, transformation is not correct
        transformPoint(center);
        transformPoint(p2);
        dxfExport->CYLINDER( center, radius, p2 - center );
        break;
        }
      case FACE3_ID:
        {
        Point p1, p2, p3;
        curLayer= readInteger();
        readPoint( p1 );
        readPoint( p2 );
        readPoint( p3 );
        transformPoint(p1);
        transformPoint(p2);
        transformPoint(p3);
        dxfExport->FACE( p1, p2, p3 );
        break;
        }
      case FACE4_ID:
        {
        Point p1, p2, p3, p4;
        curLayer= readInteger();
        readPoint( p1 );
        readPoint( p2 );
        readPoint( p3 );
        readPoint( p4 );
        transformPoint(p1);
        transformPoint(p2);
        transformPoint(p3);
        transformPoint(p4);
        dxfExport->FACE( p1, p2, p3, p4 );
        break;
        }
      case POLYLINE_ID:
        {
        int nVertices, closed;
        Point* points= 0;

        curLayer= readInteger();
        nVertices= readInteger();

        closed= readInteger();
        points= new Point [ nVertices ];
        assert( points != 0 );

        for( int i= 0; i< nVertices; ++i )
          {
          readPoint( points[i] );
          transformPoint( points[i] );
          }
        dxfExport->POLYLINE( nVertices, points, closed==1 );
        delete points;
        break;
        }
      case MESHSURFACE_ID:
        {
        int M, N, closed_M, closed_N;
        Point* points= 0;
        curLayer= readInteger();
        M= readInteger();
        closed_M= readInteger();
        N= readInteger();
        closed_N= readInteger();

        points= new Point [ M * N ];
        assert( points != 0 );
        for( int n= 0; n< N; ++n )
          for( int m= 0; m< M; ++m )
            {
            int i= m + n*M;
            readPoint( points[i] );
            transformPoint( points[i] );
            }

        dxfExport->MESHSURFACE( points, M, N, closed_M==1, closed_N==1 );

        delete points;
        break;
        }
      case INSERT_ID:
        {
        Point insertionPoint;
        Vector scale;
        DBL rotationAngle;
        curLayer= readInteger();
        readPoint( insertionPoint );
        readVector( scale );
        rotationAngle= readDouble();
        int len= readInteger();

        char* name= (char*) malloc( len );
        fread( (void*) name, len, 1, file );

        BLOCK* b= findBlock( name );
        assert( b != 0 );
        if( !b->empty && b->pos!=-1 )
          {
          if( ! explodeBlocks )
            dxfExport->insert_BLOCK( name, insertionPoint,
                                     scale, rotationAngle );
           else
            {
            if( fstderr != 0 )
              fprintf( fstderr, "exploding block %s\n", name );
            dxfExport->start_exploding_BLOCK( name, b->id );
            TRANSFORM* t= new TRANSFORM;
            t->scale= scale;
            t->sina= sin(DEGtoRAD(rotationAngle));
            t->cosa= cos(DEGtoRAD(rotationAngle));
            t->translate= Vector( insertionPoint );
            t->base= Vector(b->base);
            t->next= transform;
            transform= t;

            long save_pos= ftell( file );

            exportBlock( b );

            fseek( file, save_pos, SEEK_SET );
            transform= transform->next;
            delete t;

            dxfExport->end_exploding_BLOCK( name, b->id );
            }
          }

        free( name );
        break;
        }
      case SET_COLOR_ID:
        {
        readPoint( curColor );
        break;
        }
      default:
        assert( false );
      }
    }

  if( mainBlock )
    dxfExport->end_MAIN();
   else
    if( ! explodeBlocks )
      dxfExport->end_BLOCK();
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::go_exporting()
{
  file= fopen( tmp_filename, "rb" );
  assert( file != 0 );

  if( explodeBlocks )
    {
    BLOCK* b= findBlock( MAIN_BLOCK_NAME );
//    assert( b != 0 );
if( b == 0 )
  {
  BLOCK* b= blocks;
  fprintf( stderr, "DXFError: Read Blocks are:\n" );
  while( b != 0 )
    {
    fprintf( stderr, "'%s'\n", b->name );
    b= b->next;
    }
  return;
  }
    exportBlock( b );
    }
   else
    {
    BLOCK* b= blocks;
    while( b != 0 )
      {
      exportBlock( b );
      b= b->next;
      }
    }

  fclose( file );
  file= 0;
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::exportAll()
{
  explodeBlocks= ! dxfExport->supportBlocks();

  go_exporting();

  if( tmp_filename[0] != 0 )
    unlink( tmp_filename );
  tmp_filename[0]= 0;
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::computeBoundings()
{
  DXFExport* save_dxfExport= dxfExport;

  DXFBound* dxfBound= new DXFBound( nCircleSubdivide() );

  dxfExport= dxfBound;
  dxfBound->init();

  explodeBlocks= true;

  go_exporting();

  dxfExport= save_dxfExport;

  minPoint= dxfBound->getMinPoint();
  maxPoint= dxfBound->getMaxPoint();
  isoPoint= dxfBound->getIsoPoint();

  delete dxfBound;
  dxfBound= 0;
}

/////////////////////////////////////////////////////////////////////////////

Point DXFImport::getMinPoint() const
{
  return minPoint;
}

/////////////////////////////////////////////////////////////////////////////

Point DXFImport::getMaxPoint() const
{
  return maxPoint;
}

/////////////////////////////////////////////////////////////////////////////

Point DXFImport::getIsoPoint() const
{
  return isoPoint;
}

/////////////////////////////////////////////////////////////////////////////

void DXFImport::setEmptyBlock( const char* name )
{
  BLOCK* b= findBlock( name );
  assert( b != 0 );
  b->empty= true;
}

/////////////////////////////////////////////////////////////////////////////

