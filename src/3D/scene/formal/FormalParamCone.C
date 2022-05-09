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
   FormalParamCone.C

   February 9 1997
   Stephane Rehel
*/

#include <math.h>

#include "FormalParamCone.h"

#include "tools/file/MLRFileBlock.h"
#include "tools/file/MLWFileBlock.h"

/////////////////////////////////////////////////////////////////////////////

FormalParamCone::FormalParamCone( MLScene* _scene ):
  FormalParamSurf(_scene)
{
  closedU= 1;
  closedV= 0;

  nFacesU= 3*4; // turning around
  nFacesV= 3;

  minFacesU= 3;
  minFacesV= 2;

  point1= Point(0,0,2);
  point2= Point(0,0,0);

  radius1= 0.2 * 0;
  radius2= 1.;

  (point2-point1).makeOrthoBase( baseU, baseV );
  baseU.setNormalized();
  baseV.setNormalized();
}

/////////////////////////////////////////////////////////////////////////////

FormalParamCone::~FormalParamCone()
{}

/////////////////////////////////////////////////////////////////////////////

FormalRep::TYPE FormalParamCone::getType() const
{
  return FormalRep::PARAM_CONE;
}

/////////////////////////////////////////////////////////////////////////////

FormalRep* FormalParamCone::duplicate()
{
  FormalParamCone* f= new FormalParamCone(scene);

  FormalParamSurf::copy(f);

  f->point1= point1;
  f->point2= point2;
  f->radius1= f->radius1;
  f->radius2= f->radius2;
  f->baseU= baseU;
  f->baseV= baseV;

  return f;
}

/////////////////////////////////////////////////////////////////////////////

Point FormalParamCone::getUVPoint( int u, int v ) const
{
  double phi= double(u)/double(nFacesU) * 2. * M_PI;
  double t= double(v)/double(nFacesV);

  Point C= point1 + t * (point2 - point1);

  double r= radius1 + t * (radius2 - radius1);

  return C + (baseU * cos(phi) + baseV * sin(phi)) * r;
}

/////////////////////////////////////////////////////////////////////////////

boolean FormalParamCone::needToMergeVertices() const
{
  return true;
}

/////////////////////////////////////////////////////////////////////////////

void FormalParamCone::_read( MLRFileBlock* block )
{
  FormalParamSurf::_read(block);

  *block >> point1;
  *block >> point2;
  *block >> radius1;
  *block >> radius2;
  *block >> baseU;
  *block >> baseV;
}

/////////////////////////////////////////////////////////////////////////////

void FormalParamCone::_write( MLWFileBlock* block )
{
  FormalParamSurf::_write(block);

  *block << point1;
  *block << point2;
  *block << radius1;
  *block << radius2;
  *block << baseU;
  *block << baseV;
}

/////////////////////////////////////////////////////////////////////////////

