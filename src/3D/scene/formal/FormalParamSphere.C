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
   FormalParamSphere.C

   January 24 1997
   Stephane Rehel
*/

#include <math.h>

#include "FormalParamSphere.h"

/////////////////////////////////////////////////////////////////////////////

FormalParamSphere::FormalParamSphere( MLScene* _scene ):
  FormalParamSurf(_scene)
{
  closedU= 1;
  closedV= 0;

  nFacesU= 13 * 2;
  nFacesV= 6 * 2;

  minFacesU= 3;
  minFacesV= 2;
}

/////////////////////////////////////////////////////////////////////////////

FormalParamSphere::~FormalParamSphere()
{}

/////////////////////////////////////////////////////////////////////////////

FormalRep::TYPE FormalParamSphere::getType() const
{
  return FormalRep::PARAM_SPHERE;
}

/////////////////////////////////////////////////////////////////////////////

FormalRep* FormalParamSphere::duplicate()
{
  FormalParamSphere* f= new FormalParamSphere(scene);

  FormalParamSurf::copy(f);

  return f;
}

/////////////////////////////////////////////////////////////////////////////

Point FormalParamSphere::getUVPoint( int u, int v ) const
{
//return Point(double(u)/1.,double(v)/1.,0);

  double phi=   2.*M_PI*double(u)/double(nFacesU);
  double theta= M_PI*(double(v)/double(nFacesV) - 0.5);

  return Point( cos(phi)*cos(theta),
                sin(phi)*cos(theta),
                sin(theta) );

}

/////////////////////////////////////////////////////////////////////////////

boolean FormalParamSphere::needToMergeVertices() const
{
  return true;
}

/////////////////////////////////////////////////////////////////////////////
