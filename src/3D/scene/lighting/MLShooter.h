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
  MLShooter.h

  Stephane Rehel
  April 5 1997
*/

#ifndef __MLShooter_h
#define __MLShooter_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

class MLScene;
class MLLightingControl;
class MLLight;
class MLObject;

/////////////////////////////////////////////////////////////////////////////

// used by the lighting process
class MLShooter
{
private:
  MLScene* scene;
  MLLightingControl* control;

public:
  MLShooter( MLScene* _scene, MLLightingControl* _control );

  virtual ~MLShooter();

//// LIGHT SHOOTING

  // return true if completed
  boolean shoot( MLLight* light );
protected:
  // return true if completed
  boolean shootHier( MLLight* light, MLObject* o );

public:
  // Called by MLMeshPoolLighting::shoot()
  // not hierarchical
  // return true if completed
  boolean shoot( MLLight* light, MLMesh* mlmesh );

//// POLYGON INDIRECT SHOOTING

public:
  // return true if completed
  boolean shootPES();

public:
  // return true if completed
  boolean shootPES( MLMesh* mlmesh );

  // return true if completed & a polygon is available
  boolean selectShootingPolygon();

  // return true if completed & a polygon has been shot
  boolean indirectStep();
};

/////////////////////////////////////////////////////////////////////////////

#endif // __MLShooter_h
