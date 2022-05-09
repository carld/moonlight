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
  MLModuleLoader.C

  Stephane Rehel
  March 30 1998
*/

#include "tools/MLString.h"
#include "tools/MLDynamicLibrary.h"
#include "tools/MLPath.h"
#include "tools/MLPathList.h"

#include "MLKernel.h"
#include "MLNode.h"
#include "MLAppRoot.h"

#include "MLModuleLoader.h"

static const char* main_module_fct_name= "ml_module_main";

typedef MLAppRoot* (*MLModuleInitFct)( MLKernel* kernel, MLNode* data );

/////////////////////////////////////////////////////////////////////////////

MLModuleLoader::MLModuleLoader()
{
  approot= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLModuleLoader::~MLModuleLoader()
{
  delete approot;
  approot= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLAppRoot* MLModuleLoader::load( MLKernel* kernel,
                                 const MLString& name,
                                 MLNode* data /* = 0 */ )
{
  if( approot != 0 )
    return 0; // already loaded

  if( ! MLDynamicLoader::load(kernel,name) )
    return 0;

  MLModuleInitFct init_fct= (MLModuleInitFct)
                            dl->getfct(main_module_fct_name);

  if( init_fct == 0 )
    {
    errorMessage= MLString("unable to find function ")
                  + MLString(main_module_fct_name)
                  + MLString("() in ")
                  + full_lib_filename;

    delete dl;
    dl= 0;
    return 0;
    }

  approot= (*init_fct)(kernel,data);

  // can return 0 !

  return approot;
}

/////////////////////////////////////////////////////////////////////////////

