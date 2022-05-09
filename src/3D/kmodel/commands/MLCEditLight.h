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
  MLCEditLight.h

  Stephane Rehel

  March 19 1997
*/

#ifndef __MLCEditLight_h
#define __MLCEditLight_h

#ifndef __Color_h
#include "tools/Color.h"
#endif

#ifndef __ModelCommand_h
#include "kmodel/ModelCommand.h"
#endif

class MLLight;
class MLLightPool;
class ModelModule;

/////////////////////////////////////////////////////////////////////////////

class MLCEditLight: public ModelCommand
{
public:
  int lightIndex;
  MLLight* light;

  int poolIndex;
  MLLightPool* pool;

  int lightType;

  boolean directLight;
  boolean globalLight;

  int color_model;
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

public:
  MLCEditLight();

  virtual ~MLCEditLight();

  virtual boolean doit( ModelModule* model );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLCEditLight_h

