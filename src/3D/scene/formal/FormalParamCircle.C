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
   FormalParamCircle.C

   January 24 1998

   Stephane Rehel
*/

#include <math.h>

#include "FormalParamCircle.h"

/////////////////////////////////////////////////////////////////////////////

FormalParamCircle::FormalParamCircle( MLScene* _scene ):
  FormalParamCurve(_scene)
{
  closed= true;

  nSegments= 5 * 4;
  minSegments= 3;
}

/////////////////////////////////////////////////////////////////////////////

FormalParamCircle::~FormalParamCircle()
{}

/////////////////////////////////////////////////////////////////////////////

FormalCurveRep::TYPE FormalParamCircle::getType() const
{
  return FormalCurveRep::PARAM_CIRCLE;
}

/////////////////////////////////////////////////////////////////////////////

FormalCurveRep* FormalParamCircle::duplicate()
{
  FormalParamCircle* c= new FormalParamCircle(scene);

  FormalParamCurve::copy(c);

  return c;
}

/////////////////////////////////////////////////////////////////////////////

Point FormalParamCircle::getUPoint( int /*u*/, double t ) const
{
//return Point(double(u)/1.,double(v)/1.,0);

  double phi= 2.*M_PI * t;

  return Point( cos(phi), sin(phi), 0. );
}

/////////////////////////////////////////////////////////////////////////////
