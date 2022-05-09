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
  DXFExport.C

  Revision:
    Stephane Rehel
    April 25 1997
*/

#include <stdio.h>

#include "tools/poly2tri.h"

#include "DXFExport.h"

/////////////////////////////////////////////////////////////////////////////

Point* DXFExport::doCircle( int n, const Point& center, double radius )
{
  Point* points= new Point [ n ];
  assert( points != 0 );

  for( int i= 0; i< n; ++i )
    {
    double a= double(i) * M_PI*2. / double(n);
    points[i]= center + Vector( radius*cos(a), radius*sin(a), 0. );
    }
  return points;
}

/////////////////////////////////////////////////////////////////////////////

void DXFExport::CIRCLE( const Point& center, double radius )
{
  int n= nCircleSubdivide();
  if( n == 0 )
    return;
  Point* points= doCircle( n, center, radius );

/*
ancienne version burlesque...

  for( int i= 1; i<= n-2; ++i )
    FACE( points[0], points[i], points[i+1] );
*/
#ifdef DXF_DEBUG
  fprintf( stderr, "DXFExport: exploding CIRCLE in %d faces\n", n );
#endif
  for( int i= 0; i< n; ++i )
    {
    const Point& p1= points[ i ];
    const Point& p2= points[ (i+1) % n ];

    FACE( center, p1, p2 );
    }

  delete points;
}

/////////////////////////////////////////////////////////////////////////////

// cylinder is closed both on top and bottom
void DXFExport::CYLINDER( const Point& center, double radius,
                          const Vector& length )
{
  int n= nCircleSubdivide();
  if( n == 0 )
    return;
  Point* points= doCircle( n, center, radius );

#ifdef DXF_DEBUG
  fprintf( stderr, "DXFExport: exploding CYLINDER in %d faces\n", n );
#endif

  for( int i= 0; i< n; ++i )
    {
    const Point& p1= points[ i ];
    const Point& p2= points[ (i+1) % n ];

    FACE( center, p2, p1 );
    FACE( center + length, p1 + length , p2 + length );

    FACE( p1, p2, p2 + length, p1 + length );
    }

  delete points;
}

/////////////////////////////////////////////////////////////////////////////

void DXFExport::arc( const Point& center, double radius,
                     double startAngle, double endAngle,
                     const Vector& extrusionDir )
{
  int n= nCircleSubdivide();
  if( n == 0 )
    return;
  n= int( n * (startAngle-endAngle) / 360. );
  if( n < 2 )
    n= 2;

#ifdef DXF_DEBUG
  fprintf( stderr, "DXFExport: exploding ARC in a %d points polyline\n", n );
#endif

  Point* points= new Point [ n+1 ];
  assert( points != 0 );

  for( int i= 0; i<= n; ++i )
    {
    double a= startAngle + double(i) * (startAngle-endAngle) / double(n);
    points[i]= center + Vector( radius*cos(a), radius*sin(a), 0. );
    }

  polylineThickness( n+1, points, false, extrusionDir );
  delete points;
}

/////////////////////////////////////////////////////////////////////////////

/*
void DXFExport::TRACE( const Point& p1, const Point& p2,
                       const Point& p3, const Point& p4 )
{
  Point points[4]= { p1, p2, p3, p4 };
  polygon( 4, points, true );
}
*/

/////////////////////////////////////////////////////////////////////////////

void DXFExport::polylineThickness( int nVertices, const Point* points,
                                   boolean closed,
                                   const Vector& extrusionDir )
{
  if( extrusionDir.norm() < 1e-30 )
    {
    POLYLINE( nVertices, points, closed );
    return;
    }

  Point* p= new Point [ nVertices * 2 ];
  assert( p != 0 );

  for( int i= 0; i< nVertices; ++i )
    {
    p[i]= points[i];
    p[i+nVertices]= points[i] + extrusionDir;
    }
  MESHSURFACE( p, 2,nVertices,  closed, false );
  delete p;
}

