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
  InterfaceKernelModule.C

  Stephane Rehel

  May 31 1998
*/

#include <stdio.h>
#include <stdlib.h>

#include "tools/MLCommandLine.h"
#include "tools/MLFilename.h"

#include "kernel/MLKernel.h"
#include "kernel/MLNode.h"

#include "graphics/InitGraphicsSystem.h"
#include "graphics/MLEventManager.h"

#include "interface/Interface.h"
#include "interface/InterfaceAppRoot.h"
#include "interface/MLModule.h"

#include "InterfaceKernelModule.h"

/////////////////////////////////////////////////////////////////////////////

InterfaceKernelModule::InterfaceKernelModule( MLKernel* kernel,
                                              MLNode* data ):
  MLKernelModule(kernel,data)
{
  name= "KoalaGUI";
}

/////////////////////////////////////////////////////////////////////////////

InterfaceKernelModule::~InterfaceKernelModule()
{
}

/////////////////////////////////////////////////////////////////////////////

KERNEL_MODULE_INIT(interface,InterfaceKernelModule)

/////////////////////////////////////////////////////////////////////////////

MLAppRoot* InterfaceKernelModule::init( MLKernel* kernel, MLNode* data )
{
  MLCommandLine& command= kernel->getCommandLine();

  if( ! ::InitGraphicsSystem( kernel->getConfig(), command ) )
    {
    fprintf( stderr, "Error: unable to open graphics system\n" );
    return 0;
    }

  ::interface= new Interface(kernel);
  ::interface->init();

  MLString errormsg;
//  LOAD_KERNEL_MODULE(kmodel,kernel,0,&errormsg);
  MLModule* module= 0;

  module= ::interface->queryAppModule("kmodel",&errormsg);
  if( module == 0 || errormsg.length() != 0 )
    {
    if( errormsg.length() > 0 )
    printf("%s\n",errormsg.get());
    return 0;
    }

  ::interface->activateModule(module->getID());

  module= ::interface->queryAppModule("kbuilder",&errormsg);
  if( MLFilename(command.argv(0)).getFilename() == "kbuilder" )
    ::interface->activateModule(module->getID());
  if( errormsg.length() != 0 )
    {
    printf("%s\n",errormsg.get());
    return 0;
    }

//module= ::interface->queryAppModule("photo",&errormsg);
//module= ::interface->queryAppModule("oom",&errormsg);

  MLEventManager* eventManager= MLEventManager::getEventManager();
  eventManager->removeAnyRedraw();
  interface->refreshInterface();

  ::interface->mainloop();

  return ::interface->getInterfaceAppRoot();
}

/////////////////////////////////////////////////////////////////////////////
