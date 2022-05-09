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
  MLMRay.h

  Stephane Rehel

  September 21 1997
*/

#ifndef __MLMRay_h
#define __MLMRay_h

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

#ifndef __MLMode_h
#include "interface/MLMode.h"
#endif

class ModelModule;
class MLPolygon;
class MLMesh;
class MLCamera;

/////////////////////////////////////////////////////////////////////////////

class MLMRay: public MLMode
{
public:
  int button;

  Point hit_point;
  double distance;
  MLPolygon* hit_polygon;
  MLMesh* hit_mesh;

  boolean only_selected;

public:
  MLMRay( ModelModule* _model );

  virtual ~MLMRay();

  virtual void enter();

  virtual boolean select( int screen_x, int screen_y, MLCamera& camera );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLMRay_h
