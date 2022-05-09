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
   FormalParamTorus.C

   February 9 1997
   Stephane Rehel
*/

#include <math.h>

#include "FormalParamTorus.h"

/////////////////////////////////////////////////////////////////////////////

FormalParamTorus::FormalParamTorus( MLScene* _scene ):
  FormalParamSurf(_scene)
{
  closedU= 1;
  closedV= 1;

  nFacesU= 4*4; // around axis
  nFacesV= 3*4; // tube

  minFacesU= 3;
  minFacesV= 3;

//  nFacesU= 100; // around axis
// nFacesV= 100; // tube

//  nFacesU= 50; // around axis
//  nFacesV= 50; // tube

  center= Point(0,0,0);
  axis= Vector(0,0,1);

  majorRadius= 1.;
  minorRadius= 0.4;

  axis.makeOrthoBase( baseU, baseV );
  baseU.setNormalized();
  baseV.setNormalized();
  axis.setNormalized();
}

/////////////////////////////////////////////////////////////////////////////

FormalParamTorus::~FormalParamTorus()
{}

/////////////////////////////////////////////////////////////////////////////

FormalRep::TYPE FormalParamTorus::getType() const
{
  return FormalRep::PARAM_TORUS;
}

/////////////////////////////////////////////////////////////////////////////

FormalRep* FormalParamTorus::duplicate()
{
  FormalParamTorus* f= new FormalParamTorus(scene);

  FormalParamSurf::copy(f);

  f->center= center;
  f->axis= axis;
  f->minorRadius= minorRadius;
  f->majorRadius= majorRadius;
  f->baseU= baseU;
  f->baseV= baseV;

  return f;
}

/////////////////////////////////////////////////////////////////////////////

Point FormalParamTorus::getUVPoint( int u, int v ) const
{
  double phi= double(u)/double(nFacesU) * 2. * M_PI;
  double theta= double(v)/double(nFacesV) * 2. * M_PI;

  Vector Xphi= baseU * cos(phi) + baseV * sin(phi);

  Point P= center + (majorRadius + minorRadius*cos(theta)) * Xphi
                  +                minorRadius*sin(theta)  * axis;

  return P;
}

/////////////////////////////////////////////////////////////////////////////
