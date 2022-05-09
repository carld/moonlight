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
  MLMesh.C

  Stephane Rehel

  October 5 1997
  (from MLObject.C)
*/

#include "MLScene.h"
#include "MLMesh.h"

#include "lighting/MLMeshLighting.h"
#include "lighting/MLGlobalLighting.h"

#include "material/MLMaterial.h"
#include "formal/FormalRep.h"
#include "formal/FormalNURBS.h"

#include "MeshRep.h"

/////////////////////////////////////////////////////////////////////////////

MLMesh::MLMesh( MLObject* _father, MLAbstractMesh* _abstract /* = 0 */ ):
  MLObject(_father)
{
  material= 0; // yes, 0 for root
  material_id= 0;
  inherit_material= true;

  lighting= new MLMeshLighting(this);

  abstract= 0;

  setAbstract(_abstract); // set hierBBox info, etc.

  updateMaterials();
}

/////////////////////////////////////////////////////////////////////////////

MLMesh::~MLMesh()
{
  if( abstract != 0 )
    {
    MLAbstractMesh* remember= abstract;
    abstract->removeReference(this);
    abstract= 0; // already done normally
    ::unref(remember); // possibily deletes it
    }

  delete lighting;
  lighting= 0;
}

/////////////////////////////////////////////////////////////////////////////

// do not add children size
int MLMesh::getSizeOf() const
{
  int s= MLObject::getSizeOf();

  if( abstract != 0 )
    s+= abstract->getSizeOf();

  if( lighting != 0 )
    s+= lighting->getSizeOf();

  return s;
}

/////////////////////////////////////////////////////////////////////////////

// same father. Dont duplicate children.
MLObject* MLMesh::duplicate() const
{
  if( isRoot() )
    return 0; // why do you want to duplicate root??!!

  MLAbstractMesh* ao= 0;
  if( abstract != 0 )
    ao= abstract->duplicate();

  MLMesh* m= new MLMesh( getFather(), ao );

  ::ref(m);

  copy(m);

  return m;
}

/////////////////////////////////////////////////////////////////////////////

// protected
void MLMesh::copy( MLMesh* m ) const
{
  if( m == 0 )
    return;
  if( father == 0 )
    return; // why do you want to duplicate root??!!

  MLObject::copy( (MLObject*) m );

  m->material= material;
  m->material_id= material_id;
  m->inherit_material= inherit_material;
}

/////////////////////////////////////////////////////////////////////////////

