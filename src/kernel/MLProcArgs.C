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
  MLProcArgs.C

  Stephane Rehel
  March 30 1998
*/

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>

#include "MLProcArgs.h"
#include "MLProcedure.h"
#include "MLProcProfile.h"

#include "MLProperty.h"

/////////////////////////////////////////////////////////////////////////////

MLProcArgs::MLProcArgs( MLKernel* kernel ): MLNode(kernel)
{
  base_node= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLProcArgs::~MLProcArgs()
{
  unref(base_node);
  base_node= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLProcArgs::setBaseNode( MLNode* _base_node )
{
  ref(_base_node);
  unref(base_node);
  base_node= _base_node;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLProcArgs::toString()
{
  MLString s= getName() + MLString("( ");

  for( int i= 0; i < getNProperties(); ++i )
    {
    MLProperty* p= getProperty(i);
    MLString name= p->getName();
    s+= name + MLString("=") + p->toString();

    if( i+1 < getNProperties() )
      s+= MLString(", ");
    }

  s+= MLString( " )" );

  return s;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLProcArgs* MLProcArgs::create( MLProcedure* procedure, va_list ap )
{
  if( procedure == 0 )
    return 0;

  MLProcProfile* profile= procedure->getProfile();

  MLProcArgs* args= profile->buildArgs(ap);

  if( args != 0 )
    args->setName( procedure->getName() );

  return args;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLProcArgs* MLProcArgs::create( MLProcedure* procedure, ... )
{
  if( procedure == 0 )
    return 0;

  va_list ap;
  va_start(ap,procedure);
  MLProcArgs* args= create(procedure,ap);
  va_end(ap);

  return args;
}

/////////////////////////////////////////////////////////////////////////////
