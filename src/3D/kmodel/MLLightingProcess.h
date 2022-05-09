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
  MLLightingProcess.h

  Stephane Rehel
  February 16 1997
*/

#ifndef __MLLightingProcess_h
#define __MLLightingProcess_h

#ifndef __SIList_h
#include "tools/SIList.h"
#endif

#ifndef __MLThread_h
#include "tools/thread/MLThread.h"
#endif

#ifndef __MLLightingControl_h
#include "scene/lighting/MLLightingControl.h"
#endif

class MLScene;
class ModelModule;

/////////////////////////////////////////////////////////////////////////////

class MLLightingProcess: public MLRunnable
{
protected:
  ModelModule* model;

  MLLightingControl control;

  MLThreadCond cond;
  boolean yes_quit;

  MLMutex idle;

  int previous_sleeping_state;
  boolean created;

  boolean _inDirectDiffusing;
  boolean _inIndirectDiffusing;

  int step;

public:
  MLLightingProcess( ModelModule* _model );

  virtual ~MLLightingProcess();

  //////////////////////////////////////////////////////////////////////////
  // called by the interface
  //
  void sleep();
  void wakeup();

  // Make the lighting process work
  // Nothing happens is already computing
  void compute();

  // Force the lighting process to stop and wait for it to stop
  void stop();

  void quitThread();

  //////////////////////////////////////////////////////////////////////////
  // Lighting process
  //
  void runThread(void*);

  boolean inDirectDiffusing() const;
  boolean inIndirectDiffusing() const;
  int getStep() const;

private:
  boolean checkForDirtyPools();
  boolean checkForDirtyObjects();
  boolean shootIndirect();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLLightingProcess_h