void MLMesh::computeFinalLighting()
{
  if( lighting != 0 )
    lighting->getFinalLighting();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLMesh::inheritMaterial() const
{
  return inherit_material;
}

/////////////////////////////////////////////////////////////////////////////

void MLMesh::setMaterial( int _material_id )
{
  if( scene == 0 )
    return;

  if( _material_id == material_id )
    return;

/*
  MLMaterial* old_mat= scene->getMaterial( material);
  MLMaterial* new_mat= scene->getMaterial(_material);

  if( old_mat == new_mat &&
      (old_mat->isRadiosityContributor()?1:0) ==
      (new_mat->isRadiosityContributor()?1:0) )
    return; // no change
*/

  if( inherit_material )
    return; // mmhh?

  material_id= _material_id;
  updateMaterials();

//  if( lighting != 0 )
//    lighting->setObjectMaterial();
}

/////////////////////////////////////////////////////////////////////////////

void MLMesh::setInheritMaterial( boolean yes )
{
  inherit_material= yes;
}

/////////////////////////////////////////////////////////////////////////////

// recursive
void MLMesh::updateMaterials()
{
  if( inherit_material )
    {
    MLObject* f= getFather();
    while( f != 0 )
      {
      if( f->getObjectType() == MLObject::MESH )
        {
        MLMesh* m= (MLMesh*) f;

        material_id= m->material_id;
        material= m->material;
        break;
        }
      f= f->getFather();
      }

    if( f == 0 )
      {
      material= scene->getDefaultMaterial(); // default material
      material_id= material->getID();
      }
    }
   else
    material= scene->getMaterial(material_id);

  if( material == 0 )
    {
    // ???
    material= scene->getDefaultMaterial();
    material_id= material->getID();
    }

  MLObject::updateMaterials();
}

/////////////////////////////////////////////////////////////////////////////

double MLMesh::getPolygonArea( const MLPolygon& p )
{
  if( object_is_uniform_scaled )
    return p.area * object_uniform_area_scaling;

  if( p.nVertices < 3 )
    return 0.;

  MeshRep* mesh= getMeshRep();
  if( mesh == 0 )
    return 0.;
//  assert( mesh == p.mesh );

  if( p.nVertices == 3 )
    {
    register MLVertexArray& vertices= mesh->vertices;
    register Point& _p1= vertices [ p.elements[0].vertex ] .point;
    register Point& _p2= vertices [ p.elements[1].vertex ] .point;
    register Point& _p3= vertices [ p.elements[2].vertex ] .point;

    Point p1= global_xform.o2w(_p1);
    Point p2= global_xform.o2w(_p2);
    Point p3= global_xform.o2w(_p3);

    return ((p2-p1) ^ (p3-p1)).norm() * 0.5;
    }

  if( p.nTessTriangles == 0 )
    return 0.;

  double area= 0.;
  for( int i= 0; i < p.nTessTriangles; ++i )
    {
    register const MLPolygon& pp= mesh->polygons [ p.tesselation[i] ];

    register MLVertexArray& vertices= mesh->vertices;
    register Point& _p1= vertices [ pp.elements[0].vertex ] .point;
    register Point& _p2= vertices [ pp.elements[1].vertex ] .point;
    register Point& _p3= vertices [ pp.elements[2].vertex ] .point;

    Point p1= global_xform.o2w(_p1);
    Point p2= global_xform.o2w(_p2);
    Point p3= global_xform.o2w(_p3);

    area += ((p2-p1) ^ (p3-p1)).norm();
    }

  return area * 0.5;
}

/////////////////////////////////////////////////////////////////////////////

void MLMesh::setAbstract( MLAbstractMesh* _abstract )
{
  if( abstract == _abstract )
    return;

  if( abstract != 0 )
    {
    MLAbstractMesh* remember= abstract;
    abstract->removeReference(this);
    unref(remember); // possibly deletes it
    }

  abstract= _abstract;
  if( abstract != 0 )
    {
    abstract->addReference(this);
    ref(abstract);
    }

  if( lighting == 0 )
    lighting= new MLMeshLighting(this);

  updateHierBBox();
}

/////////////////////////////////////////////////////////////////////////////

FormalRep* MLMesh::getFormalRep() const
{
  if( abstract == 0 )
    return 0;

  return abstract->getFormalRep();
}

/////////////////////////////////////////////////////////////////////////////

FormalNURBS* MLMesh::getFormalNURBS() const
{
  FormalRep* formal= getFormalRep();
  if( formal == 0 )
    return 0;
  if( formal->getType() != FormalRep::NURBS )
    return 0;

  FormalNURBS* fnurbs= (FormalNURBS*) formal;

  return fnurbs;
}

/////////////////////////////////////////////////////////////////////////////

int MLMesh::getSVerticesSize() const
{
  MeshRep* mesh= getMeshRep();
  if( mesh == 0 )
    return 0;

  return mesh->svertices.getSize();
}

/////////////////////////////////////////////////////////////////////////////

int MLMesh::getPolygonsSize() const
{
  MeshRep* mesh= getMeshRep();
  if( mesh == 0 )
    return 0;

  return mesh->polygons.getSize();
}

/////////////////////////////////////////////////////////////////////////////

// return false if not bounded
boolean MLMesh::getLocalBBox( Box& bbox ) const
{
  MeshRep* mesh= getMeshRep();
  if( mesh == 0 )
    return false;

  bbox= mesh->bbox;
  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLMesh::getInfo( int& _nVertices, int& _nSVertices,
                      int& _nEdges,
                      int& _nPolygons,
                      int& _nTessTriangles, int& _nTriangles ) const
{
  MeshRep* mesh= getMeshRep();
  if( mesh == 0 )
    {
    _nVertices= 0;
    _nSVertices= 0;
    _nEdges= 0;
    _nPolygons= 0;
    _nTessTriangles= 0;
    _nTriangles= 0;
    return;
    }

  _nVertices= mesh->nVertices;
  _nSVertices= mesh->nSVertices;
  _nEdges= mesh->nEdges;

  _nTessTriangles= 0;
  _nTriangles= 0;
  _nPolygons= 0;

  for( int i= 1; i <= mesh->polygons.getSize(); ++i )
    {
    const MLPolygon& p= mesh->polygons.get(i);
    if( ! p.allocated() )
      continue;

    if( (p.flags & MLPolygon::TESS_TRIANGLE) != 0 )
      ++_nTessTriangles;

    if( (p.flags & MLPolygon::BASE_POLYGON) != 0 )
      ++_nPolygons;

    if( p.nVertices == 3 )
      ++_nTriangles;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLMesh::regenerate()
{
  MeshRep* meshrep= getMeshRep();

  if( meshrep == 0 )
    return;

  FormalRep* formalRep= getFormalRep();

  if( formalRep == 0 )
    return;

  formalRep->generate(meshrep);

  meshrep->invalidateBoundings();

  meshrep->computeBoundings();

  MLScene* scene= MLObject::getScene();

  scene->getRootObject()->updateHierBBox();

  MLGlobalLighting* gl= scene->getGlobalLighting();

  if( gl == 0 )
    return;

  gl->object_is_transformed(this);
}

/////////////////////////////////////////////////////////////////////////////
