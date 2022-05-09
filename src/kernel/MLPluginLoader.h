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
  MLPluginLoader.h

  this a dynamic plugin, on the server side

  Stephane Rehel
  March 30 1998
*/

#ifndef __MLPluginLoader_h
#define __MLPluginLoader_h

#include "MLDynamicLoader.h"

class MLAppRoot;
class MLNode;

/////////////////////////////////////////////////////////////////////////////

/**
 * MLPluginLoader takes care for loading and initializing plugins properly.
 */
class MLPluginLoader: public MLDynamicLoader
{
protected:
  MLNode* node;

public:
  MLPluginLoader();

  virtual ~MLPluginLoader();

  /**
   * load the plugin called 'name' from moon_$name.la
   * and initializes it in the context of approot
   */
  MLNode* load( MLAppRoot* approot,
                const MLString& name, MLNode* data = 0 );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLPluginLoader_h
