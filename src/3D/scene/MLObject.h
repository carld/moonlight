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
  MLObject.h

  Stephane Rehel

  December 2 1996
*/

#ifndef __MLObject_h
#define __MLObject_h

#ifndef __MLReference_h
#include "tools/MLReference.h"
#endif

#ifndef __Box_h
#include "tools/Box.h"
#endif

#ifndef __BoundingSphere_h
#include "tools/BoundingSphere.h"
#endif

#ifndef __Transform_h
#include "tools/Transform.h"
#endif

#ifndef __MLXForm_h
#include "MLXForm.h"
#endif

#ifndef __MLName_h
#include "MLName.h"
#endif

class BoundingCone;
class BoundingCylinder;
class MLScene;

/////////////////////////////////////////////////////////////////////////////

class MLObject: public MLReference
{
  friend class MLChildrenIterator;
  friend class MLObjectAllocator;
  friend class MLObjectsIterator;

  friend class MLScene;
  friend class ModelScene;

//  friend class MLAbstractMesh;
  friend class MLRayTracer;

  friend class MLObjectIO;

private:
  int object_id;
  struct
    {
    boolean flag;
    int nChildren;
    int* children;
    int father;
    } detached;

protected:
  MLScene* scene;
  MLObject* father;

  MLName name;

  boolean bounded; // tells if hierBBox is available
                 // depends on abstract & children list
  Box hierBBox; // in object space: object & children
  BoundingSphere globalBSphere; // in world space (object & children)
  BoundingSphere localBSphere;  // in world space (object only)

public:
  MLXForm    local_xform; // object to parent, and inverse
  Transform global_xform; // object to world, and inverse

protected:
  MLObject** children;
  int max_children;
  int nChildren;

  boolean object_is_uniform_scaled;
  double object_uniform_area_scaling;
  double object_uniform_normal_scaling;

public:
  typedef unsigned int Flag;
  Flag flags;
  enum FLAG
    {
    SELECTED=          (1<<0),
    SHOW_BOX=          (1<<1),
    TMP_HIGHLIGHTED=   (1<<2)
    };
  boolean selected() const { return (flags & Flag(SELECTED)) != Flag(0); }

  enum ObjectType
    {
    FIRST_OBJECT_TYPE = 0,
    MESH              = 1, // a set of polygons (from scratch or from formal)
    LIGHT             = 2, // a light
    CAMERA            = 3, // a camera
    CURVE             = 4, // a curve
    NULL_OBJECT       = 5, // a null object
    LAST_OBJECT_TYPE
    };

public:
  MLObject( MLObject* _father );
  virtual ~MLObject();

  virtual int getSizeOf() const;

  void addChild( MLObject* child );
  void removeChild( MLObject* child );
  MLObject* getFirstChild() const;
  int getNChildren() const
    {
    return nChildren;
    }
  boolean isRoot() const
    {
    return father == 0;
    }

  // detach this object from the scene
  boolean detach();

  // attach this object to the scene (after a detach operation)
  // Set its father again, and set its children
  boolean attach();

protected:
  void copy( MLObject* o ) const;

public:
  virtual MLObject* duplicate() const = 0;

  virtual ObjectType getObjectType() const = 0;

  int getID() const
    {
    return object_id;
    }
  const MLName& getName() const
    {
    return name;
    }
  MLScene* getScene() const
    {
    return scene;
    }
  MLObject* getFather() const
    {
    return father;
    }
  boolean isBounded() const
    {
    return bounded;
    }
  const Box& getHierBBox() const
    {
    return hierBBox;
    }

  // in world coordinates
  const BoundingSphere& getGlobalBoundingSphere() const
    {
    return globalBSphere;
    }

  // in world coordinates
  const BoundingSphere& getObjectBoundingSphere() const
    {
    return localBSphere;
    }

  void scale( const Vector& _scale );
  void rotate( const Vector& _rotate );
  void translate( const Vector& _translate );

  const Vector& getScale()     const { return local_xform.scale;     }
  const Vector& getRotate()    const { return local_xform.rotate;    }
  const Vector& getTranslate() const { return local_xform.translate; }

  // Return 1 if no transformation to be applied
  //        0 if object has been non-uniformly scaled in XYZ
  //        else: return a scaling factor for the normals
  double getNormalScaling() const;

  virtual void updateTransformations();

  void setFather( MLObject* new_father );

  void updateHierBBox();
protected:
  // return false if not bounded
  virtual boolean getLocalBBox( Box& box ) const = 0;

public:
  const Transform& getGlobalXForm() const
    {
    return global_xform;
    }

  boolean isChildOf( MLObject* o ) const;

  void setName( const MLString& _name );

  // object & children
  boolean inHalfPlane( const Point& world_point,
                     const Vector& world_normal // normalized
                   ) const;

  // only mesh
  // Cone is in world coordinates
  boolean meshPossiblyIntersect( const BoundingCone& cone ) const;

  // only mesh
  // Cylinder is in world coordinates
  boolean meshPossiblyIntersect( const BoundingCylinder& cyl ) const;

  virtual void updateMaterials();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLObject_h
