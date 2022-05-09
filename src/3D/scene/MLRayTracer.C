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
  MLRayTracer.C

  Stephane Rehel

  January 21 1997
*/

#ifndef NDEBUG
#include <stdio.h>
#endif

#include "MLRayTracer.h"

#include "MLScene.h"
#include "MLObject.h"
#include "MLMesh.h"
#include "MLChildrenIterator.h"
#include "MeshRep.h"

#include "MLHashBox.h"

#include "material/MLMaterial.h"

#include "MLRay.h"

/////////////////////////////////////////////////////////////////////////////

MLRayTracer::MLRayTracer( MLScene* _scene )
{
  scene= _scene;
}

/////////////////////////////////////////////////////////////////////////////

MLRayTracer::~MLRayTracer()
{
  scene= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLRayTracer::sendEyeRay( MLRay& ray )
{
  ray.mesh= 0;
  ray.polygon= 0;
  ray.lightRay= false;
//  ray.radiosityLightRay= false;
  sendInHier(ray,scene->getRootObject());
}

/////////////////////////////////////////////////////////////////////////////

// protected
boolean MLRayTracer::sendInHier( MLRay& ray, MLObject* object )
{
  if( ! object->bounded )
    return false; // empty hierarchy

  boolean skip_this_object= false;

  if( ray.lightRay && object->getObjectType() == MLObject::MESH )
    {
    register MLMesh* mesh= (MLMesh*) object;
    register MLMaterial* m= mesh->getMaterial();
    if( !m->generate_shadows ||
       (!m->isRadiosityContributor() && ray.radiosityLightRay) )
      {
      if( object->nChildren == 0 )
        return false;
      skip_this_object= true;
      }
    }

  Point ray_origin= ray.origin;
  Vector ray_direction= ray.direction;

  ray.origin= object->local_xform.transform.w2o(ray.origin);
  ray.direction= object->local_xform.transform.w2o(ray.direction);

  double dir_norm= ray.direction.norm();
  ray.direction /= dir_norm;

  double ray_min_t= ray.min_t;
  double ray_max_t= ray.max_t;
  ray.min_t *= dir_norm;
  ray.max_t *= dir_norm;

#define RESTORE ray.origin= ray_origin;       \
                ray.direction= ray_direction; \
                ray.min_t= ray_min_t;         \
                ray.max_t= ray_max_t;

  boolean got_one= false;

  if( ! skip_this_object )
    {
    if( object->nChildren > 0 && object->bounded )
      {
      if( ! ray.boxTest(object->hierBBox) )
        {
        RESTORE
        return false;
        }
      }

    if( object->getObjectType() != MLObject::MESH )
      goto hier_test;

    MLMesh* mlmesh= (MLMesh*) object;

    MeshRep* mesh= mlmesh->getMeshRep();
    if( mesh == 0 )
      goto hier_test;

    if( mesh->hashBox != 0 )
      {
      sendInHashBox(ray,mlmesh,got_one);

      if( ray.lightRay && ray.light_ray_blocked )
        {
        RESTORE
        return true;
        }
      }
     else
      {
      if( ! ray.boxTest(mesh->bbox) )
        {
        RESTORE
        return false;
        }

      MLPolygonArray& polygons= mesh->polygons;
      for( int i= polygons.getSize(); i; --i )
        {
        MLPolygon& polygon= polygons [ i ];
        if( ! polygon.allocated() )
          continue;
        if( polygon.nVertices != 3 )
          continue;

        if( ray.intersectPolygon(mesh,polygon) )
          {
          if( ray.lightRay )
            {
            MLMaterial* m= mlmesh->getMaterial();
            if( ! m->transmission.has_transmission )
              {
              ray.light_ray_blocked= true;

              RESTORE
              return true;
              }

            ray.light_trans &= m->transmission.compiled_color;
            }
           else
            {
            ray.max_t= ray.t;
            ray.mesh= mlmesh;
            got_one= true;
            }
          }
        }
      }
    }

hier_test:
  if( object->nChildren > 0 )
    {
    MLChildrenIterator oi(object);
    while( ! oi.eol() )
      {
      if( sendInHier(ray,oi++) )
        got_one= true;

      if( ray.lightRay && ray.light_ray_blocked )
        {
        RESTORE
        return true;
        }
      }
    }

  if( got_one && !ray.lightRay )
    {
    // these parameters belong to our hierarchy
    ray.max_t /= dir_norm;
    }
   else
    ray.max_t= ray_max_t;

  ray.origin= ray_origin;
  ray.direction= ray_direction;
  ray.min_t= ray_min_t;

  return got_one;

#undef RESTORE
}

/////////////////////////////////////////////////////////////////////////////

// return false if not intersecting hash box
boolean MLRayTracer::sendInHashBox( MLRay& ray,
                                  MLMesh* mlmesh,
                                  boolean& got_one )
{
  MeshRep* mesh= mlmesh->getMeshRep();
  MLHashBox* hashBox= mesh->hashBox;

  // 1st phase: initialisations
  double x_0= ray.origin.x();
  double y_0= ray.origin.y();
  double z_0= ray.origin.z();

  double dx= ray.direction.x();
  double dy= ray.direction.y();
  double dz= ray.direction.z();

  int stepx= (dx > 0.) ? (1) : ((dx < 0.) ? (-1) : (0));
  int stepy= (dy > 0.) ? (1) : ((dy < 0.) ? (-1) : (0));
  int stepz= (dz > 0.) ? (1) : ((dz < 0.) ? (-1) : (0));

  double x_min= hashBox->getMin().x();
  double y_min= hashBox->getMin().y();
  double z_min= hashBox->getMin().z();
  double x_max= hashBox->getMax().x();
  double y_max= hashBox->getMax().y();
  double z_max= hashBox->getMax().z();

  double t_out= 0.;

  // Computing the entry point coordinates and t
  for(;;)
    {
    boolean end= true;
    if( x_0 < x_min )
      {
      if( stepx <= 0 )
        return false;
      register double a= (x_min - x_0) / dx;
      x_0 = x_min;
      y_0 += a*dy;
      z_0 += a*dz;
      t_out += a;
      }
     else
      {
      if( x_0 > x_max )
        {
        if( stepx >= 0 )
          return false;
        register double a= (x_max - x_0) / dx;
        x_0 = x_max;
        y_0 += a*dy;
        z_0 += a*dz;
        t_out += a;
        }
      }

    if( y_0 < y_min )
      {
      if( stepy <= 0 )
        return false;
      end= false;
      register double a= (y_min - y_0) / dy;
      x_0 += a*dx;
      y_0 = y_min;
      z_0 += a*dz;
      t_out += a;
      }
     else
      {
      if( y_0 > y_max )
        {
        if( stepy >= 0 )
          return false;
        end= false;
        register double a= (y_max - y_0) / dy;
        x_0 += a*dx;
        y_0 = y_max;
        z_0 += a*dz;
        t_out += a;
        }
      }

    if( z_0 < z_min )
      {
      if( stepz <= 0 )
        return false;
      end= false;
      register double a= (z_min - z_0) / dz;
      x_0 += a*dx;
      y_0 += a*dy;
      z_0= z_min;
      t_out += a;
      }
     else
      {
      if( z_0 > z_max )
        {
        if( stepz >= 0 )
          return false;
        end= false;
        register double a= (z_max - z_0) / dz;
        x_0 += a*dx;
        y_0 += a*dy;
        z_0= z_max;
        t_out += a;
        }
      }

    if( end )
      break;
    }

  // Computing tx, ty, tz
  #define CLAMP(A,MIN,MAX) ((A)<(MIN)?(MIN):((A)>(MAX)?(MAX):(A)))

  int max_cell_x= hashBox->getX() - 1;
  int max_cell_y= hashBox->getY() - 1;
  int max_cell_z= hashBox->getZ() - 1;

  const double MAXREAL= 1e30;
  double dx_vox= hashBox->getVoxelSize().x();
  double dy_vox= hashBox->getVoxelSize().y();
  double dz_vox= hashBox->getVoxelSize().z();

  double deltax= stepx ? (dx_vox / fabs(dx)) : (MAXREAL);
  double deltay= stepy ? (dy_vox / fabs(dy)) : (MAXREAL);
  double deltaz= stepz ? (dz_vox / fabs(dz)) : (MAXREAL);

  double tx, ty, tz;
  double a;

  a= (x_0 - x_min) / dx_vox;
  int x= int(a);
  x= CLAMP(x, 0, max_cell_x);
  if( stepx > 0 )
    tx= t_out + (double(x+1) - a) * deltax;
   else
    tx= t_out + (a - double(x)) * deltax;

  a= (y_0 - y_min) / dy_vox;
  int y= int(a);
  y= CLAMP(y, 0, max_cell_y);
  if( stepy > 0 )
    ty= t_out + (double(y+1) - a) * deltay;
   else
    ty= t_out + (a - double(y)) * deltay;

  a= (z_0 - z_min) / dz_vox;
  int z= int(a);
  z= CLAMP(z, 0, max_cell_z);
  if( stepz > 0 )
    tz= t_out + (double(z+1) - a) * deltaz;
   else
    tz= t_out + (a - double(z)) * deltaz;

  int voxel_x_incr= hashBox->get_i_incr();
  int voxel_y_incr= hashBox->get_j_incr();
  int voxel_z_incr= hashBox->get_k_incr();
  int max_voxel_index= hashBox->getNVoxels();

  int voxel_index = x * voxel_x_incr + y * voxel_y_incr + z;

  if( stepx < 0 ) voxel_x_incr= -voxel_x_incr;
  if( stepy < 0 ) voxel_y_incr= -voxel_y_incr;
  if( stepz < 0 ) voxel_z_incr= -voxel_z_incr;

  // 2nd phase: incremental going through the grid
  boolean end= false;
  do{
    // Computing t_out for the last point within the grid

    if( t_out > ray.max_t || voxel_index >= max_voxel_index )
      break;

//    const MLHashBox::Voxel* voxel= hashBox->get(x,y,z);
    const MLHashBox::Voxel* voxel= hashBox->get(voxel_index);

    if( tx < ty )
      {
      if( tx < tz )
        {
        t_out= tx;
        x += stepx;
        voxel_index += voxel_x_incr;
        end= ( x < 0 || x > max_cell_x );
        tx += deltax;
        }
       else
        {
        t_out= tz;
        z += stepz;
        voxel_index += voxel_z_incr;
        end= ( z < 0 || z > max_cell_z );
        tz += deltaz;
        }
      }
     else
      {
      if( ty < tz )
        {
        t_out= ty;
        y += stepy;
        voxel_index += voxel_y_incr;
        end= ( y < 0 || y > max_cell_y );
        ty += deltay;
        }
       else
        {
        t_out= tz;
        z += stepz;
        end= ( z < 0 || z > max_cell_z );
        voxel_index += voxel_z_incr;
        tz += deltaz;
        }
      }

    if( voxel->polygons != 0 )
      {
      const int* voxel_polygons= voxel->polygons;
      MLPolygonArray& polygons= mesh->polygons;
      MLVertexArray& vertices= mesh->vertices;
      for( int i= voxel->nElements; i; --i, ++voxel_polygons )
        {
#ifndef NDEBUG
if( *voxel_polygons > mesh->nPolygons )
{
printf( "MLRayTracer Fatal (voxels): %d/%d %s (nPolygons=%d) (x,y,z)=(%d/%d,%d/%d,%d/%d)\n",
        *voxel_polygons,
        mesh->nPolygons,
        mlmesh->getName().name.get(),
        voxel->nElements,
        x, hashBox->getX(),
        y, hashBox->getY(),
        z, hashBox->getZ() );
}
#endif
        register MLPolygon& polygon= polygons[ *voxel_polygons ];

        if( ray.polygon == &polygon )
          continue; // already tested: the previous one

        register double ko= ( ray.direction | polygon.normal );
        if( ko == 0. )
          continue;

        if( ray.skip_reversed )
          {
          if( ray.lightRay )
            {
            if( ko <= 0. )
              continue;
            }
           else
            {
            if( ko >= 0. )
              continue;
            }
          }

        Point& p0= vertices[ polygon.elements[0].vertex ].point;
        register double k= ( (p0 - ray.origin) | polygon.normal ) / ko;
        if( k < ray.min_t || k > ray.max_t )
          continue;

        ray.t= k;
        Point point= ray.origin + k * ray.direction;

        register float x= point[polygon.ray_info.X] - p0[polygon.ray_info.X];
        register float y= point[polygon.ray_info.Y] - p0[polygon.ray_info.Y];

        register float alpha= x * polygon.ray_info.y2 + y * polygon.ray_info.x2;
        if( alpha < 0. || alpha > 1. )
          continue;
        register float beta=  y * polygon.ray_info.x1 + x * polygon.ray_info.y1;
        if( beta < 0. || beta > 1. )
          continue;
        register float gamma= 1. - alpha - beta;
        if( gamma < 0. || gamma > 1. )
          continue;

        if( ray.lightRay )
          {
          MLMaterial* m= mlmesh->getMaterial();
          if( ! m->transmission.has_transmission )
            {
            ray.light_ray_blocked= true;
            return true;
            }

          ray.light_trans &= m->transmission.compiled_color;
          ray.polygon= &polygon;
          }
         else
          {
          ray.I= point;
          ray.alpha= alpha;
          ray.beta= beta;
          ray.gamma= gamma;

          ray.polygon= &polygon;
          ray.polygon_k_parameter= ko;

          ray.max_t= ray.t;
          ray.mesh= mlmesh;
          got_one= true;
          }
        }
      }
    }
  while( ! end );

  return true;
#undef CLAMP
}

/////////////////////////////////////////////////////////////////////////////

// return true if blocked
// MLRay::radiosityLightRay must be set/unset before!
boolean MLRayTracer::findBlocking( MLRay& ray )
{
  ray.mesh= 0;
  ray.polygon= 0;
  ray.lightRay= true;
  ray.light_ray_blocked= false;
  ray.light_trans= Color(1,1,1);

  sendInHier(ray,scene->getRootObject());

  return ray.light_ray_blocked;
}

/////////////////////////////////////////////////////////////////////////////

