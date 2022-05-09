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
  MLGlobalLighting.h

  Stephane Rehel

  March 11 1997
*/

#ifndef __MLGlobalLighting_h
#define __MLGlobalLighting_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __Spectrum_h
#include "tools/Spectrum.h"
#endif

class BoundingSphere;
class BoundingCone;
class BoundingCylinder;
class BoundingHalfPlane;
class MLScene;
class MLLightPool;
class MLLight;
class MLObject;
class MLMesh;
class Spectrum;
class MLMaterial;

/////////////////////////////////////////////////////////////////////////////

class MLGlobalLighting
{
protected:
  MLScene* scene;

public:
  boolean dirty; // Is direct lighting dirty ? (for a pool or an object)
  boolean update_shadows;

  MLGlobalLighting( MLScene* _scene );

  virtual ~MLGlobalLighting();

  void object_is_transformed( MLObject* object );
  void dirty_mesh_lighting( MLMesh* mesh );
  void reset_mesh_lighting( MLMesh* mesh );
  void mesh_shadow_has_changed( MLMesh* mesh );

private:
  // set lighting of the light dirty if object is in the cone
  void update_shadowed_objects( MLObject* object,
                                MLLight* light,
                                const BoundingCone& cone,
                                const BoundingHalfPlane& bhp );

  // set lighting of the light dirty if object is in the cylinder
  void update_shadowed_objects( MLObject* object,
                                MLLight* light,
                                const BoundingCylinder& cyl,
                                const BoundingHalfPlane& bhp );

public:
  void update_light( MLLight* light );
  void light_is_transformed( MLLight* light );
  void change_light_pool( MLLight* light, MLLightPool* new_pool );
  void light_energy_is_changed( MLLight* light );
  void light_params_are_changed( MLLight* light ); // e.g. cone angle

  void pool_energy_is_changed( MLLightPool* pool );

  void material_has_changed( MLMaterial* m );
  void material_shadow_has_changed( MLMaterial* m );

  void set_dirty_pool( MLLightPool* pool );

  void clear();

  Spectrum getInitialEnergy() const;

  void getShotUnshot( Spectrum& direct_shot,
                      Spectrum& indirect_shot,
                      Spectrum& unshot );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLGlobalLighting_h
