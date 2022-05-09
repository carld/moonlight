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
  MLKernel.h

  Stephane Rehel
  March 24 1998

  Join us and share free software, you'll be free! -rms
*/

#ifndef __MLKernel_h
#define __MLKernel_h

#include "tools/MLString.h"
#include "tools/MLHashTable.h"

class MLInteger;
class MLNode;
class MLProperty;
class MLConfig;
class MLAppRoot;
class MLCommandLine;
class MLConfig;

/////////////////////////////////////////////////////////////////////////////

class MLKernel
{
  friend class MLNode;
  friend class MLProperty;

public:
  static const char nodes_separator;
  static const char properties_separator;

protected:
  MLNode* root;

  int node_id_counter;
  int prop_id_counter;

  // be able to find a node from its id
  MLHashTable<MLInteger,MLNode> node_id_hash;

  // be able to find a property from its id
  MLHashTable<MLInteger,MLProperty> prop_id_hash;

  MLCommandLine* cmdLine;
  MLConfig* ml_config;
  boolean _verbose;
  MLString install_prefix;
  MLString install_exec_prefix;

public:
  MLKernel();

  virtual ~MLKernel();

  virtual boolean init( int argc, char* argv[] );

protected:
  virtual boolean make_config();

public:
  boolean verbose() const
    {
    return _verbose;
    }

  MLConfig* getConfig() const
    {
    return ml_config;;
    }

  MLCommandLine& getCommandLine();

  // "/usr/local/" for instance
  const MLString& getInstallPrefix() const
    {
    return install_exec_prefix;
    }

  // "/usr/local/linux-gnulibc1" for instance
  const MLString& getInstallExecPrefix() const
    {
    return install_exec_prefix;
    }

  virtual int getNNodes() const
    {
    return node_id_hash.size();
    }

  virtual int getNProperties() const
    {
    return prop_id_hash.size();
    }

private:
  virtual void node_id_alloc( MLNode* n );
  virtual void node_free( MLNode* n );

public:
  virtual MLNode* getNode( int id );
  virtual MLNode* getNode( const MLString& long_name );

private:
  virtual void prop_id_alloc( MLProperty* p );
  virtual void prop_free( MLProperty* p );

public:
  virtual MLProperty* getProperty( int id );
  virtual MLProperty* getProperty( const MLString& long_name );

  virtual MLNode* getRootNode() const
    {
    return root;
    }

  void dump_nodes_ids();
  void dump_props_ids();

  MLAppRoot* loadModule( const MLString& name,
                         MLNode* data /*=0*/,
                         MLString* errorMsg /*= 0*/ );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLKernel_h
