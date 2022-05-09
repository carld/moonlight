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
  MLCEditLight.C

  Stephane Rehel

  March 19 1997
*/

#include <math.h>

#include "scene/MLScene.h"

#include "scene/lighting/MLLight.h"
#include "scene/lighting/MLLightPool.h"
#include "scene/lighting/MLGlobalLighting.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelCommands.h"

#include "MLCEditLight.h"

/////////////////////////////////////////////////////////////////////////////

MLCEditLight::MLCEditLight()
{
  type= ModelCommands::EDIT_LIGHT;

  lightIndex= 0; light= 0;
  poolIndex= 0; pool= 0;

  lightType= MLLight::POINT;
  directLight= true;
  globalLight= false;
  color_model= MLLight::RGB;
  color= Color(0,0,0);
  direct_intensity= 1.;
  global_intensity= 1.;
  has_extent= false;
  extent= 100.;
  generate_shadows= true;

  spot_cutoff= 0.;
  spot_exponent= 0.;

  radiosity_attenuation= true;
  att_constant.yes  = true;
  att_constant.value= 1.;
  att_linear.yes  = false;
  att_linear.value= 0.;
  att_quadratic.yes  = false;
  att_quadratic.value= 0.;
}

/////////////////////////////////////////////////////////////////////////////

MLCEditLight::~MLCEditLight()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLCEditLight::doit( ModelModule* model )
{
  MLScene* scene= model->getScene();

  if( pool == 0 ||
      light == 0 ||
      scene->getPool(poolIndex) != pool ||
      scene->getLight(lightIndex) != light )
    return true; // mmhhh?

  model->stopLightingProcess();

  if( lightType == MLLight::DIRECTIONAL )
    has_extent= false;

  int type= (lightType != int(light->getLightType()));
  boolean global= (globalLight?1:0) != (light->globalLight?1:0) ;
  boolean direct= (directLight?1:0) != (light->directLight?1:0) ;

  boolean parameters= fabs(spot_cutoff - light->spot_cutoff) > DEGtoRAD(0.01) ||
                    fabs(spot_exponent-light->spot_exponent) > 1e-4 ||
                    ((has_extent?1:0) != (light->has_extent?1:0)) ||
                    fabs(extent - light->extent) > 0.001 ||
                    ((generate_shadows?1:0) != (light->generate_shadows?1:0));

  boolean att= ((radiosity_attenuation?1:0)!=(light->radiosity_attenuation?1:0)) ||
             ((att_constant.yes?1:0)  != (light->att_constant.yes?1:0))  ||
             ( att_constant.value     !=  light->att_constant.value)     ||
             ((att_linear.yes?1:0)    != (light->att_linear.yes?1:0))    ||
             ( att_linear.value       !=  light->att_linear.value)       ||
             ((att_quadratic.yes?1:0) != (light->att_quadratic.yes?1:0)) ||
             ( att_quadratic.value    !=  light->att_quadratic.value) ;

  boolean lcolor=  fabs((color - light->color).getGray()) > 0.005;
  boolean denergy= fabs(direct_intensity - light->direct_intensity) > 1e-5;
  boolean genergy= fabs(global_intensity - light->global_intensity) > 1e-6;

  boolean lightpool= poolIndex != light->getPoolIndex();

  light->setLightType( MLLight::LightType(lightType) );

  light->directLight= directLight;
  light->globalLight= globalLight;

  light->color_model= MLLight::COLOR_MODEL(color_model);
  light->color= color;
  light->direct_intensity= direct_intensity;
  light->global_intensity= global_intensity;
  light->has_extent= has_extent;
  light->extent= extent;
  light->generate_shadows= generate_shadows;

  light->spot_cutoff= spot_cutoff;
  light->spot_exponent= spot_exponent;

  light->radiosity_attenuation= radiosity_attenuation;
  light->att_constant.yes  = att_constant.yes;
  light->att_constant.value= att_constant.value;
  light->att_linear.yes  = att_linear.yes;
  light->att_linear.value= att_linear.value;
  light->att_quadratic.yes  = att_quadratic.yes;
  light->att_quadratic.value= att_quadratic.value;

  MLGlobalLighting* glighting= scene->getGlobalLighting();

  if( lightpool )
    {
    glighting->change_light_pool( light, pool );
    goto ok;
    }

  if( global )
    {
    // ok, parameters changed
    glighting->light_params_are_changed(light);
    pool->setDirty();
    }

  if( globalLight )
    {
    if( parameters || att || type )
      {
      // ok, parameters changed
      glighting->light_params_are_changed(light);
      goto ok;
      }

    if( genergy || lcolor )
      {
      glighting->light_energy_is_changed(light);
      goto ok;
      }
    }

  if( directLight )
    {
    if( parameters || att || type || lcolor || denergy )
      goto ok;
    }

  if( global || direct )
    goto ok;

  return true; // no change??!!

ok:
  MLLightPool* pool= light->getPool();
  pool->updatePoolType();

  light->compile();

  unsigned int flags= ModelModule::REFRESH_ILLUM |
                      ModelModule::REFRESH_DIALOGS ;
  if( type || global || direct || parameters )
    flags |= ModelModule::REFRESH_GEOMETRY;

  model->refreshScreen(flags);

  return true;
}

/////////////////////////////////////////////////////////////////////////////
