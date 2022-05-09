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
   FormalNURBS.h

   January 8 1997

   Stephane Rehel
*/

#ifndef __FormalNURBS_h
#define __FormalNURBS_h

#ifndef __FormalParamSurf_h
#include "FormalParamSurf.h"
#endif

class MLWFileBlock;
class MLRFileBlock;
class MLNURBS_Surface;

/////////////////////////////////////////////////////////////////////////////

class FormalNURBS: public FormalParamSurf
{
  friend class FormalRep;

public:
  enum TYPE
       {
       FIRST_TYPE = 0,
       BILINEAR   = 1,
       LAST_TYPE  = 1000
       };

protected:
  MLNURBS_Surface* nurbs;

protected:
  FormalNURBS( MLScene* _scene );

public:
  virtual ~FormalNURBS();

  virtual FormalRep::TYPE getType() const;

  boolean isNURBS() const;

  void alloc( int n, int u_degree, int m, int v_degree );

  FormalRep* duplicate();

  MLNURBS_Surface* getSurface() const
    {
    return nurbs;
    }

protected:
  // u, v >= 0
  virtual Point getUVPoint( int u, int v ) const;

public:
  // called when moving a control point for example
  void regenerateVertices( MeshRep* mesh );

protected:
  virtual boolean needToMergeVertices() const;

  virtual void _read( MLRFileBlock* block );
  virtual void _write( MLWFileBlock* block );

public:
  void create( TYPE type );

  void createBilinear( const Point& p00, const Point& p10,
                       const Point& p11, const Point& p01 );
  void createCylinder();
  void createSphere();
  void createCone();
  void createTorus();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __FormalNURBS_h
