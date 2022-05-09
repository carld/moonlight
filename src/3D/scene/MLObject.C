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
  MLObject.C

  Stephane Rehel

  December 3 1996
*/

#include "tools/BoundingCone.h"
#include "tools/BoundingCylinder.h"

#include "MLScene.h"
#include "MLObject.h"
#include "MLObjectAllocator.h"
#include "MLObjectsIterator.h"

#include "lighting/MLLight.h"
#include "lighting/MLLightTable.h"
#include "lighting/MLLightPool.h"

/////////////////////////////////////////////////////////////////////////////

MLObject::MLObject( MLObject* _father )
{
  father= _father;
  detached.flag= false;
  detached.father= 0;
  detached.children= 0;
  detached.nChildren= 0;

  if( father != 0 )
    {
    scene= father->scene;
    object_id= scene->objectAllocator->allocID(this);
    }
   else
    {
    scene= 0;
    object_id= 0;
    }

  object_is_uniform_scaled= true;
  object_uniform_area_scaling= 1.;
  object_uniform_normal_scaling= 1.;

  if( scene != 0 )
    {
    int object_name_index= 1;
    MLString object_name= "";
    for(;;)
      {
      object_name= MLString("object-") + MLString::valueOf(object_name_index);
      boolean used= false;
      MLObjectsIterator oi(scene);
      while( ! oi.eol() )
        {
        MLObject* o= oi++;
        if( o->getName().name == object_name )
          {
          used= true;
          break;
          }
        }
      if( ! used )
        break;
      ++object_name_index;
      }
    name.name= object_name;
    }
   else
    name.name= "object-0";

  bounded= false;

  max_children= 16;
  children= new MLObject* [ max_children + 1 ];
  for( int i= 0; i <= max_children; ++i )
    children[i]= 0;
  nChildren= 0;

  if( father != 0 )
    father->addChild(this);

  flags= FLAG(0);
}

/////////////////////////////////////////////////////////////////////////////

MLObject::~MLObject()
{
  if( children != 0 )
    {
    for( int i= 1; i <= max_children; ++i )
      {
      MLObject* o= children[i];
      if( o == 0 )
        continue;
      children[i]= 0;
      delete o;
      }

    delete children;
    children= 0;
    }

  nChildren= 0;

  delete detached.children;
  detached.children= 0;
  detached.flag= true;
  detached.nChildren= 0;
  detached.father= 0;

  if( father != 0 )
    father->removeChild(this);

  if( scene != 0 )
    scene->objectAllocator->freeID(object_id);

  object_id= 0;
}

/////////////////////////////////////////////////////////////////////////////

// do not add children size
int MLObject::getSizeOf() const
{
  int s= sizeof(*this) + max_children * sizeof(children[0]);

  return s;
}

/////////////////////////////////////////////////////////////////////////////

void MLObject::addChild( MLObject* child )
{
  if( child == 0 || child == this )
    return;

  int hole= 0;
  for( int i= 1; i <= max_children; ++i )
    if( children[i] == 0 )
      {
      hole= i;
      break;
      }

  if( hole == 0 )
    {
    int new_max_children= max_children + max_children/2;
    MLObject** new_children= new MLObject* [ new_max_children + 1 ];
    int i;
    new_children[0]= 0;
    for( i= 1; i <= max_children; ++i )
      new_children[i]= children[i];
    for( i= max_children+1; i <= new_max_children; ++i )
      new_children[i]= 0;

    hole= max_children + 1;
    delete children;
    children= new_children;
    max_children= new_max_children;
    }

  children[ hole ]= child;
  ++nChildren;

  child->father= this;
}

/////////////////////////////////////////////////////////////////////////////

void MLObject::removeChild( MLObject* child )
{
  if( child == 0 )
    return;

  if( children != 0 )
    {
    for( int i= 1; i <= max_children; ++i )
      if( children[i] == child )
        {
        children[i]= 0;
        --nChildren;
        return;
        }
    }

  child->father= 0;
  return; // well, not found
}

/////////////////////////////////////////////////////////////////////////////

