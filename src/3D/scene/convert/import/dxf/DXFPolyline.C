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
  DXFPolyline.C

  Revision:
    Stephane Rehel
    April 25 1997
*/

#include <stdlib.h>

#include "DXFReader.h"
#include "DXFExport.h"

/////////////////////////////////////////////////////////////////////////////

const int DXFPolyline::maxVertices= 1000;

// number of vertices drawing an arc, including first & last points
// must be >= 2
const int DXFPolyline::nArcSubdivide= 10;

/////////////////////////////////////////////////////////////////////////////

boolean DXFPolyline::addVertex( const Point& thePoint,
                              int theFlags,
                              float theBulge,
                              int index0,
                              int index1,
                              int index2,
                              int index3 )
{
  if( nVertices >= DXFPolyline::maxVertices )
    {
    if( fstderr != 0 )
      {
      fprintf( fstderr, "too many vertices (>%d) in polyline\n", maxVertices );
      fflush(fstderr);
      }
    return false;
    }
  vertices[nVertices].p= thePoint;
  vertices[nVertices].flags= theFlags;
  vertices[nVertices].bulge= theBulge;
  vertices[nVertices].indices[0]= abs( index0 );
  vertices[nVertices].indices[1]= abs( index1 );
  vertices[nVertices].indices[2]= abs( index2 );
  vertices[nVertices].indices[3]= abs( index3 );
  ++nVertices;
  return true;
}

/////////////////////////////////////////////////////////////////////////////

int DXFPolyline::getPolyMeshVertex( int i )
{
  assert( i >= 1 );
  assert( i <= M ); // M= number of vertices defined in polyline section

  int n= -1;
  while( i-- )
    {
    ++n;
    assert( n < nVertices );
    while( !(vertices[n].isPolyMeshVertex() &&
             vertices[n].isMeshSurfaceVertex()) )
      {
      ++n;
      assert( n < nVertices );
      }
    }
  return n;
}

/////////////////////////////////////////////////////////////////////////////

DXFPolyline::Vertex* DXFPolyline::getMesh( int i )
{
  assert( i >= 1 );
  assert( i <= N ); // N= number of meshes defined in polyline section

  int n= -1;
  while( i-- )
    {
    ++n;
    assert( n < nVertices );
    while( !(vertices[n].isPolyMeshVertex() &&
             !vertices[n].isMeshSurfaceVertex()) )
      {
      ++n;
if( n >= nVertices ) return vertices; //*** ERROR!!
      assert( n < nVertices );
      }
    }
  return vertices + n;
}

/////////////////////////////////////////////////////////////////////////////

int DXFPolyline::getMeshSurfaceVertex( int m, int n )
{
  if( m == M+1 ) // for M-closed surfaces, on aurait tord de s'en priver
    m= 1;
  if( n == N+1 ) // for N-closed surfaces, on aurait tord de s'en priver
    n= 1;
  assert( m >= 1 ); // c'est peut-etre un detail pour vous
  assert( m <= M );
  assert( n >= 1 ); // mais pour moi ca veut dire beaucoup
  assert( n <= N ); // (i.e. no bug!!!)
  return (m-1)*N + (n-1);
}

/////////////////////////////////////////////////////////////////////////////

boolean DXFPolyline::allocPoints( int n )
{
  assert( points == 0 );
  assert( n >= 1 );
  points= new Point [ n ];
  nPoints= 0;
  maxPoints= n;
  return points != 0;
}

/////////////////////////////////////////////////////////////////////////////

void DXFPolyline::freePoints()
{
  assert( points != 0 );
  delete points;
  points= 0;
}

/////////////////////////////////////////////////////////////////////////////

void DXFPolyline::addPoint( const Point& p )
{
  assert( points != 0 );
  assert( nPoints < maxPoints );
  points[ nPoints++ ]= p;
}

/////////////////////////////////////////////////////////////////////////////

