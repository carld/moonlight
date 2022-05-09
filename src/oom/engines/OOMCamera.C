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
  OOMCamera.C

  Stephane Rehel

  December 2 1997
*/

#include "OOMCamera.h"

/////////////////////////////////////////////////////////////////////////////

OOMCamera::OOMCamera()
{
  x= 0;
  y= 0;
  zoom= 1.;
}

/////////////////////////////////////////////////////////////////////////////

OOMCamera::~OOMCamera()
{}

/////////////////////////////////////////////////////////////////////////////

void OOMCamera::compute( int x_res, int y_res )
{
  if( x_res <= 0 || y_res <= 0 )
    return;

  double screen_x1= double(0 - x_res/2 + x);
  double screen_x2= double(x_res - x_res/2 + x);
  double screen_y1= double(0 - y_res/2 + y);
  double screen_y2= double(y_res - y_res/2 + y);

  screen_x1 /= zoom;
  screen_x2 /= zoom;
  screen_y1 /= zoom;
  screen_y2 /= zoom;

  double left= screen_x1;
  double right= screen_x2;
  double bottom= screen_y1;
  double top= screen_y2;
  double nearval= -1.;
  double  farval= +1.;

  double x= 2. / (right-left);
  double y= 2. / (top-bottom);
  double z=-2. / (farval-nearval);
  double tx= -(right+left) / (right-left);
  double ty= -(top+bottom) / (top-bottom);
  double tz= -(farval+nearval) / (farval-nearval);

#define M(i,j) projection.aij(i,j)
  M(0,0)= x;  M(0,1)= 0.; M(0,2)= 0.; M(0,3)= tx;
  M(1,0)= 0.; M(1,1)= y;  M(1,2)= 0.; M(1,3)= ty;
  M(2,0)= 0.; M(2,1)= 0.; M(2,2)= z;  M(2,3)= tz;
  M(3,0)= 0.; M(3,1)= 0.; M(3,2)= 0.; M(3,3)= 1.;
#undef M
}

/////////////////////////////////////////////////////////////////////////////
