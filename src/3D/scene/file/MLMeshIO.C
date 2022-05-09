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
   MLMeshIO.C

   Stephane Rehel

   September 21 1997
*/

#include <stdio.h>

#include "MLObjectIO.h"
#include "MLObjectIDs.h"

#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"

#include "scene/MLMesh.h"
#include "scene/material/MLMaterial.h"
#include "scene/MLScene.h"

#include "MLAbstractMeshIO.h"
#include "MLMaterialIO.h"

/////////////////////////////////////////////////////////////////////////////

static const int mesh_version= 1;

/////////////////////////////////////////////////////////////////////////////

void MLObjectIO::writeMesh( MLMesh* mesh, MLWFileBlock& block )
{
  block.begin(MLObjectID::OBJECT_MESH);
    {
    block << int(::mesh_version);

    if( mesh->abstract == 0 )
      block << int(0);
     else
      {
      block << int(1);

      block << mesh->abstract->getName();
      }
    block << mesh->getMaterial()->getName();
    block << int(mesh->inherit_material);
    }
  block.end();
}

/////////////////////////////////////////////////////////////////////////////

void MLObjectIO::readMesh_1( MLMesh* mesh,
                             const MLName& sceneName,
                             MLRFileBlock& block )
{
  MLAbstractMesh* ao= 0;

  int hasAbstract= block.getInteger();

  if( hasAbstract )
    {
    MLName aname;
    block >> aname;
    aname.filename=
                 MLAbstractMeshIO::getAbstractObjectFilename( database,
                                                                sceneName,
                                                                aname );

    ao= scene->getAbstractByFilename(aname.filename);

    if( ao == 0 )
      {
      // not already loaded

      ao= MLAbstractMeshIO::read(database,scene,aname.filename,progress);
      if( ao == 0 )
{
fprintf( stderr, "unable to read geometry %s\n",aname.filename.get());
//      return 0; //*** give an error mesg!!
}
      }
    }

  MLName material_name;
  block >> material_name;

  int inherit_material= block.getInteger();

  // First, load default material
  MLObject* father= mesh->getFather();
  if( father->isRoot() )
    {
    MLName default_mat_name;
    default_mat_name.name= MLMaterial::getDefaultMaterialName();
    MLString filename= MLMaterialIO::getMaterialFilename( database,
                                                         sceneName,
                                                         default_mat_name );

    MLMaterial* m= MLMaterialIO::read( database,
                                       scene,
                                       filename,
                                       progress );

    if( m != 0 )
      {
      assert( scene->getDefaultMaterial() != m );

      *(scene->getDefaultMaterial()) = *m;

      scene->getDefaultMaterial()->compile();

//***      delete m;
      m= 0;
      }

    // no need to register it
    }

  // Load and set the material
  MLMaterial* find_mat= scene->getMaterial(material_name.name);

  boolean load_it= false;
  if( find_mat == 0 )
    load_it= true;
   else
    {
    // this material already exists by its name
    // Check out the scene and version of its MLName
    if( find_mat->name != material_name )
      load_it= true; // overwrite it, this is an old material

    if( material_name.name == MLString(MLMaterial::getDefaultMaterialName()) )
      load_it= false; // default material has already been loaded
    }

  if( inherit_material )
    {
    mesh->material= scene->getDefaultMaterial(); // safe
    mesh->material_id= mesh->material->getID();
    mesh->inherit_material= true;
    }
   else
    {
    MLMaterial* material= 0;
    if( ! load_it )
      material= find_mat;
     else
      {
      material= MLMaterialIO::read( database,
                                    scene,
                                    material_name,
                                    progress );

      if( material == 0 )
        {
        // unable to load this material!
        // let's get the default one
        material= scene->getDefaultMaterial();
        }
       else
        {
        // It is now loaded, but not registered
        if( find_mat == 0 )
          {
          // this a new material
          MLString save_name= material->name.name;
          material->registerID();
          material->name.name= save_name;
          }
         else
          {
          // we need to ovewrite the old material
          *find_mat= *material;
          find_mat->compile();

          MLMaterial* m= material;
          material= find_mat; // this is the good registerer one

//***          delete m;
          m= 0;
          }
        }
      }

    mesh->material= material;
    mesh->material_id= material->getID();
    mesh->inherit_material= false;
    }

  mesh->updateMaterials();

  mesh->setAbstract(ao);
}

/////////////////////////////////////////////////////////////////////////////

void MLObjectIO::readMesh( MLMesh* mesh,
                           const MLName& sceneName,
                           MLRFileBlock& block )
{
  if( ! block.go(MLObjectID::OBJECT_MESH) )
    return;

  int version= block.getInteger();

  switch( version )
    {
    case 1: readMesh_1(mesh,sceneName,block); break;
    default: break;
    }
}

/////////////////////////////////////////////////////////////////////////////
