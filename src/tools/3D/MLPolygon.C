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
   MLPolygon.C

   November 10 1996
   Stephane Rehel

   Moved to tools/ on April 3 1998
*/

#include <stdio.h>
#include <assert.h>

#include "tools/MLDArray.h"
#include "tools/xdr/XDRFile.h"

#include "MLVertex.h"
#include "MLEdge.h"
#include "MLPolygon.h"

/////////////////////////////////////////////////////////////////////////////

MLPolygon& MLPolygon::operator = ( const MLPolygon& p )
{
  if( &p == this )
    return *this;

  index= p.index;

  delete elements;
  elements= 0;

  nVertices= p.nVertices;

  if( nVertices > 0 )
    {
    elements= new MLPolygon::Element [ nVertices ];

    memcpy( (void*) elements,
            (const void*) p.elements,
            sizeof(elements[0]) * nVertices );
    }

  delete tesselation;
  tesselation= 0;

  nTessTriangles= p.nTessTriangles;
  if( p.tesselation > 0 )
    {
    tesselation= new int [ nTessTriangles ];
    memcpy( (void*) tesselation,
            (const void*) p.tesselation,
            sizeof(tesselation[0]) * nTessTriangles );
    }

  fatherPolygon= p.fatherPolygon;

  normal= p.normal;
  area= p.area;
  material= p.material;
  flags= p.flags;

  ray_info.x1= p.ray_info.x1;
  ray_info.y1= p.ray_info.y1;
  ray_info.x2= p.ray_info.x2;
  ray_info.y2= p.ray_info.y2;
  ray_info.X= p.ray_info.X;
  ray_info.Y= p.ray_info.Y;

  return *this;
}

/////////////////////////////////////////////////////////////////////////////

int MLPolygon::getSizeOf() const
{
  return sizeof(*this) + nVertices * sizeof(elements[0])
                       + nTessTriangles * sizeof(tesselation[0]);
}

/////////////////////////////////////////////////////////////////////////////

void MLPolygon::alloc( short _nVertices )
{
  delete elements;

  nVertices= max( (int)1, (int)_nVertices );

  elements= new MLPolygon::Element [ nVertices ];

  for( int i= 0; i < nVertices; ++i )
    {
    Element& e= elements[i];
    e.vertex= 0;
    e.edge= 0;
    e.svertex= 0;
    }

  flags |= Flag(ALLOCATED);
}

/////////////////////////////////////////////////////////////////////////////

void MLPolygon::allocTriangle( int v1, int v2, int v3 )
{
  nVertices= 3;

  elements= new MLPolygon::Element [ 3 ];

  elements[0].edge=
  elements[1].edge=
  elements[2].edge= 0;

  elements[0].svertex=
  elements[1].svertex=
  elements[2].svertex= 0;

  elements[0].vertex= v1;
  elements[1].vertex= v2;
  elements[2].vertex= v3;

  flags |= Flag(ALLOCATED);
}

/////////////////////////////////////////////////////////////////////////////

void MLPolygon::allocQuad( int v1, int v2, int v3, int v4 )
{
  nVertices= 4;

  elements= new MLPolygon::Element [ 4 ];

  elements[0].edge=
  elements[1].edge=
  elements[2].edge=
  elements[3].edge= 0;

  elements[0].svertex=
  elements[1].svertex=
  elements[2].svertex=
  elements[3].svertex= 0;

  elements[0].vertex= v1;
  elements[1].vertex= v2;
  elements[2].vertex= v3;
  elements[3].vertex= v4;

  flags |= Flag(ALLOCATED);
}

/////////////////////////////////////////////////////////////////////////////

