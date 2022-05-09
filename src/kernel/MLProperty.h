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

#ifndef __MLProperty_h
#define __MLProperty_h

#include <assert.h>

#include "tools/boolean.h"
#include "tools/MLString.h"
#include "tools/MLArray.h"
#include "tools/MLReference.h"
#include "tools/MLSerializable.h"

#include "MLPropertyType.h"

class MLNode;
class MLOperator;
class MLKernel;
class MLPropertyChangeListener;

/////////////////////////////////////////////////////////////////////////////

class MLProperty: public MLReference,
                  public MLSerializable
{
  friend class MLNode;
  friend class MLKernel;
  friend class MLOperator;

protected:
  MLKernel* kernel;

  // owner node
  MLNode* node;

  // property name
  MLString name;

  // property unique identifier (>0)
  int id;

  // the operator that generates this property (0 if none)
  MLOperator* sourceOp;

  // the list of operators that depend on this property
  MLArray<MLOperator*> destOps;

  // plug-in that have created this property
  int plugin_id;

  // for updating/saving
  boolean modified;

  // for operators updating
  boolean op_modified;

  // we don't own these listeners
  // there are call just after the property has been changed
  MLArray<MLPropertyChangeListener*> listeners;

private:
  void property_constructor( MLNode* _node, const char* _name );

protected:
  MLProperty( MLNode* _node, const char* _name );

public:
  MLProperty( MLNode* _node );

  virtual ~MLProperty();

  void attach( MLNode* n );
  void detach();

  virtual MLPropertyType::Type getType() const = 0;

  virtual boolean canBeAnimated() const
    {
    return false;
    }

  MLKernel* getKernel() const
    {
    return kernel;
    }

  MLNode* getNode() const
    {
    return node;
    }

  int getID() const
    {
    return id;
    }

  const MLString& getName() const
    {
    return name;
    }

  MLString getLongName() const;

  // return false if unable to set it
  boolean setName( const MLString& _name );

  MLOperator* getSourceOp() const
    {
    return sourceOp;
    }

  const MLArray<MLOperator*>& getDestOps() const
    {
    return destOps;
    }

  void setSourceOp( MLOperator* op );

  void unsetSourceOp();

  void addDestOp( MLOperator* op );

  void removeDestOp( MLOperator* op );

protected:
  void dest_ops_to_be_updated();

public:
  void addListener( MLPropertyChangeListener* l );

  void removeListener( MLPropertyChangeListener* l );

protected:
  void call_listeners();

public:
  void touch()
    {
    modified= op_modified= true;
    }

  // return true if property has changed
  virtual boolean setFrame( int _frame )
    {
    return false;
    }

  virtual boolean isArray() const
    {
    return false;
    }
  virtual int getArrayLength() const
    {
    return 0;
    }

  // return false if error
  boolean serialWrite( XDRFile* file );

  // return false if error
  boolean serialRead( XDRFile* file );

  // return the size (in bytes) that will be read/written by
  // serialRead()/serialWrite() on this property
  int serialSize() const;

  // return 0 if error or unknown property
  static MLProperty* serialReadNew( XDRFile* file, boolean* unknown );

protected:
  // return false if error
  boolean serialWrite_fromNode( XDRFile* file );

  // return false if error
  // static
  static MLProperty* serialReadNew_fromNode( XDRFile* file,
                                             int type,
                                             const MLString& _name,
                                             boolean* unknown );

  // return false if error
  boolean serialRead_fromNode( XDRFile* file );

  // return false if error
  boolean serialReadProperty( XDRFile* file );

protected:
  virtual boolean _serialWrite( XDRFile* file ) = 0;
  virtual boolean _serialRead( XDRFile* file ) = 0;
  virtual int _serialSize() const = 0;

public:
  virtual boolean copy( MLProperty* p ) const = 0;

  // the cloned property is not attached to any parent node
  // the name is left *empty*
  // source & dest operators and *not* duplicated
  // listeners are *not* duplicated
  virtual MLProperty* clone() const = 0;

  virtual MLString toString() const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLProperty_h
