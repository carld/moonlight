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
  MLRay.C

  Stephane Rehel
  January 22 1997
*/

#include "tools/Box.h"
#include "MLRay.h"

#include "tools/3D/MLPolygon.h"
#include "MeshRep.h"

/////////////////////////////////////////////////////////////////////////////

boolean MLRay::intersectPolygon( MeshRep* mesh, register MLPolygon& p )
{
  if( p.nVertices > 3 )
    {
    if( p.tesselation == 0 )
      return false;

    for( int i= 0; i < p.nTessTriangles; ++i )
      if( intersectPolygon( mesh,
                            mesh->polygons[ p.tesselation[i] ] ) )
        return true;

    return false;
    }

  register double ko= ( direction | p.normal );
  if( fabs(ko) < 1e-10 )
    return false;

  if( skip_reversed )
    {
    if( lightRay )
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
#define vertex(i) global_vertices[ p.elements[i].vertex ].point
  Point& p0= vertex(0);
  register double k= ( (p0 - origin) | p.normal ) / ko;
  t= k;
  if( k < min_t || k > max_t )
    return false;

  Point point= origin + k * direction;

  register float x= point[p.ray_info.X] - p0[p.ray_info.X];
  register float y= point[p.ray_info.Y] - p0[p.ray_info.Y];

  register float alpha= x * p.ray_info.y2 + y * p.ray_info.x2;
  if( alpha < 0. || alpha > 1. )
    return false;
  register float beta=  y * p.ray_info.x1 + x * p.ray_info.y1;
  if( beta < 0. || beta > 1. )
    return false;
  register float gamma= 1. - alpha - beta;
  if( gamma < 0. || gamma > 1. )
    return false;

  I= point;
  alpha= alpha;
  beta= beta;
  gamma= gamma;

  polygon= &p;
  polygon_k_parameter= ko;

  return true;
#undef vertex
}

/////////////////////////////////////////////////////////////////////////////
