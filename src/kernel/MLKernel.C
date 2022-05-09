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
  MLKernel.C

  Stephane Rehel
  March 24 1998

  Join us and share free software, you'll be free! -rms
*/

#include <stdio.h>
#include <assert.h>

#include "tools/MLCommandLine.h"
#include "tools/MLConfig.h"
#include "tools/MLInteger.h"
#include "tools/MLHashTableEnumerator.h"

#include "MLKernel.h"
#include "MLNode.h"
#include "MLPString.h"
#include "MLModuleLoader.h"
#include "MLAppRoot.h"

/////////////////////////////////////////////////////////////////////////////

// static
const char MLKernel::nodes_separator= '.';
const char MLKernel::properties_separator= ':';

/////////////////////////////////////////////////////////////////////////////

MLKernel::MLKernel()
{
  node_id_counter= 0;
  prop_id_counter= 0;

  root= 0;

  cmdLine= 0;
  ml_config= 0;
  _verbose= false;
  install_prefix= "/usr/local/";
  install_exec_prefix= "/usr/local/";
}

/////////////////////////////////////////////////////////////////////////////

MLKernel::~MLKernel()
{
  delete root;
  root= 0;

  MLHashTableEnumerator<MLInteger,MLNode> eni(node_id_hash);
  while( eni.hasMoreElements() )
    {
    MLInteger* i= eni.nextKey();
    delete i;
    }

  MLHashTableEnumerator<MLInteger,MLProperty> ep(prop_id_hash);
  while( ep.hasMoreElements() )
    {
    MLInteger* i= ep.nextKey();
    delete i;
    }

  delete cmdLine;
  cmdLine= 0;

  delete ml_config;
  ml_config= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLKernel::init( int argc, char* argv[] )
{
  if( root != 0 )
    return true; // already initialized!

  cmdLine= new MLCommandLine(argc,(const char**)argv);

  if( ! make_config() )
    return false;

  root= new MLNode(this);
  root->p_name->set("root");
  ref(root);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

MLCommandLine& MLKernel::getCommandLine()
{
  assert( cmdLine != 0 );
  return *cmdLine;
}

/////////////////////////////////////////////////////////////////////////////

void MLKernel::node_id_alloc( MLNode* n )
{
  if( n == 0 )
    return; // silly boy

  if( n->id != 0 )
    return; // already allocated

  ++node_id_counter;

  MLInteger* id= new MLInteger(node_id_counter);
  if( node_id_hash.put( id, n ) )
    {
    delete id;
    id= 0;
    }

  n->id= node_id_counter;
}

/////////////////////////////////////////////////////////////////////////////

void MLKernel::node_free( MLNode* n )
{
  if( n == 0 )
    return;

  MLInteger idi(n->id);

  // ok, let's free the id from the id hash table
  delete node_id_hash.remove( &idi );
}

/////////////////////////////////////////////////////////////////////////////

MLNode* MLKernel::getNode( int id )
{
  if( id <= 0 )
    return 0;

  MLInteger idi(id);

  return node_id_hash.get( &idi );
}

/////////////////////////////////////////////////////////////////////////////

MLNode* MLKernel::getNode( const MLString& long_name )
{
  if( root == 0 )
    return 0;

  int i1= 0;

  MLNode* base= root;

  for(;;)
    {
    if( i1 >= long_name.length() )
      break;
    int i2= long_name.indexOf( nodes_separator, i1 );
    if( i2 == -1 )
      i2= long_name.length();
    --i2;

    if( i2 < i1 )
      break;

    MLString node_name= MLString( long_name.get(), i1, i2-i1+1 );
    MLNode* child= base->getChild(node_name);
    if( child == 0 )
      return 0;

    i1= i2 + 1;
    base= child;
    }

  return base;
}

/////////////////////////////////////////////////////////////////////////////

void MLKernel::prop_id_alloc( MLProperty* p )
{
  if( p == 0 )
    return; // silly boy

  if( p->id != 0 )
    return; // already allocated

  ++prop_id_counter;

  MLInteger* id= new MLInteger(prop_id_counter);

  if( prop_id_hash.put( id, p ) )
    {
    delete id;
    id= 0;
    }

  p->id= prop_id_counter;
}

/////////////////////////////////////////////////////////////////////////////

void MLKernel::prop_free( MLProperty* p )
{
  if( p == 0 )
    return;

  MLInteger idi(p->id);

  // ok, let's free the id from the id hash table
  delete prop_id_hash.remove( &idi );
}

/////////////////////////////////////////////////////////////////////////////

MLProperty* MLKernel::getProperty( int id )
{
  if( id <= 0 )
    return 0;

  MLInteger idi(id);

  return prop_id_hash.get( &idi );
}

/////////////////////////////////////////////////////////////////////////////

MLProperty* MLKernel::getProperty( const MLString& long_name )
{
  int i= long_name.indexOf(properties_separator);
  if( i == -1 )
    return 0;

  MLNode* node= getNode( long_name.substring( 0, i - 1 ) );

  if( node == 0 )
    return 0;

  MLString prop_name= long_name.substring( i + 1, long_name.length()-1 );

  return node->getProperty(prop_name);
}

/////////////////////////////////////////////////////////////////////////////

void MLKernel::dump_nodes_ids()
{
  printf( "Nodes Hash Table by ids (%d elements):\n",
          node_id_hash.size() );

  MLHashTableEnumerator<MLInteger,MLNode> ei(node_id_hash);
  MLHashTableEnumerator<MLInteger,MLNode> en(node_id_hash);
  while( ei.hasMoreElements() )
    {
    MLInteger* i= ei.nextKey();
    MLNode* n= en.nextElement();

    printf( "node #%d name=\"%s\", parent(id=%d)\n",
            i->intValue(), n->getName().get(),
            (n->getParentNode()==0) ? 0 : n->getParentNode()->getID()
          );
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLKernel::dump_props_ids()
{
  printf( "Properties Hash Table by ids (%d elements):\n",
          prop_id_hash.size() );

  MLHashTableEnumerator<MLInteger,MLProperty> ei(prop_id_hash);
  MLHashTableEnumerator<MLInteger,MLProperty> en(prop_id_hash);
  while( ei.hasMoreElements() )
    {
    MLInteger* i= ei.nextKey();
    MLProperty* p= en.nextElement();

    printf( "prop_id=%d name=\"%s\"\n",
            i->intValue(), p->getName().get() );
    }
}

/////////////////////////////////////////////////////////////////////////////

MLAppRoot* MLKernel::loadModule( const MLString& name,
                                 MLNode* data /*=0*/,
                                 MLString* errorMsg /*= 0*/ )
{
  MLNode* p= root->getChild(name);
  if( p != 0 )
    {
    if( ! p->nodeOf(MLNodeType::APP_ROOT) )
      return 0;

    return (MLAppRoot*) p;
    }

  MLModuleLoader* ml= new MLModuleLoader;

  MLAppRoot* ar= ml->load(this,name,data);

  if( errorMsg != 0 )
    *errorMsg= ml->getErrorMessage();

  // we don't delete pl

  if( ar != 0 )
    {
    if( ! ar->nodeOf(MLNodeType::APP_ROOT) )
      {
      fprintf( stderr,
               "Warning: module plug-in '%s' is not an AppRoot (type=%d)\n",
               name.get(), int(ar->getNodeType()) );
      }
    }

  return ar;
}

/////////////////////////////////////////////////////////////////////////////
