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
   FormalParamSphere.h

   January 24 1997
   Stephane Rehel
*/

#ifndef __FormalParamSphere_h
#define __FormalParamSphere_h

#ifndef __FormalParamSurf_h
#include "FormalParamSurf.h"
#endif

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class FormalParamSphere: public FormalParamSurf
{
  friend class FormalRep;

protected:
  FormalParamSphere( MLScene* _scene );

public:
  virtual ~FormalParamSphere();

  FormalRep::TYPE getType() const;

  FormalRep* duplicate();

protected:
  Point getUVPoint( int u, int v ) const;

  virtual boolean needToMergeVertices() const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __FormalParamSphere_h
