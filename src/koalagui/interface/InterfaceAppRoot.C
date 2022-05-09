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
  InterfaceAppRoot.C

  Stephane Rehel

  May 28 1998
*/

#include <assert.h>

#include "InterfaceAppRoot.h"
#include "KoalaNodeTypes.h"

/////////////////////////////////////////////////////////////////////////////

InterfaceAppRoot::InterfaceAppRoot( MLKernel* _kernel,
                                    Interface* _interface ):
  MLAppRoot(_kernel)
{
  interface= _interface;

  koalaNodeTypes= new KoalaNodeTypes(this);

  MLAppRoot::setName("KoalaInterface");
}

/////////////////////////////////////////////////////////////////////////////

InterfaceAppRoot::~InterfaceAppRoot()
{
  delete koalaNodeTypes;
  koalaNodeTypes= 0;

  interface= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean InterfaceAppRoot::init()
{
  if( ! MLAppRoot::init() )
    return false;

  if( ! koalaNodeTypes->init() )
    return false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

MLNodeType::Type InterfaceAppRoot::getNodeType() const
{
  assert( koalaNodeTypes != 0 );

  return koalaNodeTypes->interface;
}

/////////////////////////////////////////////////////////////////////////////

KoalaNodeTypes* InterfaceAppRoot::getNodeTypes() const
{
  return koalaNodeTypes;
}

/////////////////////////////////////////////////////////////////////////////
