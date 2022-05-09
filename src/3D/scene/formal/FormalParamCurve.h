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
   FormalParamCurve.h

   Stephane Rehel

   January 24 1998
*/

#ifndef __FormalParamCurve_h
#define __FormalParamCurve_h

#ifndef __FormalCurveRep_h
#include "FormalCurveRep.h"
#endif

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class FormalParamCurve: public FormalCurveRep
{
public:
  int nSegments;
  int minSegments;
  boolean closed;

protected:
  FormalParamCurve( MLScene* _scene );

public:
  virtual ~FormalParamCurve();

  virtual void generate( CurveRep* curve ) const;

  virtual FormalCurveRep* duplicate() = 0;

  virtual boolean isParamCurve() const;
  virtual boolean isNURBS() const;

protected:
  void copy( FormalParamCurve* pc ) const;

  // u, v >= 0
  virtual Point getUPoint( int u, double t ) const = 0;

  virtual void _read( MLRFileBlock* block );
  virtual void _write( MLWFileBlock* block );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __FormalParamCurve_h
