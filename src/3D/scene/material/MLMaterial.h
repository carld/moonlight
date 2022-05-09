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
  MLMaterial.h

  Stephane Rehel

  March 7 1997
*/

#ifndef __MLMaterial_h
#define __MLMaterial_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __Color_h
#include "tools/Color.h"
#endif

#ifndef __Spectrum_h
#include "tools/Spectrum.h"
#endif

#ifndef __MLName_h
#include "scene/MLName.h"
#endif

#ifndef __MLMaterialEmittance_h
#include "MLMaterialEmittance.h"
#endif

#ifndef __MLMaterialDiffuse_h
#include "MLMaterialDiffuse.h"
#endif

#ifndef __MLMaterialSpecular_h
#include "MLMaterialSpecular.h"
#endif

#ifndef __MLMaterialReflection_h
#include "MLMaterialReflection.h"
#endif

#ifndef __MLMaterialTransmission_h
#include "MLMaterialTransmission.h"
#endif

class MLScene;
class MLImage;
class Texture;

/////////////////////////////////////////////////////////////////////////////

class MLMaterial
{
  friend class MLMaterialAllocator;

private:
  int material_id;
  static const char* defaultMaterialName;

protected:
  MLScene* scene;

public:
  MLName name;

  boolean direct_shaded;
  boolean rad_contributor;

  MLMaterialEmittance    emittance;
  MLMaterialDiffuse      diffuse;
  MLMaterialSpecular     specular;
  MLMaterialReflection   reflection;
  MLMaterialTransmission transmission;

  boolean generate_shadows;
  boolean get_shadows;

  boolean has_reflect_or_trans;

public:
  MLMaterial( MLScene* _scene );

  MLMaterial( const MLMaterial& m );

private:
  void init_material();

public:
  virtual ~MLMaterial();

  void registerID();

  int getID() const
    {
    return material_id;
    }

  MLScene* getScene() const
    {
    return scene;
    }

  const MLName& getName() const
    {
    return name;
    }

  static const char* getDefaultMaterialName()
    {
    return defaultMaterialName;
    }

  boolean isDirectShaded() const
    {
    return direct_shaded;
    }
  boolean isRadiosityContributor() const
    {
    return rad_contributor;
    }

  virtual void compile();
  virtual void loadImages();

  MLMaterial& operator = ( const MLMaterial& m );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLMaterial_h
