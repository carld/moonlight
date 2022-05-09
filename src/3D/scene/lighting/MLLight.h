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
  MLLight.h

  Stephane Rehel

  January 12 1997
*/

#ifndef __MLLight_h
#define __MLLight_h

#ifndef __BoundingCone_h
#include "tools/BoundingCone.h"
#endif

#ifndef __Color_h
#include "tools/Color.h"
#endif

#ifndef __Spectrum_h
#include "tools/Spectrum.h"
#endif

#ifndef __MLObject_h
#include "scene/MLObject.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLLight: public MLObject
{
  friend class MLLightTable; // sets light_index when indexing lights
  friend class MLLightPool; // set pool_index
  friend class MLObjectIO;
  friend class MLPoolTable; // update pool_index when removing a pool

private:
  int light_index;
  int pool_index;

public:
  enum LightType
    {
    FIRST_TYPE= 0,

    POINT,
    DIRECTIONAL,
    SPOT,

    LAST_TYPE
    };

public:
  static const double DEFAULT_MAX_EXTENT;

  boolean globalLight;
  boolean directLight;

  LightType lightType;

  enum COLOR_MODEL { RGB= 1, HSV= 2 };
  COLOR_MODEL color_model;

  Color color;
  double direct_intensity;
  double global_intensity;

  boolean has_extent;
  double extent;

  boolean generate_shadows;

  double spot_cutoff; // radians, >0
  double spot_exponent;

  boolean radiosity_attenuation;
  struct Attenuation
    {
    boolean yes;
    double value;
    };
  Attenuation att_constant;
  Attenuation att_linear;
  Attenuation att_quadratic;

  Spectrum unshotEnergy; // energy to shoot
  boolean energy_has_been_shot;

public:
  BoundingCone cone; // bounding cone angle is 'cutoff'
                     // -> in world space

  Point worldPosition; // updated by updateTransformations()
  Vector worldDirection; // updated by updateTransformations()
                         // normalized
  // Compiled:
  struct Compiled
    {
    double cos_cutoff;
    Color direct_color;
    Color global_color;
    boolean has_direct_power;
    boolean has_global_power;
    double att_constant,
           att_linear,
           att_quadratic;
    double extent2;
    } compiled;

public:
  MLLight( MLObject* _father,
           MLLightPool* pool, // pool may be NULL
           boolean _directLight = true,
           boolean _globalLight = true );

  virtual ~MLLight();

  LightType getLightType() const
    {
    return lightType;
    }

  virtual ObjectType getObjectType() const
    {
    return MLObject::LIGHT;
    }

  void setLightType( LightType _lightType );

protected:
  void copy( MLLight* light ) const;
public:
  virtual MLObject* duplicate() const;

protected:
  virtual boolean getLocalBBox( Box& box ) const;

public:
  void resetUnshotEnergy();

  void clearUnshotEnergy();

  int getLightIndex() const
    {
    return light_index;
    }
  int getPoolIndex() const
    {
    return pool_index;
    }

  MLLightPool* getPool() const;

  Spectrum getEnergy() const
    {
    return Spectrum(color,global_intensity);
    }

  virtual void updateTransformations();

  // This object *and* its children
  // (using global bounding sphere)
  boolean objectIsUnreachable( MLObject* o ) const;

  // Only this local object (mesh)
  // (using bounding box)
  boolean meshIsUnreachable( MLObject* o ) const;

  virtual void compile();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLLight_h
