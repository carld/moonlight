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
  MLNURBS_ControlPoint.C

  Stephane Rehel

  January 8 1998
*/

#include "MLNURBS_ControlPoint.h"

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_ControlPoint::set( const Point& _point, MLNURBS::real _w )
{
  point= _point;
  w= _w;
  hpoint= HVector( w * point.x(),
                   w * point.y(),
                   w * point.z(),
                   w );
}

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_ControlPoint::set( const HVector& _hpoint )
{
  DBL h= _hpoint.h();

  if( fabs(h) < 1e-30 )
    {
    point= Point(1,0,0);
    w= 0.;
    hpoint= HVector(1,0,0,0);
    return;
    }

  point= Point( _hpoint.x(), _hpoint.y(), _hpoint.z() ) / h;
  w= h;
  hpoint= _hpoint;
}

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_ControlPoint::setPoint( const Point& _point )
{
  point= _point;
  hpoint= HVector( w * point.x(),
                   w * point.y(),
                   w * point.z(),
                   w );
}

/////////////////////////////////////////////////////////////////////////////

void MLNURBS_ControlPoint::setWeight( MLNURBS::real _w )
{
  w= _w;
  hpoint= HVector( w * point.x(),
                   w * point.y(),
                   w * point.z(),
                   w );
}

/////////////////////////////////////////////////////////////////////////////
