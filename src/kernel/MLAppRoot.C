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
  MLAppRoot.C

  Stephane Rehel
  April 6 1998
*/

#include "tools/MLConfig.h"

#include "MLAppRoot.h"
#include "MLProperty.h"
#include "MLPluginLoader.h"
#include "MLKernel.h"

#include "MLProcedure.h"
#include "MLProcProfile.h"
#include "MLProcArgs.h"
#include "MLProcManager.h"

#include "tools/MLLogPrinter.h"
#include "tools/MLLog.h"

/////////////////////////////////////////////////////////////////////////////

static MLLogPrinter* log_printer= 0;

/////////////////////////////////////////////////////////////////////////////

MLAppRoot::MLAppRoot( MLKernel* kernel ): MLNode(kernel)
{
  types= 0;
  plugins= 0;
  resources= 0;
  procManager= 0;

  kernel->getRootNode()->attachChild(this);

  if( ::log_printer != 0 )
    ::log_printer= new MLLogPrinter();

  log= new MLLog(::log_printer);

  MLNode::approot= this;
}

/////////////////////////////////////////////////////////////////////////////

MLAppRoot::~MLAppRoot()
{
  plugins= 0;
  resources= 0;

  delete procManager;
  procManager= 0;

  delete types;
  types= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLAppRoot::init()
{
  types= new MLNodeType(this);
  if( ! types->init() )
    return false;

  plugins= MLNode::newChild("Plug-ins");
  resources= MLNode::newChild("Resources");

  procManager= new MLProcManager(this);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

MLNode* MLAppRoot::queryPlugin( const MLString& name,
                                MLNode* data /*=0*/,
                                MLString* errorMsg /*= 0*/ )
{
  MLNode* p= findPlugin(name);
  if( p != 0 )
    return p;

  MLPluginLoader* pl= new MLPluginLoader;

  p= pl->load(this,name,data);

  if( errorMsg != 0 )
    *errorMsg= pl->getErrorMessage();

  // we don't delete pl

  return p;
}

/////////////////////////////////////////////////////////////////////////////

MLNode* MLAppRoot::findPlugin( const MLString& name )
{
  return (plugins==0) ? 0 : plugins->getChild(name);
}

/////////////////////////////////////////////////////////////////////////////

void MLAppRoot::build_resources( MLConfig* config )
{
  if( config == 0 || resources == 0 )
    return;

  for( int i= 0; i < config->getNGroups(); ++i )
    {
    const MLConfig::Group* g= config->getGroup(i);
    if( g == 0 )
      continue;

    MLNode* group_node= resources->newChild(g->name.get());

    ConstSIListIterator<MLConfig::Variable> li(g->vars);
    while( ! li.eol() )
      {
      const MLConfig::Variable* v= li.next();

      group_node->newString(v->name,v->value);
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

/*
void MLAppRoot::addProcedure( MLProcedure* proc )
{
  addProcedure( MLNodeType::APP_ROOT, pp, proc );
}
*/

/////////////////////////////////////////////////////////////////////////////

/*
void MLAppRoot::addProcedure( MLNodeType::Type t,
                              MLProcProfile* pp, MLProcedure* proc )
{
  MLNode* procedures= types->getProcedures(t);
  if( procedures == 0 )
    return;

  assert( pp->nodeOf(MLNodeType::PROC_PROFILE) );
  assert( proc->nodeOf(MLNodeType::PROCEDURE) );

  pp->newNodeRef( "*proc_body", proc );

  procedures->attachChild(pp);
}
*/

/////////////////////////////////////////////////////////////////////////////

MLProcedure* MLAppRoot::findProcedure( const MLString& name )
{
  return findProcedure( MLNodeType::APP_ROOT, name );
}

/////////////////////////////////////////////////////////////////////////////

MLProcedure* MLAppRoot::findProcedure( MLNodeType::Type t,
                                       const MLString& name )
{
  return types->findProcedure(t,name);
}

/////////////////////////////////////////////////////////////////////////////

MLProcedure* MLAppRoot::findProcedure( MLNode* base_node,
                                       const MLString& name )
{
  if( base_node == 0 )
    base_node= this;

  MLNodeType::Type t= base_node->getNodeType();

  return findProcedure( t, name );
}

/////////////////////////////////////////////////////////////////////////////

void MLAppRoot::_postProc( MLNode* base_node,
                           const MLString& name, MLProcArgs* args )
{
  if( procManager == 0 )
    return;

  procManager->post( findProcedure(base_node,name), args );
}

/////////////////////////////////////////////////////////////////////////////

void MLAppRoot::postProc( const MLString& name, ... )
{
  if( procManager == 0 )
    return;

  MLProcedure* procedure= findProcedure(name);
  if( procedure == 0 )
    return;

  va_list ap;
  va_start(ap,&name);

  MLProcArgs* args= MLProcArgs::create(procedure,ap);
  args->setBaseNode(this);

  va_end(ap);

  procManager->post( procedure, args );
}

/////////////////////////////////////////////////////////////////////////////

void MLAppRoot::postProc( MLNode* base_node, const MLString& name, ... )
{
  if( procManager == 0 )
    return;

  MLProcedure* procedure= findProcedure(base_node,name);
  if( procedure == 0 )
    return;

  va_list ap;
  va_start(ap,&name);

  MLProcArgs* args= MLProcArgs::create(procedure,ap);
  args->setBaseNode(base_node);

  va_end(ap);

  procManager->post( procedure, args );
}

/////////////////////////////////////////////////////////////////////////////

void MLAppRoot::procGroupStart()
{
  if( procManager == 0 )
    return;

  procManager->groupStart();
}

/////////////////////////////////////////////////////////////////////////////

void MLAppRoot::procGroupEnd()
{
  if( procManager == 0 )
    return;

  procManager->groupEnd();
}

/////////////////////////////////////////////////////////////////////////////

MLNode* MLAppRoot::_runFunction( MLNode* base_node,
                                 const MLString& name,
                                 va_list ap,
                                 boolean& error )
{
  if( base_node == 0 )
    base_node= this;

  MLNodeType::Type t= base_node->getNodeType();

  MLProcedure* procedure= findProcedure(t,name);
  if( procedure == 0 )
    {
    error= true;
    return 0;
    }

  MLProcArgs* args= MLProcArgs::create(procedure,ap);
  if( args == 0 )
    {
    error= true;
    return 0;
    }
  ref(args);
  args->setBaseNode(base_node);

  MLNode* result= procedure->run(args);
  ref(result);

  unref(args);

  error= false;

  return result;
}

/////////////////////////////////////////////////////////////////////////////

MLNode* MLAppRoot::runFunction( const MLString& name, ... )
{
  boolean error= false;

  va_list ap;
  va_start(ap,&name);

  MLNode* result= _runFunction(this,name,ap,error);

  va_end(ap);

  if( error )
    {
    unref(result);
    return 0;
    }

  return result;
}

/////////////////////////////////////////////////////////////////////////////

MLNode* MLAppRoot::runFunction( MLNode* base_node,
                                const MLString& name, ... )
{
  boolean error= false;

  va_list ap;
  va_start(ap,&name);

  MLNode* result= _runFunction(base_node,name,ap,error);

  va_end(ap);

  if( error )
    {
    unref(result);
    return 0;
    }

  return result;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLAppRoot::runProc( const MLString& name, ... )
{
  boolean error= false;

  va_list ap;
  va_start(ap,&name);

  MLNode* result= _runFunction(this,name,ap,error);

  va_end(ap);

  unref(result);

  return error;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLAppRoot::runProc( MLNode* base_node,
                            const MLString& name, ... )
{
  boolean error= false;

  va_list ap;
  va_start(ap,&name);

  MLNode* result= _runFunction(base_node,name,ap,error);

  va_end(ap);

  unref(result);

  return error;
}

/////////////////////////////////////////////////////////////////////////////
