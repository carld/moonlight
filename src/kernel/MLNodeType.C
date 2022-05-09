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
  MLNodeType.C

  Stephane Rehel

  April 6 1998
*/

#include "tools/MLArray.h"

#include "MLNodeType.h"

#include "MLAppRoot.h"

/////////////////////////////////////////////////////////////////////////////

MLNodeType::MLNodeType( MLAppRoot* _approot )
{
  approot= _approot;
}

/////////////////////////////////////////////////////////////////////////////

MLNodeType::~MLNodeType()
{
  for( int i= types.length(); i > 0; --i )
    {
    NodeType* nt= types.get(i);
    types.remove(i);
    delete nt;
    }

  approot= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLNodeType::init()
{
  newType( NULL_TYPE, "NULL", 0 );
  newType( NULL_TYPE, "Default", 0 );
  newType( "AppRoot", 0 );
  newType( "Procedure", 0 );
  newType( "ProcProfile", 0 );
  newType( "Operator", 0 );

  return true;
}

/////////////////////////////////////////////////////////////////////////////

const MLString& MLNodeType::getName( Type t ) const
{
  int i= int(t)+1;
  if( i < 1 || i > types.length() )
    return MLString::empty();

  return types.get(i)->name;
}

/////////////////////////////////////////////////////////////////////////////

MLNodeType::Type MLNodeType::newType( const MLString& name,
                                      NodeCreator creator,
                                      void* data /* = 0 */ )
{
  return newType( DEFAULT, name, creator, data );
}

/////////////////////////////////////////////////////////////////////////////

MLNodeType::Type MLNodeType::newType( MLNodeType::Type super_type,
                                      const MLString& name,
                                      NodeCreator creator,
                                      void* data /* = 0 */ )
{
  NodeType* super_nt= 0;

  if( super_type != NULL_TYPE )
    {
    super_nt= getNodeType(super_type);
    if( super_nt == 0 )
      return NULL_TYPE;
    }

  Type i= getType(name);

  if( i != NULL_TYPE )
    return i;

  NodeType* nt= new NodeType;
  nt->super_type= super_type;
  nt->name= name;
  nt->creator= creator;
  nt->data= data;
  nt->procedures= MLNode::newNode(approot);

  Type this_type= Type(types.length());

  if( super_nt != 0 )
    super_nt->sub_types.append( this_type );

  types.append(nt);

  return this_type;
}

/////////////////////////////////////////////////////////////////////////////

MLNodeType::Type MLNodeType::getType( const MLString& name ) const
{
  for( int i= 1; i <= types.length(); ++i )
    if( types.get(i)->name == name )
      return Type(i-1);

  return NULL_TYPE;
}

/////////////////////////////////////////////////////////////////////////////

MLNode* MLNodeType::newNode( Type t, MLNode* parent )
{
  int i= int(t) + 1;
  if( i < 1 || i > types.length() )
    return 0;

  NodeType* nt= types.get(i);
  if( nt == 0 )
    return 0;

  if( nt->creator == 0 )
    return 0;

  assert( parent->getAppRoot() == approot );
  return (*nt->creator)( approot, parent );
}

/////////////////////////////////////////////////////////////////////////////

// static
MLNode* MLNodeType::newBasicNode( Type t, MLNode* parent )
{
#warning newBasicNode: fixme
  assert(0);
  return 0;
}

/////////////////////////////////////////////////////////////////////////////

MLNodeType::NodeType* MLNodeType::getNodeType( Type t ) const
{
  int i= int(t) + 1;
  if( i < 1 || i > types.length() )
    return 0;

  return types.get(i);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLNodeType::nodeOf( Type t, Type super_to_test ) const
{
  if( t == super_to_test )
    return true;

  NodeType* nt= getNodeType(t);
  if( nt == 0 )
    return false;

  for(;;)
    {
    if( nt->super_type == int(NULL_TYPE) )
      break;

    if( nt->super_type == super_to_test )
      return true;

    nt= getNodeType(Type(nt->super_type));
    if( nt == 0 )
      break;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

MLNode* MLNodeType::getProcedures( Type t ) const
{
  NodeType* nt= getNodeType(t);
  if( nt == 0 )
    nt= getNodeType(APP_ROOT);

  if( nt == 0 )
    return 0;

  return nt->procedures;
}

/////////////////////////////////////////////////////////////////////////////

MLProcedure* MLNodeType::findProcedure( MLNodeType::Type t,
                                        const MLString& name )
{
  NodeType* nt= getNodeType(t);
  if( nt == 0 )
    return 0;

  for(;;)
    {
    MLNode* n= nt->procedures->getChild(name);
    if( n != 0 )
      {
      assert( n->nodeOf(MLNodeType::PROCEDURE) );
      return (MLProcedure*) n;
      }

    nt= getNodeType( Type(nt->super_type) );
    if( nt == 0 )
      break;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////
