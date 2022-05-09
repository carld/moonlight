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
  MLAbstractMesh.h

  Stephane Rehel

  December 12 1996
*/

#ifndef __MLAbstractMesh_h
#define __MLAbstractMesh_h

#ifndef __Box_h
#include "tools/Box.h"
#endif

#ifndef __MLAbstractObject_h
#include "MLAbstractObject.h"
#endif

class FormalRep;
class MeshRep;
class MLScene;
class MLMesh;

/////////////////////////////////////////////////////////////////////////////

class MLAbstractMesh: public MLAbstractObject
{
  friend class MLAbstractMeshAllocator;
  friend class ModelDraw;
  friend class MLMesh;

  friend class MLAbstractMeshIO;
  friend class MLSceneIO; // set name.filename when reading

private:
  int abstract_mesh_id;

protected:
  FormalRep* formalRep;
  MeshRep* meshRep;

//  Box bbox;

public:
  MLAbstractMesh( MLScene* _scene, FormalRep* _formalRep = 0 );

  virtual ~MLAbstractMesh();

  int getSizeOf() const;

  void addReference( MLMesh* instance );
  void removeReference( MLMesh* instance );

  MLAbstractMesh* duplicate();

  MeshRep* getMeshRep() const
    {
    return meshRep;
    }

  FormalRep* getFormalRep() const
    {
    return formalRep;
    }

  void setFormalRep( FormalRep* _formalRep );
  void generateMesh();
  Box getBBox() const;

  int getID() const
    {
    return abstract_mesh_id;
    }

  void polygonize();
  void unpolygonize( FormalRep* _formalRep );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLAbstractMesh_h
