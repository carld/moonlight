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
  MLPlugin.h

  Stephane Rehel
  April 6 1998
*/

#ifndef __MLPlugin_h
#define __MLPlugin_h

#include "tools/boolean.h"

#include "MLNode.h"

class MLString;
class MLKernel;
class MLAppRoot;

/////////////////////////////////////////////////////////////////////////////

/**
 * represents a plugin in the node tree
 */
class MLPlugin: public MLNode
{
public:
  /** 
   * initializes the plugin for the given approot tree.
   * currently plugins and data are no-ops.
   */
  MLPlugin( MLAppRoot* approot, MLNode* plugins, MLNode* data );

  virtual ~MLPlugin();

  /**
   * contains the name of the plugin, 0 when not set (ie the plugin
   * isn't loaded properly)
   */
  virtual const MLString& getName() const = 0;

  /** initialization, to be implemented by the plugin itself */
  virtual boolean init( MLAppRoot* approot, MLNode* plugins, MLNode* data ) = 0;
};

/////////////////////////////////////////////////////////////////////////////

#define PLUGIN_INIT(PLUGIN_CLASS_NAME) \
extern "C"                                                      \
MLNode* ml_plugin_main( MLAppRoot* approot,                     \
                        MLNode* plugins, MLNode* data )         \
{                                                               \
  MLPlugin* p= new PLUGIN_CLASS_NAME(approot,plugins,data);     \
                                                                \
  const MLString& name= p->getName();                           \
                                                                \
  if( plugins->getChild(name) != 0 )                            \
    return plugins->getChild(name);                             \
                                                                \
  p->setName(name);                                             \
                                                                \
  if( ! p->init( approot, plugins, data ) )                     \
    {                                                           \
    plugins->detachChild(p);                                    \
    return 0;                                                   \
    }                                                           \
                                                                \
  return p;                                                     \
}

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLPlugin_h
