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
  MLObject_1_0.C

  Stephane Rehel
  April 14 1997

  MLObject* MLObjectIO::read_1_0( MLRFileBlock* block );
*/

#include "MLObjectIO.h"
#include "MLObjectIDs.h"

#include "tools/file/MLRFile.h"
#include "tools/file/MLRFileBlock.h"

#include "MLAbstractMeshIO.h"

#include "scene/MLObjectAllocator.h"
#include "scene/MLObject.h"
#include "scene/MLMesh.h"
#include "scene/lighting/MLLight.h"
#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

MLObject* MLObjectIO::read_1_0( MLObject* father, MLRFileBlock& block )
{
  if( ! block.find(MLObjectID::OBJECT_DEF) )
    return 0;

  MLName sceneName, name;
  MLAbstractMesh* ao= 0;
  int objectType;
  unsigned int flags;

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

    MLString mat_name;
    block >> mat_name; //***!!
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

  // set default material for the object reader version 1.0
  if( mesh != 0 )
    mesh->material= scene->getDefaultMaterial();

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
    block.go(MLObjectID::OBJECT_LIGHT);
      {
/*
      MLLight* light= (MLLight*) o;
      int lightType;
      block >> lightType;
      light->setLightType(MLLight::LightType(lightType));

      block >> light->color;
      light->intensity= block.getDouble();
      light->extent= block.getDouble();
      light->coneAngle= block.getDouble();
      light->spreadAngle= block.getDouble();
      block >> light->unshotEnergy;
      light->energy_has_been_shot= block.getInteger();
      Point coneOrigin; Vector coneAxis; double coneCosAlpha;
      block >> coneOrigin >> coneAxis >> coneCosAlpha;
      light->cone.set(coneOrigin,coneAxis,coneCosAlpha);
*/
      MLLight* light= (MLLight*) o;
      int lightType;
      block >> lightType;
      light->setLightType(MLLight::LightType(lightType));

      light->directLight= false;
      light->globalLight= true;

      block >> light->color;
      light->global_intensity= block.getDouble();
      light->extent= block.getDouble();
      light->has_extent= false;
//      light->coneAngle= block.getDouble();
      light->spot_cutoff= block.getDouble();
//      light->spreadAngle= block.getDouble();
      block.getDouble();
      block >> light->unshotEnergy;
      light->energy_has_been_shot= block.getInteger();
      Point coneOrigin; Vector coneAxis; double coneCosAlpha;
      block >> coneOrigin >> coneAxis >> coneCosAlpha;
      light->cone.set(coneOrigin,coneAxis,coneCosAlpha);
      }
    }

  block.go(MLObjectID::OBJECT_CHILDREN);
    {
    int nChildren;
    block >> nChildren;

    for( int i= 1; i <= nChildren; ++i )
      {
      MLRFileBlock b;
      b.open(block.getFile());
        if( read_1_0(o,b) == 0 )
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
