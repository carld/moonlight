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
  MLAbstractObject.C

  Stephane Rehel

  February 8 1998
*/

#include <stdio.h>
#include <assert.h>

#include "MLAbstractObject.h"

#include "MLObject.h"
#include "MLScene.h"

/////////////////////////////////////////////////////////////////////////////

static const int null_id= -1;

/////////////////////////////////////////////////////////////////////////////

MLAbstractObject::MLAbstractObject( MLScene* _scene )
{
  scene= _scene;

  name.name= "";

  flags= FLAG(0);

  max_instances= 8;
  instances= new int [ max_instances + 1 ];
  for( int i= 0; i <= max_instances; ++i )
    instances[i]= null_id;
  nInstances= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractObject::~MLAbstractObject()
{
  if( instances != 0 )
    {
    for( int i= 1; i <= max_instances; ++i )
      instances[i]= null_id;
    }

  delete instances;
  instances= 0;
  nInstances= 0;
  max_instances= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLAbstractObject::setName( const MLString& _name )
{
  name.name= _name;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLAbstractObject::_addReference( MLObject* instance )
{
  if( instance == 0 )
    return false;

  int hole= 0;
  for( int i= 1; i <= max_instances; ++i )
    if( instances[i] == null_id )
      {
      hole= i;
      break;
      }

  if( hole == 0 )
    {
    int new_max_instances= max_instances + max_instances/2;
    int* new_instances= new int [ new_max_instances + 1 ];
    int i;
    new_instances[0]= null_id;
    for( i= 1; i <= max_instances; ++i )
      new_instances[i]= instances[i];
    for( i= max_instances+1; i <= new_max_instances; ++i )
      new_instances[i]= null_id;

    hole= max_instances + 1;
    delete instances;
    instances= new_instances;
    max_instances= new_max_instances;
    }

  instances[ hole ]= instance->getID();
  ++nInstances;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLAbstractObject::_removeReference( MLObject* instance )
{
  if( instance == 0 )
    return false;

  for( int i= 1; i <= max_instances; ++i )
    if( instances[i] == instance->getID() )
      {
      instances[i]= null_id;
      --nInstances;
//      if( nInstances > 0 )
//        instance->abstract= 0;
        // else: dont delete me please, i'm still referenced
      return true;
      }

  return false; // well, not found
}

/////////////////////////////////////////////////////////////////////////////

