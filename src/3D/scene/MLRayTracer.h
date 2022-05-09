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
  MLRayTracer.h

  Stephane Rehel

  January 21 1997
*/

#ifndef __MLRayTracer_h
#define __MLRayTracer_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

class MLScene;
class MLRay;
class MLObject;
class MLMesh;
class MeshRep;

/////////////////////////////////////////////////////////////////////////////

class MLRayTracer
{
  friend class MLRayRenderer; // calls sendInHier()

protected:
  MLScene* scene;

public:
  MLRayTracer( MLScene* _scene );

  virtual ~MLRayTracer();

  void sendEyeRay( MLRay& ray );

protected:

  boolean sendInHier( MLRay& ray, MLObject* object );

  // return false if not intersecting hash box
  boolean sendInHashBox( MLRay& ray, MLMesh* mlmesh, boolean& got_one );

public:
  // return true if blocked
  // MLRay::radiosityLightRay must be set/unset before!
  boolean findBlocking( MLRay& ray );

};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLRayTracer_h
