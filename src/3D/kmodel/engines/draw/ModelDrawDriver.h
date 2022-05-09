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
  ModelDrawDriver.h

  Stephane Rehel

  October 28 1997

  Abstract class
*/

#ifndef __ModelDrawDriver_h
#define __ModelDrawDriver_h

class ModelDraw;
class ModelModule;
class MLNullObject;
class MLMesh;
class MLCurve;
class MLLight;
class MLScene;
class MLObject;
class MLCamera;

/////////////////////////////////////////////////////////////////////////////

class ModelDrawDriver
{
protected:
  ModelModule* model;
  ModelDraw* modelDraw;

public:
  ModelDrawDriver( ModelDraw* _modelDraw );

  virtual ~ModelDrawDriver();

  virtual void clear() = 0;

  virtual void setLighting()
    {}

  virtual void drawGrid()
    {}

  virtual void drawCamera( const MLCamera& )
    {}

  virtual void drawNullObject( MLNullObject* no ) = 0;
  virtual void drawMesh( MLMesh* mesh ) = 0;
  virtual void drawCurve( MLCurve* curve ) = 0;
  virtual void drawLight( MLLight* l ) = 0;

  // for picking
  void drawVertices( MLObject* object );

protected:
  void drawMeshVertices( MLMesh* mlmesh );
  void drawCurveVertices( MLCurve* mlcurve );

public:
  // for drawing and picking
  void pickNURBSControlPoints( MLObject* object );

protected:
  void pickNURBSMeshControlPoints( MLMesh* mesh );
  void pickNURBSCurveControlPoints( MLCurve* curve );

public:
  virtual void drawTagVertices( MLObject* )
    {}

  virtual void drawTagPolygons( MLObject* )
    {}

};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelDrawDriver_h
