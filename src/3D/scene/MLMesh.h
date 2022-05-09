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
  MLMesh.h

  Stephane Rehel

  October 5 1997
  (from MLObject.h)
*/

#ifndef __MLMesh_h
#define __MLMesh_h

#ifndef __MLObject_h
#include "MLObject.h"
#endif

// for MLMesh::getMeshRep()
#ifndef __MLAbstractMesh_h
#include "MLAbstractMesh.h"
#endif

class FormalRep;
class FormalNURBS;
class MeshRep;
class MLMaterial;
class MLMeshLighting;
class MLPolygon;

/////////////////////////////////////////////////////////////////////////////

class MLMesh: public MLObject
{
  friend class MLScene;
  friend class MLAbstractMesh;
  friend class ModelScene;
  friend class MLRayTracer;

  friend class MLObjectIO;

protected:
  MLAbstractMesh* abstract;

  MLMaterial* material; // not owner
  boolean inherit_material;
  int material_id;

  MLMeshLighting* lighting;

  void copy( MLMesh* m ) const;

public:
  MLMesh( MLObject* _father, MLAbstractMesh* _abstract = 0 );

  virtual ~MLMesh();

  virtual int getSizeOf() const;

  virtual MLObject* duplicate() const;

  virtual ObjectType getObjectType() const
    {
    return MLObject::MESH;
    }

  MLAbstractMesh* getAbstract() const
    {
    return abstract;
    }

  // MeshRep must be access quickly since raytracing needs it
  MeshRep* getMeshRep() const
    {
    if( abstract == 0 )
      return 0;
    return abstract->getMeshRep();
    }

  FormalRep* getFormalRep() const;
  boolean isFormal() const
    {
    return getFormalRep() != 0;
    }

  FormalNURBS* getFormalNURBS() const;
  boolean isNURBS() const
    {
    return getFormalNURBS() != 0;
    }

  int getSVerticesSize() const;
  int getPolygonsSize() const;

  // return false if not bounded
  virtual boolean getLocalBBox( Box& bbox ) const;

  MLMeshLighting* getLighting() const
    {
    return lighting;
    }
  void computeFinalLighting();

  // material should updated (and compiled) before calling this
  MLMaterial* getMaterial() const
    {
    return material;
    }
  boolean inheritMaterial() const;
  void setMaterial( int _material_id );
  void setInheritMaterial( boolean yes );
  // recursive
  virtual void updateMaterials();

  double getPolygonArea( const MLPolygon& p );

  void setAbstract( MLAbstractMesh* _abstract );

  void getInfo( int& _nVertices, int& _nSVertices,
                int& _nEdges,
                int& _nPolygons,
                int& _nTessTriangles, int& _nTriangles ) const;

  void regenerate();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLMesh_h
