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
  OOConnection.C

  Stephane Rehel

  December 4 1997
*/

#include "OOConnection.h"

#include "OOClass.h"
#include "OORelation.h"

/////////////////////////////////////////////////////////////////////////////

OOConnection::OOConnection( OORelation* _relation, int _i )
{
  relation= _relation;
  i= _i;

  the_class= 0;
  position= 0.;
  point= IPoint(0,0);
  normal= IVector(1,0);
}

/////////////////////////////////////////////////////////////////////////////

OOConnection::~OOConnection()
{
  unconnect();

  relation= 0;
  position= 0.;
}

/////////////////////////////////////////////////////////////////////////////

void OOConnection::computePoint()
{
  if( relation == 0 || the_class == 0 )
    return; // not connected

  point= the_class->getConnectionPoint(position);
  normal= the_class->getConnectionNormal(position);
}

/////////////////////////////////////////////////////////////////////////////

void OOConnection::connect( OOClass* c )
{
  if( relation == 0 )
    return;

  if( the_class == c )
    return;

  if( c == 0 )
    {
    unconnect();
    return;
    }

  the_class= c;
  computePoint();
}

/////////////////////////////////////////////////////////////////////////////

void OOConnection::unconnect()
{
  if( relation == 0 )
    return;

  if( the_class == 0 )
    return;

  the_class= 0;
}

/////////////////////////////////////////////////////////////////////////////
