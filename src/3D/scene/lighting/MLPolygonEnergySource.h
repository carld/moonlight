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
  MLPolygonEnergySource.h

  Stephane Rehel
  April 4 1997
*/

#ifndef __MLPolygonEnergySource_h
#define __MLPolygonEnergySource_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

#ifndef __MLReference_h
#include "tools/MLReference.h"
#endif

class Spectrum;
class MLMesh;
class MLScene;
class MLPolygon;

/////////////////////////////////////////////////////////////////////////////

class MLPolygonEnergySource: public MLReference
{
protected:
  MLScene* scene;

public:
  int nPools;
  Spectrum* energy; // nPools long
  double max_abs_power;

  MLMesh* mesh;
  int object_index;

  int polygon;
  double polygonArea;

  // speed-ups
  Point world_location;
  Vector world_normal; // normalized

  MLPolygonEnergySource( MLScene* _scene );
  ~MLPolygonEnergySource();

  boolean isValid();

  MLPolygon* getPolygon();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLPolygonEnergySource_h
