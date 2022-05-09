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
  OOMFunctions.C

  Stephane Rehel

  October 19 1997
*/

#include "graphics/KeyCodes.h"

#include "interface/MLFunctions.h"

#include "OOMFunctions.h"
#include "OOMModule.h"

#include "functions/OOMFSelect.h"
#include "functions/OOMFCameraZoom.h"

/////////////////////////////////////////////////////////////////////////////

// static
void OOMFunctions::init( OOMModule* oom )
{
  if( oom == 0 )
    return;

  MLFunctions* functions= oom->getFunctions();

  if( functions == 0 )
    return;

#define add(id,function,key) functions->add( id, Key(key), new function(oom) )

  add( SELECT, OOMFSelect, ' ' );
  add( CAMERA_ZOOM, OOMFCameraZoom, 'z' );

#undef add
}

/////////////////////////////////////////////////////////////////////////////
