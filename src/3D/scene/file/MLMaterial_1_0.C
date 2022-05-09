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
  MLMaterial_1_0.C

  Stephane Rehel

  September 14 1997

  MLMaterial* MLMaterialIO::read_1_0( MLRFileBlock* block );
*/

#include "MLMaterialIO.h"
#include "MLMaterialIDs.h"

#include "tools/file/MLRFile.h"
#include "tools/file/MLRFileBlock.h"

#include "scene/material/MLMaterial.h"

#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

MLMaterial* MLMaterialIO::read_1_0( MLRFileBlock& block )
{
  if( ! block.find(MLMaterialID::MATERIAL_DEF) )
    return 0;

  MLName sceneName,
         name;

  block.go(MLMaterialID::MATERIAL_NAMES);
    {
    block >> sceneName;
    block >> name;
    }

  MLMaterial* m= new MLMaterial(scene);

//  MLMaterial* m= 0;
//  MLMaterial* find_it= scene->getMaterial(name.name);
//  if( find_it != 0 )
//    m= find_it; // overwrite the existing material
//   else
//    m= new MLMaterial(scene);

  block.go(MLMaterialID::MATERIAL_DEF);
    {
    m->direct_shaded= (block.getInteger() != 0);
    m->rad_contributor= (block.getInteger() != 0);

    m->generate_shadows= (block.getInteger() != 0);
    m->get_shadows= (block.getInteger() != 0);
    }

  block.go(MLMaterialID::MATERIAL_EMITTANCE);
    {
    MLMaterialEmittance& c= m->emittance;
    c.color_model= MLMaterialComponent::COLOR_MODEL(block.getInteger());
    block >> c.color;
    c.factor= block.getDouble();
    c.use_texture= (block.getInteger() != 0);
    block >> c.image_name;
    c.x_offset= block.getInteger();
    c.y_offset= block.getInteger();
    c.x_scale= block.getDouble();
    c.y_scale= block.getDouble();
    c.use_alpha= (block.getInteger() != 0);
    }

  block.go(MLMaterialID::MATERIAL_DIFFUSE);
    {
    MLMaterialDiffuse& c= m->diffuse;
    c.color_model= MLMaterialComponent::COLOR_MODEL(block.getInteger());
    block >> c.color;
    c.factor= block.getDouble();
    c.use_texture= (block.getInteger() != 0);
    block >> c.image_name;
    c.x_offset= block.getInteger();
    c.y_offset= block.getInteger();
    c.x_scale= block.getDouble();
    c.y_scale= block.getDouble();
    c.use_alpha= (block.getInteger() != 0);
    }

  block.go(MLMaterialID::MATERIAL_SPECULAR);
    {
    MLMaterialSpecular& c= m->specular;
    c.shininess= block.getDouble();
    c.color_model= MLMaterialComponent::COLOR_MODEL(block.getInteger());
    block >> c.color;
    c.factor= block.getDouble();
    c.use_texture= (block.getInteger() != 0);
    block >> c.image_name;
    c.x_offset= block.getInteger();
    c.y_offset= block.getInteger();
    c.x_scale= block.getDouble();
    c.y_scale= block.getDouble();
    c.use_alpha= (block.getInteger() != 0);
    }

  block.go(MLMaterialID::MATERIAL_REFLECTION);
    {
    MLMaterialReflection& c= m->reflection;
    c.color_model= MLMaterialComponent::COLOR_MODEL(block.getInteger());
    block >> c.color;
    c.factor= block.getDouble();
    c.use_texture= (block.getInteger() != 0);
    block >> c.image_name;
    c.x_offset= block.getInteger();
    c.y_offset= block.getInteger();
    c.x_scale= block.getDouble();
    c.y_scale= block.getDouble();
    c.use_alpha= (block.getInteger() != 0);
    }

  block.go(MLMaterialID::MATERIAL_TRANSMISSION);
    {
    MLMaterialTransmission& c= m->transmission;
    c.trans_n= block.getDouble();
    c.color_model= MLMaterialComponent::COLOR_MODEL(block.getInteger());
    block >> c.color;
    c.factor= block.getDouble();
    c.use_texture= (block.getInteger() != 0);
    block >> c.image_name;
    c.x_offset= block.getInteger();
    c.y_offset= block.getInteger();
    c.x_scale= block.getDouble();
    c.y_scale= block.getDouble();
    c.use_alpha= (block.getInteger() != 0);
    }

  m->name= name;

  // it's the correct filename ... normally
  m->name.filename= getMaterialFilename( database, sceneName, m->name );

  m->compile();

  return m;
}

/////////////////////////////////////////////////////////////////////////////