/////////////////////////////////////////////////////////////////////////////

void DXFExport::MESHSURFACE( const Point* vertices,
                             int M, int N,
                             boolean closed_M, boolean closed_N )
{
  #define getv(m,n) (vertices[ ((m)%M) + M * ((n)%N) ])

#ifdef DXF_DEBUG
  fprintf( stderr, "DXFExport: exploding MESHSURFACE %dx%d faces\n", M, N );
#endif

  Point p1, p2, p3, p4;
  int end_M= (closed_M==1) ? M : (M-1);
  int end_N= (closed_N==1) ? N : (N-1);
  for( int n= 0; n< end_N; ++n )
    for( int m= 0; m< end_M; ++m )
      {
      p1= getv( m  , n   );
      p2= getv( m+1, n   );
      p3= getv( m+1, n+1 );
      p4= getv( m  , n+1 );
      FACE( p1, p2, p3, p4 );
      }

  #undef getv
}

/////////////////////////////////////////////////////////////////////////////

void DXFExport::SOLID( const Point& p1, const Point& p2,
                       const Point& p3, const Point& p4 )
{
#ifdef DXF_DEBUG
  fprintf( stderr, "DXFExport: converting SOLID4 to FACE4\n" );
#endif
  FACE( p1, p2, p3, p4 );
}

/////////////////////////////////////////////////////////////////////////////

void DXFExport::SOLID( const Point& p1, const Point& p2, const Point& p3 )
{
#ifdef DXF_DEBUG
  fprintf( stderr, "DXFExport: converting SOLID3 to FACE3\n" );
#endif
  FACE( p1, p2, p3 );
}

/////////////////////////////////////////////////////////////////////////////

void DXFExport::POINT( const Point& /* p */ )
{
#ifdef DXF_DEBUG
  fprintf( stderr, "DXFExport: unused POINT\n" );
#endif
}

/////////////////////////////////////////////////////////////////////////////

void DXFExport::FACE( const Point& /* p1 */ , const Point& /* p2 */,
                      const Point& /* p3 */ )
{
#ifdef DXF_DEBUG
  fprintf( stderr, "DXFExport: unused FACE3\n" );
#endif
}

/////////////////////////////////////////////////////////////////////////////

void DXFExport::FACE( const Point& /* p1 */, const Point& /* p2 */,
                      const Point& /* p3 */, const Point& /* p4 */ )
{
#ifdef DXF_DEBUG
  fprintf( stderr, "DXFExport: unused FACE4\n" );
#endif
}

/////////////////////////////////////////////////////////////////////////////

void DXFExport::POLYLINE( int nVertices,
                          const Point* points,
                          boolean closed )
{
/*
  if( ! closed )
    {
#ifdef DXF_DEBUG
    fprintf( stderr, "DXFExport: unused %sPOLYLINE(%d)\n",
             closed ? "closed " : "", nVertices );
#endif
    }
*/

#ifdef DXF_DEBUG
  if( ! closed )
    fprintf( stderr, "DXFExport: converting unclosed POLYLINE(%d) into ",
             nVertices );
   else
    fprintf( stderr, "DXFExport: converting %sPOLYLINE(%d) into ",
             closed ? "closed " : "", nVertices );
#endif

  Point* pts= new Point [ nVertices ];
  int i;

  for( i= 0; i < nVertices; ++i )
    pts[i]= points[i];

  int nFaces= 0;
  Point p1, p2, p3;
  while( remove_triangle( pts, nVertices, p1, p2, p3 ) )
    {
    FACE( p1, p2, p3 );
    ++nFaces;
    }

  delete pts;
  pts= 0;

#ifdef DXF_DEBUG
  fprintf( stderr, "%d FACE3\n", nFaces );
#endif
}

/////////////////////////////////////////////////////////////////////////////

