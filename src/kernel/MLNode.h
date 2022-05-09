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
  MLNode.h

  Stephane Rehel
  March 24 1998
*/

#ifndef __MLNode_h
#define __MLNode_h

#include "tools/MLArray.h"
#include "tools/MLString.h"
#include "tools/MLReference.h"
#include "tools/MLHashTable.h"
#include "tools/MLSerializable.h"
#include "tools/MLSynchronized.h"
#include "tools/Color.h"
#include "tools/Vector.h"

#include "MLNodeType.h"

class MLKernel;
class MLAppRoot;
class MLProperty;
class MLOperator;
class MLProcedure;
class MLProcResult;

class MLPBoolean;
class MLPInteger;
class MLPReal;
class MLPString;
class MLPVector;
class MLPPoint;
class MLPColor;
class MLPNodeRef;

/////////////////////////////////////////////////////////////////////////////

class MLNode: public MLReference,
              public MLSynchronized,
              public MLSerializable
{
  friend class MLKernel;

protected:
  MLKernel* kernel;
  MLAppRoot* approot;

  // parent node
  MLNode* parent;

  // children nodes
  MLArray<MLNode*> children;

  // be able to find a child node from its name
  MLHashTable<MLString,MLNode> child_name_hash;

  // node unique identifier (>0)
  int id;

  // the list of properties owned by this node
  MLArray<MLProperty*> props;

  // be able to find a property from a name
  MLHashTable<MLString,MLProperty> prop_name_hash;

  // plug-in that have created this node
  int plugin_id;

  MLPString* p_name;

protected:
  void MLNode_constructor( MLKernel* _kernel,
                           MLAppRoot* _approot,
                           MLNode* _parent,
                           const MLString& name );

  MLNode( MLNode* _parent, const MLString& name );

  MLNode( MLKernel* kernel );

  MLNode( MLNode* _parent );

  MLNode( MLAppRoot* _approot, MLNode* _parent );

public:
  virtual ~MLNode();

  MLKernel* getKernel() const
    {
    return kernel;
    }

  MLAppRoot* getAppRoot() const
    {
    return approot;
    }

  MLNode* getParentNode() const
    {
    return parent;
    }

  boolean nodeOf( MLNodeType::Type t ) const;

  virtual MLNodeType::Type getNodeType() const
    {
    return MLNodeType::DEFAULT;
    }

  int getID() const
    {
    return id;
    }

  int getNChildren() const
    {
    return children.length();
    }

  // get a child node (0 <= i < nChildren)
  MLNode* getChild( int i ) const
    {
    if( i < 1 || i > children.length() )
      return 0;

    return children.get(i);
    }

  MLNode* getChild( const MLString& name ) const
    {
    return child_name_hash.get(&name);
    }

private:
  void child_name_hash_add( const MLString& name, MLNode* child );
  void child_name_hash_remove( const MLString& name );

public:
  void attachChild( MLNode* child );
  void attachToParent( MLNode* _parent );

  void detachChild( MLNode* child );
  void detachFromParent();

  // no name and no parent
  static MLNode* newNode( MLAppRoot* approot )
    {
    return new MLNode(approot,0);
    }

  MLNode* newChild( const MLString& name );
  MLNode* newChild()
    {
    return newChild("");
    }

  MLString getName() const;

  // return false if unable to set it
  boolean setName( const MLString& name );

  // return "Scene.robot.head" for instance
  MLString getLongName() const;

  int getNProperties() const
    {
    return props.length();
    }

  // get a property (0 <= i < nProperties)
  MLProperty* getProperty( int i )
    {
    if( i < 1 || i > props.length() )
      return 0;

    return props.get(i);
    }

  // get a property (0 <= i < nProperties)
  MLProperty* getProperty( int i ) const
    {
    if( i < 1 || i > props.length() )
      return 0;

    return props.get(i);
    }

  MLProperty* getProperty( const MLString& name )
    {
    return prop_name_hash.get(&name);
    }

private:
  void prop_name_hash_add( const MLString& name, MLProperty* p );
  void prop_name_hash_remove( const MLString& name );

public:
  // called by MLProperty::MLProperty();
  void attachProperty( MLProperty* p );
  void detachProperty( MLProperty* p );

  // return false if unable to set it
  boolean setPropertyName( MLProperty* p, const MLString& name );

  // update animated properties according to this frame number
  void setFrame( int f );

  void print( int indent = 0 );

  // return false if error
  virtual boolean serialWrite( XDRFile* file );

  // return false if error
  static MLNode* nodeSerialRead( MLAppRoot* approot,
                                 MLNode* parent,
                                 XDRFile* file );
  virtual boolean serialRead( XDRFile* file );
private:
  virtual boolean _serialRead( XDRFile* file );

public:
  // return the size (in bytes) that will be read/written by
  // serialRead()/serialWrite() on this property
  virtual int serialSize() const;

  virtual boolean serialReadFinalize()
    {
    return true;
    }

  virtual MLNode* clone() const;

  virtual MLString toString() const;

#define new_prop(method_name,P,type) \
          P* method_name( const MLString& name, type value )
  new_prop( newBoolean, MLPBoolean, boolean );
  new_prop( newInteger, MLPInteger, int );
  new_prop( newReal, MLPReal, double );
  new_prop( newVector, MLPVector, const Vector& );
  new_prop( newPoint, MLPPoint, const Point& );
  new_prop( newColor, MLPColor, const Color& );
  new_prop( newString, MLPString, const MLString& );
  new_prop( newNodeRef, MLPNodeRef, MLNode* );
#undef new_prop

#define get_prop(method_name,type,typer) \
          type method_name( const MLString& name, typer default_value )
  get_prop( getBoolean, boolean, boolean );
  get_prop( getInteger, int, int );
  get_prop( getReal, double, double );
  get_prop( getVector, Vector, const Vector& );
  get_prop( getPoint, Point, const Point& );
  get_prop( getColor, Color, const Color& );
  get_prop( getString, MLString, const MLString& );
  get_prop( getNodeRef, MLNode*, MLNode* );
#undef get_prop

};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLNode_h
