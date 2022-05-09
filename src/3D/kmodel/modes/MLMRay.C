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
  MLMRay.C

  Stephane Rehel

  September 21 1997
*/

#include "scene/MLRay.h"
#include "scene/MLRayTracer.h"
#include "scene/MLCamera.h"
#include "scene/MLMesh.h"

#include "interface/MLMode.h"
#include "interface/MLModes.h"

#include "kmodel/ModelModule.h"

#include "MLMRay.h"

/////////////////////////////////////////////////////////////////////////////

MLMRay::MLMRay( ModelModule* _model ):
  MLMode(_model)
{
  button= 0;
  hit_point= Point(0,0,0);
  distance= 0.;
  hit_polygon= 0;
  hit_mesh= 0;

  name= "Select a 3D point by ray-casting";
  action= MLMode::CLICKED_MOTION_ACTION;

  only_selected= false;
}

/////////////////////////////////////////////////////////////////////////////

MLMRay::~MLMRay()
{
  button= 0;
  hit_point= Point(0,0,0);
  distance= 0.;
  hit_polygon= 0;
  hit_mesh= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLMRay::enter()
{
  button= 0;
  hit_point= Point(0,0,0);
  distance= 0.;
  hit_polygon= 0;
  hit_mesh= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLMRay::select( int screen_x, int screen_y, MLCamera& camera )
{
  ModelModule* model= (ModelModule*) module;
  MLScene* scene= model->getScene();

  MLRayTracer raytracer(scene);
  MLRay ray;

  Point screen_point= Point( double(screen_x), double(screen_y), 0. );
  Point space_point= camera.unproject(screen_point);

  if( camera.ortho )
    {
    ray.direction= camera.direction;
    ray.origin= space_point;
    }
   else
    {
    ray.origin= camera.location;
    ray.direction= space_point - camera.location;
    }

  ray.direction.setNormalized();
  ray.skip_reversed= true;
  ray.min_t= camera.nearClippingPlane;
  ray.max_t= MLRAY_HUGE_DIST;
  ray.ray_index= 1;

  int infinite_loop= 0;

  for(;;)
    {
    raytracer.sendEyeRay(ray);

    if( ray.polygon == 0 || ray.mesh == 0 )
      return false;

    if( ! only_selected )
      break;

    if( ray.mesh->selected() )
      break;

    ray.min_t= ray.t;

    if( infinite_loop++ >= 500000 )
      return false; // just in case...:-)
    }

  hit_polygon= ray.polygon;
  hit_mesh= ray.mesh;
  hit_point= ray.mesh->getGlobalXForm().o2w(ray.I);
  distance= (ray.I - camera.location).norm();

  return true;
}

/////////////////////////////////////////////////////////////////////////////