//
// Note: Polyline Vertex
//
// BULGE is the ratio between height and width of the arc:
//
//                /--------`
//              /     |      `
//            /       | h      `
//           /   d    |         `       bulge = h/d
//          ++++++++++++++++++++++
//
//                    +
//                  center of arc
//
// bulge is negative if the arc goes clockwise from the start point to
// the endpoint; a bulge of 0 indicates a straight segment.
//
boolean DXFPolyline::writeVertex( int i )
{
  DXFPolyline::Vertex* pv= getVertex( i );
  Point p1= pv->p;
  if( pv->bulge == 0. )
    {
    addPoint( p1 );
    return true;
    }
  Point& p2= (i+1==nVertices) ? getVertex( 0 )->p
                              : getVertex( i+1 )->p;
  Vector corde= p2 - p1;
  double c= corde.norm();

//  assert( c != 0. );
  if( c == 0. )
    {
    if( fstderr != 0 )
      fprintf( fstderr,
               "Warning: c==0 in DXFPolyline::writeVertex()\n" );
    return true;
    }

  Vector n= Vector( corde.y(), -corde.x(), 0. ) / c;
  double d= c / 2.;
  double h= pv->bulge * d;
  double radius= (sqr(d)+sqr(h)) / (2.*h);
  Point center= (p1+p2)/2. + n * ( h - radius );

  double alpha= 2.*atan2( d, radius - h );
  if( alpha > M_PI ) alpha-= M_PI*2.;
  double d_alpha= alpha / double(nArcSubdivide-1);
  double cosa= cos( d_alpha );
  double sina= sin( d_alpha );
  Vector rotx= Vector( cosa,-sina, 0. );
  Vector roty= Vector( sina, cosa, 0. );
  Vector v= p1 - center; // le rayon qui parcourt l'arc

  for( int j= 2; j<= nArcSubdivide; ++j )
    {
    v= Vector( v | rotx, v | roty, 0. );
    p2= center + v;
    addPoint( p1 );
    p1= p2;
    }
  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean DXFPolyline::writePolyline3D( DXFExport* dxfExport )
{
  int i;

  int totalVertices= 0;

  for( i= 0; i< nVertices; ++i )
    totalVertices+= getVertexLength( i );

  if( !allocPoints( totalVertices ) )
    return false;

  for( i= 0; i< nVertices; ++i )
    if( !writeVertex( i ) )
      return false;
  if( nPoints >= 2 )
    dxfExport->polylineThickness( nPoints, points, isClosed(),
                                  extrusionDir * thickness );

  freePoints();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean DXFPolyline::writePolyMesh( DXFExport* dxfExport )
{
  // grrrr, sometimes (!?) groups 71 and 72 are NOT defined
  // in a polyface mesh object
  // let's find them... remember: P&L!
  if( N == 0 || M == 0 )
    {
    for(;;)
      {
      assert( M < nVertices );
      if( !vertices[M].isPolyMeshVertex() ||
          !vertices[M].isMeshSurfaceVertex() )
        break;
      ++M;
      }
    N= nVertices - M;
    assert( N >= 1 );
    }
if( N <= 0 || M <= 0 )
  return true;
  for( int i= 0; i< N; ++i )
    {
    Vertex* v= getMesh( i+1 );
    int k;

    int totalVertices= 0;
    int nIndices= (v->indices[3]==0) ? 3 : 4;
//***
//printf( "%d/%d %d %d\n",i,N,M,v->indices[3]);
    for( k= 0; k < nIndices; ++k )
      totalVertices+= getVertexLength( getPolyMeshVertex( v->indices[k] ) );
    if( !allocPoints( totalVertices ) )
      return false;

    for( k= 0; k< nIndices; ++k )
      writeVertex( getPolyMeshVertex( v->indices[k] ) );

    if( nPoints == 3 )
      dxfExport->FACE( points[0], points[1], points[2] );
     else
      if( nPoints == 4 )
        dxfExport->FACE( points[0], points[1], points[2], points[3] );
       else
        dxfExport->polylineThickness( nPoints,
                                      points,
                                      true, // a mesh is always closed...
                                      extrusionDir * thickness
                                    );
    freePoints();
    }
  return true;
}

/////////////////////////////////////////////////////////////////////////////

//
// Export a mesh surface= english('surface maillee');
// We assume there's no bulge!=0 vertex in such a surface
// And also, if we look m as x coordinate, and n as the y one,
// normal follows z direction
//
//       normal to the surface
//              /|`
//               |
//               |    n
//               |   /
//               |  /
//               | /
//               |/
//               +-------------- m
//             (1,1)
//
boolean DXFPolyline::writeMeshSurface( DXFExport* dxfExport )
{
  if( group75 == 5 )
    {
    if( fstderr != 0 )
      {
      fprintf( fstderr, "Warning: square B-Spline surface not supported!\n" );
      fflush(fstderr);
      }
    return true;
    }
  if( group75 == 6 )
    {
    if( fstderr != 0 )
      {
      fprintf( fstderr, "Warning: cubic B-Spline surface not supported!\n" );
      fflush(fstderr);
      }
    return true;
    }
  if( group75 == 8 )
    {
    if( fstderr != 0 )
      {
      fprintf( fstderr, "Warning: Bezier surface not supported!\n" );
      fflush(fstderr);
      }
    return true;
    }
  if( group75 != 0 )
    {
    if( fstderr != 0 )
      {
      fprintf( fstderr, "Warning: Unknown group 75 integer (%d) in polyline command\n",
               group75 );
      fflush(fstderr);
      }
    return true;
    }

//  if( dxfExport->supportMeshSurface() )
    {
    Point* points= new Point [ M * N ];
    assert( points != 0 );
    Point* p= points;
    for( int n= 1; n<= N; ++n )
      for( int m= 1; m<= M; ++m, ++p )
        *p= getVertex( getMeshSurfaceVertex(m,n) )->p;

    dxfExport->MESHSURFACE( points, M, N, isClosedM(), isClosedN() );
    delete points;
    return true;
    }

/*
  // one mesh:
  Point p1, p2, p3, p4;

  int end_M= (isClosedM()) ? M : (M-1);
  int end_N= (isClosedN()) ? N : (N-1);
  for( int n= 1; n<= end_N; ++n )
    for( int m= 1; m<= end_M; ++m )
      {
      p1= getVertex( getMeshSurfaceVertex( m  , n   ) )->p;
      p2= getVertex( getMeshSurfaceVertex( m+1, n   ) )->p;
      p3= getVertex( getMeshSurfaceVertex( m+1, n+1 ) )->p;
      p4= getVertex( getMeshSurfaceVertex( m  , n+1 ) )->p;

      dxfExport->FACE( p1, p2, p3, p4 );
      }
  return true;
*/
}

/////////////////////////////////////////////////////////////////////////////
