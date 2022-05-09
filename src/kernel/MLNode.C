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
  MLNode.C

  Stephane Rehel
  March 24 1998
*/

#include <stdio.h>

#include "tools/xdr/XDRFile.h"

#include "MLNode.h"

#include "MLProperty.h"
#include "MLPBoolean.h"
#include "MLPInteger.h"
#include "MLPReal.h"
#include "MLPString.h"
#include "MLPVector.h"
#include "MLPPoint.h"
#include "MLPColor.h"
#include "MLPNodeRef.h"

#include "MLOperator.h"

#include "MLKernel.h"
#include "MLAppRoot.h"

static const char* name_property_name= "*name";

/////////////////////////////////////////////////////////////////////////////

void MLNode::MLNode_constructor( MLKernel* _kernel,
                                 MLAppRoot* _approot,
                                 MLNode* _parent,
                                 const MLString& name )
{
  parent= _parent;
  id= 0;
  p_name= 0;

  if( parent != 0 )
    {
    kernel= parent->kernel;
    approot= parent->approot;
    }
   else
    {
    approot= _approot;
    kernel= (approot==0) ? _kernel : approot->getKernel();
    }

  if( kernel != 0 )
    kernel->node_id_alloc(this);

  // set a default name
  MLString def_name= (name.length()==0)
                     ? ((MLString("node-") + MLString::valueOf(id)))
                     : name;

  assert( parent==0 || parent->getChild(def_name) == 0 );

  p_name= new MLPString(this,name_property_name);
  *p_name= def_name;

  if( parent != 0 )
    parent->children.append(this);
}

/////////////////////////////////////////////////////////////////////////////

MLNode::MLNode( MLNode* _parent, const MLString& name ):
  child_name_hash(10,0.75),
  prop_name_hash(10,0.75)
{
  MLNode_constructor( (_parent==0) ? 0 : _parent->getKernel(),
                      (_parent==0) ? 0 : _parent->getAppRoot(),
                      _parent,
                      name );
}

/////////////////////////////////////////////////////////////////////////////

MLNode::MLNode( MLKernel* _kernel ): child_name_hash(10,0.75),
                                     prop_name_hash(10,0.75)
{
  MLNode_constructor( _kernel, 0, 0, "" );
}

/////////////////////////////////////////////////////////////////////////////

MLNode::MLNode( MLNode* _parent ):
  child_name_hash(10,0.75),
  prop_name_hash(10,0.75)
{
  MLNode_constructor( 0, 0, _parent, "" );
}

/////////////////////////////////////////////////////////////////////////////

MLNode::MLNode( MLAppRoot* _approot, MLNode* _parent /* = 0 */ ):
  child_name_hash(10,0.75),
  prop_name_hash(10,0.75)
{
  MLNode_constructor( (_approot==0) ? 0 : _approot->getKernel(),
                      _approot,
                      _parent,
                      "" );
}

/////////////////////////////////////////////////////////////////////////////

