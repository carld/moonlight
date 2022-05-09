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
  MLEOrthoAxis.C

  Stephane Rehel

  December 1 1996
*/

#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"
#include "tools/Vector2.h"

#include "graphics/mlgl.h"
#include "graphics/MLEvent.h"
#include "graphics/SystemWindow.h"

#include "MLEOrthoAxis.h"

/////////////////////////////////////////////////////////////////////////////

MLEOrthoAxis::MLEOrthoAxis( ModelCanvas* _canvas, MLEOrthoAxis::AXIS _axis ):
  MLEOrtho(_canvas)
{
  axis= _axis;

  switch( axis )
    {
    case X_AXIS: setName("OrthoX"); break;
    case Y_AXIS: setName("OrthoY"); break;
    default:
    case Z_AXIS: setName("OrthoZ"); axis= Z_AXIS; break;
    }

  resetCamera();
}

/////////////////////////////////////////////////////////////////////////////

MLEOrthoAxis::~MLEOrthoAxis()
{}

/////////////////////////////////////////////////////////////////////////////

void MLEOrthoAxis::write( MLWFileBlock& block )
{
  MLEOrtho::write(block);

  block << int(0);
}

/////////////////////////////////////////////////////////////////////////////

void MLEOrthoAxis::read( MLRFileBlock& block )
{
  MLEOrtho::read(block);

  if( block.getInteger() == 0 )
    return;
}

/////////////////////////////////////////////////////////////////////////////

void MLEOrthoAxis::resetCamera()
{
  Point eye;
  Vector up;

  switch( axis )
    {
    case X_AXIS:
      {
      eye= Point(-6,0,0);
      up= Vector(0,1,0);
      break;
      }
    case Y_AXIS:
      {
      eye= Point(0,-6,0);
      up= Vector(0,0,1);
      break;
      }
    default:
    case Z_AXIS:
      {
      eye= Point(0,0,6);
      up= Vector(0,1,0);
      break;
      }
    }

  MLEOrtho::resetCamera();

  camera.initByLookat( eye, Point(0,0,0), up );
  camera.ortho= true;
}

/////////////////////////////////////////////////////////////////////////////

void MLEOrthoAxis::resetAll()
{
  MLEOrtho::resetAll();

  resetCamera();
}

/////////////////////////////////////////////////////////////////////////////

