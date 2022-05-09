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
  MLObject_1_1.C

  Stephane Rehel
  August 31 1997

  MLObject* MLObjectIO::read_1_1( MLRFileBlock* block );

  -> don't forget to update the recursive call of read_?_?()
*/

#include "MLObjectIO.h"
#include "MLObjectIDs.h"

#include "tools/file/MLRFile.h"
#include "tools/file/MLRFileBlock.h"

#include "MLAbstractMeshIO.h"
#include "MLMaterialIO.h"

#include "scene/MLObjectAllocator.h"
#include "scene/MLObject.h"
#include "scene/MLMesh.h"
#include "scene/lighting/MLLight.h"
#include "scene/material/MLMaterial.h"
#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

MLObject* MLObjectIO::read_1_1( MLObject* father, MLRFileBlock& block )
{
  if( ! block.find(MLObjectID::OBJECT_DEF) )
    return 0;

  MLName sceneName, name;
  MLAbstractMesh* ao= 0;
  int objectType;
  unsigned int flags;
  MLName material_name;
  int inherit_material;

  block.go(MLObjectID::OBJECT_DEF);
    {
    block >> sceneName;
    block >> name;

    block >> objectType;

    int hasAbstract;
    block >> hasAbstract;
    if( hasAbstract )
      {
      MLName aname;
      block >> aname;
      aname.filename=
                   MLAbstractMeshIO::getAbstractObjectFilename( database,
                                                                  sceneName,
                                                                  aname );

      ao= MLAbstractMeshIO::read(database,scene,aname.filename,progress);
      if( ao == 0 )
        return 0; //*** give an error mesg!!
      }

    block >> flags;

    block >> material_name;
    block >> inherit_material;
    }

  MLObject* o= 0;
  switch( objectType )
    {
    case MLObject::MESH: o= new MLMesh(father,ao); break;
    case MLObject::LIGHT: o= new MLLight(father,0); break;
    default: return 0;
    }

  ref(o);
  o->name= name;
  o->flags= MLObject::Flag(flags);

  MLMesh* mesh= (objectType==MLObject::MESH) ? ((MLMesh*)o) : 0;

  // First, load default material
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
    if( mesh != 0 )
      {
      mesh->material= scene->getDefaultMaterial(); // safe
      mesh->material_id= mesh->material->getID();
      mesh->inherit_material= true;
      }
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

    if( mesh != 0 )
      {
      mesh->material= material;
      mesh->material_id= material->getID();
      mesh->inherit_material= false;
      }
    }

  o->updateMaterials();

  block.go(MLObjectID::OBJECT_PARAMS);
    {
    block >> o->hierBBox;
    o->bounded= block.getInteger() != 0;
    double sphere_radius;
    Point sphere_center;
    block >> sphere_center;
    block >> sphere_radius;
    o->globalBSphere= BoundingSphere(sphere_center,sphere_radius);
    block >> o->local_xform.preXForm;
    block >> o->local_xform.scale;
    block >> o->local_xform.rotate;
    block >> o->local_xform.translate;
    block >> o->local_xform.transform;
    block >> o->global_xform;
    }

  if( mesh != 0 )
    mesh->setAbstract(ao);

  if( o->getObjectType() == MLObject::LIGHT )
    {
    // in MLLightIO.C
    readLight( (MLLight*) o, block );
    }

  block.go(MLObjectID::OBJECT_CHILDREN);
    {
    int nChildren;
    block >> nChildren;

    for( int i= 1; i <= nChildren; ++i )
      {
      MLRFileBlock b;
      b.open(block.getFile());
        if( read_1_1(o,b) == 0 )
          {
          delete o;
          return 0; //***!!!
          }
      b.close();
      }
    }

  // it's the correct filename ... normally
  o->name.filename= getObjectFilename( database, sceneName, o->name );

  // adjust any other object name that poinst to this o->name
  // this prevents other objects to have the same filename
  // when we save next time
  scene->getObjectAllocator()->adjustNameIndices(o);

  scene->getRootObject()->updateTransformations();

  if( o != 0 )
    if( o->getObjectType() == MLObject::LIGHT )
      ((MLLight*)o)->compile();

  return o;
}

/////////////////////////////////////////////////////////////////////////////
