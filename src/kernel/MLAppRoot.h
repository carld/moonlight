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
  MLAppRoot.h

  Stephane Rehel
  April 6 1998
*/

#ifndef __MLAppRoot_h
#define __MLAppRoot_h

#include <stdarg.h>

#include "MLNode.h"

class MLConfig;
class MLNodeType;
class MLProcProfile;
class MLProcArgs;
class MLProcedure;
class MLProcManager;
class MLLog;

/////////////////////////////////////////////////////////////////////////////

class MLAppRoot: public MLNode
{
protected:
  MLNodeType* types;
  MLNode* plugins; // "Plug-ins"
  MLNode* resources; // "Resources"

  MLProcManager* procManager;

  MLLog* log;

public:
  MLAppRoot( MLKernel* kernel );

  virtual ~MLAppRoot();

  virtual MLNodeType::Type getNodeType() const
    {
    return MLNodeType::APP_ROOT;
    }

  virtual boolean init();

  virtual MLLog* getLog() const
    {
    return log;
    }

  virtual MLNode* getPlugins() const
    {
    return plugins;
    }

  MLNodeType* getTypes() const
    {
    return types;
    }

  MLNode* queryPlugin( const MLString& name,
                       MLNode* data = 0,
                       MLString* errorMsg = 0 );

  MLNode* findPlugin( const MLString& name );

  void build_resources( MLConfig* config );

/*
  void addProcedure( MLProcProfile* pp, MLProcedure* proc );
  void addProcedure( MLNodeType::Type t,
                     MLProcProfile* pp, MLProcedure* proc );
*/

  MLProcedure* findProcedure( const MLString& name );
  MLProcedure* findProcedure( MLNodeType::Type t, const MLString& name );
  MLProcedure* findProcedure( MLNode* base_node, const MLString& name );

  void _postProc( MLNode* base_node, const MLString& name, MLProcArgs* args );
  void postProc( MLNode* base_node, const MLString& name, ... );
  void postProc( const MLString& name, ... );
  void procGroupStart();
  void procGroupEnd();

  MLNode* _runFunction( MLNode* base_node, const MLString& name,
                        va_list ap, boolean& error );

  MLNode* runFunction( const MLString& name, ... );
  MLNode* runFunction( MLNode* base_node, const MLString& name, ... );

  boolean runProc( const MLString& name, ... );
  boolean runProc( MLNode* base_node, const MLString& name, ... );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLAppRoot_h
