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
  MLRay.h

  Stephane Rehel

  January 22 1997
*/

#ifndef __MLRay_h
#define __MLRay_h

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

#ifndef __Box_h
#include "tools/Box.h"
#endif

#ifndef __Color_h
#include "tools/Color.h"
#endif

class MLMesh;
class MeshRep;
class MLPolygon;

/////////////////////////////////////////////////////////////////////////////

#define MLRAY_HUGE_DIST 1e30
#define MLRAY_SMALL_DIST 1e-6

/////////////////////////////////////////////////////////////////////////////

class MLRay
{
public:
  Point origin;
  Vector direction; // normalized

  double min_t, max_t;

  boolean skip_reversed;

  Point I; // intersection point in a triangle
  double t; // intersection distance
  float alpha, beta, gamma; // barycentric coord. for triangle
                            // intersection point

  MLPolygon* polygon; // the triangle
  MLMesh* mesh;

  boolean lightRay; // i.e. going to a light
                  // i.e. just find the first blocking object:
                  // distance sorting is no matter
  boolean radiosityLightRay; // i.e. check for radiosity objects for shadowing

  // return true can intersect objects residing in box
  boolean boxTest( const Box& box ) const
    {
    if( box.into(origin) )
      {
      return box.intersect_from_interior( origin, direction,
                                          min_t, MLRAY_HUGE_DIST );
      }

    return box.intersect_from_exterior( origin, direction, 0., max_t );
    }

  // Variables used by MLRayRenderer
  // current returned rgb color of the ray
  Color color;
  // Current ray index (==1 for eye ray)
  int ray_index;
  // Normal
  Vector normal;
  // Refraction index
  double refract_n;
  // Have we intersected a reversed polygon?
  double polygon_k_parameter; // (ray.direction|polygon.normal)
  // light ray transmission factor (initialy = Color(1,1,1))
  Color light_trans;
  // Speedup for light_trans
  boolean light_ray_blocked;

  boolean intersectPolygon( MeshRep* mesh, register MLPolygon& p );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLRay_h
