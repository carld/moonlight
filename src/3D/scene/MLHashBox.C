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
  MLHashBox.C

  Stephane Rehel
  February 24 1997
*/

#include <string.h>

#include "tools/Box.h"
#include "tools/IVector3.h"

#include "tools/3D/MLVertex.h"
#include "tools/3D/MLPolygon.h"

#include "MeshRep.h"
#include "MLHashBox.h"

static const unsigned short defaultVoxelsCapacity= 4;

/////////////////////////////////////////////////////////////////////////////

boolean MLHashBox::Voxel::add( int id )
{
  if( polygons == 0 )
    {
    polygons= new int [ ::defaultVoxelsCapacity ];
    maxElements= ::defaultVoxelsCapacity;
    polygons[0]= id;
    nElements= 1;
    return true;
    }

  for( int i= 0; i < nElements; ++i )
    if( polygons[i] == id )
      return false; // already have it

  if( maxElements == nElements )
    {
    if( int(maxElements) == 65535 )
      return false; // full
    int new_maxElements= int(maxElements) + int(maxElements) / 2;
    if( new_maxElements > 65535 )
      new_maxElements= 65535;

    int* new_polygons= new int [ new_maxElements ];

    memcpy( (void*) new_polygons,
            (void*) polygons,
            maxElements * sizeof(polygons[0]) );

    delete polygons;
    maxElements= new_maxElements;
    polygons= new_polygons;
    }

  polygons[ nElements++ ]= id;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// reading from disk
void MLHashBox::Voxel::alloc( int _nElements )
{
  delete polygons;
  if( _nElements <= 0 )
    {
    polygons= 0;
    nElements= 0;
    maxElements= 0;
    return;
    }

  nElements= (unsigned short) _nElements;

  if( nElements < ::defaultVoxelsCapacity )
    maxElements= ::defaultVoxelsCapacity;
   else
    maxElements= nElements;

  polygons= new int [ maxElements ];
  for( unsigned short i= 0; i < maxElements; ++i )
    polygons[i]= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLHashBox::MLHashBox( const Box& box )
{
  nx= ny= nz= nVoxels= 0;
  raw= 0;
  array= 0;

  minP= box.getMin();
  maxP= box.getMax();
  size= maxP - minP;
  voxelSize= size;
  voxelsph_radius= 0.;

  i_incr= j_incr= k_incr= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLHashBox::~MLHashBox()
{
  destroy();
}

/////////////////////////////////////////////////////////////////////////////

int MLHashBox::getSizeOf() const
{
  int s= sizeof(*this);

  if( raw != 0 )
    {
    s += nVoxels * sizeof(raw[0]);
    int n= 0;
    for( int i= nVoxels-1; i >= 0; --i )
      if( raw[i].polygons != 0 )
        n+= raw[i].maxElements;

    s+= n * sizeof(raw[0].polygons[0]);
    }

  return s;
}

/////////////////////////////////////////////////////////////////////////////

void MLHashBox::destroy()
{
  delete [] raw; raw= 0;

  if( array != 0 )
    {
    for( int i= 0; i < nx; ++i )
      delete array[i];
    delete array;
    array= 0;
    }

  nx= ny= nz= nVoxels= 0;

  i_incr= j_incr= k_incr= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLHashBox::create( int _nx, int _ny, int _nz )
{
  destroy();

  nx= _nx;
  ny= _ny;
  nz= _nz;

  if( nx <= 0 ) nx= 1;
  if( ny <= 0 ) ny= 1;
  if( nz <= 0 ) nz= 1;

  nVoxels= nx * ny * nz;
  raw= new Voxel [ nVoxels ];

  array= new Voxel** [ nx ];
  for( int i= 0; i < nx; ++i )
    {
    array[i]= new Voxel* [ ny ];
    for( int j= 0; j < ny; ++j )
      array[i][j]= raw + (i * ny + j) * nz;
    }

  voxelSize[0]= size.x() / double(nx);
  voxelSize[1]= size.y() / double(ny);
  voxelSize[2]= size.z() / double(nz);

  k_incr= 1;
  j_incr= k_incr * nz;
  i_incr= j_incr * ny;

  voxelsph_radius= (voxelSize * 0.5).norm();
}

/////////////////////////////////////////////////////////////////////////////

void MLHashBox::locate( const Point& p, IPoint3& ip )
{
  Vector v= p - minP;
  double x= v.x() / voxelSize.x();
  double y= v.y() / voxelSize.y();
  double z= v.z() / voxelSize.z();

  if( x < 0. ) ip[0]= 0; else
  if( x >= double(nx) ) ip[0]= nx-1; else
  ip[0]= int(x);

  if( y < 0. ) ip[1]= 0; else
  if( y >= double(ny) ) ip[1]= ny-1; else
  ip[1]= int(y);

  if( z < 0. ) ip[2]= 0; else
  if( z >= double(nz) ) ip[2]= nz-1; else
  ip[2]= int(z);
}

/////////////////////////////////////////////////////////////////////////////

void MLHashBox::addTriangle( MeshRep* mesh, MLPolygon& polygon )
{
  if( !allocated() )
    return;
  if( polygon.nVertices != 3 )
    return;

  if( polygon.area < 1e-20 )
    return;

  Point& p1= mesh->vertices[ polygon.elements[0].vertex ].point;
  Point& p2= mesh->vertices[ polygon.elements[1].vertex ].point;
  Point& p3= mesh->vertices[ polygon.elements[2].vertex ].point;

  Point pp= p1 - voxelSize * 0.5;

  IPoint3 ip1, ip2;

  locate( min(p1,min(p2,p3)), ip1 );
  locate( max(p1,max(p2,p3)), ip2 );

  int id= polygon.index;

  Point voxelMinP0= minP + (voxelSize & Vector(ip1[0],ip1[1],ip1[2]));
  Point voxelMinP= voxelMinP0;
  Point voxelMaxP= voxelMinP0 + voxelSize;

  int i_index= ip1[0] * i_incr;
  int j_index0= ip1[1] * j_incr;
  for( int i= ip1[0]; i <= ip2[0]; ++i, i_index+= i_incr )
    {
    voxelMinP[1]= voxelMinP0[1];
    voxelMaxP[1]= voxelMinP0[1] + voxelSize.y();
    int j_index= i_index + j_index0;
    for( int j= ip1[1]; j <= ip2[1]; ++j, j_index+= j_incr )
      {
      voxelMinP[2]= voxelMinP0[2];
      voxelMaxP[2]= voxelMinP0[2] + voxelSize.z();
      int index= j_index + ip1[2];
      for( int k= ip1[2]; k <= ip2[2]; ++k, ++index )
        {

        if( fabs( (pp-voxelMinP) | polygon.normal ) > voxelsph_radius )
          goto dont_add;

        if( p1.x() >= voxelMinP.x() && p1.x() <= voxelMaxP.x() &&
            p1.y() >= voxelMinP.y() && p1.y() <= voxelMaxP.y() &&
            p1.z() >= voxelMinP.z() && p1.z() <= voxelMaxP.z() )
          goto add_it;
        if( p2.x() >= voxelMinP.x() && p2.x() <= voxelMaxP.x() &&
            p2.y() >= voxelMinP.y() && p2.y() <= voxelMaxP.y() &&
            p2.z() >= voxelMinP.z() && p2.z() <= voxelMaxP.z() )
          goto add_it;
        if( p3.x() >= voxelMinP.x() && p3.x() <= voxelMaxP.x() &&
            p3.y() >= voxelMinP.y() && p3.y() <= voxelMaxP.y() &&
            p3.z() >= voxelMinP.z() && p3.z() <= voxelMaxP.z() )
          goto add_it;

        {
#define TRI_EDGE_TEST1(Z,X,Y)                              \
        if( direction.Z() != 0. )                                \
          {                                                      \
          register double t= -(O.Z()-voxelSize.Z()) / direction.Z(); \
          if( t >= 0. && t <= 1. )                               \
            {                                                    \
            register double I= O.X() + t * direction.X();        \
            if( I >= 0. && I <= voxelSize.X() )                  \
              {                                                  \
              I= O.Y() + t * direction.Y();                      \
              if( I >= 0. && I <= voxelSize.Y() )                \
                goto add_it;                                     \
              }                                                  \
            }                                                    \
          t= -(O.Z()) / direction.Z();                           \
          if( t >= 0. && t <= 1. )                               \
            {                                                    \
            register double I= O.X() + t * direction.X();        \
            if( I >= 0. && I <= voxelSize.X() )                  \
              {                                                  \
              I= O.Y() + t * direction.Y();                      \
              if( I >= 0. && I <= voxelSize.Y() )                \
                goto add_it;                                     \
              }                                                  \
            }                                                    \
          }

#define TRI_EDGE_TEST(p1,p2)  \
        {                           \
        Vector O= p1 - voxelMinP;   \
        Vector direction= p2 - p1;  \
                                    \
        TRI_EDGE_TEST1(z,x,y)       \
        TRI_EDGE_TEST1(y,z,x)       \
        TRI_EDGE_TEST1(x,y,z)       \
        }

        TRI_EDGE_TEST(p1,p2)
        TRI_EDGE_TEST(p2,p3)
        TRI_EDGE_TEST(p3,p1)

#undef TRI_EDGE_TEST
#undef TRI_EDGE_TEST1
        }

#define BOX_EDGE_TEST( _origin, fail_label, DIR ) \
          {                                                        \
          Point origin= _origin;                                   \
          register double k= ( (p1 - origin) | polygon.normal )    \
                            / polygon.normal[DIR];                 \
                                                                   \
          if( k < 0. || k > voxelSize[DIR] )                       \
            goto fail_label;                                       \
                                                                   \
          Point point= origin;                                     \
          point[DIR] += k;                                         \
                                                                   \
          register float x= point[polygon.ray_info.X]              \
                           -p1[polygon.ray_info.X];                \
          register float y= point[polygon.ray_info.Y]              \
                           -p1[polygon.ray_info.Y];                \
                                                                   \
          register float alpha= x * polygon.ray_info.y2            \
                              + y * polygon.ray_info.x2;           \
          if( alpha < 0. || alpha > 1. )                           \
            goto fail_label;                                       \
          register float beta=  y * polygon.ray_info.x1            \
                              + x * polygon.ray_info.y1;           \
          if( beta < 0. || beta > 1. )                             \
            goto fail_label;                                       \
          register float gamma= 1. - alpha - beta;                 \
          if( gamma < 0. || gamma > 1. )                           \
            goto fail_label;                                       \
                                                                   \
          goto add_it;                                             \
          }                                                        \
          fail_label:;

        if( fabs(polygon.normal.x()) > 1e-20 )
          {
          BOX_EDGE_TEST( Point(voxelMinP.x(),voxelMinP.y(),voxelMinP.z()),
                         fail1x, 0 )
          BOX_EDGE_TEST( Point(voxelMinP.x(),voxelMaxP.y(),voxelMinP.z()),
                         fail2x, 0 )
          BOX_EDGE_TEST( Point(voxelMinP.x(),voxelMaxP.y(),voxelMaxP.z()),
                         fail3x, 0 )
          BOX_EDGE_TEST( Point(voxelMinP.x(),voxelMinP.y(),voxelMaxP.z()),
                         fail4x, 0 )
          }

        if( fabs(polygon.normal.y()) > 1e-20 )
          {
          BOX_EDGE_TEST( Point(voxelMinP.x(),voxelMinP.y(),voxelMinP.z()),
                         fail1y, 1 )
          BOX_EDGE_TEST( Point(voxelMaxP.x(),voxelMinP.y(),voxelMinP.z()),
                         fail2y, 1 )
          BOX_EDGE_TEST( Point(voxelMaxP.x(),voxelMinP.y(),voxelMaxP.z()),
                         fail3y, 1 )
          BOX_EDGE_TEST( Point(voxelMinP.x(),voxelMinP.y(),voxelMaxP.z()),
                         fail4y, 1 )
          }

        if( fabs(polygon.normal.z()) > 1e-20 )
          {
          BOX_EDGE_TEST( Point(voxelMinP.x(),voxelMinP.y(),voxelMinP.z()),
                         fail1z, 2 )
          BOX_EDGE_TEST( Point(voxelMaxP.x(),voxelMinP.y(),voxelMinP.z()),
                         fail2z, 2 )
          BOX_EDGE_TEST( Point(voxelMaxP.x(),voxelMaxP.y(),voxelMinP.z()),
                         fail3z, 2 )
          BOX_EDGE_TEST( Point(voxelMinP.x(),voxelMaxP.y(),voxelMinP.z()),
                         fail4z, 2 )
          }

#undef BOX_EDGE_TEST

        goto dont_add;

add_it:
        raw[index].add(id);
dont_add:
        voxelMinP[2] += voxelSize.z();
        voxelMaxP[2] += voxelSize.z();
        }
      voxelMinP[1] += voxelSize.y();
      voxelMaxP[1] += voxelSize.y();
      }
    voxelMinP[0] += voxelSize.x();
    voxelMaxP[0] += voxelSize.x();
    }
}

/////////////////////////////////////////////////////////////////////////////

