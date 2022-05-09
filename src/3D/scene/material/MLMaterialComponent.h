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
  MLMaterialComponent.h

  Stephane Rehel

  August 30 1997
*/

#ifndef __MLMaterialComponent_h
#define __MLMaterialComponent_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __Color_h
#include "tools/Color.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

class MLMaterialImage;
class MLScene;

/////////////////////////////////////////////////////////////////////////////

class MLMaterialComponent
{
public:
  MLScene* scene;

  enum COLOR_MODEL { RGB= 1, HSV= 2 };

  Color color;
  COLOR_MODEL color_model;
  double factor;

  boolean use_texture;
  MLString image_name;
  MLMaterialImage* image;
  int x_offset, y_offset;
  double x_scale, y_scale;
  boolean use_alpha;

public:
  // compiled things:
  Color compiled_color;

public:
  MLMaterialComponent( MLScene* _scene );

  virtual ~MLMaterialComponent();

  virtual void compile();
  virtual void loadImages();

  const Color& get() const
    {
    return compiled_color;
    }

  virtual MLMaterialComponent& operator = ( const MLMaterialComponent& mc );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLMaterialComponent_h
