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
  KoalaNodeTypes.C

  Stephane Rehel

  May 28 1998
*/

#include <stdio.h>

#include "tools/MLArray.h"

#include "kernel/MLAppRoot.h"

#include "KoalaNodeTypes.h"

/////////////////////////////////////////////////////////////////////////////

KoalaNodeTypes::KoalaNodeTypes( MLAppRoot* ar )
{
  approot= ar;

  interface= MLNodeType::Type(0);
  module= MLNodeType::Type(0);
  canvas= MLNodeType::Type(0);
  engine= MLNodeType::Type(0);
}

/////////////////////////////////////////////////////////////////////////////

KoalaNodeTypes::~KoalaNodeTypes()
{
  approot= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean KoalaNodeTypes::init()
{
  if( approot == 0 )
    return false;

  MLNodeType* types= approot->getTypes();

  if( types == 0 )
    {
    fprintf( stderr, "KoalaNodeTypes: approot->types == 0 !?\n" );
    return false;
    }

  interface= types->newType( MLNodeType::APP_ROOT, "Interface", 0, 0 );
  module= types->newType( MLNodeType::APP_ROOT, "Module", 0, 0 );
  canvas= types->newType( MLNodeType::APP_ROOT, "Canvas", 0, 0 );
  engine= types->newType( MLNodeType::APP_ROOT, "Engine", 0, 0 );

  return true;
}

/////////////////////////////////////////////////////////////////////////////
