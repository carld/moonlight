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
  MLProperty.h

  Stephane Rehel
  March 24 1998
*/

#include "tools/xdr/XDRFile.h"

#include "MLProperty.h"
#include "MLPropertyChangeListener.h"
#include "MLNode.h"
#include "MLKernel.h"

#include "MLOperator.h"

/////////////////////////////////////////////////////////////////////////////

void MLProperty::property_constructor( MLNode* _node, const char* _name )
{
  node= 0;

  modified= false;
  op_modified= false;

  id= 0;

  sourceOp= 0;

  kernel= (_node==0) ? 0 : _node->getKernel();
  if( kernel != 0 )
    kernel->prop_id_alloc(this);

  if( _name == 0 || *_name == '\0' )
    name= MLString("prop-") + MLString::valueOf(id);
   else
    name= _name;

  if( _node != 0 )
    _node->attachProperty(this);
}

/////////////////////////////////////////////////////////////////////////////

MLProperty::MLProperty( MLNode* _node, const char* _name )
{
  property_constructor(_node,_name);
}

/////////////////////////////////////////////////////////////////////////////

MLProperty::MLProperty( MLNode* _node )
{
  property_constructor(_node,0);
}

/////////////////////////////////////////////////////////////////////////////

MLProperty::~MLProperty()
{
  // be sure of not beeing delete'd again
  ref(this);

  for( int i= destOps.length(); i > 0; --i )
    removeDestOp( destOps.get(i) );

  unsetSourceOp();

  if( kernel != 0 )
    kernel->prop_free(this);

  detach();

  id= 0;
  kernel= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLProperty::attach( MLNode* n )
{
  if( node == n )
    return;

  detach();

  if( n != 0 )
    n->attachProperty(this);
}

/////////////////////////////////////////////////////////////////////////////

void MLProperty::detach()
{
  if( node != 0 )
    node->detachProperty(this);
}

/////////////////////////////////////////////////////////////////////////////

MLString MLProperty::getLongName() const
{
  if( node == 0 )
    return name;

  return   node->getLongName()
         + MLString::singleChar(MLKernel::nodes_separator)
         + name;
}

/////////////////////////////////////////////////////////////////////////////

// return false if unable to set it
boolean MLProperty::setName( const MLString& _name )
{
  if( _name.length() == 0 )
    return false;

  if( name == _name )
    return true;

  if( node != 0 )
    node->setPropertyName( this, _name );
   else
    name= _name;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLProperty::setSourceOp( MLOperator* op )
{
  if( sourceOp == op )
    return;

  ref(op);

  unsetSourceOp();

  sourceOp= op;

  if( sourceOp != 0 )
    sourceOp->addDestProp(this);
}

/////////////////////////////////////////////////////////////////////////////

void MLProperty::unsetSourceOp()
{
  if( sourceOp == 0 )
    return;

  sourceOp->removeDestProp(this);

  unref(sourceOp);

  sourceOp= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLProperty::addDestOp( MLOperator* op )
{
  if( op == 0 )
    return;

  for( int i= destOps.length(); i > 0; --i )
    if( destOps.get(i) == op )
      return; // already have it

  ref(op);

  destOps.append(op);

  op->addSourceProp(this);
}

/////////////////////////////////////////////////////////////////////////////

void MLProperty::removeDestOp( MLOperator* op )
{
  if( op == 0 )
    return;

  int i;
  for( i= destOps.length(); i > 0; --i )
    if( destOps.get(i) == op )
      break; // found

  if( i < 0 )
    return; // not found

  destOps.remove(i);

  op->removeSourceProp(this);

  unref(op);
}

/////////////////////////////////////////////////////////////////////////////

void MLProperty::dest_ops_to_be_updated()
{
  for( int i= 1; i <= destOps.length(); ++i )
    destOps[i]->to_be_updated();
}

/////////////////////////////////////////////////////////////////////////////

void MLProperty::addListener( MLPropertyChangeListener* l )
{
  if( l == 0 )
    return;

  for( int i= 1; i <= listeners.length(); ++i )
    if( listeners.get(i) == l )
      return;

  listeners.append(l);
}

/////////////////////////////////////////////////////////////////////////////

void MLProperty::removeListener( MLPropertyChangeListener* l )
{
  if( l == 0 )
    return;

  int i;
  for( i= listeners.length(); i > 0; --i )
    if( listeners.get(i) == l )
      break;

  if( i < 0 )
    return;

  listeners.remove(i);
}

/////////////////////////////////////////////////////////////////////////////

void MLProperty::call_listeners()
{
//  for( int i= 1; i <= listeners.length(); ++i )
//    listeners.get(i)->propertyChange(this,data);
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLProperty::serialWrite( XDRFile* file )
{
  if( file == 0 )
    return false;

  if( ! file->putString(name) )
    return false;

  if( ! file->putInteger( int(getType()) ) )
    return false;

  return serialWrite_fromNode(file);
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLProperty::serialWrite_fromNode( XDRFile* file )
{
  if( file == 0 )
    return false;

  int propSize= _serialSize();
  if( ! file->putInteger(propSize) )
    return false;

  boolean outcome= _serialWrite(file);

  return outcome;
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
// static
MLProperty* MLProperty::serialReadNew( XDRFile* file, boolean* unknown )
{
  if( unknown != 0 )
    *unknown= false;

  if( file == 0 )
    return 0;

  MLString _name;
  if( ! file->getString(&_name) )
    return 0;

  int t;
  if( ! file->getInteger(&t) )
    return 0;

  return serialReadNew_fromNode( file, t, _name, unknown );
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
// static
MLProperty* MLProperty::serialReadNew_fromNode( XDRFile* file,
                                                int type,
                                                const MLString& _name,
                                                boolean* unknown )
{
  if( unknown != 0 )
    *unknown= false;

  if( file == 0 )
    return 0;

  int propSize= 0;
  if( ! file->getInteger(&propSize) )
    return 0;

  MLProperty* p= MLPropertyType::newProperty( MLPropertyType::Type(type) );
  if( p == 0 )
    {
    // this type is unknown!
    // let's skip it
    assert( propSize % XDROp::getIntegerSize() == 0 );
    int u= propSize / XDROp::getIntegerSize();
    while( u >= 0 )
      {
      int dummy;
      if( ! file->getInteger(&dummy) )
        return 0;
      --u;
      }
    if( unknown != 0 )
      *unknown= true;
    }
   else
    {
    p->name= _name;
    if( ! p->serialReadProperty(file) )
      return 0;
    }

  return p;
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLProperty::serialReadProperty( XDRFile* file )
{
  if( file == 0 )
    return false;

  if( ! _serialRead(file) )
    return false;

  touch();

#warning should we call the listeners just after reading?
  if( listeners.length() > 0 )
    call_listeners();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLProperty::serialRead( XDRFile* file )
{
  if( file == 0 )
    return false;

  if( ! file->getString(&name) )
    return false;

  int t;
  if( ! file->getInteger(&t) )
    return false;

  if( MLPropertyType::Type(t) != getType() )
    return false;

  int propSize= 0;
  if( ! file->getInteger(&propSize) )
    return false;

  return serialReadProperty(file);
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLProperty::serialRead_fromNode( XDRFile* file )
{
  if( file == 0 )
    return false;

  int propSize= 0;
  if( ! file->getInteger(&propSize) )
    return false;

  return serialReadProperty(file);
}

/////////////////////////////////////////////////////////////////////////////

// return the size (in bytes) that will be read/written by
// serialRead()/serialWrite() on this property
int MLProperty::serialSize() const
{
  int s= 0;

  // name
  s+= XDROp::getIntegerSize() + name.length() * XDROp::getCharSize();

  // type
  s+= XDROp::getIntegerSize();

  // propsize
  s+= XDROp::getIntegerSize();

  s+= _serialSize();

  return s;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLProperty::toString() const
{
  return MLString::printf( "MLProperty@%p", this );
}

/////////////////////////////////////////////////////////////////////////////
