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
  OOClass.C

  Stephane Rehel

  December 2 1997
*/

#include <math.h>

#include "OOClass.h"

/////////////////////////////////////////////////////////////////////////////

OOClass::OOClass( OOPackage* _package, const MLString& _name )
{
  package= _package;
  name= _name;

  draw.x1= 0;
  draw.y1= 0;
  draw.x2= 50;
  draw.y2= 50;
}

/////////////////////////////////////////////////////////////////////////////

OOClass::~OOClass()
{
  package= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean OOClass::connected( OORelation* relation )
{
  if( relation == 0 )
    return false;

  SIListIterator<OORelation> li(relations);
  while( ! li.eol() )
    {
    OORelation* r= li++;
    if( r == relation )
      return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

void OOClass::connect( OORelation* relation )
{
  if( relation == 0 )
    return;

  if( connected(relation) )
    return;

  relations.append(relation);
}

/////////////////////////////////////////////////////////////////////////////

void OOClass::unconnect( OORelation* relation )
{
  if( relation == 0 )
    return;

  relations.remove(relation);
}

/////////////////////////////////////////////////////////////////////////////

IVector OOClass::getConnectionNormal( double s ) const
{
  s+= 0.5;

  while( s < 0. )
    s+= 4.;

  s= fmod( s, 4. );

  if( s <= 1. )
    return IVector(0,-1);
  s-= 1.;

  if( s <= 1. );
    return IVector(1,0);
  s-= 1.;

  if( s <= 1. )
    return IVector(0,1);
  s-= 1.;

  return IVector(-1,0);
}

/////////////////////////////////////////////////////////////////////////////

IPoint OOClass::getConnectionPoint( double s ) const
{
  int width= draw.x2 - draw.x1 + 1;
  int height= draw.y2 - draw.y1 + 1;
  if( width <= 0 || height <= 0 )
    return IPoint(draw.x1,draw.y1);

  s += .5;

  while( s < 0. )
    s+= 4.;

  s= fmod( s, 4. );

  if( s <= 1. )
    return IPoint(draw.x1+int(s*double(width)),draw.y1);
  s-= 1.;

  if( s <= 1. )
    return IPoint(draw.x2,draw.y1+int(s*double(height)));
  s-= 1.;

  if( s <= 1. )
    return IPoint(draw.x2-int(s*double(width)),draw.y2);
  s-= 1.;

  return IPoint(draw.x1,draw.y2-int(s*double(height)));
}

/////////////////////////////////////////////////////////////////////////////