MLNode::~MLNode()
{
  int i;

  for( i= props.length(); i > 0; --i )
    {
    MLProperty* p= props.get(i);
    if( p != p_name )
      detachProperty(p);
    }

  for( i= children.length(); i > 0; --i )
    {
    MLNode* child= children[i];
    if( child == 0 )
      continue;
    delete child; // this will call detachChild()
    }

  if( parent != 0 )
    parent->detachChild(this);

  assert( parent == 0 );

  detachProperty(p_name);
  p_name= 0;

  if( kernel != 0 )
    kernel->node_free(this);

  approot= 0;
  kernel= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLNode::nodeOf( MLNodeType::Type t ) const
{
  MLNodeType::Type node_type= getNodeType();

  if( approot == 0 )
    {
    if( t == MLNodeType::DEFAULT || t == node_type )
      return true;
    return false;
    }

  return approot->getTypes()->nodeOf(node_type,t);
}

/////////////////////////////////////////////////////////////////////////////

void MLNode::child_name_hash_add( const MLString& name, MLNode* child )
{
  if( name.length() == 0 )
    return;

  MLString* new_name= new MLString(name);
  if( child_name_hash.put( new_name, child ) )
    {
    delete new_name;
    new_name= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLNode::child_name_hash_remove( const MLString& name )
{
  if( name.length() == 0 )
    return;

  delete child_name_hash.remove(&name);
}

/////////////////////////////////////////////////////////////////////////////

void MLNode::attachChild( MLNode* child )
{
  if( child == 0 || child == this )
    return;

  child->parent= this;

  if( child->kernel == 0 )
    child->kernel= kernel;
  if( child->approot == 0 && approot != 0 )
    child->approot= approot;

  if( id == 0 && kernel != 0 )
    kernel->node_id_alloc(this);

  for( int i= children.length(); i > 0; --i )
    if( children.get(i) == child )
      return;

  children.append(child);
  child_name_hash_add(child->getName(),child);
}

/////////////////////////////////////////////////////////////////////////////

void MLNode::attachToParent( MLNode* _parent )
{
  if( _parent == 0 )
    return;

  if( _parent == parent )
    return;

  _parent->attachChild(this);
}

/////////////////////////////////////////////////////////////////////////////

void MLNode::detachChild( MLNode* child )
{
  if( child == 0 )
    return;

  int i;
  for( i= children.length(); i > 0; --i )
    if( children.get(i) == child )
      break;

  if( i < 0 )
    return;

  children.remove(i);
  child_name_hash_remove(child->getName());

  child->parent= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLNode::detachFromParent()
{
  if( parent == 0 )
    return;

  parent->detachChild(this);
}

/////////////////////////////////////////////////////////////////////////////

MLNode* MLNode::newChild( const MLString& name )
{
  if( name.length() > 0 )
    {
    if( getChild(name) != 0 )
      return 0;
    }

  return new MLNode(this,name);
}

/////////////////////////////////////////////////////////////////////////////

MLString MLNode::getName() const
{
  return (p_name==0) ? "" : p_name->get();
}

/////////////////////////////////////////////////////////////////////////////

// return false if unable to set it
boolean MLNode::setName( const MLString& name )
{
  if( p_name == 0 )
    return false;

  if( name.length() == 0 )
    return false;

  const MLString& prev_name= p_name->get();
  if( prev_name == name )
    return true;

  if( parent != 0 )
    {
    if( parent->getChild(name) )
      return false;
    }

  if( parent != 0 )
    parent->child_name_hash_remove(prev_name);

  *p_name= name;

  if( parent != 0 )
    parent->child_name_hash_add(name,this);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return "Scene.robot.head" for instance
MLString MLNode::getLongName() const
{
  if( parent == 0 )
    return "";

  if( parent->parent == 0 )
    return p_name->get();

  return   parent->getLongName()
         + MLString::singleChar(MLKernel::properties_separator)
         + p_name->get();
}

/////////////////////////////////////////////////////////////////////////////

void MLNode::prop_name_hash_add( const MLString& name, MLProperty* p )
{
  if( name.length() == 0 )
    return;

  MLString* new_name= new MLString(name);

  if( prop_name_hash.put( new_name, p ) )
    {
    delete new_name;
    new_name= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLNode::prop_name_hash_remove( const MLString& name )
{
  if( name.length() == 0 )
    return;

  delete prop_name_hash.remove(&name);
}

/////////////////////////////////////////////////////////////////////////////

// called by MLProperty::MLProperty();
void MLNode::attachProperty( MLProperty* p )
{
  if( p == 0 )
    return;

  if( p->node == this )
    return; // already have it

  if( p->node != 0 )
    p->detach();

  if( p->id == 0 && kernel != 0 )
    {
    // this node id has not been allocated
    kernel->prop_id_alloc(p);
    }

  ref(p);

  MLString& name= p->name;
  boolean is_name_prop= (name == MLString(name_property_name));
  if( is_name_prop )
    {
    detachProperty(p_name);
    p_name= 0;
    }
   else
    {
    if( getProperty(name) != 0 )
      {
      unref(p);
      return; // already have a prop with such a name.
      }
    }

  props.append(p);
  prop_name_hash_add( name, p );

  p->node= this;

  if( p->kernel == 0 )
    p->kernel= kernel;

  if( is_name_prop )//&& p->getType() == MLPropertyType::STRING )
    p_name= (MLPString*) p;
}

/////////////////////////////////////////////////////////////////////////////

void MLNode::detachProperty( MLProperty* p )
{
  if( p == 0 )
    return;

  int i;
  for( i= props.length(); i > 0; --i )
    if( props.get(i) == p )
      break; // found it

  if( i < 0 )
    return; // not found

  p->node= 0;

  props.remove(i);
  prop_name_hash_remove(p->name);

  if( p == p_name )
    p_name= 0;

  unref(p);
}

/////////////////////////////////////////////////////////////////////////////

// return false if unable to set it
boolean MLNode::setPropertyName( MLProperty* p, const MLString& name )
{
  if( p == 0 )
    return false;

  if( name.length() == 0 )
    return false;

  if( p->node != this )
    return false;

  if( p->name == name )
    return true;

  if( getProperty(name) != 0 )
    return false;

  prop_name_hash_remove(p->name);
  p->name= name;
  prop_name_hash_add(name,p);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// update animated properties according to this frame number
void MLNode::setFrame( int f )
{
  for( int i= props.length(); i > 0; --i )
    {
    MLProperty* p= props.get(i);

    p->setFrame(f);
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLNode::print( int indent )
{
  MLString indent_string(indent);

  printf( "%s+%s\n", indent_string.get(),
                     (p_name==0) ? "?" : p_name->get().get() );

  indent += 2;
  indent_string += MLString("  ");

  int i;
  for( i= 1; i <= props.length(); ++i )
    {
    MLProperty* p= props[i];
    if( p == 0 )
      continue;

    printf( "%s%s=", indent_string.get(), p->getName().get() );

    switch( p->getType() )
      {
      case MLPropertyType::NULL_TYPE:
        printf( "NULL" );
        break;
      case MLPropertyType::BOOLEAN:
        printf( (((MLPBoolean*)p)->get()) ? "true" : "false" );
        break;
      case MLPropertyType::INTEGER:
        printf( "%d", (((MLPInteger*)p)->get()) );
        break;
      case MLPropertyType::REAL:
        printf( "%g", (((MLPReal*)p)->get()) );
        break;
      case MLPropertyType::VECTOR:
        printf( "(%g %g %g)",
                (((MLPVector*)p)->get()).x(),
                (((MLPVector*)p)->get()).y(),
                (((MLPVector*)p)->get()).z() );
        break;
      case MLPropertyType::POINT:
        printf( "(%g %g %g)",
                (((MLPPoint*)p)->get()).x(),
                (((MLPPoint*)p)->get()).y(),
                (((MLPPoint*)p)->get()).z() );
        break;
      case MLPropertyType::COLOR:
        printf( "<%g %g %g>",
                (((MLPColor*)p)->get()).r(),
                (((MLPColor*)p)->get()).g(),
                (((MLPColor*)p)->get()).b() );
        break;
      case MLPropertyType::STRING:
        printf( "\"%s\"", (((MLPString*)p)->get()).get() );
        break;
      default:
        printf( "<#%d not_impl>", p->getType() );
        break;
      }

    printf( "\n" );
    }

  for( i= 1; i <= children.length(); ++i )
    {
    MLNode* n= children[i];
    if( n == 0 )
      continue;

    n->print(indent);
    }
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLNode::serialWrite( XDRFile* file )
{
  if( file == 0 || p_name == 0 )
    return false;

  lock();

  if( ! file->putInteger( int(getNodeType()) ) )
    {
    unlock();
    return false;
    }

  if( ! file->putInteger(props.length()) )
    {
    unlock();
    return false;
    }

  // first send the node name
  if( ! p_name->serialWrite(file) )
    {
    unlock();
    return false;
    }

  int i;

  // just write their properties
  for( i= 1; i <= props.length(); ++i )
    {
    MLProperty* p= props[i];
    if( p != p_name )
      {
      if( ! p->serialWrite(file) )
        {
        unlock();
        return false;
        }
      }
    }

  // let's walk through children!
  if( ! file->putInteger(children.length()) )
    {
    unlock();
    return false;
    }

  for( i= 1; i <= children.length(); ++i )
    {
    MLNode* child= children[i];
    if( ! child->serialWrite(file) )
      {
      unlock();
      return false;
      }
    }

  unlock();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLNode* MLNode::nodeSerialRead( MLAppRoot* approot,
                                MLNode* parent,
                                XDRFile* file )
{
  if( file == 0 )
    return 0;

  int node_type;
  if( ! file->getInteger(&node_type) )
    return 0;

  MLNode* node= 0;

  if( node_type > int(MLNodeType::NULL_TYPE) &&
      node_type < int(MLNodeType::LAST_TYPE) )
    node= MLNodeType::newBasicNode( MLNodeType::Type(node_type), parent );
   else
    {
    if( approot == 0 )
      return 0;

    node= approot->getTypes()->newNode( MLNodeType::Type(node_type), parent );
    }

  if( node == 0 )
    return 0;

  if( ! node->_serialRead(file) )
    {
    unref(node);
    return 0;
    }

  return node;
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLNode::_serialRead( XDRFile* file )
{
  if( file == 0 )
    return false;

  lock();

  int i;

  int n_props= 0;
  if( ! file->getInteger(&n_props) )
    {
    unlock();
    return false;
    }

  for( i= 1; i <= n_props; ++i )
    {
    MLString name;
    if( ! file->getString(&name) )
      break;
    int type;
    if( ! file->getInteger(&type) )
      break;

    MLProperty* p= getProperty(name);
    if( p != 0 )
      {
      if( int(p->getType()) != type )
        {
        p->detach();
        p= 0;
        }
      }

    if( p == 0 )
      {
      boolean unknown_type= false;
      MLProperty* p= MLProperty::serialReadNew(file,&unknown_type);

      if( p == 0 )
        {
        if( !unknown_type )
          {
          // error wihle reading
          unlock();
          return false;
          }

        // this property type is unknown!!
        // let's remove it and skip it...
        delete p;
        continue;
        }
      attachProperty(p);
      continue;
      }

    // this property already exists and is of the same type
    if( ! p->serialRead_fromNode(file) )
      break;
    }

  if( i < n_props )
    {
    // an error occured
    unlock();
    return false;
    }

  // let's read then children!
  int nchildren;
  if( ! file->putInteger(nchildren) )
    {
    unlock();
    return false;
    }

  for( i= 1; i <= nchildren; ++i )
    {
    MLNode* child= MLNode::nodeSerialRead(approot,this,file);
    if( child == 0 )
      {
      unlock();
      return false;
      }
    children.append(child);
    }

  if( ! serialReadFinalize() )
    {
    unlock();
    return false;
    }

  unlock();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLNode::serialRead( XDRFile* file )
{
  if( file == 0 )
    return false;

  // let's check the node type
  int node_type;
  if( ! file->getInteger(&node_type) )
    return false;

  if( MLNodeType::Type(node_type) != getNodeType() )
    return false; // type don't match

  return _serialRead(file);
}

/////////////////////////////////////////////////////////////////////////////

// return the size (in bytes) that will be read/written by
// serialRead()/serialWrite() on this property
int MLNode::serialSize() const
{
  int s= 0;

  // node type
  s+= XDROp::getIntegerSize();

  // props length
  s+= XDROp::getIntegerSize();

  int i;

  // just write their properties
  for( i= 1; i <= props.length(); ++i )
    {
    MLProperty* p= props.get(i);
    if( p == 0 )
      continue;

    s+= p->serialSize();
    }

  // children length
  s+= XDROp::getIntegerSize();

  for( i= 0; i < children.length(); ++i )
    {
    MLNode* child= children.get(i);

    s+= child->serialSize();
    }

  return s;
}

/////////////////////////////////////////////////////////////////////////////

// only clone properties
MLNode* MLNode::clone() const
{
  MLNode* node= 0;

  int node_type= int(getNodeType());

  // We don't set parent node, since the cloned node will have the same
  // name as the current node. They can't be children of the same parent

  if( node_type > int(MLNodeType::NULL_TYPE) &&
      node_type < int(MLNodeType::LAST_TYPE) )
    node= MLNodeType::newBasicNode( MLNodeType::Type(node_type), 0 );
   else
    {
    if( approot == 0 )
      return 0;

    node= approot->getTypes()->newNode( MLNodeType::Type(node_type), 0 );
    }

  if( node == 0 )
    return 0;

  for( int i= 1; i <= getNProperties(); ++i )
    {
    MLProperty* p= getProperty(i);
    MLProperty* p0= node->getProperty(p->getName());
    if( p0 != 0 )
      p->copy(p0); // copy p in p0
     else
      node->attachProperty( p->clone() );
    }

  return node;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLNode::toString() const
{
  MLString s= getName() + MLString(" { ");

  for( int i= 1; i <= getNProperties(); ++i )
    {
    MLProperty* p= getProperty(i);
    s += p->toString() + MLString("; ");
    }

  return s + MLString("}");
}

/////////////////////////////////////////////////////////////////////////////

#define new_prop(method_name,P,type) \
  P* MLNode::method_name( const MLString& name, type value ) \
  { \
    P* p= new P(this,name.get()); \
    p->set(value); \
    return p; \
  }
new_prop( newBoolean, MLPBoolean, boolean )
new_prop( newInteger, MLPInteger, int )
new_prop( newReal, MLPReal, double )
new_prop( newVector, MLPVector, const Vector& )
new_prop( newPoint, MLPPoint, const Point& )
new_prop( newColor, MLPColor, const Color& )
new_prop( newString, MLPString, const MLString& )
new_prop( newNodeRef, MLPNodeRef, MLNode* )
#undef new_prop

/////////////////////////////////////////////////////////////////////////////

#define get_prop(method_name,type,typer,PTYPE,P) \
  type MLNode::method_name( const MLString& name, typer default_value ) \
  {                                                                     \
    MLProperty* p= getProperty(name);                                   \
    if( p == 0 )                                                        \
      return default_value;                                             \
    if( p->getType() != MLPropertyType::PTYPE )                         \
      return default_value;                                             \
    return ((P*)p)->get();                                              \
  }
get_prop( getBoolean, boolean, boolean, BOOLEAN, MLPBoolean );
get_prop( getInteger, int, int, INTEGER, MLPInteger );
get_prop( getReal, double, double, REAL, MLPReal );
get_prop( getVector, Vector, const Vector&, VECTOR, MLPVector );
get_prop( getPoint, Point, const Point&, POINT, MLPPoint );
get_prop( getColor, Color, const Color&, COLOR, MLPColor );
get_prop( getString, MLString, const MLString&, STRING, MLPString );
get_prop( getNodeRef, MLNode*, MLNode*, NODE_REF, MLPNodeRef );
#undef get_prop

/////////////////////////////////////////////////////////////////////////////
