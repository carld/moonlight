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
   FormalParamCylinder.C

   November 12 1996
   Stephane Rehel
*/

#include <math.h>

#include "tools/Vector.h"

#include "FormalParamCylinder.h"

/////////////////////////////////////////////////////////////////////////////

FormalParamCylinder::FormalParamCylinder( MLScene* _scene ):
  FormalParamSurf(_scene)
{
  closedU= 1;
  closedV= 0;

  nFacesU= 13;
  nFacesV= 3; // on the axis

  minFacesU= 2;
  minFacesV= 1;
}

/////////////////////////////////////////////////////////////////////////////

FormalParamCylinder::~FormalParamCylinder()
{}

/////////////////////////////////////////////////////////////////////////////

FormalRep::TYPE FormalParamCylinder::getType() const
{
  return FormalRep::PARAM_CYLINDER;
}

/////////////////////////////////////////////////////////////////////////////

FormalRep* FormalParamCylinder::duplicate()
{
  FormalParamCylinder* f= new FormalParamCylinder(scene);

  FormalParamSurf::copy(f);

  return f;
}

/////////////////////////////////////////////////////////////////////////////

Point FormalParamCylinder::getUVPoint( int u, int v ) const
{
  return Point( .5*cos(2.*M_PI*double(u)/double(nFacesU)),
                .5*sin(2.*M_PI*double(u)/double(nFacesU)),
                -.5 + double(v)/double(nFacesV) );
}

/////////////////////////////////////////////////////////////////////////////
