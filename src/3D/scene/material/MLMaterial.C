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
  MLMaterial.C

  Stephane Rehel

  March 7 1997
*/

#include "MLMaterial.h"
#include "MLMaterialAllocator.h"

#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

// static
const char* MLMaterial::defaultMaterialName= "Default";

/////////////////////////////////////////////////////////////////////////////

MLMaterial::MLMaterial( MLScene* _scene ):
  emittance(_scene),
  diffuse(_scene),
  specular(_scene),
  reflection(_scene),
  transmission(_scene)
{
  scene= _scene;
  init_material();
}

/////////////////////////////////////////////////////////////////////////////

MLMaterial::MLMaterial( const MLMaterial& m ):
  emittance(m.scene),
  diffuse(m.scene),
  specular(m.scene),
  reflection(m.scene),
  transmission(m.scene)
{
  scene= m.scene;
  init_material();
}

/////////////////////////////////////////////////////////////////////////////

void MLMaterial::init_material()
{
  material_id= -1;

  direct_shaded= true;
  rad_contributor= true;

  generate_shadows= true;
  get_shadows= true;

  compile();
}

/////////////////////////////////////////////////////////////////////////////

MLMaterial::~MLMaterial()
{
  if( material_id != -1 && scene != 0 )
    {
    MLMaterialAllocator* allocator= scene->getMaterialAllocator();
    if( allocator != 0 )
      allocator->freeID(material_id);
    }

  material_id= -1;

  scene= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLMaterial::registerID()
{
  if( material_id != -1 )
    return; // already registered

  if( scene->materialAllocator != 0 )
    material_id= scene->getMaterialAllocator()->allocID(this);
   else
    {
    material_id= 0; // this is the case in default material creation
    name.name= MLMaterial::defaultMaterialName;
    }

  if( material_id == 0 )
    {
    name.name= MLString("material-") + MLString::valueOf(material_id);

    int material_name_index= 1;
    MLString material_name= "";
    for(;;)
      {
      material_name= MLString("material-") + MLString::valueOf(material_name_index);
      boolean used= false;
      MLMaterialAllocator* allocator= scene->getMaterialAllocator();
      for( int i= 1; i <= allocator->getNMaterials(); ++i )
        {
        MLMaterial* m= allocator->getMaterial(i);
        if( m == 0 )
          continue;
        if( m->getName().name == material_name )
          {
          used= true;
          break;
          }
        }
      if( ! used )
        break;
      ++material_name_index;
      }
    name.name= material_name;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLMaterial::compile()
{
  emittance.compile();
  diffuse.compile();
  specular.compile();
  reflection.compile();
  transmission.compile();

  has_reflect_or_trans= reflection.has_reflection ||
                        transmission.has_transmission;
}

/////////////////////////////////////////////////////////////////////////////

void MLMaterial::loadImages()
{
  emittance.loadImages();
  diffuse.loadImages();
  specular.loadImages();
  reflection.loadImages();
  transmission.loadImages();
}

/////////////////////////////////////////////////////////////////////////////

MLMaterial& MLMaterial::operator = ( const MLMaterial& m )
{
  name= m.name;

  direct_shaded= m.direct_shaded;
  rad_contributor= m.rad_contributor;

  emittance= m.emittance;
  diffuse= m.diffuse;
  specular= m.specular;
  reflection= m.reflection;
  transmission= m.transmission;

  generate_shadows= m.generate_shadows;
  get_shadows= m.get_shadows;

  has_reflect_or_trans= m.has_reflect_or_trans;

  return *this;
}

/////////////////////////////////////////////////////////////////////////////