MLObject* MLObject::getFirstChild() const
{
  if( nChildren <= 0 || children == 0 )
    return 0;

  for( int i= 1; i <= max_children; ++i )
    if( children[i] != 0 )
      return children[i];

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

// detach this object from the scene
boolean MLObject::detach()
{
  if( detached.flag )
    return false; // already detached

  MLObject* root= scene->getRootObject();

  if( this == root )
    return false; // crazy?

  if( getObjectType() == MLObject::LIGHT )
    {
    MLLight* light= (MLLight*) this;
    scene->getLightTable()->removeLight(light);
    MLLightPool* pool= light->getPool();
    if( pool != 0 )
      pool->removeLight(light);
    }

  detached.nChildren= nChildren;
  if( nChildren == 0 )
    detached.children= 0;
   else
    detached.children= new int [ nChildren ];
  detached.father= (father==0) ? root->getID() : father->getID();

  MLObject* previous_father= father;

  setFather(root);

  int j;
  for( j= 0; j < nChildren; ++j )
    detached.children[j]= -1;

  j= 0;
  for( int i= 0; i < max_children; ++i )
    {
    MLObject* child= children[i];
    if( child == 0 )
      continue;
    if( j < detached.nChildren )
      detached.children[j++]= child->getID();

    child->setFather(previous_father);
    }

  if( scene != 0 )
    scene->objectAllocator->detach(object_id);

  // now, detach me from the root object
  root->removeChild(this);
  root->updateTransformations();
  root->updateMaterials();

  detached.flag= true;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// attach this object to the scene (after a detach operation)
// Set its father again, and set its children
boolean MLObject::attach()
{
  if( ! detached.flag )
    return false; // not detached

  MLObject* root= scene->getRootObject();

  if( this == root )
    return false; // crazy.

  // re-insert this object in root object
  root->addChild(this);
  root->updateHierBBox();

  MLObject* new_father= scene->getObject(detached.father);
  if( new_father == 0 )
    new_father= root;

  setFather(new_father);

  if( detached.children != 0 )
    {
    for( int i= 0; i < detached.nChildren; ++i )
      {
      int id= detached.children[i];
      if( id < 0 )
        continue;
      MLObject* new_child= scene->getObject(id);
      if( new_child == 0 )
        continue;
      new_child->setFather(this);
      }

    delete detached.children;
    detached.children= 0;
    }

  detached.father= 0;
  detached.nChildren= 0;

  if( scene != 0 )
    scene->objectAllocator->attach(object_id);

  detached.flag= false;

  if( getObjectType() == MLObject::LIGHT )
    {
    MLLight* light= (MLLight*) this;
    scene->getLightTable()->addLight(light);
    MLLightPool* pool= light->getPool();
    if( pool != 0 )
      pool->addLight(light);
    }

  root->updateMaterials();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// protected
void MLObject::copy( MLObject* o ) const
{
  if( o == 0 )
    return;
  if( father == 0 )
    return; // why do you want to duplicate root??!!

//  ref(o);

  o->hierBBox= hierBBox;
  o->bounded= bounded;
  o->globalBSphere= globalBSphere;
  o->localBSphere= localBSphere;

  o->local_xform= local_xform;
  o->global_xform= global_xform;

  o->flags= flags;
}

/////////////////////////////////////////////////////////////////////////////

void MLObject::scale( const Vector& _scale )
{
  if( getObjectType() == MLObject::LIGHT )
    {
    if( fabs(_scale.x() / _scale.y() - 1.) > 1e-5 ||
        fabs(_scale.x() / _scale.z() - 1.) > 1e-5 )
      return;
    }

  Vector s= _scale;
  if( fabs(s[0]) < 1e-10 ) s[0]= (s[0]<0.) ? -1e-10 : +1e-10;
  if( fabs(s[1]) < 1e-10 ) s[1]= (s[1]<0.) ? -1e-10 : +1e-10;
  if( fabs(s[2]) < 1e-10 ) s[2]= (s[2]<0.) ? -1e-10 : +1e-10;
  local_xform.setScale(s);
}

/////////////////////////////////////////////////////////////////////////////

void MLObject::rotate( const Vector& _rotate )
{
  local_xform.setRotate(_rotate);
}

/////////////////////////////////////////////////////////////////////////////

void MLObject::translate( const Vector& _translate )
{
  local_xform.setTranslate(_translate);
}

/////////////////////////////////////////////////////////////////////////////

// Return 1 if no transformation to be applied
//        0 if object has been non-uniformly scaled in XYZ
//        else: return a scaling factor for the normals
double MLObject::getNormalScaling() const
{
  if( object_is_uniform_scaled )
    return object_uniform_normal_scaling;

  return 0.;
}

/////////////////////////////////////////////////////////////////////////////

void MLObject::updateTransformations()
{
  local_xform.update();

  if( father != 0 )
    {
    global_xform= father->global_xform;
    global_xform.concat(local_xform.transform);
    }

  const Matrix4x4& o2w= global_xform.get_o2w();
  if( o2w(1,1) == o2w(2,2) && o2w(1,1) == o2w(3,3) )
    {
    object_is_uniform_scaled= true;
    object_uniform_area_scaling= sqr(o2w(1,1));
    object_uniform_normal_scaling= o2w(1,1);
    }
   else
    {
    object_is_uniform_scaled= false;
    object_uniform_area_scaling= 1.;
    object_uniform_normal_scaling= 0.;
    }

  for( int i= 1; i <= max_children; ++i )
    if( children[i] != 0 )
      children[i]->updateTransformations();

  updateHierBBox();
}

/////////////////////////////////////////////////////////////////////////////

void MLObject::setFather( MLObject* new_father )
{
  if( father == 0 || new_father == 0 )
    return;
  if( new_father == father )
    return;

  Transform& preXForm= local_xform.preXForm;
  Matrix4x4 o2w=  new_father->global_xform.get_w2o()
                *     father->global_xform.get_o2w()
                * preXForm.get_o2w();
  Matrix4x4 w2o=  preXForm.get_w2o()
                *     father->global_xform.get_w2o()
                * new_father->global_xform.get_o2w();
  preXForm.set( o2w, w2o );

  father->removeChild(this);
  new_father->addChild(this);

  new_father->updateHierBBox();

  updateTransformations();
  updateMaterials();
}

/////////////////////////////////////////////////////////////////////////////

void MLObject::updateHierBBox()
{
  Box local_bbox;
  boolean local_bounded= getLocalBBox(local_bbox);
  bounded= local_bounded;

  Point minP, maxP;
  if( ! local_bounded )
    {
    minP= Point(+1e30,+1e30,+1e30);
    maxP= Point(-1e30,-1e30,-1e30); // will be overriden by children
    }
   else
    {
    minP= local_bbox.getMin();
    maxP= local_bbox.getMax();
    }

  for( int i= 1; i <= max_children; ++i )
    {
    MLObject* o= children[i];
    if( o == 0 )
      continue;

    if( ! o->bounded )
      continue;

    bounded= true;
    Point m= o->hierBBox.getMin();
    Point M= o->hierBBox.getMax();
    Transform& local_t= o->local_xform.transform;
    Point p000= local_t.o2w( Point(m[0],m[1],m[2]) );
    Point p001= local_t.o2w( Point(m[0],m[1],M[2]) );
    Point p010= local_t.o2w( Point(m[0],M[1],m[2]) );
    Point p011= local_t.o2w( Point(m[0],M[1],M[2]) );
    Point p100= local_t.o2w( Point(M[0],m[1],m[2]) );
    Point p101= local_t.o2w( Point(M[0],m[1],M[2]) );
    Point p110= local_t.o2w( Point(M[0],M[1],m[2]) );
    Point p111= local_t.o2w( Point(M[0],M[1],M[2]) );

    minP= min( minP, p000 );
    minP= min( minP, p001 );
    minP= min( minP, p010 );
    minP= min( minP, p011 );
    minP= min( minP, p100 );
    minP= min( minP, p101 );
    minP= min( minP, p110 );
    minP= min( minP, p111 );

    maxP= max( maxP, p000 );
    maxP= max( maxP, p001 );
    maxP= max( maxP, p010 );
    maxP= max( maxP, p011 );
    maxP= max( maxP, p100 );
    maxP= max( maxP, p101 );
    maxP= max( maxP, p110 );
    maxP= max( maxP, p111 );
    }

  if( ! bounded )
    {
    hierBBox= Box( Point(0,0,0), Point(1e-8,1e-8,1e-8) );
    globalBSphere= BoundingSphere( Point(0,0,0), 1e-8 );
    localBSphere= globalBSphere;
    }
   else
    {
    hierBBox= Box( minP, maxP );

    const Point& m= hierBBox.getMin();
    const Point& M= hierBBox.getMax();
    const Matrix4x4& tm= global_xform.get_o2w();

    Point p[8];
    p[0]= tm.apply( Point(m.x(),m.y(),m.z()) );
    p[1]= tm.apply( Point(m.x(),m.y(),M.z()) );
    p[2]= tm.apply( Point(m.x(),M.y(),m.z()) );
    p[3]= tm.apply( Point(m.x(),M.y(),M.z()) );
    p[4]= tm.apply( Point(M.x(),m.y(),m.z()) );
    p[5]= tm.apply( Point(M.x(),m.y(),M.z()) );
    p[6]= tm.apply( Point(M.x(),M.y(),m.z()) );
    p[7]= tm.apply( Point(M.x(),M.y(),M.z()) );

    globalBSphere.getFromPoints( p, 8 );

    if( ! local_bounded )
      {
      localBSphere= BoundingSphere( Point(0,0,0), 1e-8 );
      }
     else
      {
      const Point& m= local_bbox.getMin();
      const Point& M= local_bbox.getMax();

      p[0]= tm.apply( Point(m.x(),m.y(),m.z()) );
      p[1]= tm.apply( Point(m.x(),m.y(),M.z()) );
      p[2]= tm.apply( Point(m.x(),M.y(),m.z()) );
      p[3]= tm.apply( Point(m.x(),M.y(),M.z()) );
      p[4]= tm.apply( Point(M.x(),m.y(),m.z()) );
      p[5]= tm.apply( Point(M.x(),m.y(),M.z()) );
      p[6]= tm.apply( Point(M.x(),M.y(),m.z()) );
      p[7]= tm.apply( Point(M.x(),M.y(),M.z()) );

      localBSphere.getFromPoints( p, 8 );
      }
    }

  if( father != 0 )
    father->updateHierBBox();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLObject::isChildOf( MLObject* o ) const
{
  if( o == 0 || o == this )
    return false;

  MLObject* next_father= father;
  while( next_father != 0 )
    {
    if( next_father == o )
      return true;

    next_father= next_father->getFather();
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

void MLObject::setName( const MLString& _name )
{
  name.name= _name;
}

/////////////////////////////////////////////////////////////////////////////

// object & children
boolean MLObject::inHalfPlane( const Point& world_point,
                             const Vector& world_normal // normalized
                           ) const
{
  if( !bounded )
    return false;

  if( ! globalBSphere.inHalfPlane(world_point,world_normal) )
    return false;

  Point point= global_xform.get_w2o().apply(world_point);
  Vector normal= global_xform.get_w2o().apply(world_normal);

  // more precise
  return hierBBox.inHalfPlane(point,normal);
}

/////////////////////////////////////////////////////////////////////////////

// only mesh
// Cone is in world coordinates
boolean MLObject::meshPossiblyIntersect( const BoundingCone& cone ) const
{
/*
  MeshRep* mesh= getMeshRep();
  if( mesh == 0 )
    return false;

  // Transform local bounding box in world coordinates
  const Matrix4x4& o2w= getGlobalXForm().get_o2w();
  const Box& bbox= mesh->bbox;

//  const Point& minP= bbox.getMin();
//  const Point& maxP= bbox.getMax();
//  Point p000= o2w.apply( Point(minP.x(),minP.y(),minP.z()) );
//  Point p001= o2w.apply( Point(minP.x(),minP.y(),maxP.z()) );
//  Point p010= o2w.apply( Point(minP.x(),maxP.y(),minP.z()) );
//  Point p011= o2w.apply( Point(minP.x(),maxP.y(),maxP.z()) );
//  Point p100= o2w.apply( Point(maxP.x(),minP.y(),minP.z()) );
//  Point p101= o2w.apply( Point(maxP.x(),minP.y(),maxP.z()) );
//  Point p110= o2w.apply( Point(maxP.x(),maxP.y(),minP.z()) );
//  Point p111= o2w.apply( Point(maxP.x(),maxP.y(),maxP.z()) );
//  Point center= (p000+p001+p010+p011+p100+p101+p110+p111) / 8.;

  Point minP= o2w.apply( bbox.getMin() );
  Point maxP= o2w.apply( bbox.getMax() );

  // Get a the bounding sphere of this world coordinates box
  Point center= (minP + maxP) * 0.5;
  double radius= (center - minP).norm();

  BoundingSphere bs(center,radius);

  // and test it with the world cone
  return cone.intersectSphere(bs);
*/

  if( ! isBounded() )
    return true;

  return cone.intersectSphere(localBSphere);
}

/////////////////////////////////////////////////////////////////////////////

// only mesh
// Cylinder is in world coordinates
boolean MLObject::meshPossiblyIntersect( const BoundingCylinder& cyl ) const
{
  if( ! isBounded() )
    return true;

  return cyl.intersectSphere(localBSphere);
}

/////////////////////////////////////////////////////////////////////////////

void MLObject::updateMaterials()
{
  for( int i= 1; i <= max_children; ++i )
    if( children[i] != 0 )
      children[i]->updateMaterials();
}

/////////////////////////////////////////////////////////////////////////////
