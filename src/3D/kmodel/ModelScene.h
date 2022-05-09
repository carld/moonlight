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
  ModelScene.h

  Stephane Rehel

  December 15 1996
*/

#ifndef __ModelScene_h
#define __ModelScene_h

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

#ifndef __IToggle_h
#include "tools/IToggle.h"
#endif

class MLScene;
class MLObject;
class ModelCommand;
class ModelModule;
class IntegerArray;
class MLLightPool;
class MeshRep;

/////////////////////////////////////////////////////////////////////////////

class ModelScene
{
protected:
  ModelModule* model;
  MLScene* scene; // not owner

public:
  ModelScene( ModelModule* _model, MLScene* _scene );

  virtual ~ModelScene();

  // Check for boundings/hashboxes to update
  void updateBoundingHashBox();

  void translate( ModelCommand* cmd );
  void _translate( MLObject* o, const Vector& v, boolean done );

  void translateVertices( ModelCommand* cmd );

private:
  void finalizeVerticesTransformation( MeshRep* meshrep );

public:
  void translatePolygons( ModelCommand* cmd );

private:
  void finalizePolygonsTransformation( MeshRep* meshrep );

public:
  boolean scale( const Vector& s );
  void _scale( MLObject* o, const Vector& s, boolean done );

  void scaleVertices( ModelCommand* cmd );
  void scalePolygons( ModelCommand* cmd );

  boolean rotate( const Vector& a );
  void _rotate( MLObject* o, const Vector& a, boolean done );

  void rotateVertices( ModelCommand* cmd );

  boolean world_rotate( const Vector& axis, double angle );
  void _world_rotate( MLObject* o,
                      const Vector& axis, double angle, boolean done );

  void world_rotateVertices( ModelCommand* cmd,
                             const Vector& axis, double angle );

  void selectAll( IToggle toggle );

  // return 0 if none selected or more than a single one
  int getSingleSelected();

  int getSelectedObjects( ModelCommand* cmd );

  int getAllObjects( ModelCommand* cmd );
  int getAllLights( ModelCommand* cmd );

  void selectObjects( ModelCommand* cmd, IToggle toggle );

  boolean reparent( MLObject* child, MLObject* new_parent );

  void deleteObjects( ModelCommand* cmd );
  void wakeupObjects( ModelCommand* cmd );
  void duplicateObjects( ModelCommand* cmd );
  void instantiateObjects( ModelCommand* cmd );
  void hideObjects( ModelCommand* cmd, IToggle toggle );

  void resetAll();

  void resetObjectsRadiosity();

  boolean removeAllHighlighting();
  void setHighlighted( IntegerArray& );

  // Return the selected pool. Only lights must be selected.
  // All these lights must belong to the same pool
  MLLightPool* getSelectedPool();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelScene_h
