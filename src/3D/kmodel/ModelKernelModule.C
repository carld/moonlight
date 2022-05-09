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
  ModelKernelModule.C

  Stephane Rehel

  April 16 1998
*/

#include <stdio.h>
#include <stdlib.h>

#include "tools/MLCommandLine.h"

#include "kernel/MLKernel.h"
#include "kernel/MLNode.h"

#include "interface/Interface.h"
#include "interface/InterfaceAppRoot.h"

#include "scene/MLScene.h"

#include "ModelKernelModule.h"
#include "ModelModule.h"

/////////////////////////////////////////////////////////////////////////////

ModelKernelModule::ModelKernelModule( MLKernel* kernel, MLNode* data ):
  MLKernelModule(kernel,data)
{
  name= "Moonlight-3D";
}

/////////////////////////////////////////////////////////////////////////////

ModelKernelModule::~ModelKernelModule()
{
}

/////////////////////////////////////////////////////////////////////////////

KERNEL_MODULE_INIT(kmodel,ModelKernelModule)

/////////////////////////////////////////////////////////////////////////////

MLAppRoot* ModelKernelModule::init( MLKernel* kernel, MLNode* data )
{
//  MLCommandLine& command= kernel->getCommandLine();

  if( data->getName() != "KoalaInterface" )
    return 0;

  InterfaceAppRoot* interfaceAppRoot= (InterfaceAppRoot*) data;
  Interface* interface= interfaceAppRoot->getInterface();

  ModelModule* model= new ModelModule(interface);
  if( ! model->init() )
    return 0;

  return model->getScene();
}

/////////////////////////////////////////////////////////////////////////////
