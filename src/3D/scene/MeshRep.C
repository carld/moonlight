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
   MeshRep.C

   November 10 1996
   Stephane Rehel
*/

#include <stdio.h>
#include <math.h>

#include "tools/CommonMath.h"
#include "tools/Sorts.h"
#include "tools/BoundingSphereBuilder.h"

#include "tools/3D/MLVertex.h"
#include "tools/3D/MLEdge.h"
#include "tools/3D/MLPolygon.h"

#include "MeshRep.h"

#include "MLHashBox.h"

#ifndef NDEBUG
// #define MESHREP_DEBUG 1
#endif

//#define MESHREP_PROFILE

#ifdef MESHREP_PROFILE
#include "tools/Chrono.h"
#endif

/////////////////////////////////////////////////////////////////////////////

MeshRep::MeshRep()
{
  nVertices= 0;
  nEdges= 0;
  nPolygons= 0;
  nSVertices= 0;

  discAngle= 45.;
  cos_discAngle= cos(DEGtoRAD(discAngle));
  smallDistance= 1e-6;

  hashBox= 0;

  dirtyBoundings= false;
  dirtyHashBox= false;

  bbox= Box( Point(0,0,0), Point(1,1,1) * 1e-8 );
  boundingSphere= BoundingSphere( Point(0,0,0), 1e-8 );
}

/////////////////////////////////////////////////////////////////////////////

MeshRep::~MeshRep()
{
  clear();
}

/////////////////////////////////////////////////////////////////////////////

int MeshRep::getSizeOf() const
{
  int s=  sizeof(*this);
  int i;

  for( i= 1; i <= vertices.length(); ++i )
    s+= vertices.get(i).getSizeOf();
  for( i= 1; i <= svertices.length(); ++i )
    s+= svertices.get(i).getSizeOf();
  for( i= 1; i <= edges.length(); ++i )
    s+= edges.get(i).getSizeOf();
  for( i= 1; i <= polygons.length(); ++i )
    s+= polygons.get(i).getSizeOf();

  if( hashBox != 0 )
    s+= hashBox->getSizeOf();

#ifndef NDEBUG
/*
  fprintf( stderr, "Size: v=%d e=%d sv=%d p=%d hb=[%dx%dx%d]=%d\n",
           vertices.getSizeOf(),
           edges.getSizeOf(),
           svertices.getSizeOf(),
           polygons.getSizeOf(),
*/
  fprintf( stderr, "Size: hb=[%dx%dx%d]=%d\n",
           (hashBox!=0) ? (hashBox->getX()) : 0,
           (hashBox!=0) ? (hashBox->getY()) : 0,
           (hashBox!=0) ? (hashBox->getZ()) : 0,
           (hashBox!=0) ? (hashBox->getSizeOf()) : 0 );
#endif

/*
  for( i= 1; i <= vertices.length(); ++i )
    {
    const MLVertex& v= vertices.get(i);
    if( v.allocated() )
      s+= v.getSizeOf() - sizeof(v);
    }
  for( i= 1; i <= edges.length(); ++i )
    {
    const MLEdge& e= edges.get(i);
    if( e.allocated() )
      s+= e.getSizeOf() - sizeof(e);
    }
  for( i= 1; i <= svertices.length(); ++i )
    {
    const MLSVertex& sv= svertices.get(i);
    if( sv.allocated() )
      s+= sv.getSizeOf() - sizeof(sv);
    }
  for( i= 1; i <= polygons.length(); ++i )
    {
    const MLPolygon& p= polygons.get(i);
    if( p.allocated() )
      s+= p.getSizeOf() - sizeof(p);
    }
*/
  return s;
}

/////////////////////////////////////////////////////////////////////////////

MeshRep* MeshRep::duplicate()
{
  MeshRep* mesh= new MeshRep;

  mesh->discAngle= discAngle;
  mesh->cos_discAngle= cos_discAngle;
  mesh->smallDistance= smallDistance;
  mesh->bbox= bbox;
  mesh->boundingSphere= boundingSphere;

  mesh->nVertices= nVertices;
  for( int i= 1; i <= vertices.length(); ++i )
    {
    MLVertex& v= mesh->vertices.append();
    v= vertices[i];
    }

  mesh->nEdges= nEdges;
  for( int i= 1; i <= edges.length(); ++i )
    {
    MLEdge& e= mesh->edges.append();
    e= edges[i];
    }

  mesh->nPolygons= nPolygons;
  for( int i= 1; i <= polygons.length(); ++i )
    {
    MLPolygon& p= mesh->polygons.append();
    p= polygons[i];
//    p.mesh= mesh;
    p.index= mesh->polygons.length();
    }

  mesh->nSVertices= nSVertices;
  for( int i= 1; i <= svertices.length(); ++i )
    {
    MLSVertex& sv= mesh->svertices.append();
    sv= svertices[i];
    }

  if( hashBox != 0 )
    mesh->buildNewHashBox();

  mesh->dirtyBoundings= dirtyBoundings;
  mesh->dirtyHashBox= dirtyHashBox;

  return mesh;
}

/////////////////////////////////////////////////////////////////////////////

