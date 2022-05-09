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
   MLCamera.h

   Stephane Rehel

   Creation: SR, August 18th, 1995
   Revised: January 19 1997
*/

#ifndef __MLCamera_h
#define __MLCamera_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

#ifndef __Matrix4x4_h
#include "tools/Matrix4x4.h"
#endif

class MLWFileBlock;
class MLRFileBlock;

/////////////////////////////////////////////////////////////////////////////

class MLCamera
{
public:
  // these parameters are set by init() functions
  Point location;
  Vector direction; // normalized
  Vector sky; // normalized

  Point lookat; // also known as orbit point

  boolean ortho;
  double orthoZoom;

  Vector right, up; // normalized

  int x_res, y_res; // resolution of screen

  double angle; // radians
  double nearClippingPlane,
         farClippingPlane;

  // set by MLCamera::compute()
  Matrix4x4 projection;
  Matrix4x4 inv_projection;

  double phi, theta; // spheric coord. relative to lookat. (radians)

public:
  MLCamera();
  MLCamera( const MLCamera& c );

  void init( const Point& _location,
             const Vector& _direction, // not normalized
             const Vector& _sky // not normalized
           );

  void initByLookat( const Point& _location,
                     const Point& _lookat,
                     const Vector& _sky // not normalized
                   );

  // relavite to lookat point
  void initSpheric( double _phi, // radians
                    double _theta, // tadians
                    double _radius );

  void getOrthoSize( double& sx, double& sy ) const;

  // set up, right, and projection/inv_projection matrices
  void compute();

  static void makePickMatrix( double x, double y,
                              double width, double height,
                              int viewport[4],
                              Matrix4x4& pick );

  // from screen to space
  Point unproject( const Point& win ) const;

  Matrix4x4 getPixelProjection() const;

  void write( MLWFileBlock& block ) const;
  void read( MLRFileBlock& block );
};

/////////////////////////////////////////////////////////////////////////////

MLWFileBlock& operator << ( MLWFileBlock& block, const MLCamera& c );
MLRFileBlock& operator >> ( MLRFileBlock& block, MLCamera& c );

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLCamera_h

