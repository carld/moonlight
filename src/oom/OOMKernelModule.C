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
  OOMKernelModule.C

  Stephane Rehel

  June 7 1998
*/

#include "OOMKernelModule.h"

#include "kernel/MLKernel.h"
#include "kernel/MLNode.h"

#include "interface/Interface.h"
#include "interface/InterfaceAppRoot.h"

#include "OOMModule.h"

/////////////////////////////////////////////////////////////////////////////

OOMKernelModule::OOMKernelModule( MLKernel* kernel, MLNode* data ):
  MLKernelModule(kernel,data),
  MLAppRoot(kernel)
{
  name= "KBuilder";
}

/////////////////////////////////////////////////////////////////////////////

OOMKernelModule::~OOMKernelModule()
{
}

/////////////////////////////////////////////////////////////////////////////

KERNEL_MODULE_INIT(oom,OOMKernelModule)

/////////////////////////////////////////////////////////////////////////////

MLAppRoot* OOMKernelModule::init( MLKernel* kernel, MLNode* data )
{
  if( ! MLAppRoot::init() )
    return 0;

  if( data->getName() != "KoalaInterface" )
    return 0;

  InterfaceAppRoot* interfaceAppRoot= (InterfaceAppRoot*) data;
  Interface* interface= interfaceAppRoot->getInterface();

  OOMModule* module= new OOMModule(interface);
  if( ! module->init(this) )
    return 0;

  return this;
}

/////////////////////////////////////////////////////////////////////////////
