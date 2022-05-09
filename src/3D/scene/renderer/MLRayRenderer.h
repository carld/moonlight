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
  MLRayRenderer.h

  Stephane Rehel

  January 21 1997
*/

#ifndef __MLRayRenderer_h
#define __MLRayRenderer_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLRay_h
#include "scene/MLRay.h"
#endif

class MLRendererOptions;
class MLRayTracer;
class MLCamera;
class MLScene;
class MLRay;
class MLLight;
class MLGammaCorrection;

struct MaterialLightSpeedup; // in MLRayRenderer.C

/////////////////////////////////////////////////////////////////////////////

class MLRayRenderer
{
protected:
  MLScene* scene;
  MLRendererOptions* options; // from scene

  MLCamera* camera;

  int xres, yres; // resolution
  int x1, y1, x2, y2; // rendered area

  boolean own_buffer;
  unsigned char* buffer;

  MLRayTracer* raytracer;

  boolean do_radiosity_shadows;

  int* nRaytracedLights; // this array is nMaterials long
  MaterialLightSpeedup** matlights;
  int nMatLights; // number of materials + 1

  MLGammaCorrection* gammaCorrection;

  // initialized by init_trace()
  struct TraceInfo
    {
    Point screen_point;
    double viewport_width;
    double viewport_height;
    double x_incr;
    MLRay ray;
    const unsigned char* gamma_table;
    int width;
    int height;
    int j, y;
    };
  TraceInfo ti;

public:
  MLRayRenderer( MLScene* _scene,
                 const MLCamera& _camera,
                 boolean use_camera_resolution = false );

  virtual ~MLRayRenderer();

  // optional
  void setRenderedArea( int _x1, int _y1, int _x2, int _y2 );

  // required
  void allocBuffer( unsigned char* _buffer = 0 );

  // required
  void init();

  void init_trace();
  boolean trace_next_line();
  void trace();

private:
  void compute_eye_ray( MLRay& ray );

};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLRayRenderer_h
