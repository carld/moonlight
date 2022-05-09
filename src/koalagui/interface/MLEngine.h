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
  MLEngine.h

  Stephane Rehel

  November 14 1996
*/

#ifndef __MLEngine_h
#define __MLEngine_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __MLAllocObject_h
#include "MLAllocObject.h"
#endif

#ifndef __MLActionHandler_h
#include "MLActionHandler.h"
#endif

#ifndef __MLCommand_h
#include "MLCommand.h"
#endif

#ifndef __MLNode_h
#include "kernel/MLNode.h"
#endif

class MLEvent;
class MLCanvas;
class MLModule;
class SystemWindow;
class MLCanvasActionsPool;
class MLAction;
class MLMode;
class MLWFileBlock;
class MLRFileBlock;
class MLFunction;

/////////////////////////////////////////////////////////////////////////////

class MLEngine: public MLNode,
                public MLActionHandler
{
  friend class MLCanvas;
private:
  int _canvas_popup_label;

protected:
  MLCanvas* canvas;
  MLCanvasActionsPool* actions;

  MLMode* pointerMode; // current action mode
  MLAction* pointerAction;

public:
  MLEngine( MLCanvas* _canvas );

  virtual ~MLEngine();

  virtual MLNodeType::Type getNodeType() const;

  int getCanvasID() const;
  int getModuleID() const;

  MLCanvas* getCancas() const
    {
    return canvas;
    }
  MLModule* getModule() const;

  virtual void draw();

  virtual void enterMode( MLMode* mm );
  virtual void leaveMode( MLMode* mm );
  virtual boolean engineEventHandler( MLEvent& );

  SystemWindow* getGfxWindow() const;
  boolean mapped() const;

  void post( MLCommand* command ) const
    {
    command->module= getModuleID();
    command->canvas= getCanvasID();
    command->engine= getID();
    command->post();
    }

  void postRefresh();

  virtual void handleCommand( MLCommand* cmd );

protected:
  virtual boolean handleAction( MLAction* );

  boolean validPointerAction( MLAction* action ) const
    {
    return action != 0 &&
           pointerAction != 0 &&
           action == pointerAction &&
           pointerMode != 0;
    }

  MLMode* getMode( int matching_id = 0 ) const;

  virtual void write( MLWFileBlock& block );
  virtual void read( MLRFileBlock& block );

  virtual void resetAll();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLEngine_h
