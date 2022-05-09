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
  DXFBound.C

  Stephane Rehel [SCFR::Kepler] Stephane.Rehel@imag.fr

  Stephane Rehel
  April 25 1997
*/

#include "DXFBound.h"

/////////////////////////////////////////////////////////////////////////////

// DXFBound class: used for computing bounding box, etc...
void DXFBound::init()
{
  nPoints= 0;
  isoPoint= Point(0.,0.,0.);
  minPoint= Point(1.,1.,1.) * 1e30;
  maxPoint= Point(1.,1.,1.) *-1e30;
}

/////////////////////////////////////////////////////////////////////////////

void DXFBound::POINT( const Point& p )
{
  bound( p );
}

/////////////////////////////////////////////////////////////////////////////

void DXFBound::FACE( const Point& p1, const Point& p2, const Point& p3 )
{
  bound( p1 );
  bound( p2 );
  bound( p3 );
}

/////////////////////////////////////////////////////////////////////////////

void DXFBound::FACE( const Point& p1, const Point& p2,
                     const Point& p3, const Point& p4 )
{
  bound( p1 );
  bound( p2 );
  bound( p3 );
  bound( p4 );
}

/////////////////////////////////////////////////////////////////////////////

void DXFBound::POLYLINE( int nVertices, const Point* points,
                         boolean /* closed */ )
{
  for( int i= 0; i< nVertices; ++i )
     bound( points[i] );
}

/////////////////////////////////////////////////////////////////////////////

void DXFBound::start_MAIN()
{}

/////////////////////////////////////////////////////////////////////////////

void DXFBound::end_MAIN()
{
  if( nPoints != 0 )
    isoPoint= isoPoint / double(nPoints);
}

/////////////////////////////////////////////////////////////////////////////
