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
  FormalSquare.C

  Stephane Rehel

  January 24 1998
*/

#include "FormalSquare.h"

#include "scene/CurveRep.h"
#include "scene/MLCurveVertex.h"

#include "tools/file/MLRFileBlock.h"
#include "tools/file/MLWFileBlock.h"

/////////////////////////////////////////////////////////////////////////////

FormalSquare::FormalSquare( MLScene* _scene ):
  FormalCurveRep(_scene)
{}

/////////////////////////////////////////////////////////////////////////////

FormalSquare::~FormalSquare()
{}

/////////////////////////////////////////////////////////////////////////////

FormalCurveRep::TYPE FormalSquare::getType() const
{
  return FormalCurveRep::SQUARE;
}

/////////////////////////////////////////////////////////////////////////////

void FormalSquare::generate( CurveRep* curve ) const
{
  if( curve == 0 )
    return;

  curve->clear();

  curve->alloc(4);
  curve->closed= true;

  float f= 0.5;

  MLCurveVertex* vertices= curve->vertices;
  vertices[1].set( 0.,   Point(-f,-f,0.) );
  vertices[2].set( 0.25, Point( f,-f,0.) );
  vertices[3].set( 0.50, Point( f, f,0.) );
  vertices[4].set( 0.75, Point(-f, f,0.) );
}

/////////////////////////////////////////////////////////////////////////////

FormalCurveRep* FormalSquare::duplicate()
{
  return new FormalSquare(scene);
}

/////////////////////////////////////////////////////////////////////////////

void FormalSquare::_read( MLRFileBlock* /*block*/ )
{
}

/////////////////////////////////////////////////////////////////////////////

void FormalSquare::_write( MLWFileBlock* /*block*/ )
{
}

/////////////////////////////////////////////////////////////////////////////