void MLPolygon::updateNormalArea( const MLDArray<MLVertex>& all_vertices,
                                  MLDArray<MLPolygon>& all_polygons,
                                  boolean recurse /* = false */ )
{
  normal= Vector(0,0,0);
  for( int j= 0; j < nVertices; ++j )
    {
    register int vi1= elements[j].vertex;
    register int vi2= elements[ (j+1) % nVertices ].vertex;

    register const Point& v1= all_vertices.get(vi1).point;
    register const Point& v2= all_vertices.get(vi2).point;
    normal[0]+= (v1.geti(1)-v2.geti(1)) * (v2.geti(2)+v1.geti(2));
    normal[1]+= (v1.geti(2)-v2.geti(2)) * (v2.geti(0)+v1.geti(0));
    normal[2]+= (v1.geti(0)-v2.geti(0)) * (v2.geti(1)+v1.geti(1));
    }

  double norm= normal.norm();
  area= norm * 0.5;

  if( norm < 1e-20 )
    {
    ray_info.X= 0;
    ray_info.Y= 1;
    ray_info.x1= ray_info.y1= ray_info.x2= ray_info.y2= 0.;
    return;
    }

  normal /= norm;

  if( nVertices != 3 )
    {
    ray_info.X= 0;
    ray_info.Y= 1;
    ray_info.x1= ray_info.y1= ray_info.x2= ray_info.y2= 0.;
    }
   else
    {
#define vertex(i) all_vertices.get( elements[i].vertex ).point
    char& X= ray_info.X;
    char& Y= ray_info.Y;
    if( fabs(normal.x()) > fabs(normal.y()) )
      {
      if( fabs(normal.x()) > fabs(normal.z()) )
        { X= 1; Y= 2; } // max = 0
       else
        { X= 0; Y= 1; } // max = 2
      }
     else
      {
      if( fabs(normal.y()) > fabs(normal.z()) )
        { X= 2; Y= 0; } // max = 1
       else
        { X= 0; Y= 1; } // max = 2
      }

//    if( normal[Z] < 0. )
//      swap(X,Y);

    const Point& p0= vertex(0);
    const Point& p1= vertex(1);
    const Point& p2= vertex(2);

    ray_info.x1= p1.geti(X) - p0.geti(X);
    ray_info.y1=-p1.geti(Y) + p0.geti(Y);
    ray_info.x2=-p2.geti(X) + p0.geti(X);
    ray_info.y2= p2.geti(Y) - p0.geti(Y);
    float delta= ray_info.x1*ray_info.y2 - ray_info.x2*ray_info.y1;
    if( fabs(delta) > 1e-20 )
      {
      ray_info.x1 /= delta;
      ray_info.y1 /= delta;
      ray_info.x2 /= delta;
      ray_info.y2 /= delta;
      }
#undef vertex
    }

  if( recurse && tesselation != 0 )
    {
    for( int i= 0; i < nTessTriangles; ++i )
      {
      int ti= tesselation[i];
      if( ti == 0 )
        continue;
      MLPolygon& p= all_polygons[ti];
      p.updateNormalArea( all_vertices, all_polygons, true );
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

/*
// ray.direction is to be normalized
boolean MLPolygon::intersect( MLRay& ray )
{
  if( nVertices > 3 )
    {
    if( tesselation == 0 )
      return false;

    for( int i= 0; i < nTessTriangles; ++i )
      if( mesh->polygons[ tesselation[i] ].intersect(ray) )
        return true;

    return false;
    }

  register double ko= ( ray.direction | normal );
  if( fabs(ko) < 1e-10 )
    return false;

  if( ray.skip_reversed )
    {
    if( ray.lightRay )
      {
      if( ko <= 0. )
        return false;
      }
     else
      {
      if( ko >= 0. )
        return false;
      }
    }

  MLVertexArray& global_vertices= mesh->vertices;
#define vertex(i) global_vertices[ elements[i].vertex ].point
  Point& p0= vertex(0);
  register double k= ( (p0 - ray.origin) | normal ) / ko;
  ray.t= k;
  if( k < ray.min_t || k > ray.max_t )
    return false;

  Point point= ray.origin + k * ray.direction;

  register float x= point[ray_info.X] - p0[ray_info.X];
  register float y= point[ray_info.Y] - p0[ray_info.Y];

  register float alpha= x * ray_info.y2 + y * ray_info.x2;
  if( alpha < 0. || alpha > 1. )
    return false;
  register float beta=  y * ray_info.x1 + x * ray_info.y1;
  if( beta < 0. || beta > 1. )
    return false;
  register float gamma= 1. - alpha - beta;
  if( gamma < 0. || gamma > 1. )
    return false;

  ray.I= point;
  ray.alpha= alpha;
  ray.beta= beta;
  ray.gamma= gamma;

  ray.polygon= this;
  ray.polygon_k_parameter= ko;

  return true;
#undef vertex
}
*/

/////////////////////////////////////////////////////////////////////////////

void MLPolygon::unallocate()
{
  delete elements; elements= 0;
  nVertices= 0;
  delete tesselation; tesselation= 0;
  nTessTriangles= 0;

  flags &= ~Flag(ALLOCATED);
}

/////////////////////////////////////////////////////////////////////////////

Point MLPolygon::getCenter( const MLDArray<MLVertex>& all_vertices ) const
{
  Point center(0,0,0);

  if( nVertices == 0 )
    return center;

  for( int i= 0; i < nVertices; ++i )
    {
    const MLVertex& v= all_vertices.get( elements[i].vertex );
    center+= v.point;
    }

  return center / double(nVertices);
}

/////////////////////////////////////////////////////////////////////////////

MLPolygon& MLPolygon::getFatherPolygon( MLDArray<MLPolygon>& all_polygons )
{
  MLPolygon* p= this;
  for(;;)
    {
    if( p->fatherPolygon == 0 )
      return *p;
    assert( p->fatherPolygon != index );
    p= &(all_polygons[ p->fatherPolygon ]);
    }
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPolygon::serialWrite( XDRFile* file )
{
  if( file == 0 )
    return false;

#define write_double(r) if( ! file->putDouble(r) ) return false;
#define write_int(it) if( ! file->putInteger(it) ) return false;

  write_int(int(flags));

  int j;

  write_int(nVertices);
  for( j= 0; j < nVertices; ++j )
    {
    Element* e= elements+j;
    write_int(e->vertex);
    write_int(e->edge);
    write_int(e->svertex);
    }

  write_int(fatherPolygon);
  write_int(nTessTriangles);
  for( j= 0; j < nTessTriangles; ++j )
    write_int( tesselation[j] );

  write_int(material);

#undef write_double
#undef write_int

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPolygon::serialRead( XDRFile* file )
{
  if( file == 0 )
    return false;

#define read_double(c) { double r; if(!file->getDouble(&r)) return false; c=r; }
#define read_int(it) { int _it; if(!file->getInteger(&_it)) return false; it=_it; }

  read_int(flags);

  read_int(nVertices);
  delete elements;
  elements= 0;
  if( nVertices > 0 )
    {
    elements= new Element [ nVertices ];
    for( int j= 0; j < nVertices; ++j )
      {
      Element* e= elements+j;
      read_int(e->vertex);
      read_int(e->edge);
      read_int(e->svertex);
      }
    }

  read_int(fatherPolygon);
  read_int(nTessTriangles);
  delete tesselation;
  tesselation= 0;
  if( nTessTriangles > 0 )
    {
    tesselation= new int [ nTessTriangles ];
    for( int j= 0; j < nTessTriangles; ++j )
      read_int( tesselation[j] );
    }

  read_int(material);

#undef read_double
#undef read_int

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return the size (in bytes) that will be read/written by
// serialRead()/serialWrite() on this property
int MLPolygon::serialSize() const
{
  int s= 0;

  // flags
  s+= XDROp::getIntegerSize();

  // vertices
  s+= XDROp::getIntegerSize() * 3 * nVertices;

  // tess
  s+= XDROp::getIntegerSize() * (2 + nTessTriangles);

  // material
  s+= XDROp::getIntegerSize();

  return s;
}

/////////////////////////////////////////////////////////////////////////////
