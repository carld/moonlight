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
  MLPolygonTesselate.C

  Defines:
    void MeshRep::tesselatePolygon( int i );

  Stephane Rehel
  February 9 1997
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "GL/openglu.h"

#include "tools/3D/MLPolygon.h"
#include "tools/3D/MLEdge.h"
#include "tools/3D/MLVertex.h"

#include "MeshRep.h"

#ifndef NDEBUG
//#define TESS_DEBUG
#endif

/////////////////////////////////////////////////////////////////////////////

static GLenum tess_type= GLenum(0);
static int vertices[2];
static int vertex_index= 0;
struct TessTriangle
  {
  int e[3];
  };
static GLenum tess_error= GLenum(0);

static TessTriangle* triangles= 0;
static int maxTriangles= 0;
static int nTriangles= 0;

static void CALLBACK ml_tesselate_begin( GLenum type )
{
  tess_type= type;
  vertex_index= 0;
}

static void CALLBACK ml_tesselate_end()
{
  tess_type= GLenum(0);
  vertex_index= 0;
}

static void CALLBACK ml_tesselate_vertex( void* data )
{
  if( tess_type == 0 )
    return;

  int e= int(data);
  if( vertex_index++ < 2 )
    {
    vertices[vertex_index-1]= e;
    return;
    }

  if( nTriangles < maxTriangles )
    {
    triangles[nTriangles].e[0]= vertices[0];
    triangles[nTriangles].e[1]= vertices[1];
    triangles[nTriangles].e[2]= e;
    ++nTriangles;
    }

  switch( tess_type )
    {
    case GL_TRIANGLE_STRIP:
      {
      if( vertex_index & 1 )
        vertices[0]= e;
       else
        vertices[1]= e;
      break;
      }
    case GL_TRIANGLE_FAN:
      {
      vertices[1]= e;
      break;
      }
    case GL_TRIANGLES:
    default:
      {
      vertex_index= 0;
      break;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

static void CALLBACK ml_tesselate_error( GLenum errno )
{
  ::tess_error= errno;
}

/////////////////////////////////////////////////////////////////////////////

void MeshRep::tesselatePolygon( int pi )
{
  if( pi < 0 || pi >= polygons.length() )
    return;

  MLPolygon& p= polygons[pi];
  if( ! p.allocated() )
    return;

  if( p.nVertices <= 3 )
    {
    delete p.tesselation;
    p.tesselation= 0;
    p.nTessTriangles= 0;
    return; // nothing to do
    }

  if( ::maxTriangles < p.nVertices )
    {
    ::maxTriangles= p.nVertices; // normally: nVertices-2
    delete ::triangles;
    ::triangles= new ::TessTriangle [ ::maxTriangles ];
    }
  ::nTriangles= 0;
  ::vertex_index= 0;
  ::tess_error= GLenum(0);
  ::tess_type= GLenum(0);

  GLUtriangulatorObj* tess= gluNewTess();
  if( tess == 0 )
    return; // out of memory

  typedef void CALLBACK (*glucallback)(void);

  gluTessCallback( tess,
                   (GLenum)GLU_BEGIN,
                   glucallback(ml_tesselate_begin) );
  gluTessCallback( tess,
                   (GLenum)GLU_END,
                   glucallback(ml_tesselate_end) );
  gluTessCallback( tess,
                   (GLenum)GLU_VERTEX,
                   glucallback(ml_tesselate_vertex) );
  gluTessCallback( tess,
                   (GLenum)GLU_ERROR,
                   glucallback(ml_tesselate_error) );

  gluBeginPolygon(tess);
  gluNextContour(tess,(GLenum)GLU_UNKNOWN);

  int i;

  for( i= 0; i < p.nVertices; ++i )
    {
    MLPolygon::Element* e= & p.elements[i];
    Point& point= vertices[e->vertex].point;
    GLdouble v[3];
    v[0]= point.x();
    v[1]= point.y();
    v[2]= point.z();
    gluTessVertex( tess, v, (void*) i );
    }
  gluEndPolygon(tess);

  if( ::nTriangles <= 0 )
    {
    // ok, tesselation failed
    fprintf( stderr, "TessError for polygon(%d): %d vertices = %s\n",
             p.index, p.nVertices, gluErrorString(tess_error) );
    gluDeleteTess(tess);
    return; // oups, what are we gonna do ?
    }

  gluDeleteTess(tess);

  p.nTessTriangles= ::nTriangles;
  p.tesselation= new int [ p.nTessTriangles ];

#ifdef TESS_DEBUG
printf( "Tess(%d): %d vertices, %d triangles: ",
        p.index, p.nVertices, p.nTessTriangles );
#endif

  // Create tesselation triangles
  for( i= 0; i < p.nTessTriangles; ++i )
    {
    MLPolygon& t= addPolygon();
    ++nPolygons;

    p.tesselation[i]= t.index;
    t.flags |= MLPolygon::TESS_TRIANGLE;
    t.fatherPolygon= p.index;
#ifdef TESS_DEBUG
printf( "%d(%x)",t.index,t.flags);
#endif

    TessTriangle& tt= ::triangles[i];
    MLPolygon::Element& e1= p.elements[tt.e[0]];
    MLPolygon::Element& e2= p.elements[tt.e[1]];
    MLPolygon::Element& e3= p.elements[tt.e[2]];

    t.allocTriangle( e1.vertex, e2.vertex, e3.vertex );
    t.elements[0].svertex= e1.svertex;
    t.elements[1].svertex= e2.svertex;
    t.elements[2].svertex= e3.svertex;
    t.updateNormalArea( vertices.getArray(), polygons.getArray() );

    // check for original edges, thanx to -funroll-loops
    for( int j= 0; j < 3; ++j )
      {
      int vi1= tt.e[ j ];
      int vi2= tt.e[ (j==2) ? 0 : (j+1) ];
      if( abs(vi2-vi1) == 1 || abs(vi2-vi1) == p.nVertices-1 )
        {
        // ok, this is an original edge
        t.elements[j].edge= p.elements[ tt.e[j] ].edge;
#ifndef NDEBUG
int vertex1= p.elements[vi1].vertex;
int vertex2= p.elements[vi2].vertex;
MLEdge& e= edges[t.elements[j].edge];
assert( (e.v1 == vertex1 && e.v2 == vertex2) ||
        (e.v2 == vertex1 && e.v1 == vertex2) );
#endif
        }
      }
    }
#ifdef TESS_DEBUG
printf("\n");
#endif

  // Update edge info in newly created tesselation triangles
  for( i= 0; i < p.nTessTriangles; ++i )
    {
    MLPolygon& t= polygons[ p.tesselation[i] ];
#ifdef TESS_DEBUG
printf("Updating TessTriangle %d info\n",t.index);
#endif
    // let's loop on the edges
    for( int j= 0; j < 3; ++j )
      {
      // First: is it an original edge?
      if( t.elements[j].edge != 0 )
        {
        // this is a original edge
        // replace polygon neighbouring information in this edge
        MLEdge& e= edges[ t.elements[j].edge ];

        if( e.polygon1 == p.index ) e.polygon1= t.index; else
        if( e.polygon2 == p.index ) e.polygon2= t.index; /*else assert(0);*/
        }
       else
        {
        int vi1= t.elements[ j ].vertex;
        int vi2= t.elements[ (j==2) ? 0 : (j+1) ].vertex;

        // not an original edge
        // let's try to find if we've already created this edge by a
        // previous tesselation triangle insertion
//printf( "%d - %d\n",vi1,vi2);

        int ei= vertices[vi1].findEdge( edges.getArray(), vi1, vi2 );
        if( ei == 0 )
          {
          // well, we gonna create a new tesselation edge
          MLEdge& e= edges.append();
          ei= edges.length();
          t.elements[j].edge= ei;
          ++nEdges;

          // this not a discontinuity edge, neither a border edge
          e.flags |= MLEdge::TESSELATION;
          e.v1= vi1;
          e.v2= vi2;
          e.polygon1= t.index;
          e.polygon2= 0;

          MLVertex& v1= vertices[ vi1 ];
          MLVertex& v2= vertices[ vi2 ];
          v1.addEdge(ei);
          v2.addEdge(ei);
          }
         else
          {
          // god, this edge already exists
          MLEdge& e= edges[ ei ];
#ifdef TESS_DEBUG
printf( " %d (%x)\n",e.polygon1,polygons[e.polygon1].flags );
#endif
          // assert we have a tesselation edge ... :-)) cross your fingers
//          assert( (e.flags & MLEdge::TESSELATION) != 0 );
          // assert this edge was just created by us
//          assert( e.polygon1 != 0 );
//          assert( (mesh->polygons[ e.polygon1 ].flags
//                   & MLPolygon::TESS_TRIANGLE) != 0 );
//          assert( e.polygon2 == 0 );

          t.elements[j].edge= ei;
//          e.polygon2= t.index;
          if( e.polygon1 == p.index || e.polygon1 == 0 )
            e.polygon1= t.index;
          else
          if( e.polygon2 == p.index || e.polygon2 == 0 )
            e.polygon2= t.index;
//          else
//            assert(0);
          }
        }
      } // for j
    } // for each tesselation triangle
  // ough!
}

/////////////////////////////////////////////////////////////////////////////

