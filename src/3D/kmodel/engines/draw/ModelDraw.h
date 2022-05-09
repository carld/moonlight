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
  ModelDraw.h

  Stephane Rehel

  December 8 1996
*/

#ifndef __ModelDraw_h
#define __ModelDraw_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

class MLCamera;
class MLObject;
class MLScene;
class ModelModule;
class ModelEngine;
class IntegerArray;
class ModelDrawDriver;
class MLTagVertexArray;

/////////////////////////////////////////////////////////////////////////////

class ModelDraw
{
  friend class ModelDrawDriver;
  friend class ModelDrawWire;
  friend class ModelDrawPreview;

protected:
  ModelModule* model;
  ModelEngine* engine;

  MLCamera* camera;
  boolean selectMode;

  ModelDrawWire* modelDrawWire;
  ModelDrawPreview* modelDrawPreview;

  ModelDrawDriver* drawDriver;

public:
  enum DISPLAY_TYPE
    {
    DISPLAY_NONE       = 0,
    DISPLAY_WIRE       = 1,
    DISPLAY_PREVIEW    = 2,
    DISPLAY_WIRE_FRONT = 3
    };

protected:
  DISPLAY_TYPE displayType;

public:
  ModelDraw( ModelEngine* _engine );
  virtual ~ModelDraw();

  ModelModule* getModel() const
    {
    return model;
    }

  DISPLAY_TYPE getDisplayType() const
    {
    return displayType;
    }

  void setDisplayType( DISPLAY_TYPE dt );

  void setCamera( MLCamera& _camera );

  void clear();
  void drawGrid();

  void draw( MLScene* );
  void draw( MLObject* );

public:
  // returns a objectIDs array
  void pickObjects( MLScene* scene,
                    const IPoint& _p1, const IPoint& _p2,
                    IntegerArray& objects );

  // returns a (objectID,vertexID) array
  void pickVertices( MLScene* scene,
                     const IPoint& _p1, const IPoint& _p2,
                     MLTagVertexArray& vertices );

  void drawCamera( const MLCamera& c );

protected:
  void drawHier( MLObject* o );
  void drawObject( MLObject* o );

public:
  void resetAll();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelDraw_h
