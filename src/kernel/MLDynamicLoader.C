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
  MLDynamicLoader.C

  Stephane Rehel
  March 30 1998
*/

#include <stdlib.h>
#include <unistd.h>

#include <string>
#include <iostream>

#include "tools/MLString.h"
#include "tools/MLDynamicLibrary.h"
#include "tools/MLPath.h"
#include "tools/MLPathList.h"

#include "MLKernel.h"

#include "MLDynamicLoader.h"

/////////////////////////////////////////////////////////////////////////////

MLDynamicLoader::MLDynamicLoader()
{
  dl= 0;
  errorMessage= "";
}

/////////////////////////////////////////////////////////////////////////////

MLDynamicLoader::~MLDynamicLoader()
{
  delete dl; // close it
  dl= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDynamicLoader::load( MLKernel* kernel,
                               const MLString a_filename )
{
  errorMessage= "";
  full_lib_filename= "";

  std::string filename;

  delete dl;
  dl= new MLDynamicLibrary;

  std::string full_path("");

  full_path= kernel->getInstallExecPrefix().get() + std::string("lib/moonlight/");

  if( kernel->verbose() )
    std::cerr << "searching for plug-ins in " << full_path << std::endl;

  filename= full_path + dl->getRealFilename(a_filename.get());

  full_lib_filename= filename.c_str();

  if( ! dl->open(filename) )
    {
    errorMessage= dl->getErrorMessage().c_str();
    delete dl;
    dl= 0;
    return 0;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLDynamicLoader::getErrorMessage()
{
  MLString em= errorMessage;
  errorMessage= "";
  return em;
}

/////////////////////////////////////////////////////////////////////////////
