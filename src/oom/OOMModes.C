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
  OOMModes.C

  Stephane Rehel

  October 19 1997
*/

#include "OOMModes.h"

#include "interface/MLModes.h"

#include "modes/OOMMSelect.h"
#include "modes/OOMMDrag.h"
#include "modes/OOMMClick.h"
#include "modes/OOMMCameraZoom.h"

/////////////////////////////////////////////////////////////////////////////

// static
void OOMModes::init( OOMModule* oom, MLModes* m )
{
  if( m == 0 )
    return;

  m->add( SELECT, new OOMMSelect(oom) );
  m->add( DRAG,   new OOMMDrag(oom) );
  m->add( CLICK,  new OOMMClick(oom) );
  m->add( CAMERA_ZOOM, new OOMMCameraZoom(oom) );
}

/////////////////////////////////////////////////////////////////////////////
