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
  MLCSetMaterial.C

  Stephane Rehel

  September 2 1997
*/

#include "scene/MLMesh.h"
#include "scene/MLScene.h"
#include "scene/lighting/MLGlobalLighting.h"
#include "scene/material/MLMaterial.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelCommands.h"

#include "MLCSetMaterial.h"

/////////////////////////////////////////////////////////////////////////////

MLCSetMaterial::MLCSetMaterial( MLScene* scene )
{
  type= ModelCommands::SET_MATERIAL;

  material= new MLMaterial(scene);

  object_id= 0;
  inherit_material= true;
}

/////////////////////////////////////////////////////////////////////////////

MLCSetMaterial::~MLCSetMaterial()
{
  delete material;
  material= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLCSetMaterial::doit( ModelModule* model )
{
  MLScene* scene= model->getScene();

  MLMesh* mesh= (MLMesh*) scene->getObject(object_id,MLObject::MESH);
  if( mesh == 0 )
    return true;

  model->stopLightingProcess();

  // let's keep a copy of it
  MLMaterial old_m( *(mesh->getMaterial()) );

  mesh->setInheritMaterial(inherit_material);
//  if( ! inherit_material )
    {
    MLMaterial* m= scene->getMaterial(material->getName().name);
    if( m == 0 )
      {
      // This is a new material
      m= new MLMaterial(scene);
      m->registerID();
      }

    // set new parameters
    *m= *material;

    m->loadImages();
    m->compile();

    mesh->setMaterial( m->getID() );
    }

  mesh->updateMaterials();

  MLMaterial* m= mesh->getMaterial();

  boolean rad_contrib_changed= ((old_m.isRadiosityContributor()?1:0) !=
                                 (m->isRadiosityContributor()?1:0) );
  boolean gen_shad_changed= ( (old_m.generate_shadows?1:0) !=
                               (m->generate_shadows?1:0) );

  boolean global_attr= (old_m.emittance.compiled_color -
                         m->emittance.compiled_color).cabs() > 1e-5 ||
                     (old_m.diffuse.compiled_color -
                         m->diffuse.compiled_color).cabs() > 1e-5 ||
                     (old_m.transmission.compiled_color -
                         m->transmission.compiled_color).cabs() > 1e-5;

  boolean global_changed= false;

  if( rad_contrib_changed )
    global_changed= true;
   else
    {
    if( m->isRadiosityContributor() )
      global_changed= global_attr;
    }

  if( gen_shad_changed || rad_contrib_changed )
    {
    MLGlobalLighting* gl= scene->getGlobalLighting();
    boolean save_rad_contributor= m->rad_contributor;
    boolean save_generate_shadows= m->generate_shadows;
    m->rad_contributor= true;
    m->generate_shadows= true;

    gl->material_shadow_has_changed(m);

    m->rad_contributor= save_rad_contributor;
    m->generate_shadows= save_generate_shadows;
    }

  if( global_changed )
    {
    MLGlobalLighting* gl= scene->getGlobalLighting();
    gl->material_has_changed(m);
    }

//  scene->getGlobalLighting()->object_is_transformed(o);
//  scene->updateTransformations();

  model->refreshScreen( ModelModule::REFRESH_MATERIALS );

  return true;
}

/////////////////////////////////////////////////////////////////////////////

