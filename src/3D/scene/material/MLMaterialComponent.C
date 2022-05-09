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
  MLMaterialComponent.C

  Stephane Rehel

  August 30 1997
*/

#include "MLMaterialComponent.h"
#include "MLMaterialImage.h"

#include "MLMaterialImages.h"

#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

MLMaterialComponent::MLMaterialComponent( MLScene* _scene )
{
  scene= _scene;

  color= Color(1,1,1) * 0.5;
  color_model= RGB;
  factor= 1.;

  use_texture= false;
  image_name= "";
  image= 0;
  x_offset= y_offset= 0;
  x_scale= y_scale= 1.;
  use_alpha= false;

  compile();
}

/////////////////////////////////////////////////////////////////////////////

MLMaterialComponent::~MLMaterialComponent()
{
  if( scene != 0 )
    {
    MLMaterialImages* images= scene->getMaterialImages();
    if( images != 0 )
      images->unref(image);
    }

  image= 0;

  scene= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLMaterialComponent::compile()
{
  compiled_color= color * factor;
}

/////////////////////////////////////////////////////////////////////////////

void MLMaterialComponent::loadImages()
{
  MLMaterialImages* images= scene->getMaterialImages();

  if( ! use_texture )
    {
    images->unref(image);
    image= 0;
    }
   else
    {
    boolean name_changed= false;

    if( image != 0 )
      {
      if( image->getName() != image_name )
        name_changed= true;
      }
     else
      name_changed= true;

    if( name_changed )
      {
      MLMaterialImage* old_image= image;
      images->ref(old_image);
      image= images->ref(image_name);
      images->unref(old_image);
      images->unref(old_image); // yes, two times
      }

    image->load();
    }
}

/////////////////////////////////////////////////////////////////////////////

MLMaterialComponent& MLMaterialComponent::operator =
                                           ( const MLMaterialComponent& mc )
{
  color= mc.color;
  color_model= mc.color_model;
  factor= mc.factor;

  use_texture= mc.use_texture;

  image_name= mc.image_name;
  image= mc.image;
  scene->getMaterialImages()->ref(image);

  x_offset= mc.x_offset;
  y_offset= mc.y_offset;

  x_scale= mc.x_scale;
  y_scale= mc.y_scale;

  use_alpha= mc.use_alpha;

  return *this;
}

/////////////////////////////////////////////////////////////////////////////
