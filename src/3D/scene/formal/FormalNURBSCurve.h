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
   FormalNURBSCurve.h

   January 24 1997

   Stephane Rehel
*/

#ifndef __FormalNURBSCurve_h
#define __FormalNURBSCurve_h

#ifndef __FormalParamCurve_h
#include "FormalParamCurve.h"
#endif

class MLWFileBlock;
class MLRFileBlock;
class MLNURBS_Curve;
class MLCurve;

/////////////////////////////////////////////////////////////////////////////

class FormalNURBSCurve: public FormalParamCurve
{
  friend class FormalCurveRep;

public:
  enum TYPE
       {
       FIRST_TYPE = 0,
       BILINEAR   = 1,
       CIRCLE     = 2,
       LAST_TYPE  = 1000
       };

protected:
  MLNURBS_Curve* nurbs;

protected:
  FormalNURBSCurve( MLScene* _scene );

public:
  virtual ~FormalNURBSCurve();

  virtual FormalCurveRep::TYPE getType() const;

  boolean isNURBS() const;

  void alloc( int n, int degree );

  FormalCurveRep* duplicate();

  MLNURBS_Curve* getCurve() const
    {
    return nurbs;
    }

protected:
  // u, v >= 0
  virtual Point getUPoint( int u, double t ) const;

public:
  // called when moving a control point for example
  void regenerateVertices( CurveRep* curve );

protected:
  virtual void _read( MLRFileBlock* block );
  virtual void _write( MLWFileBlock* block );

public:
  void create( TYPE type );

  void createSegment( const Point& p0, const Point& p1 );
  void createCircle();
/*
  void createSphere();
  void createCone();
  void createTorus();
*/
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __FormalNURBSCurve_h
