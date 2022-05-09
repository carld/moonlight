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
  DXFBound.h

  Stephane Rehel [SCFR::Kepler] Stephane.Rehel@imag.fr

  Stephane Rehel
  April 25 1997
*/

#ifndef __DXFBound_h
#define __DXFBound_h

#ifndef __DXFExport_h
#include "DXFExport.h"
#endif

/////////////////////////////////////////////////////////////////////////////

// DXFBound class: used for computing bounding box, etc...
class DXFBound: public DXFExport
{
private:
  int nPoints;
  Point minPoint, maxPoint, isoPoint;

  void bound( const Point& p )
    {
    minPoint= minPoint.min( p );
    maxPoint= maxPoint.max( p );
    isoPoint+= p;
    ++nPoints;
    }
  int circle_subdivision;

public:

  DXFBound( int _circle_subdivision )
    {
    circle_subdivision= _circle_subdivision;
    }
  ~DXFBound()
    {}

  Point getMinPoint() const { return minPoint; }
  Point getMaxPoint() const { return maxPoint; }
  Point getIsoPoint() const { return isoPoint; }

  void init();

private:

  int nCircleSubdivide() const
    {
    return circle_subdivision;
    }

  void POINT( const Point& p );
  void FACE( const Point& p1, const Point& p2, const Point& p3 );
  void FACE( const Point& p1, const Point& p2,
             const Point& p3, const Point& p4 );
  void POLYLINE( int nVertices, const Point* points, boolean closed );

  boolean supportBlocks() const
    {
    return false;
    }

  void start_MAIN();
  void end_MAIN();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __DXFBound_h
