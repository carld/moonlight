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
  MLLightTable.C

  Stephane Rehel
  March 7 1997
*/

#include "MLLightTable.h"

#include "MLLight.h"

#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

MLLightTable::MLLightTable( MLScene* _scene )
{
  scene= _scene;
  max_lights= 8;
  lights= new MLLight* [ max_lights + 1 ];
  nLights= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLLightTable::~MLLightTable()
{
  delete lights;
  lights= 0;
  max_lights= 0;
  nLights= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLLightTable::addLight( MLLight* light )
{
  if( light == 0 )
    return;

  // First, check out if this light isn't already in the table
  int i;
  for( i= 1; i <= nLights; ++i )
    if( lights[i] == light )
      return;

  if( nLights == max_lights )
    {
    int new_max_lights= max_lights + max_lights / 2;
    MLLight** new_lights= new MLLight* [ new_max_lights + 1 ];
    for( i= 1; i <= max_lights; ++i )
      new_lights[i]= lights[i];

    for( i= max_lights+1; i <= new_max_lights; ++i )
      new_lights[i]= 0;

    delete lights;
    lights= new_lights;
    max_lights= new_max_lights;
    }

  ++nLights;
  lights[nLights]= light;
  light->light_index= nLights;
}

/////////////////////////////////////////////////////////////////////////////

void MLLightTable::removeLight( MLLight* light )
{
  if( light == 0 )
    return;

  int index= light->light_index;
  if( index < 1 || index > nLights )
    return; // ???

  int i= index+1;
  while( i <= nLights )
    {
    lights[i-1]= lights[i];
    lights[i-1]->light_index= i-1;
    ++i;
    }

  lights[nLights]= 0;
  light->light_index= 0;
  --nLights;
}

/////////////////////////////////////////////////////////////////////////////

void MLLightTable::clear()
{
  for( int i= 1; i <= max_lights; ++i )
    lights[i]= 0;

  nLights= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLLightTable::compileLights()
{
  for( int i= 1; i <= getNLights(); ++i )
    {
    MLLight* light= getLight(i);
    if( light == 0 )
      continue;
    light->compile();
    }
}

/////////////////////////////////////////////////////////////////////////////
