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
  MLModuleLoader.h

  Stephane Rehel
  April 21 1998
*/

#ifndef __MLModuleLoader_h
#define __MLModuleLoader_h

#include "MLDynamicLoader.h"

class MLKernel;
class MLAppRoot;

/////////////////////////////////////////////////////////////////////////////

class MLModuleLoader: public MLDynamicLoader
{
protected:
  MLAppRoot* approot;

public:
  MLModuleLoader();

  virtual ~MLModuleLoader();

  /**
   * loads the shared object specified in libmoon_$name.la and calls
   * ml_module_main in there to init the module
   */
  MLAppRoot* load( MLKernel* kernel,
                   const MLString& name, MLNode* data = 0 );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLModuleLoader_h
