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
  InitGraphicsSystem.C

  Stephane Rehel
  June 29, 1996
*/

#include "InitGraphicsSystem.h"

#include "GraphicsSystem.h"
#include "MLEventManager.h"

#ifdef __X11__
#include "X11/XGraphicsSystem.h"
#include "X11/XEventManager.h"
#endif

#ifdef __WINDOWS__
#include "Win/WGraphicsSystem.h"
#include "Win/WEventManager.h"
#endif

/////////////////////////////////////////////////////////////////////////////

boolean InitGraphicsSystem( MLConfig* config, MLCommandLine& options )
{
  assert( GraphicsSystem::getGraphicsSystem() == 0 );
  assert( MLEventManager::getEventManager() == 0 );

  GraphicsSystem* graphicsSystem= 0;

#ifdef __X11__
  graphicsSystem= (GraphicsSystem*) new XGraphicsSystem;
  if( ! graphicsSystem->init(config,options) )
    return false;

//  eventManager= (MLEventManager*)
  new XEventManager;
  return true;
#endif

#ifdef __WINDOWS__
  graphicsSystem= (GraphicsSystem*) new WGraphicsSystem;
  if( ! graphicsSystem->init(config,options) )
    return false;

//  eventManager= (MLEventManager*)
  new WEventManager;
  return true;
#endif

  return false;
}

/////////////////////////////////////////////////////////////////////////////
