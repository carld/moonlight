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
  OOMEBlue.C

  Stephane Rehel

  October 19 1997
*/

#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"

#include "graphics/mlgl.h"
#include "graphics/MLEvent.h"
#include "graphics/SystemWindow.h"

#include "OOMEBlue.h"

/////////////////////////////////////////////////////////////////////////////

OOMEBlue::OOMEBlue( OOMCanvas* _canvas ):
  OOMEColored( _canvas, 0., 0., 0.5 )
{
  setName("Blue");
}

/////////////////////////////////////////////////////////////////////////////

OOMEBlue::~OOMEBlue()
{}

/////////////////////////////////////////////////////////////////////////////

void OOMEBlue::write( MLWFileBlock& block )
{
  OOMEColored::write(block);

  block << int(0);
}

/////////////////////////////////////////////////////////////////////////////

void OOMEBlue::read( MLRFileBlock& block )
{
  OOMEColored::read(block);

  if( block.getInteger() == 0 )
    return;
}

/////////////////////////////////////////////////////////////////////////////

void OOMEBlue::resetAll()
{
  OOMEColored::resetAll();

  OOMEColored::r= .0;
  OOMEColored::g= .0;
  OOMEColored::b= .5;
}

/////////////////////////////////////////////////////////////////////////////
