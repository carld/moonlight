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
  MLPluginLoader.C

  Stephane Rehel
  March 30 1998
*/

#include "tools/MLString.h"
#include "tools/MLDynamicLibrary.h"

#include "MLNode.h"
#include "MLAppRoot.h"

#include "MLPluginLoader.h"

static const char* main_plugin_fct_name= "ml_plugin_main";

typedef MLNode* (*MLPluginInitFct)( MLAppRoot* approot,
                                    MLNode* plugins, MLNode* data );

/////////////////////////////////////////////////////////////////////////////

MLPluginLoader::MLPluginLoader()
{
  node= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLPluginLoader::~MLPluginLoader()
{
  delete node;
  node= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLNode* MLPluginLoader::load( MLAppRoot* approot,
                              const MLString& name,
                              MLNode* data /* = 0 */ )
{
  if( node != 0 )
    return 0; // already loaded

  assert( approot != 0 );

  if( ! MLDynamicLoader::load(approot->getKernel(),name) )
    return 0;


  MLPluginInitFct init_fct= (MLPluginInitFct)
                            dl->getfct(main_plugin_fct_name);

  if( init_fct == 0 )
    {
    errorMessage= MLString("unable to find function ")
                  + MLString(main_plugin_fct_name)
                  + MLString("() in ")
                  + full_lib_filename;

    delete dl;
    dl= 0;
    return 0;
    }

  node= (*init_fct)(approot,approot->getPlugins(),data);

  if( node == 0 )
    {
    errorMessage= MLString("error in initialization function");
    delete dl;
    dl= 0;
    return 0;
    }

  return node;
}

/////////////////////////////////////////////////////////////////////////////

