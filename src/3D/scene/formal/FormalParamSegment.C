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
   FormalParamSegment.C

   January 24 1998

   Stephane Rehel
*/

#include <math.h>

#include "FormalParamSegment.h"

#include "tools/file/MLRFileBlock.h"
#include "tools/file/MLWFileBlock.h"

/////////////////////////////////////////////////////////////////////////////

FormalParamSegment::FormalParamSegment( MLScene* _scene ):
  FormalParamCurve(_scene)
{
  closed= false;

  nSegments= 10;
  minSegments= 1;

  origin= Point(0,0,0);
  dir= Vector(1,0,0) * double(nSegments) / 2.;
  origin -= dir / 2.;
}

/////////////////////////////////////////////////////////////////////////////

FormalParamSegment::~FormalParamSegment()
{}

/////////////////////////////////////////////////////////////////////////////

FormalCurveRep::TYPE FormalParamSegment::getType() const
{
  return FormalCurveRep::PARAM_SEGMENT;
}

/////////////////////////////////////////////////////////////////////////////

FormalCurveRep* FormalParamSegment::duplicate()
{
  FormalParamSegment* s= new FormalParamSegment(scene);

  FormalParamCurve::copy(s);

  s->origin= origin;
  s->dir= dir;

  return s;
}

/////////////////////////////////////////////////////////////////////////////

Point FormalParamSegment::getUPoint( int /*u*/, double t ) const
{
  return origin + dir * t;
}

/////////////////////////////////////////////////////////////////////////////

void FormalParamSegment::_read( MLRFileBlock* block )
{
  FormalParamCurve::_read(block);

  *block >> origin;
  *block >> dir;
}

/////////////////////////////////////////////////////////////////////////////

void FormalParamSegment::_write( MLWFileBlock* block )
{
  FormalParamCurve::_write(block);

  *block << origin;
  *block << dir;
}

/////////////////////////////////////////////////////////////////////////////
