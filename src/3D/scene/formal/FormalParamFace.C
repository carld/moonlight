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
   FormalParamFace.C

   February 9 1997
   Stephane Rehel
*/

#include <math.h>

#include "FormalParamFace.h"

#include "tools/file/MLRFileBlock.h"
#include "tools/file/MLWFileBlock.h"

/////////////////////////////////////////////////////////////////////////////

FormalParamFace::FormalParamFace( MLScene* _scene ):
  FormalParamSurf(_scene)
{
  closedU= 0;
  closedV= 0;

  nFacesU= 10;
  nFacesV= 10;

  minFacesU= 1;
  minFacesV= 1;

  origin= Point(0,0,0);
  Udir= Vector(1,0,0) * double(nFacesU) / 2.;
  Vdir= Vector(0,1,0) * double(nFacesV) / 2.;
}

/////////////////////////////////////////////////////////////////////////////

FormalParamFace::~FormalParamFace()
{}

/////////////////////////////////////////////////////////////////////////////

FormalRep::TYPE FormalParamFace::getType() const
{
  return FormalRep::PARAM_FACE;
}

/////////////////////////////////////////////////////////////////////////////

FormalRep* FormalParamFace::duplicate()
{
  FormalParamFace* f= new FormalParamFace(scene);

  FormalParamSurf::copy(f);

  f->origin= origin;
  f->Udir= Udir;
  f->Vdir= Vdir;

  return f;
}

/////////////////////////////////////////////////////////////////////////////

Point FormalParamFace::getUVPoint( int u, int v ) const
{
  return origin + Udir*(double(u)/double(nFacesU)-.5)
                + Vdir*(double(v)/double(nFacesV)-.5);
}

/////////////////////////////////////////////////////////////////////////////

void FormalParamFace::_read( MLRFileBlock* block )
{
  FormalParamSurf::_read(block);

  *block >> origin;
  *block >> Udir;
  *block >> Vdir;
}

/////////////////////////////////////////////////////////////////////////////

void FormalParamFace::_write( MLWFileBlock* block )
{
  FormalParamSurf::_write(block);

  *block << origin;
  *block << Udir;
  *block << Vdir;
}

/////////////////////////////////////////////////////////////////////////////

