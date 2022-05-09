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
  MLLightingProcess.C

  Stephane Rehel
  February 16 1997
  Full rewritten: March 21 1997
*/

#include <stdio.h>

#include "scene/MLObject.h"
#include "scene/MLMesh.h"
#include "scene/MLMeshesIterator.h"
#include "scene/MLScene.h"
#include "scene/lighting/MLPoolTable.h"
#include "scene/lighting/MLLightPool.h"
#include "scene/lighting/MLMeshLighting.h"
#include "scene/lighting/MLPolygonEnergySource.h"
#include "scene/lighting/MLShooter.h"

#include "ModelModule.h"
#include "ModelFlags.h"

#include "MLLightingProcess.h"

#ifndef NDEBUG
//  #define LP_DEBUG 1
#endif

#ifdef LP_DEBUG
  #include "tools/Chrono.h"
#endif

// define this if you want the lighting process to support sleeping
// (when slow mesa drawing for example)
#define SUPPORT_SLEEPING

/////////////////////////////////////////////////////////////////////////////

MLLightingProcess::MLLightingProcess( ModelModule* _model )
{
  model= _model;

  yes_quit= false;

  previous_sleeping_state= int(MLLightingControl::RUNNING);

  created= false;
  _inDirectDiffusing= false;
  _inIndirectDiffusing= false;

  step= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLLightingProcess::~MLLightingProcess()
{
  quitThread();
}

/////////////////////////////////////////////////////////////////////////////
// called by the interface
//

void MLLightingProcess::sleep()
{
#ifdef SUPPORT_SLEEPING
  if( control.state != MLLightingControl::RUNNING )
    return;
  previous_sleeping_state= control.state;
  control.state= MLLightingControl::SLEEPING;
#endif
}

/////////////////////////////////////////////////////////////////////////////

// if sleeping
void MLLightingProcess::wakeup()
{
#ifdef SUPPORT_SLEEPING
  if( control.state != MLLightingControl::SLEEPING )
    return;
  control.state= previous_sleeping_state;
  control.sleep.signal();
#endif
}

/////////////////////////////////////////////////////////////////////////////

// Make the lighting process work
// Nothing happens is already computing
void MLLightingProcess::compute()
{
  if( control.state == MLLightingControl::SLEEPING ||
      control.state == MLLightingControl::RUNNING )
    return; // well, it's already running!

  cond.signal();

  control.state= MLLightingControl::RUNNING;
}

/////////////////////////////////////////////////////////////////////////////

void MLLightingProcess::quitThread()
{
  // first, let's reset
  stop();

  // lighting process is now waiting for a new event
  // Tell him to cancel its thread
  yes_quit= true;
  cond.signal();
}

/////////////////////////////////////////////////////////////////////////////

// Called by the interface thread
// Force the lighting process to stop & wait for it to stop
void MLLightingProcess::stop()
{
  if( ! created )
    return;

  if( control.state == MLLightingControl::STOPPED )
    return;

#ifdef LP_DEBUG
  Chrono c; c.start();
  fprintf( stderr, "Wait for Cancelling\n");
#endif

  control.state= MLLightingControl::CANCELLING;
  control.sleep.signal(); // wake it up if it was sleeping

  // now, try to enter the 'idle' area to make sure
  // the lighting process is stopped
  idle.lock();

  // success!!
  idle.unlock();

  control.state= MLLightingControl::STOPPED;

#ifdef LP_DEBUG
  fprintf( stderr, "Cancelling delay= %f\n", c.peek());
#endif
}

/////////////////////////////////////////////////////////////////////////////

// Threaded Lighting process
//
void MLLightingProcess::runThread(void*)
{
#ifdef LP_DEBUG
  fprintf( stderr, "MLLightingProcess: thread running\n" );
#endif

  created= true;
  yes_quit= false;

  for(;;)
    {
stopped_state:

#ifdef LP_DEBUG
    fprintf(stderr,"MLP: WAITING\n");
#endif

    _inDirectDiffusing= false;
    _inIndirectDiffusing= false;

    // Release 'idle' since we enter a idle area
    idle.unlock();

    // wait for a signal. Current state is: STOPPED
    cond.wait();

    if( yes_quit )
      break; // god, we've got to exit immediatly! What's happening?

    // We enter the 'computing' area
    idle.lock();

    if( model->getFlags()->computeDirectLighting )
      {
      if( ! checkForDirtyPools() )
        goto stopped_state;

      if( ! checkForDirtyObjects() )
        goto stopped_state;
      }

    _inDirectDiffusing= false;

    if( model->getFlags()->computeIndirectLighting )
      shootIndirect();

    _inIndirectDiffusing= false;
    }

#ifdef LP_DEBUG
  fprintf( stderr, "MLLightingProcess: thread exiting\n" );
#endif

  exitThread();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLLightingProcess::inIndirectDiffusing() const
{
  return _inIndirectDiffusing;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLLightingProcess::inDirectDiffusing() const
{
  return _inDirectDiffusing;
}

/////////////////////////////////////////////////////////////////////////////

int MLLightingProcess::getStep() const
{
  return step;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLLightingProcess::checkForDirtyPools()
{
  MLScene* scene= model->getScene();
  if( scene == 0 )
    return true;

  if( ! scene->dirtyPoolsLighting )
    return true;

#ifdef LP_DEBUG
  fprintf( stderr, "check for dirty pools.\n" );
  int nDirty= 0;
#endif

  _inDirectDiffusing= true;
  _inIndirectDiffusing= false;

  MLPoolTable* poolTable= scene->getPoolTable();

  if( poolTable == 0 )
    return true;

  for( int i= 1; i <= poolTable->getNPools(); ++i )
    {
    MLLightPool* pool= poolTable->getPool(i);

    if( pool == 0 )
      continue;

    if( ! pool->isDirty() )
      continue;

#ifdef LP_DEBUG
    ++nDirty;
#endif

    ++step;
    if( ! pool->shoot(control) )
      {
     _inDirectDiffusing= false;
#ifdef LP_DEBUG
      fprintf( stderr, "Cancelled.\n" );
#endif
      return false;
      }

    model->refreshScreen(ModelModule::REFRESH_ILLUM);
    }

  _inDirectDiffusing= false;

  scene->dirtyPoolsLighting= false;

#ifdef LP_DEBUG
  fprintf( stderr, "Found dirty pools: %d\n", nDirty );
#endif

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLLightingProcess::checkForDirtyObjects()
{
  MLScene* scene= model->getScene();
  if( scene == 0 )
    return true;

  if( ! scene->dirtyObjectsLighting )
    return true;

#ifdef LP_DEBUG
  fprintf( stderr, "check for dirty objects.\n" );
  int nDirty= 0;
#endif

  _inDirectDiffusing= true;
  _inIndirectDiffusing= false;

  MLMeshesIterator mi(scene);
  while( ! mi.eol() )
    {
    MLMesh* m= mi++;
    MLMeshLighting* lighting= m->getLighting();
    if( lighting == 0 )
      continue;

    if( ! lighting->isDirty() )
      continue;

#ifdef LP_DEBUG
    ++nDirty;
fprintf(stderr,"Dirty object: '%s'\n",m->getName().name.get());
#endif

    ++step;
    if( ! lighting->shoot(control) )
      {
      _inDirectDiffusing= false;
#ifdef LP_DEBUG
      fprintf( stderr, "Cancelled.\n" );
#endif
      return false;
      }

    model->refreshScreen(ModelModule::REFRESH_ILLUM);
    }

  _inDirectDiffusing= false;

  scene->dirtyObjectsLighting= false;

#ifdef LP_DEBUG
  fprintf( stderr, "Found dirty objects: %d\n", nDirty );
#endif

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLLightingProcess::shootIndirect()
{
  MLScene* scene= model->getScene();
  if( scene == 0 )
    return true;

  int i;
  MLPoolTable* poolTable= scene->getPoolTable();

  if( poolTable == 0 )
    return true;

  for( i= 1; i <= poolTable->getNPools(); ++i )
    {
    MLLightPool* pool= poolTable->getPool(i);

    if( pool == 0 )
      continue;

    if( pool->dirtyIndirect )
      pool->initiateIndirect();
    }

  MLShooter shooter(scene,&control);
  for(;;)
    {
    _inDirectDiffusing= false;
    _inIndirectDiffusing= true;

    ++step;
    if( ! shooter.indirectStep() )
      break;

#ifdef LP_DEBUG2
    fprintf( stderr, "Indirect diffusing step.\n" );
#endif
    model->refreshScreen(ModelModule::REFRESH_ILLUM);
    }

  _inIndirectDiffusing= false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////
