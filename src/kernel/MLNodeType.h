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
  MLNodeType.h

  Stephane Rehel

  April 6 1998
*/

#ifndef __MLNodeType_h
#define __MLNodeType_h

#include "tools/MLString.h"

class MLNode;
class MLKernel;
class MLAppRoot;
class MLProcedure;
class MLProcProfile;

template<class T> class MLArray;

/////////////////////////////////////////////////////////////////////////////

class MLNodeType
{
public:
  typedef MLNode* (*NodeCreator)( MLAppRoot* approot, MLNode* parent );

  enum Type
       {
       NULL_TYPE = 0,

       DEFAULT = 1, // MLNode

       APP_ROOT = 2,
       PROCEDURE = 3,
       PROC_PROFILE = 4,
       OPERATOR = 5,

       LAST_TYPE
       };

public:
  struct NodeType
    {
    int super_type;
    MLArray<int> sub_types;
    MLString name;
    NodeCreator creator;
    void* data;
    MLNode* procedures; // procedures associated to this type
    };

  MLArray<NodeType*> types;

  MLAppRoot* approot;

public:
  MLNodeType( MLAppRoot* approot );

  virtual ~MLNodeType();

  boolean init();

  const MLString& getName( Type t ) const;

  Type newType( const MLString& name,
                NodeCreator creator, void* data = 0 );

  Type newType( Type super_type,
                const MLString& name,
                NodeCreator creator, void* data = 0 );

  // return NULL_TYPE if not found
  Type getType( const MLString& type_name ) const;

  MLNode* newNode( Type t, MLNode* parent );

  static MLNode* newBasicNode( Type t, MLNode* parent );

private:
  NodeType* getNodeType( Type t ) const;

public:
  boolean nodeOf( Type t, Type super_to_test ) const;

  MLNode* getProcedures( Type t ) const;

  MLProcedure* findProcedure( MLNodeType::Type t, const MLString& name );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLNodeType_h
