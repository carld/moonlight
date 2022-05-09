// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//                 2002       Patrick Mauritz
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
  ModelModes.C

  Stephane Rehel

  December 9 1996
*/

#include <string.h>

#include "ModelModes.h"

#include "interface/MLModes.h"

#include "modes/MLMSelect.h"
#include "modes/MLMDrag.h"
#include "modes/MLMClick.h"
#include "modes/MLMCameraOrbit.h"
#include "modes/MLMCameraZoom.h"
#include "modes/MLMCameraWalk.h"
#include "modes/MLMRay.h"
#include "modes/MLMRaySelected.h"
#include "modes/MLMTagVertex.h"

/////////////////////////////////////////////////////////////////////////////

// static
void ModelModes::init( ModelModule* model, MLModes* m )
{
  if( m == 0 )
    return;

  m->add( SELECT,       new MLMSelect(model) );
  m->add( DRAG,         new MLMDrag(model) );
  m->add( CLICK,        new MLMClick(model) );
  m->add( CAMERA_ORBIT, new MLMCameraOrbit(model) );
  m->add( CAMERA_ZOOM,  new MLMCameraZoom(model) );
  m->add( CAMERA_WALK,  new MLMCameraWalk(model) );
  m->add( RAY,          new MLMRay(model) );
  m->add( RAY_SELECTED, new MLMRaySelected(model) );
  m->add( TAG_VERTEX,   new MLMTagVertex(model) );
}

/////////////////////////////////////////////////////////////////////////////
