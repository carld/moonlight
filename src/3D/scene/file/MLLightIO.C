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
   MLLightIO.C

   Stephane Rehel

   September 21 1997
*/

#include "MLObjectIO.h"
#include "MLObjectIDs.h"

#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"

#include "scene/lighting/MLLight.h"

/////////////////////////////////////////////////////////////////////////////

static const int light_version= 2;
static const int light_version_magic= 15671;

/////////////////////////////////////////////////////////////////////////////

void MLObjectIO::writeLight( MLLight* light, MLWFileBlock& block )
{
  block.begin(MLObjectID::OBJECT_LIGHT);
    {
    int version= ::light_version_magic + ::light_version;

    block << int(version);

    block << int(light->lightType);

    block << int(light->directLight);
    block << int(light->globalLight);

    block << int(light->color_model);
    block << light->color;
    block << double(light->direct_intensity);
    block << double(light->global_intensity);

    block << int(light->has_extent);
    block << double(light->extent);

    block << int(light->generate_shadows);

    block << double(light->spot_cutoff);
    block << double(light->spot_exponent);

    block << int(light->radiosity_attenuation);

    block << int(light->att_constant.yes);
    block << double(light->att_constant.value);
    block << int(light->att_linear.yes);
    block << double(light->att_linear.value);
    block << int(light->att_quadratic.yes);
    block << double(light->att_quadratic.value);

    block << light->unshotEnergy;
    block << int(light->energy_has_been_shot);

    block << light->cone.getOrigin();
    block << light->cone.getAxis();
    block << double(light->cone.getCosAlpha());
    }
  block.end();
}

/////////////////////////////////////////////////////////////////////////////

void MLObjectIO::readLight_1( MLLight* light, MLRFileBlock& block )
{
  int lightType;
  block >> lightType;
  light->setLightType(MLLight::LightType(lightType));

  light->directLight= block.getInteger() != 0;
  light->globalLight= block.getInteger() != 0;

  light->color_model= MLLight::COLOR_MODEL(block.getInteger());
  block >> light->color;
  light->direct_intensity= block.getDouble();
  light->global_intensity= block.getDouble();

  light->has_extent= block.getInteger() != 0;
  light->extent= block.getDouble();

  light->generate_shadows= block.getInteger() != 0;

  light->spot_cutoff= block.getDouble();
  light->spot_exponent= block.getDouble();

  light->att_constant.yes  = block.getInteger() != 0;
  light->att_constant.value= block.getDouble();
  light->att_linear.yes  = block.getInteger() != 0;
  light->att_linear.value= block.getDouble();
  light->att_quadratic.yes  = block.getInteger() != 0;
  light->att_quadratic.value= block.getDouble();

  block >> light->unshotEnergy;
  light->energy_has_been_shot= block.getInteger() != 0;
  Point coneOrigin;
  Vector coneAxis;
  double coneCosAlpha;
  block >> coneOrigin;
  block >> coneAxis;
  block >> coneCosAlpha;
  light->cone.set(coneOrigin,coneAxis,coneCosAlpha);
}

/////////////////////////////////////////////////////////////////////////////

void MLObjectIO::readLight_2( MLLight* light, MLRFileBlock& block )
{
  int lightType;
  block >> lightType;
  light->setLightType(MLLight::LightType(lightType));

  light->directLight= block.getInteger() != 0;
  light->globalLight= block.getInteger() != 0;

  light->color_model= MLLight::COLOR_MODEL(block.getInteger());
  block >> light->color;
  light->direct_intensity= block.getDouble();
  light->global_intensity= block.getDouble();

  light->has_extent= block.getInteger() != 0;
  light->extent= block.getDouble();

  light->generate_shadows= block.getInteger() != 0;

  light->spot_cutoff= block.getDouble();
  light->spot_exponent= block.getDouble();

  light->radiosity_attenuation= block.getInteger() != 0;

  light->att_constant.yes  = block.getInteger() != 0;
  light->att_constant.value= block.getDouble();
  light->att_linear.yes  = block.getInteger() != 0;
  light->att_linear.value= block.getDouble();
  light->att_quadratic.yes  = block.getInteger() != 0;
  light->att_quadratic.value= block.getDouble();

  block >> light->unshotEnergy;
  light->energy_has_been_shot= block.getInteger() != 0;
  Point coneOrigin;
  Vector coneAxis;
  double coneCosAlpha;
  block >> coneOrigin;
  block >> coneAxis;
  block >> coneCosAlpha;
  light->cone.set(coneOrigin,coneAxis,coneCosAlpha);
}

/////////////////////////////////////////////////////////////////////////////

void MLObjectIO::readLight( MLLight* light, MLRFileBlock& block )
{
  if( ! block.go(MLObjectID::OBJECT_LIGHT) )
    return;

  int version= block.getInteger() - light_version_magic;
  if( version < 0 )
    {
    // this is version 1
    // this version didn't record the light version: we've just read
    // the light type!
    version= 1;

    // rewind...
    // Sorry, it's ugly!
    block.go(MLObjectID::OBJECT_LIGHT);
    }

  switch( version )
    {
    case 1: readLight_1(light,block); break;
    case 2: readLight_2(light,block); break;
    default: break;
    }
}

/////////////////////////////////////////////////////////////////////////////
