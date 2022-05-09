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
  MLTextGenerate.C

  Stephane Rehel

  February 8 1998
*/

#include <assert.h>

#include "tools/MLString.h"

#include "gltt/FTGlyph.h"
#include "gltt/FTGlyphVectorizer.h"
#include "gltt/FTFont.h"
#include "gltt/GLTTFont.h"
#include "gltt/GLTTGlyphTriangulator.h"

#include "MLTextGenerate.h"

#include "scene/MeshRep.h"

#include "FormalText.h"

/////////////////////////////////////////////////////////////////////////////

class MLGlyphTriangulator: public GLTTGlyphTriangulator
{
protected:
  MLTextGenerate* gen;
public:
  MLGlyphTriangulator( FTGlyphVectorizer* vec, MLTextGenerate* _gen ):
    GLTTGlyphTriangulator(vec)
    {
    gen= _gen;
    }

  virtual ~MLGlyphTriangulator()
    {}

  void triangle( FTGlyphVectorizer::POINT* p1,
                 FTGlyphVectorizer::POINT* p2,
                 FTGlyphVectorizer::POINT* p3 )
    {
    // please delegate it
    gen->triangle(p1,p2,p3);
    }
};

/////////////////////////////////////////////////////////////////////////////

MLTextGenerate::MLTextGenerate()
{
  nVertices= 0;
  nTriangles= 0;
  count_triangles= true;
  triangle_index= 0;
  polygons= 0;
  front_z= back_z= 0.;
  ft= 0;
  back_vertices= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLTextGenerate::~MLTextGenerate()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLTextGenerate::generate( MeshRep* mesh,
                                GLTTFont& font,
                                const FormalText* _ft )
{
  if( mesh == 0 || _ft == 0 )
    return false;

  ft= _ft;

  const MLString& text= ft->text;

  if( text.length() == 0 )
    return false;
  if( !ft->front && !ft->side && !ft->back )
    return true; // silly boy

  int i;

  double precision= ft->precision;

  front_z= 0.;
  back_z= front_z - ft->extrude_height;

  FTGlyph** glyphs= new FTGlyph* [ text.length() ];
  FTGlyphVectorizer** vectorizers= new FTGlyphVectorizer* [ text.length() ];
  MLGlyphTriangulator** tris= new MLGlyphTriangulator* [ text.length() ];

  nVertices= 0;
  nTriangles= 0;
  count_triangles= true;
  polygons= 0;

  Point minP( 1e30, 1e30, 1e30 );
  Point maxP(-1e30,-1e30,-1e30 );

  double x= 0.;

  for( i= 0; i < text.length(); ++i )
    {
    glyphs[i]= 0;
    vectorizers[i]= 0;
    tris[i]= 0;

    int ch= (int) (unsigned char) text.charAt(i);

    FTGlyph* g= font.getFont()->getGlyph(ch);
    if( g == 0 )
      continue;

    glyphs[i]= 0;
    vectorizers[i]= new FTGlyphVectorizer;

    FTGlyphVectorizer*& v= vectorizers[i];

    v->setPrecision(precision);

    if( ! v->init(g) )
      {
      delete v;
      v= 0;
      continue;
      }

    if( ! v->vectorize() )
      {
      delete v;
      v= 0;
      x+= v->getAdvance();
      continue;
      }

    tris[i]= new MLGlyphTriangulator(v,this);

    if( ! tris[i]->triangulate() )
      {
      delete tris[i];
      tris[i]= 0;
      x+= v->getAdvance();
      continue;
      }

    for( int c= 0; c < v->getNContours(); ++c )
      {
      FTGlyphVectorizer::Contour* contour= v->getContour(c);
      if( contour == 0 )
        continue;
      for( int j= 0; j < contour->nPoints; ++j )
        {
        FTGlyphVectorizer::POINT* point= contour->points + j;
        point->data= (void*) new int;
        ++nVertices;
        *((int*) point->data)= nVertices;

        Point p= Point( x + point->x, point->y, 0. );
        minP= min( minP, p );
        maxP= max( maxP, p );
        }
      }

    x+= v->getAdvance();
    }

  if( nVertices == 0 )
    return false;

  int real_nVertices= nVertices;

  back_vertices= 0;
  if( ft->side )
    {
    real_nVertices+= nVertices;
    back_vertices= nVertices;
    }
   else
    {
    if( ft->front )
      {
      if( ft->back )
        {
        real_nVertices+= nVertices;
        back_vertices= nVertices;
        }
      }
     else
      {
      // ok, only back
      back_vertices= 0;
      }
    }

  int real_nTriangles= 0;
  if( ft->side )
    real_nTriangles+= nVertices;
  if( ft->front )
    real_nTriangles+= nTriangles;
  if( ft->back )
    real_nTriangles+= nTriangles;

  mesh->alloc( real_nVertices, 0, real_nTriangles );

  minP[2]= min( front_z, back_z );
  maxP[2]= max( front_z, back_z );
  Vector center= (minP + maxP) / 2.;

  // This is the real creation here

  count_triangles= false;
  polygons= &(mesh->polygons);
  triangle_index= 1;

  x= 0.;
  for( i= 0; i < text.length(); ++i )
    {
    GLTTGlyphTriangulator* t= tris[i];
    FTGlyphVectorizer* v= vectorizers[i];

    if( t == 0 || v == 0 )
      continue;

    for( int c= 0; c < v->getNContours(); ++c )
      {
      FTGlyphVectorizer::Contour* contour= v->getContour(c);
      if( contour == 0 )
        continue;
      if( contour->nPoints <= 0 )
        continue;
      for( int j= 0; j < contour->nPoints; ++j )
        {
        FTGlyphVectorizer::POINT* point= contour->points + j;
        Point p( x + point->x, point->y, front_z );
        p= (p - center) / 100.;

        int vi= *((int*) point->data);

        if( ft->front || ft->side )
          mesh->vertices[ vi ].point= p;

        if( ft->back || ft->side )
          {
          int vi_back= vi + back_vertices;
          mesh->vertices[ vi_back ].point= p;
          mesh->vertices[ vi_back ].point[2] -= ft->extrude_height;
          }
        }

      if( ft->side )
        {
        // ok, lets create this side

        for( int j= 0; j < contour->nPoints; ++j )
          {
          int j1= j;
          int j2= (j==0) ? (contour->nPoints-1) : (j-1);

          FTGlyphVectorizer::POINT* p1= contour->points + j1;
          FTGlyphVectorizer::POINT* p2= contour->points + j2;

          int vi11= *((int*) p1->data); // front
          int vi21= *((int*) p2->data);
          int vi12= vi11 + back_vertices; // back
          int vi22= vi21 + back_vertices;

          assert( triangle_index <= polygons->getSize() );

          MLPolygon& polygon= (*polygons)[triangle_index];
          if( ft->reversed_sides )
            polygon.allocQuad( vi11, vi21, vi22, vi12 );
           else
            polygon.allocQuad( vi12, vi22, vi21, vi11 );
          ++triangle_index; // well, in fact this was a quad, no matter:-)
          }
        } // if ft->side
      }

    if( ft->front || ft->back )
      tris[i]->triangulate();

    x+= v->getAdvance();
    }

  count_triangles= true;
  polygons= 0;

  // ok, free back all the memory we've just allocated

  for( i= 0; i < text.length(); ++i )
    {
    delete tris[i];
    tris[i]= 0;

    FTGlyphVectorizer* v= vectorizers[i];
    if( v != 0 )
      {
      for( int c= 0; c < v->getNContours(); ++c )
        {
        FTGlyphVectorizer::Contour* contour= v->getContour(c);
        if( contour == 0 )
          continue;
        for( int j= 0; j < contour->nPoints; ++j )
          {
          FTGlyphVectorizer::POINT* point= contour->points + j;
          delete point->data;
          point->data= 0;
          }
        }

      delete vectorizers[i];
      vectorizers[i]= 0;
      }

    delete glyphs[i];
    glyphs[i]= 0;
    }

  delete tris;
  tris= 0;

  delete vectorizers;
  vectorizers= 0;

  delete glyphs;
  glyphs= 0;

  ft= 0;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLTextGenerate::triangle( FTGlyphVectorizer::POINT* p1,
                               FTGlyphVectorizer::POINT* p2,
                               FTGlyphVectorizer::POINT* p3 )
{
  if( count_triangles )
    {
    ++nTriangles;
    return;
    }

  if( polygons == 0 || ft == 0 )
    return;
  if( triangle_index > polygons->getSize() || p1 == 0 || p2 == 0 || p3 == 0 )
    return;

  int v1= *((int*)(p1->data));
  int v2= *((int*)(p2->data));
  int v3= *((int*)(p3->data));

  if( v1 < 0 || v1 > nVertices ||
      v2 < 0 || v2 > nVertices ||
      v3 < 0 || v3 > nVertices )
    return;

  if( ft->front )
    {
    MLPolygon& polygon= (*polygons)[triangle_index];
    polygon.allocTriangle( v1, v2, v3 );
    ++triangle_index;
    }

  if( ft->back )
    {
    MLPolygon& polygon= (*polygons)[triangle_index];

    int back_v1= v1 + back_vertices;
    int back_v2= v2 + back_vertices;
    int back_v3= v3 + back_vertices;
    polygon.allocTriangle( back_v3, back_v2, back_v1 );
    ++triangle_index;
    }
}

/////////////////////////////////////////////////////////////////////////////
