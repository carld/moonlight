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
   FormalParamCone.h

   February 9 1997
   Stephane Rehel
*/

#ifndef __FormalParamCone_h
#define __FormalParamCone_h

#ifndef __FormalParamSurf_h
#include "FormalParamSurf.h"
#endif

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class FormalParamCone: public FormalParamSurf
{
  friend class FormalRep;

protected:
  Point point1, point2;
  double radius1, radius2;
  Vector baseU, baseV;

protected:
  FormalParamCone( MLScene* _scene );

public:
  virtual ~FormalParamCone();

  FormalRep::TYPE getType() const;

  FormalRep* duplicate();

protected:
  Point getUVPoint( int u, int v ) const;

  virtual boolean needToMergeVertices() const;

  virtual void _read( MLRFileBlock* block );
  virtual void _write( MLWFileBlock* block );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __FormalParamCone_h
