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
  MLMaterialAllocator.C

  Stephane Rehel

  March 7 1997
*/

#include "MLMaterialAllocator.h"

#include "MLMaterial.h"

/////////////////////////////////////////////////////////////////////////////

MLMaterialAllocator::MLMaterialAllocator( MLScene* _scene )
{
  scene= _scene;

  max_materials= 16;
  materials= new MLMaterial* [ max_materials + 1 ];

  for( int i= 0; i <= max_materials; ++i )
    materials[i]= 0;

  nMaterials= 0;

  // default material has no material id, and it's not editable
  defaultMaterial= 0;
  defaultMaterial= new MLMaterial(scene);

  clear();
}

/////////////////////////////////////////////////////////////////////////////

MLMaterialAllocator::~MLMaterialAllocator()
{
  if( materials != 0 )
    {
    for( int i= 0; i <= max_materials; ++i )
      {
      MLMaterial* m= materials[i];
      materials[i]= 0;
      delete m;
      }

    delete materials;
    materials= 0;
    }

  max_materials= 0;
  nMaterials= 0;
}

/////////////////////////////////////////////////////////////////////////////

int MLMaterialAllocator::allocID( MLMaterial* material )
{
  if( material == 0 )
    return 0;

  int hole= 0;

  for( int i= 1; i <= max_materials; ++i )
    {
    register MLMaterial* m= materials[i];
    if( m == material )
      return i; // already allocated
    if( m == 0 && hole == 0 )
      {
      hole= i;
      break;
      }
    }

  if( hole == 0 )
    {
    hole= max_materials + 1;

    int new_max_materials= max_materials + max_materials / 2;
    MLMaterial** new_materials= new MLMaterial* [ new_max_materials + 1 ];

    int j;
    for( j= 0; j <= max_materials; ++j )
      new_materials[j]= materials[j];
    for( j= max_materials+1; j <= new_max_materials; ++j )
      new_materials[j]= 0;

    delete materials;
    materials= new_materials;
    max_materials= new_max_materials;
    }

  ++nMaterials;
  materials[ hole ]= material;

  return hole;
}

/////////////////////////////////////////////////////////////////////////////

void MLMaterialAllocator::freeID( int id )
{
  if( id < 1 || id > max_materials )
    return;
  if( materials[id] == 0 )
    return;

  materials[id]= 0;

  --nMaterials;
}

/////////////////////////////////////////////////////////////////////////////

void MLMaterialAllocator::freeID( MLMaterial* material )
{
  if( material == 0 )
    return;

  int id= material->material_id;
  if( id < 1 || id > max_materials )
    return;

  for( int i= 1; i <= max_materials; ++i )
    {
    register MLMaterial*& m= materials[i];
    if( m == material )
      {
      --nMaterials;
      m= 0;
      return;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLMaterialAllocator::clear()
{
  for( int i= 1; i <= max_materials; ++i )
    {
    MLMaterial* m= materials[i];
    if( m == 0 )
      continue;
    materials[i]= 0;
    delete m;
    }

  nMaterials= 0;

  defaultMaterial->material_id= 0;
  materials[0]= defaultMaterial;

  MLMaterial dm(scene);
  *defaultMaterial= dm;
  defaultMaterial->name.name= MLMaterial::defaultMaterialName;
}

/////////////////////////////////////////////////////////////////////////////
