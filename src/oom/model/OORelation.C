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
  OORelation.C

  Stephane Rehel

  December 2 1997
*/

#include "OORelation.h"

#include "OOPackage.h"
#include "OOClass.h"
#include "OOConnection.h"

/////////////////////////////////////////////////////////////////////////////

OORelation::OORelation( OOPackage* _package, int _n )
{
  package= _package;
  name= "relation";

  n= _n;

  connections= new OOConnection* [ n + 1 ];

  int i;
  for( i= 0; i <= n; ++i )
    connections[i]= 0;
  for( i= 1; i <= n; ++i )
    connections[i]= new OOConnection(this,i);
}

/////////////////////////////////////////////////////////////////////////////

OORelation::~OORelation()
{
  for( int i= 1; i <= n; ++i )
    if( connections[i] != 0 )
      {
      connections[i]->unconnect();
      connections[i]= 0;
      }

  delete connections;
  connections= 0;

  package= 0;
}

/////////////////////////////////////////////////////////////////////////////

void OORelation::connect( int i, OOClass* _class )
{
  if( package == 0 || i < 1 || i > n )
    return;

  if( _class == 0 )
    return;

  if( connections[i] != 0 )
    connections[i]->connect(_class);
}

/////////////////////////////////////////////////////////////////////////////

void OORelation::unconnect( int i )
{
  if( package == 0 || i < 1 || i > n )
    return;

  if( connections[i] != 0 )
    connections[i]->unconnect();
}

/////////////////////////////////////////////////////////////////////////////
