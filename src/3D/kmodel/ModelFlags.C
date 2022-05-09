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
  ModelFlags.C

  Stephane Rehel

  August 5 1997
*/

#include "ModelFlags.h"

/////////////////////////////////////////////////////////////////////////////

ModelFlags::ModelFlags( ModelModule* _model )
{
  model= _model;

  reset();
}

/////////////////////////////////////////////////////////////////////////////

ModelFlags::~ModelFlags()
{}

/////////////////////////////////////////////////////////////////////////////

void ModelFlags::reset()
{
  singleSelection= true;
  showViewAxes= true;
  showObjectAxes= false;
  showCamera= false;
  showHierStruct= true;
  showTesselation= false;
  showNormals= false;
  showHashBox= false;
  writeSnapshot= true;

  computeDirectLighting= true;
  computeIndirectLighting= false;
  updateShadows= true;

  defaultLightsDirect= true;
  defaultLightsGlobal= false;

  modellingComponent= COMPONENT_OBJ;
}

/////////////////////////////////////////////////////////////////////////////