void MeshRep::clear()
{
  vertices.clear();
  nVertices= 0;

  edges.clear();
  nEdges= 0;

  polygons.clear();
  nPolygons= 0;

  svertices.clear();
  nSVertices= 0;

  delete hashBox;
  hashBox= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MeshRep::alloc( int _nVertices, int _nEdges, int _nPolygons )
{
  clear();

  nVertices= max( 1, _nVertices );
  nEdges= max( 1, _nEdges );
  nPolygons= max( 1, _nPolygons );

  int i;

  vertices.realloc(nVertices);
  for( i= 1; i <= nVertices; ++i )
    vertices[i].flags |= MLVertex::BASE_VERTEX;

  edges.realloc(nEdges);
  for( i= 1; i <= nEdges; ++i )
    edges[i].flags |= MLEdge::BASE_EDGE;

  polygons.realloc(nPolygons);

  for( i= 1; i <= nPolygons; ++i )
    {
//    polygons[i].mesh= this;
    polygons[i].index= i;
    polygons[i].flags |= MLPolygon::BASE_POLYGON;
    }
}

/////////////////////////////////////////////////////////////////////////////

struct HEDGE
  {
  int hash;
  int v1, v2;
  int operator < ( const HEDGE& he ) const
    {
    return hash < he.hash;
    }
  void swap( HEDGE& he )
    {
    ::swap( hash, he.hash );
    ::swap( v1, he.v1 );
    ::swap( v2, he.v2 );
    }
  };

// find edges from vertices[] and polygons.vertices[]
// -> Build edges[].v{1,2}
// Cost=O(nEdges*log(nEdges))
void MeshRep::findEdges()
{
  edges.clear();
  nEdges= 0;

  if( nVertices == 0 )
    return;

/*
  for( int i= 1; i <= polygons.length(); ++i )
    {
    MLPolygon& p= polygons[i];
    if( ! p.allocated() )
      continue;
    for( int j= 0; j < p.nVertices; ++j )
      {
      int vi1= p.elements[ (j==0) ? (p.nVertices-1) : (j-1) ].vertex;
      int vi2= p.elements[ j ].vertex;

      if( vi1 == vi2 )
        continue;

      int k;
      for( k= 1; k <= edges.length(); ++k )
        {
        MLEdge& e= edges[k];
        if( (vi1 == e.v1 && vi2 == e.v2) || (vi1 == e.v2 && vi2 == e.v1) )
          break;
        }

      if( k > edges.length() )
        {
        MLEdge& e= edges.append();
        e.flags |= MLEdge::BASE_EDGE;
        e.v1= vi1;
        e.v2= vi2;
#ifdef MESHREP_DEBUG
        fprintf( stderr,"Found edge %d: %d %d\n", edges.length(),vi1,vi2);
#endif
        }
      }
    }
  nEdges= edges.length();
*/

  int i;
  int nTotalEdges= 0;
  for( i= 1; i <= polygons.length(); ++i )
    {
    MLPolygon& p= polygons[i];
    if( ! p.allocated() )
      continue;
    if( p.nVertices <= 1 )
      continue;
    nTotalEdges += p.nVertices;
    }

  if( nTotalEdges <= 0 )
    return;

  HEDGE* hedges= new HEDGE [ nTotalEdges ];

  int k= 0;
  for( i= 1; i <= polygons.length(); ++i )
    {
    MLPolygon& p= polygons[i];
    if( ! p.allocated() )
      continue;
    if( p.nVertices <= 1 )
      continue;

    for( int j= 0; j < p.nVertices; ++j )
      {
      HEDGE& e= hedges[k];

      int vi1= p.elements[ (j==0) ? (p.nVertices-1) : (j-1) ].vertex;
      int vi2= p.elements[ j ].vertex;

      e.hash= vi1 * vi2;
      if( e.hash == 0 )
        e.hash= 1;
      e.v1= min( vi1, vi2 );
      e.v2= max( vi1, vi2 );
      ++k;
      }
    }

  assert( k == nTotalEdges );

  // QuickSort hedges[]
  Sorts<HEDGE> sorts;
  sorts.quickSort( hedges, nTotalEdges );

  k= 0;
  for(;;)
    {
    if( k >= nTotalEdges )
      break;
    HEDGE& e= hedges[k];
    int hash= e.hash;
    int i= k+1;
    int j= k+1;

    for(;;)
      {
      if( i >= nTotalEdges )
        break;
      HEDGE& ei= hedges[i];
      if( ei.hash != hash )
        break;
      if( e.v1 == ei.v1 && e.v2 == ei.v2 )
        {
        if( i > j )
          ei.swap( hedges[j] );
        ++j;
        }
      ++i;
      }

    // we have partitionned an single edge: hedges[ k..(j-1) ]
    MLEdge& edge= edges.append();
    edge.flags |= MLEdge::BASE_EDGE;
    edge.v1= e.v1;
    edge.v2= e.v2;
#ifdef MESHREP_DEBUG
    fprintf( stderr,"Found edge %d: %d %d\n", edges.length(),e.v1,e.v2);
#endif

    k= j;
    }

  nEdges= edges.length();

  delete hedges;
}

/////////////////////////////////////////////////////////////////////////////

// Input: vertices[].point
//        edges[].v{1,2}
//        polygons.vertices[]
// Output: vertices.edges[]
//         vertices.polygons[]
//         edges.p{1,2}
//         polygons.edges[]
//         svertices[]
//         polygons.svertices[]
void MeshRep::finalizeRep()
{
#ifdef MESHREP_PROFILE
  Chrono chrono;
  chrono.start();
#endif

  // Find vertices sharing edges
  // Cost: O(nEdges+nVertices)
  int i;

  // Check edges consistency
  // Remove zero-lengthed edges
  for( i= 1; i <= edges.length(); ++i )
    {
    MLEdge& e= edges[i];
    if( ! e.allocated() )
      continue;
    if( e.v1 == e.v2 )
      {
#ifdef MESHREP_DEBUG
      fprintf( stderr, "Removing degenerated edge %d\n",i);
#endif
      e.flags &= ~MLEdge::Flag(MLEdge::ALLOCATED);
      e.v1= e.v2= 0;
      e.polygon1= e.polygon2= 0;
      --nEdges;
      }
    }

  // Delete MLVertex::edges
  for( i= 1; i <= vertices.length(); ++i )
    {
    MLVertex& v= vertices[i];
    delete v.edges;
    v.edges= 0;
    v.nEdges= 0;
    }

  // How many edges refer to a vertex?
  for( i= 1; i <= edges.length(); ++i )
    {
    MLEdge& e= edges[i];
    if( ! e.allocated() )
      continue;
    if( e.v1 != 0 )
      ++vertices[e.v1].nEdges;
    if( e.v2 != 0 )
      ++vertices[e.v2].nEdges;
    }

  int* edgesCounter= new int [ vertices.length() + 1 ];
  for( i= 1; i <= vertices.length(); ++i )
    {
    edgesCounter[i]= 0;
    MLVertex& v= vertices[i];
    if( ! v.allocated() )
      continue;
    if( v.nEdges != 0 )
      {
#ifdef MESHREP_DEBUG
      fprintf( stderr,"Vertex %d: %d edges (%f %f %f)\n", i, v.nEdges,
               v.point.x(), v.point.y(), v.point.z() );
#endif
      v.edges= new int [ v.nEdges ];
      for( int j= 0; j < v.nEdges; ++j )
        v.edges[j]= 0;
      }
    }

  for( i= 1; i <= edges.length(); ++i )
    {
    MLEdge& e= edges[i];
    if( ! e.allocated() )
      continue;
    if( e.v1 != 0 )
      {
      MLVertex& v= vertices[e.v1];
      v.edges[ edgesCounter[e.v1] ]= i;
      ++edgesCounter[e.v1];
      }
    if( e.v2 != 0 )
      {
      MLVertex& v= vertices[e.v2];
      v.edges[ edgesCounter[e.v2] ]= i;
      ++edgesCounter[e.v2];
      }
    }

  delete edgesCounter;
  edgesCounter= 0;

  // check polygons consistency
  // Cost: O(nPolygons * max(nVertices by polygon))
  for( i= 1; i <= polygons.length(); ++i )
    {
    MLPolygon& p= polygons[i];
    if( ! p.allocated() )
      continue;

    // Cost=O(polygon.nVertices);
    for( int j= 0; j < p.nVertices; ++j )
      {
      int vi= p.elements[j].vertex;

      while( p.nVertices > 1 )
        {
        int vi_next= p.elements[ (j==p.nVertices-1) ? 0 : (j+1) ].vertex;
        if( vi != vi_next )
          break;

#ifdef MESHREP_DEBUG
        fprintf( stderr, "Removing vertex %d from polygon %d\n",vi,i);
#endif
        // remove this vertex from polygon's vertices list
        int k= j+1;
        while( k < p.nVertices )
          {
          p.elements[k-1].vertex= p.elements[k].vertex;
          ++k;
          }

        --p.nVertices;
        }
      }

    if( p.nVertices < 3 )
      {
      p.flags &= ~MLPolygon::Flag(MLPolygon::ALLOCATED);
      --nPolygons;
#ifdef MESHREP_DEBUG
      fprintf( stderr, "Removing degenerated polygon %d\n",i);
#endif
      }
    }

  // Set polygons edges and in the same time edges polygons
  // Set polygons normals too using Newell's method (Gems III, pg 517)
  // Cost: O(nPolygons)
  for( i= 1; i <= polygons.length(); ++i )
    {
    MLPolygon& p= polygons[i];
    if( ! p.allocated() )
      continue;

    p.normal= Vector(0,0,0);
    for( int j= 0; j < p.nVertices; ++j )
      {
      int vi1= p.elements[ j ].vertex;
      int vi2= p.elements[ (j+1) % p.nVertices ].vertex;
      if( vi1 == 0 || vi2 == 0 )
{
//printf("!!! %d !!!\n",j);
        continue;
}
      MLVertex& v1= vertices[vi1];
      MLVertex& v2= vertices[vi2];
      p.normal[0]+= (v1.point[1]-v2.point[1]) * (v2.point[2]+v1.point[2]);
      p.normal[1]+= (v1.point[2]-v2.point[2]) * (v2.point[0]+v1.point[0]);
      p.normal[2]+= (v1.point[0]-v2.point[0]) * (v2.point[1]+v1.point[1]);

      p.elements[j].edge= 0;

      MLVertex& v= vertices[vi1];
      if( v.edges != 0 )
        {
        for( int k= 0; k < v.nEdges; ++k )
          {
          int ei= v.edges[k];
          if( ei == 0 )
            continue;
          MLEdge& e= edges[ei];
          if( e.v1 == vi1 && e.v2 == vi2 )
            {
            p.elements[j].edge= ei;
            e.polygon1= i;
            break;
            }
          if( e.v1 == vi2 && e.v2 == vi1 )
            {
            p.elements[j].edge= ei;
            e.polygon2= i;
            break;
            }
          }
        }
#ifndef NDEBUG
      if( p.elements[j].edge == 0 )
        {
        fprintf( stderr,
                 "* MeshRep::complete(): edge %d of polygon %d not found"
                 " (vertices %d and %d)\n",
                 j, i, vi1, vi2 );
        fprintf( stderr, "  Edges refering to vertex %d:", vi1 );
        int k;
        for( k= 0; k < v1.nEdges; ++k )
          fprintf( stderr, " %d", v1.edges[k] );
        fprintf( stderr, "\n" );
        fprintf( stderr, "  Edges refering to vertex %d:", vi2 );
        for( k= 0; k < v2.nEdges; ++k )
          fprintf( stderr, " %d", v2.edges[k] );
        fprintf( stderr, "\n" );
        }
#endif
      }

    double norm= p.normal.norm();
    p.area= norm * 0.5;
    if( norm > 1e-20 )
      p.normal /= norm;

#ifdef MESHREP_DEBUG
    fprintf(stderr,"normal of polygon %d: %1.1f %1.1f %1.1f\n",i,
                   p.normal.x(),p.normal.y(),p.normal.z());
#endif
    } // for each polygon

  // set polygons infos in each MLVertex
  // Cost: about O(nVertices)
  for( i= 1; i <= vertices.length(); ++i )
    {
    MLVertex& v= vertices[i];
    if( ! v.allocated() )
      continue;
    int* polygons= new int [ v.nEdges * 2 ];
    int n= 0;
    // Cost: O(v.nEdges)
    for( int j= 0; j < v.nEdges; ++j )
      {
      int ei= v.edges[j];
      if( ei == 0 ) continue;
      MLEdge& e= edges[ei];
      int k;
      if( e.polygon1 != 0 )
        {
        for( k= 0; k < n; ++k )
          if( polygons[k] == e.polygon1 )
            break;
        if( k == n )
          polygons[n++]= e.polygon1;
        }
      if( e.polygon2 != 0 )
        {
        for( k= 0; k < n; ++k )
          if( polygons[k] == e.polygon2 )
            break;
        if( k == n )
          polygons[n++]= e.polygon2;
        }
      }

#ifdef MESHREP_DEBUG
    fprintf(stderr,"polygons of vertex %d: ", i );
#endif

    v.nPolygons= n;
    delete v.polygons;
    if( n != 0 )
      {
      v.polygons= new int [ n ];
      for( int k= 0; k < n; ++k )
        {
        v.polygons[k]= polygons[k];
#ifdef MESHREP_DEBUG
        fprintf( stderr, "%d%s", polygons[k], (k==n-1)?"":", ");
#endif
        }
      }
     else
      v.polygons= 0;

    delete polygons;

    if( v.nPolygons == 0 && v.nEdges == 0 )
      {
      // this is an unused vertex
      v.flags &= ~MLVertex::ALLOCATED;
      --nVertices;
#ifdef MESHREP_DEBUG
      fprintf(stderr,"unused: removed.");
#endif
      }

#ifdef MESHREP_DEBUG
    fprintf(stderr,"\n");
#endif
    }

  // ok, now we've to set polygons svertices from their geometrical
  // vertices

#ifdef MESHREP_DEBUG
  int nDiscEdges= 0;
#endif
  // Find discontinuity edges
  for( i= 1; i <= edges.length(); ++i )
    {
    MLEdge& e= edges[i];
    if( ! e.allocated() )
      continue;
    e.computeDisc(polygons.getArray(),cos_discAngle);
#ifdef MESHREP_DEBUG
    if( (e.flags & MLEdge::DISC) != 0 ) ++nDiscEdges;
#endif
    }

#ifdef MESHREP_DEBUG
  fprintf( stderr, "Found %d discontinuity edges\n", nDiscEdges );
#endif

  // set normal, area, and x,y projection of triangles
  for( i= 1; i <= polygons.length(); ++i )
    {
    MLPolygon& p= polygons[i];
    if( ! p.allocated() )
      continue;
    p.updateNormalArea(vertices.getArray(),polygons.getArray());
    }

  // find svertices of each MLVertex
  svertices.clear();
  nSVertices= 0;
  for( i= 1; i <= vertices.length(); ++i )
    updateSVertices(i);

#ifdef MESHREP_PROFILE
  chrono.stop();
  double finalize_time= chrono.elapsed();
  chrono.start();
#endif

  computeBoundings();

#ifdef MESHREP_PROFILE
  chrono.stop();
  double bbox_time= chrono.elapsed();
  chrono.start();
#endif

  tesselate();

#ifdef MESHREP_PROFILE
  chrono.stop();
  double tesselate_time= chrono.elapsed();
  chrono.start();
#endif

  buildNewHashBox();

#ifdef MESHREP_PROFILE
  chrono.stop();
  double hashbox_time= chrono.elapsed();

  fprintf( stderr,
           "MeshRep_Profile: finalize(%.2f) bbox(%.2f) tesselate(%.2f) hashbox(%.2f)\n",
           finalize_time, bbox_time, tesselate_time, hashbox_time );
#endif
}

/////////////////////////////////////////////////////////////////////////////

// Update SVertices of Vertex vi
// Cost: O(v.nPolygons ** 2)
void MeshRep::updateSVertices( int vi )
{
  MLVertex& v= vertices[vi];
  if( ! v.allocated() )
    return;

  if( v.nPolygons == 0 || v.nEdges == 0 )
    {
#ifdef MESHREP_DEBUG
    fprintf( stderr, "updateSVertices(%d): v.nPolygons=%d, v.nEdges=%d\n",
             vi, v.nPolygons, v.nEdges );
#endif
    return;
    }

  delete v.svertices;
  v.svertices= 0;
  v.nSVertices= 0;

  // sv[i] = partition i (i>=0)
  // sv[i][0] = number n of polygons of partition i
  // sv[i][1] to sv[i][n] = polygons of partition i

  #define SV(i,j) (svi[(i)*v.nPolygons + (j)])

  int* svi= new int [ v.nPolygons * (v.nPolygons + 1) ];

  int nPart= 0;
  int i, j;

  boolean* processed_polygon= new boolean [ v.nPolygons ];
  int totalProcessed= 0;
  int nProcessed= 0;
  for( i= 0; i < v.nPolygons; ++i )
    {
    MLPolygon& p= polygons[ v.polygons[i] ];
    if( ! p.allocated() )
      processed_polygon[i]= true;
     else
      {
      processed_polygon[i]= false;
      ++totalProcessed;
      }
    }

  int curPart= 0;
  int neighbour_polygon1= 0;
  int neighbour_polygon2= 0;
  int neighbour_edge1= 0;
  int neighbour_edge2= 0;

/*
#define getPolygonEdges(pi,e1,e2)                                \
        {                                                        \
        MLPolygon& p= polygons[pi];                              \
        int i= p.getVertexInternalIndex(vi);                     \
        if( i == -1 )                                            \
          {                                                      \
          e1= e2= 0;                                             \
          }                                                      \
         else                                                    \
          {                                                      \
          e1= p.edges[i];                                        \
          i += p.nVertices-1;                                    \
          if( i >= p.nVertices ) i-= p.nVertices;                \
          e2= p.edges[i];                                        \
          assert( edges[e1].polygon1 == pi || edges[e1].polygon2 == pi );    \
          assert( edges[e2].polygon1 == pi || edges[e2].polygon2 == pi );    \
          }                                                      \
        }
*/

#define getPolygonEdges(pi,e1,e2)                                \
        {                                                        \
        MLPolygon& p= polygons[pi];                              \
        e1= e2= 0;                                               \
        int i;                                                   \
        for( i= 0; i < p.nVertices; ++i )                        \
          {                                                      \
          MLEdge& e= edges[ p.elements[i].edge ];                \
          if( e.v1 == vi || e.v2 == vi )                         \
            {                                                    \
            if(e1==0)                                            \
              e1= p.elements[i].edge;                            \
             else                                                \
              { e2= p.elements[i].edge; break; }                 \
            }                                                    \
          }                                                      \
        }

#if MESHREP_DEBUG >= 2
fprintf( stderr,"Vertex %d\n",vi);
#endif
int inf_loop= 0;
  while( nProcessed < totalProcessed )
    {
if( ++inf_loop > 200000 )
  {
//#if MESHREP_DEBUG
fprintf( stderr,"Infinite loop for vertex %d\n",vi);
//#endif
  break;
  }
    if( curPart >= nPart )
      {
      // ok, create a new partition
      // Choose a polygon, and go
      int i;
      for( i= 0; i < v.nPolygons; ++i )
        if( ! processed_polygon[i] )
          break;

      assert( i < v.nPolygons );
      processed_polygon[i]= true;
      SV(nPart,0)= 1;
      SV(nPart,1)= i;
      curPart= nPart;
      ++nPart;
      ++nProcessed;
      int e1_index, e2_index;
      int p_index= v.polygons [ i ];
      getPolygonEdges( p_index, e1_index, e2_index )
      MLEdge& e1= edges[e1_index];
      MLEdge& e2= edges[e2_index];
      if( (e1.flags & MLEdge::DISC) != 0 )
        {
        neighbour_polygon1= 0;
        neighbour_edge1= 0;
        }
       else
        {
        neighbour_edge1= e1_index;
        if( e1.polygon1 == p_index )
          neighbour_polygon1= e1.polygon2;
         else
          neighbour_polygon1= e1.polygon1;
        }
      if( (e2.flags & MLEdge::DISC) != 0 )
        {
        neighbour_polygon2= 0;
        neighbour_edge2= 0;
        }
       else
        {
        neighbour_edge2= e2_index;
        if( e2.polygon1 == p_index )
          neighbour_polygon2= e2.polygon2;
         else
          neighbour_polygon2= e2.polygon1;
        }
#if MESHREP_DEBUG >= 2
fprintf( stderr,"New partition (pol=%d): neighbour(pol,edg): 1(%d,%d) 2(%d,%d)\n",
         p_index,
         neighbour_polygon1,neighbour_edge1,
         neighbour_polygon2,neighbour_edge2 );
#endif
      }

    if( neighbour_edge1 == neighbour_edge2 ||
        (neighbour_polygon1 == 0 && neighbour_polygon2 == 0) )
      {
      if( nProcessed >= totalProcessed )
        break;
      // no neighbour at all or same neighbour
      ++curPart;
#if MESHREP_DEBUG >= 2
printf("NEW: e1=%d e2=%d   p1=%d p2=%d\n",
  neighbour_edge1, neighbour_edge2,neighbour_polygon1, neighbour_polygon2 );
#endif
      continue;
      }

    int i;
    for( i= 0; i < v.nPolygons; ++i )
      {
      if( processed_polygon[i] )
        continue;
      int p_index= v.polygons[i];

      if( p_index == neighbour_polygon1 )
        {
#if MESHREP_DEBUG >= 2
fprintf( stderr, "Found neighbour1 (pol %d), ", p_index );
#endif
        int e1_index, e2_index;
        getPolygonEdges( p_index, e1_index, e2_index )
        if( e1_index == neighbour_edge1 ) e1_index= e2_index;
        MLEdge& new_edge= edges[e1_index];
        if( (new_edge.flags & MLEdge::DISC) != 0 )
          {
          neighbour_polygon1= 0;
          neighbour_edge1= 0;
          }
         else
          {
          neighbour_edge1= e1_index;
          if( new_edge.polygon1 == p_index )
            neighbour_polygon1= new_edge.polygon2;
           else
            neighbour_polygon1= new_edge.polygon1;
          }
        int n= SV(curPart,0)+1;
        SV(curPart,n)= i;
        ++SV(curPart,0);
        processed_polygon[i]= true;
        ++nProcessed;
#if MESHREP_DEBUG >= 2
fprintf( stderr, "Next pol=%d, Next edge=%d\n",
         neighbour_polygon1, neighbour_edge1 );
#endif
        break;
        }
      if( p_index == neighbour_polygon2 )
        {
#if MESHREP_DEBUG >= 2
fprintf( stderr, "Found neighbour2 (pol %d), ",p_index );
#endif
        int e1_index, e2_index;
        getPolygonEdges( p_index, e1_index, e2_index )
        if( e1_index == neighbour_edge2 ) e1_index= e2_index;
        MLEdge& new_edge= edges[e1_index];
        if( (new_edge.flags & MLEdge::DISC) != 0 )
          {
          neighbour_polygon2= 0;
          neighbour_edge2= 0;
          }
         else
          {
          neighbour_edge2= e1_index;
          if( new_edge.polygon1 == p_index )
            neighbour_polygon2= new_edge.polygon2;
           else
            neighbour_polygon2= new_edge.polygon1;
          }
        int n= SV(curPart,0)+1;
        SV(curPart,n)= i;
        ++SV(curPart,0);
        processed_polygon[i]= true;
        ++nProcessed;
#if MESHREP_DEBUG >= 2
fprintf( stderr, "Next pol=%d, Next edge=%d\n",
         neighbour_polygon2, neighbour_edge2 );
#endif
        break;
        }
      }

//    if( i >= v.nPolygons )
//      break; // ERROR
    }

delete processed_polygon;
processed_polygon= 0;
/*
  for( int o= 0; o < v.nPolygons; ++o )
    {
    MLPolygon& p= polygons[ v.polygons[o] ];
    if( ! p.allocated() )
      continue;

    // ok, in which partition can we put this polygon?
    boolean found= false;
    for( i= 0; i < nPart; ++i )
      {
      int n= SV(i,0);
      int* svp= & SV(i,1);
      boolean failed= false;
      for( j= 1; j <= n; ++j, ++svp )
        {
        MLPolygon& pp= polygons[ v.polygons[*svp] ];
        if( p.material != pp.material )
          {
          failed= true;
          break;
          }
        float cos_a= (p.normal | pp.normal);
        if( cos_a <= cos_discAngle )
          {
          failed= true;
          break;
          }
        }

      if( ! failed )
        {
        // polygon p can go in partition sv[i]
        ++n;
        SV(i,n)= o;
        ++SV(i,0);
        found= true;
        break;
        }
      } // for each partition

    if( ! found )
      {
      // we cannot find a partition for polygon p
      // -> let'screate a new one
      SV(nPart,0)= 1;
      SV(nPart,1)= o;
      ++nPart;
      }
    }
*/

  if( nPart == 0 )
    {
    // we did not create any partition :-)) ?
#ifdef MESHREP_DEBUG
    fprintf( stderr, "updateSVertices(%d): no partition ??\n", vi );
#endif
    delete svi;
    return;
    }

#ifdef MESHREP_DEBUG
  fprintf( stderr, "Vertex %d has %d partitions\n",vi,nPart);
#endif

  // ok, we build nPart partitions of smoothed polygons that share
  // one point
  // Then we've got our nSVertices
  v.nSVertices= nPart;
  v.svertices= new int [ nPart ];
  for( i= 0; i < nPart; ++i )
    {
    int n= SV(i,0);

    // ok, this partition refers to SVertex si
    svertices.append();
    ++nSVertices;
    int si= svertices.length();
    MLSVertex& sv= svertices[si];
    if( (v.flags & MLVertex::REFINEMENT) != 0 )
      sv.flags |= MLSVertex::REFINEMENT;
     else
      sv.flags |= MLSVertex::BASE_SVERTEX;

    v.svertices[i]= si;

    sv.vertex= vi;
    sv.normal= Vector(0,0,0);
    sv.txtCoord= Vector2(0,0);
    sv.material= 0;

#ifdef MESHREP_DEBUG
    fprintf( stderr, "   Polygons of Part. %d:", i+1 );
#endif
    int* svp= &SV(i,1);
    for( j= 1; j <= n; ++j, ++svp )
      {
#ifdef MESHREP_DEBUG
      fprintf( stderr, " %d", v.polygons[*svp] );
#endif
      MLPolygon& p= polygons[ v.polygons[*svp] ];
      sv.material= p.material;
      sv.normal+= p.normal;
      int vertex_index= p.getVertexInternalIndex(vi);
      if( vertex_index != -1 )
        p.elements[ vertex_index ].svertex= si;
      }
#ifdef MESHREP_DEBUG
    fprintf( stderr, "\n" );
#endif

    double norm= sv.normal.norm();
    if( norm > 1e-20 )
      sv.normal /= norm;
    }

  delete svi;
  #undef SV

/*
  // sv[i] = partition i (i>=0)
  // sv[i][0] = number n of polygons of partition i
  // sv[i][1] to sv[i][n] = polygons of partition i

  #define SV(i,j) (svi[(i)*v.nPolygons + (j)])

  int* svi= new int [ v.nPolygons * (v.nPolygons + 1) ];

  int nPart= 0;
  int i, j;

  for( int o= 0; o < v.nPolygons; ++o )
    {
    MLPolygon& p= polygons[ v.polygons[o] ];
    if( ! p.allocated() )
      continue;

    // ok, in which partition can we put this polygon?
    boolean found= false;
    for( i= 0; i < nPart; ++i )
      {
      int n= SV(i,0);
      int* svp= & SV(i,1);
      boolean failed= false;
      for( j= 1; j <= n; ++j, ++svp )
        {
        MLPolygon& pp= polygons[ v.polygons[*svp] ];
        if( p.material != pp.material )
          {
          failed= true;
          break;
          }
        float cos_a= (p.normal | pp.normal);
        if( cos_a <= cos_discAngle )
          {
          failed= true;
          break;
          }
        }

      if( ! failed )
        {
        // polygon p can go in partition sv[i]
        ++n;
        SV(i,n)= o;
        ++SV(i,0);
        found= true;
        break;
        }
      } // for each partition

    if( ! found )
      {
      // we cannot find a partition for polygon p
      // -> let'screate a new one
      SV(nPart,0)= 1;
      SV(nPart,1)= o;
      ++nPart;
      }
    }

  if( nPart == 0 )
    {
    // we did not create any partition :-)) ?
#ifdef MESHREP_DEBUG
    fprintf( stderr, "updateSVertices(%d): no partition ??\n", vi );
#endif
    delete svi;
    return;
    }

#ifdef MESHREP_DEBUG
  fprintf( stderr, "Vertex %d has %d partitions\n",vi,nPart);
#endif

  // ok, we build nPart partitions of smoothed polygons that share
  // one point
  // Then we've got our nSVertices
  v.nSVertices= nPart;
  v.svertices= new int [ nPart ];
  for( i= 0; i < nPart; ++i )
    {
    int n= SV(i,0);

    // ok, this partition refers to SVertex si
    svertices.append();
    ++nSVertices;
    int si= svertices.length();
    MLSVertex& sv= svertices[si];
    if( (v.flags & MLVertex::REFINEMENT) != 0 )
      sv.flags |= MLSVertex::REFINEMENT;
     else
      sv.flags |= MLSVertex::BASE_SVERTEX;

    v.svertices[i]= si;

    sv.vertex= vi;
    sv.normal= Vector(0,0,0);
    sv.txtCoord= Vector2(0,0);
    sv.material= 0;

#ifdef MESHREP_DEBUG
    fprintf( stderr, "   Polygons of Part. %d:", i+1 );
#endif
    int* svp= &SV(i,1);
    for( j= 1; j <= n; ++j, ++svp )
      {
#ifdef MESHREP_DEBUG
      fprintf( stderr, " %d", v.polygons[*svp] );
#endif
      MLPolygon& p= polygons[ v.polygons[*svp] ];
      sv.material= p.material;
      sv.normal+= p.normal;
      int vertex_index= p.getVertexInternalIndex(vi);
      if( vertex_index != -1 )
        p.elements[ vertex_index ].svertex= si;
      }
#ifdef MESHREP_DEBUG
    fprintf( stderr, "\n" );
#endif

    double norm= sv.normal.norm();
    if( norm > 1e-20 )
      sv.normal /= norm;
    }

  delete svi;
  #undef SV
*/
}

/////////////////////////////////////////////////////////////////////////////

struct HVERTEX
  {
  int index;
  int replace_by;
  double hash;
  int operator < ( const HVERTEX& hv ) const
    {
    return hash < hv.hash;
    }
  void swap( HVERTEX& hv )
    {
    ::swap( index, hv.index );
    ::swap( replace_by, hv.replace_by );
    ::swap( hash, hv.hash );
    }
  };

// private
// Cost: O(n*log(n)), n= nVertices
// Updates: vertices[], edges[].v{1,2}, polygons[].vertices[]
void MeshRep::mergeVertices()
{
  int i;

  if( nVertices <= 1 )
    return;

  int n= vertices.length();

  HVERTEX* hv= new HVERTEX [ n + 1 ];
  for( i= 1; i <= n; ++i )
    {
    MLVertex& v= vertices[i];
    if( ! v.allocated() )
      {
      hv[i].index= 0;
      hv[i].replace_by= 0;
      hv[i].hash= 0.;
      continue;
      }

    hv[i].index= i;
    hv[i].replace_by= 0;
    hv[i].hash= v.point.x() + v.point.y() + v.point.z();
    }

  // QuickSort hv[]
  Sorts<HVERTEX> sorts;
  sorts.quickSort( hv+1, n );

#ifdef MESHREP_DEBUG
  int nReplaces= 0;
#endif

  // set hv[].replace_by
  i= 1;
  while( i <= n )
    {
    HVERTEX& h= hv[i];
    if( h.index == 0 )
      {
      ++i;
      continue;
      }
    double hash= h.hash;
    ++i;
    int j= i;
    while( i <= n )
      {
      if( hv[i].index == 0 )
        break;
      if( fabs(hv[i].hash - hash) > smallDistance )
        break;

      DBL delta= (vertices[hv[i].index].point - vertices[h.index].point)
                .absNorm();
      if( delta < smallDistance )
        {
        hv[i].replace_by= h.index;
        if( i != j )
          hv[j].swap(hv[i]);
        ++j;
#ifdef MESHREP_DEBUG
        ++nReplaces;
#endif
        }
      ++i;
      }

    i= j;
    }

#ifdef MESHREP_DEBUG
   if( nReplaces > 0 )
     fprintf( stderr, "Merging %d vertices\n", nReplaces );
#endif

  int* replace= new int [ n + 1 ];
  replace[0]= 0;

  for( i= 1; i <= n; ++i )
    replace[i]= i;

  for( i= 1; i <= n; ++i )
    if( hv[i].replace_by != 0 )
      replace[ hv[i].index ]= hv[i].replace_by;

  for( i= 1; i <= n; ++i )
    if( replace[i] != i )
      {
#ifdef MESHREP_DEBUG
      fprintf( stderr, "Merging vertex %d into vertex %d\n", i, replace[i] );
#endif
      vertices[i].flags &= ~MLVertex::Flag(MLVertex::ALLOCATED);
      --nVertices;
      }

  for( i= 1; i <= edges.length(); ++i )
    {
    MLEdge& e= edges[i];
    if( ! e.allocated() )
      continue;

    e.v1= replace[e.v1];
    e.v2= replace[e.v2];
#ifdef MESHREP_DEBUG
fprintf(stderr,"edge %d: %d %d\n",i,e.v1,e.v2);
#endif

#ifdef MESHREP_DEBUG
    assert( vertices[e.v1].allocated() );
    assert( vertices[e.v2].allocated() );
#endif
    }

  for( i= 1; i <= polygons.length(); ++i )
    {
    MLPolygon& p= polygons[i];
    if( ! p.allocated() )
      continue;
#ifdef MESHREP_DEBUG
fprintf(stderr,"polygon %d: ",i);
#endif
    for( int j= 0; j < p.nVertices; ++j )
      {
      p.elements[j].vertex= replace[p.elements[j].vertex];
#ifdef MESHREP_DEBUG
fprintf(stderr,"%d%s",p.elements[j].vertex,(j==p.nVertices-1)?"\n":", ");
#endif

#ifdef MESHREP_DEBUG
      assert( vertices[p.elements[j].vertex].allocated() );
#endif
      }
    }

  delete replace;
  delete hv;
}

/////////////////////////////////////////////////////////////////////////////

// bounding box and bounding sphere
// return false if mesh is unable to bound
boolean MeshRep::computeBoundings()
{
  dirtyBoundings= false;

  boolean found= false;

  Point minP(0,0,0),
        maxP(0,0,0);
//  BoundingSphereBuilder bsb;

  int i;
  for( i= 1; i <= vertices.length(); ++i )
    {
    register MLVertex& v= vertices[i];
    if( ! v.allocated() )
      continue;

    const Point& p= v.point;

//    bsb.pass1(p);

    if( found )
      {
      minP= min( minP, p );
      maxP= max( maxP, p );
      }
     else
      {
      minP= p;
      maxP= p;
      }

    found= true;
    }

  if( ! found )
    {
    minP= Point(0,0,0);
    maxP= Point(1,1,1) * 1e-8;
    bbox= Box(minP,maxP);

    boundingSphere= BoundingSphere( Point(0,0,0), 1e-8 );

    return false;
    }

  Vector size= maxP - minP;

  Vector delta(0,0,0);
  if( size.x() < 1e-10 )
    delta[0]= max( size.norm() / 1000., 1e-8 );
   else
    delta[0]= size.x() * 1e-5;
  if( size.y() < 1e-10 )
    delta[1]= max( size.norm() / 1000., 1e-8 );
   else
    delta[1]= size.y() * 1e-5;
  if( size.z() < 1e-10 )
    delta[2]= max( size.norm() / 1000., 1e-8 );
   else
    delta[2]= size.z() * 1e-5;

  minP -= delta;
  maxP += delta;

  bbox= Box(minP,maxP);

//  bsb.end_pass1();
  Point bs_center= bbox.getCenter();
  double bs_radius2= 0.;

  for( i= 1; i <= vertices.length(); ++i )
    {
    register MLVertex& v= vertices[i];
    if( ! v.allocated() )
      continue;

    register const Point& p= v.point;

//    bsb.pass2(p);
    register double r2= (bs_center - p).norm2();
    if( r2 > bs_radius2 )
      bs_radius2= r2;
    }

  boundingSphere= BoundingSphere( bs_center, sqrt(bs_radius2) * (1.+1e-4) );

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MeshRep::buildNewHashBox()
{
  if( dirtyBoundings )
    computeBoundings();

  dirtyHashBox= false;

  delete hashBox;
  hashBox= 0;

  int nTriangles= 0;
  int i;
  Vector avgSize(0,0,0);
  Point minP(+1e30,+1e30,+1e30);
  Point maxP(-1e30,-1e30,-1e30);

  for( i= 1; i <= polygons.length(); ++i )
    {
    MLPolygon& p= polygons[i];
    if( ! p.allocated() )
      continue;
    if( p.nVertices != 3 )
      continue;

    Point& p1= vertices[ p.elements[0].vertex ].point;
    Point& p2= vertices[ p.elements[1].vertex ].point;
    Point& p3= vertices[ p.elements[2].vertex ].point;
    Point m= min(p1,min(p2,p3));
    Point M= max(p1,max(p2,p3));
    avgSize+= M - m;
    minP= min(minP,m);
    maxP= max(maxP,M);

    ++nTriangles;
    }

  if( nTriangles < 16 )
    return false; // not enough

  avgSize /= double(nTriangles);

  if( avgSize[0] < 1e-8 ) avgSize[0]= 1e-8;
  if( avgSize[1] < 1e-8 ) avgSize[1]= 1e-8;
  if( avgSize[2] < 1e-8 ) avgSize[2]= 1e-8;

  Vector totalSize= maxP - minP;
  double totalSizeNorm= totalSize.norm();
  if( totalSizeNorm < 1e-8 )
    return false; // too small

  double dnx= (totalSize.x() * 0.999) / avgSize.x() + 1.;
  double dny= (totalSize.y() * 0.999) / avgSize.y() + 1.;
  double dnz= (totalSize.z() * 0.999) / avgSize.z() + 1.;

  if( totalSize.x() / totalSizeNorm < 1e-6 )
    dnx= 1;
  if( totalSize.y() / totalSizeNorm < 1e-6 )
    dny= 1;
  if( totalSize.z() / totalSizeNorm < 1e-6 )
    dnz= 1;

  const double maxHashBox_AxisSize= 10000.;
  const double maxHashBox_Voxels= 4000000. / 8.;

  if( dnx > maxHashBox_AxisSize ) dnx= maxHashBox_AxisSize;
  if( dny > maxHashBox_AxisSize ) dny= maxHashBox_AxisSize;
  if( dnz > maxHashBox_AxisSize ) dnz= maxHashBox_AxisSize;

  if( dnx < 1. ) dnx= 1.;
  if( dny < 1. ) dny= 1.;
  if( dnz < 1. ) dnz= 1.;

  while( dnx * dny * dnz > maxHashBox_Voxels )
    {
    if( dnx > dny && dnx > dnz ) { dnx /= 1.5; continue; }
    if( dny > dnx && dny > dnz ) { dny /= 1.5; continue; }
    dnz /= 1.5;
    }

  int nx= int(dnx);
  int ny= int(dny);
  int nz= int(dnz);
#ifdef MESHREP_DEBUG
  fprintf( stderr, "HashBox: size=[%f %f %f]\n"
                   "         avg=[%f %f %f]\n"
                   "         %d voxels=%dx%dx%d\n",
                   totalSize.x(), totalSize.y(), totalSize.z(),
                   avgSize.x(), avgSize.y(), avgSize.z(),
                   nx*ny*nz, nx, ny, nz );
#endif
  hashBox= new MLHashBox(bbox);
  hashBox->create(nx,ny,nz);

  for( i= 1; i <= polygons.length(); ++i )
    {
    MLPolygon& p= polygons[i];
    if( ! p.allocated() )
      continue;
    if( p.nVertices != 3 )
      continue;

    hashBox->addTriangle(this,p);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// translating a vertex for example
// also invalidates hashbox
boolean MeshRep::invalidateBoundings()
{
  invalidateHashBox();

  if( dirtyBoundings )
    return false;

  dirtyBoundings= true;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MeshRep::invalidateHashBox()
{
  dirtyHashBox= true;

  delete hashBox;
  hashBox= 0;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// this functions doesn't increment MeshRep::nPolygons
MLPolygon& MeshRep::addPolygon()
{
  // when do we do garbage collecting?
  MLPolygon& p= polygons.append();
//  p.mesh= this;
  p.index= polygons.length();

  return p;
}

/////////////////////////////////////////////////////////////////////////////

void MeshRep::tesselate()
{
  for( int i= 1; i <= polygons.length(); ++i )
    tesselatePolygon(i);
}

/////////////////////////////////////////////////////////////////////////////

int MeshRep::computeNBaseEdges() const
{
  int n= 0;

  for( int i= 1; i <= edges.length(); ++i )
    {
    const MLEdge& e= edges.get(i);
    if( ! e.allocated() )
      continue;
    if( (e.flags & MLEdge::BASE_EDGE) != 0 )
      ++n;
    }

  return n;
}

/////////////////////////////////////////////////////////////////////////////

int MeshRep::computeNBasePolygons() const
{
  int n= 0;

  for( int i= 1; i <= polygons.length(); ++i )
    {
    const MLPolygon& p= polygons.get(i);
    if( ! p.allocated() )
      continue;
    if( (p.flags & MLPolygon::BASE_POLYGON) != 0 )
      ++n;
    }

  return n;
}

/////////////////////////////////////////////////////////////////////////////
