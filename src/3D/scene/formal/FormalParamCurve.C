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
  FormalParamCurve.C

  Stephane Rehel

  January 24 1998
*/

#include "FormalParamCurve.h"

#include "scene/CurveRep.h"
#include "scene/MLCurveVertex.h"

#include "tools/file/MLRFileBlock.h"
#include "tools/file/MLWFileBlock.h"

/////////////////////////////////////////////////////////////////////////////

FormalParamCurve::FormalParamCurve( MLScene* _scene ):
  FormalCurveRep(_scene)
{
  nSegments= 0;
  minSegments= 1;
  closed= false;
}

/////////////////////////////////////////////////////////////////////////////

FormalParamCurve::~FormalParamCurve()
{}

/////////////////////////////////////////////////////////////////////////////

void FormalParamCurve::generate( CurveRep* curve ) const
{
  if( curve == 0 )
    return;

  int nVertices= closed ? nSegments : (nSegments+1);

  curve->alloc( nVertices );
  curve->closed= closed;

  // generate vertices
  for( int u= 1; u <= nVertices; ++u )
    {
    double t= double(u-1) / double(nSegments);
    curve->vertices[u].set( t, getUPoint(u-1,t) );
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean FormalParamCurve::isParamCurve() const
{
  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean FormalParamCurve::isNURBS() const
{
  return false;
}

/////////////////////////////////////////////////////////////////////////////

void FormalParamCurve::copy( FormalParamCurve* pc ) const
{
  if( pc == 0 )
    return;

  pc->nSegments= nSegments;
  pc->minSegments= minSegments;
  pc->closed= closed;
}

/////////////////////////////////////////////////////////////////////////////

void FormalParamCurve::_read( MLRFileBlock* block )
{
  *block >> nSegments;
  *block >> minSegments;
  closed= block->getInteger() != 0;
}

/////////////////////////////////////////////////////////////////////////////

void FormalParamCurve::_write( MLWFileBlock* block )
{
  *block << nSegments;
  *block << minSegments;
  *block << int(closed);
}

/////////////////////////////////////////////////////////////////////////////
