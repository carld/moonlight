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
   FormalParamSurf.h

   November 12 1996
   Stephane Rehel
*/

#ifndef __FormalParamSurf_h
#define __FormalParamSurf_h

#ifndef __FormalRep_h
#include "FormalRep.h"
#endif

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class FormalParamSurf: public FormalRep
{
public:
  int nFacesU, nFacesV;
  int minFacesU, minFacesV;
  int closedU, closedV;

protected:
  FormalParamSurf( MLScene* _scene );

public:
  virtual ~FormalParamSurf();

  virtual void generate( MeshRep* mesh ) const;

  virtual FormalRep* duplicate() = 0;

  virtual boolean isParamSurf() const;
  virtual boolean isNURBS() const;

protected:
  void copy( FormalParamSurf* ps ) const;

  // u, v >= 0
  virtual Point getUVPoint( int u, int v ) const = 0;

  virtual boolean needToMergeVertices() const;

  virtual void _read( MLRFileBlock* block );
  virtual void _write( MLWFileBlock* block );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __FormalParamSurf_h
