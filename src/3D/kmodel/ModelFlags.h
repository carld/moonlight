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
  ModelFlags.h

  Stephane Rehel

  August 5 1997
*/

#ifndef __ModelFlags_h
#define __ModelFlags_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

class ModelModule;

/////////////////////////////////////////////////////////////////////////////

class ModelFlags
{
protected:
  ModelModule* model;

public:
  enum VARIABLE_NAME
    {
    FIRST_VARIABLE_NAME = 0,

    SINGLE_SELECTION,
    SHOW_VIEW_AXES,
    SHOW_OBJECT_AXES,
    SHOW_CAMERA,
    SHOW_HIER_STRUCT,
    SHOW_TESSELATION,
    SHOW_NORMALS,
    SHOW_HASHBOX,
    WRITE_SNAPSHOT,
    COMPUTE_DIRECT_LIGHTING,
    COMPUTE_INDIRECT_LIGHTING,
    UPDATE_SHADOWS,
    DEFAULT_LIGHTS_DIRECT,
    DEFAULT_LIGHTS_GLOBAL,

    MODELLING_COMPONENT,

    LAST_VARIABLE_NAME
    };

  boolean singleSelection;
  boolean showViewAxes;
  boolean showObjectAxes;
  boolean showCamera;
  boolean showHierStruct;
  boolean showTesselation;
  boolean showNormals;
  boolean showHashBox;
  boolean writeSnapshot;

  boolean computeDirectLighting;
  boolean computeIndirectLighting;
  boolean updateShadows;

  boolean defaultLightsDirect;
  boolean defaultLightsGlobal;

  enum MODELLING_COMPONENT_TYPE
    {
    COMPONENT_OBJ= 1,
    COMPONENT_VTX= 2,
    COMPONENT_POL= 3
    };

  MODELLING_COMPONENT_TYPE modellingComponent;

  ModelFlags( ModelModule* _model );
  virtual ~ModelFlags();

  void reset();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelFlags_h
