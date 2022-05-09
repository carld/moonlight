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
  MLScene.h

  Stephane Rehel

  December 8 1996
*/

#ifndef __MLScene_h
#define __MLScene_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLName_h
#include "MLName.h"
#endif

#include "kernel/MLAppRoot.h"

class MLObject;
class MLNullObject;
class MLLight;
class MLMaterial;

class MLString;
class MLObjectAllocator;
class MLAbstractMeshAllocator;
class MLAbstractCurveAllocator;
class MLMaterialAllocator;
class MLLightTable;
class MLLightPool;
class MLPoolTable;
class MLGlobalLighting;
class MLMaterialImages;
class MLRendererOptions;

class MLConfig;

/////////////////////////////////////////////////////////////////////////////

class MLScene: public MLAppRoot
{
  friend class MLObject;
  friend class MLAbstractMesh;
  friend class MLAbstractCurve;
  friend class MLMaterial;
  friend class MLSceneIO;
  friend class MLSaveProject;

protected:
  MLObjectAllocator* objectAllocator;
  MLAbstractMeshAllocator* abstractAllocator;
  MLAbstractCurveAllocator* abstractCurveAllocator;
  MLMaterialAllocator* materialAllocator;
  MLLightTable* lights;
  MLPoolTable* poolTable;
  MLGlobalLighting* globalLighting;
  MLMaterialImages* materialImages;
  MLRendererOptions* rendererOptions;

  MLObject* rootObject; // this is a NullObject

  MLName name;
  MLConfig* config;
  MLString rc_group;

  boolean _verbose;

public:
  boolean dirtyPoolsLighting; // need to refresh some pool lightings
  boolean dirtyObjectsLighting; // need to refresh some object lightings
  boolean dirtyFinalLighting; // i.e. please refresh illum

public:
  MLScene( MLKernel* _kernel );

  virtual ~MLScene();

  boolean init();

  boolean verbose() const
    {
    return _verbose;
    }

  void setConfigGroup( const MLString& _rc_group );

  MLConfig* getConfig() const
    {
    return config;
    }
  const MLString& getConfigGroup() const
    {
    return rc_group;
    }

  const MLName& getName() const
    {
    return name;
    }
  void setName( const MLString& _name );
  void setName( const MLName& _name );

  MLObject* getRootObject() const
    {
    return rootObject;
    }

  int getNObjects() const;
  MLObject* getObject( int id, int object_type = 0 );
  MLObject* getDetachedObject( int id );
  MLObject* getObject( const MLName& name, int object_type = 0 );
  MLObject* getObjectByFilename( const MLString& filename ) const;
  MLObjectAllocator* getObjectAllocator() const
    {
    return objectAllocator;
    }

  MLAbstractMesh* getAbstractObject( int id ) const;
  MLAbstractMesh* getAbstractObject( const MLName& name ) const;
  MLAbstractMesh* getAbstractByFilename( const MLString& filename ) const;
  MLAbstractMeshAllocator* getAbstractAllocator() const
    {
    return abstractAllocator;
    }

  MLAbstractCurve* getAbstractCurve( int id ) const;
  MLAbstractCurve* getAbstractCurve( const MLName& name ) const;
  MLAbstractCurve* getAbstractCurveByFilename( const MLString& filename ) const;
  MLAbstractCurveAllocator* getAbstractCurveAllocator() const
    {
    return abstractCurveAllocator;
    }

  int getNLights() const;
  MLLight* getLight( int light_index ) const;
  MLLight* getLight( const MLName& name ) const;
  MLLightTable* getLightTable() const
    {
    return lights;
    }
  void compileLights();

  int getNMaterials() const;
  MLMaterial* getMaterial( int id );
  MLMaterial* getMaterial( const MLString& name );
  MLMaterial* getDefaultMaterial() const;

  // Allow this since raytracing must get materials very quickllyyyy
  MLMaterialAllocator* getMaterialAllocator() const
    {
    return materialAllocator;
    }

  void updateTransformations();

  void computeFinalLighting();

  int getNPools() const;
  MLLightPool* getPool( int pool_index ) const;
  MLLightPool* getPool( const MLString& name ) const;
  MLPoolTable* getPoolTable() const
    {
    return poolTable;
    }

  MLGlobalLighting* getGlobalLighting() const
    {
    return globalLighting;
    }

  MLMaterialImages* getMaterialImages() const
    {
    return materialImages;
    }

  MLRendererOptions* getRendererOptions() const
    {
    return rendererOptions;
    }

  void clear();

  // Check for boundings/hashboxes to update
  void updateBoundingHashBox();

protected:
  void createRootObject();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLScene_h
