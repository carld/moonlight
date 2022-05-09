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
  MLCEditPool.C

  Stephane Rehel

  March 23 1997
*/

#include <math.h>

#include "scene/MLScene.h"
#include "scene/lighting/MLLightPool.h"
#include "scene/lighting/MLGlobalLighting.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelCommands.h"

#include "MLCEditPool.h"

/////////////////////////////////////////////////////////////////////////////

MLCEditPool::MLCEditPool()
{
  type= ModelCommands::EDIT_POOL;

  pool= 0;
  intensity= 0;
  color= Color(0,0,0);
}

/////////////////////////////////////////////////////////////////////////////

MLCEditPool::~MLCEditPool()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLCEditPool::doit( ModelModule* model )
{
  MLScene* scene= model->getScene();

  if( pool == 0 || scene->getPool(string) != pool )
    return true; // mmhhh?

  model->stopLightingProcess();

  pool->color= color;
  pool->intensity= intensity;

  MLGlobalLighting* glighting= scene->getGlobalLighting();

  glighting->pool_energy_is_changed(pool);

  model->refreshScreen( ModelModule::REFRESH_ILLUM |
                        ModelModule::REFRESH_DIALOGS );//only pool dialog...

  return true;
}

/////////////////////////////////////////////////////////////////////////////
