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
  MLCanvas.h

  Stephane Rehel

  November 15 1996
*/

#ifndef __MLCanvas_h
#define __MLCanvas_h

#ifndef __MLEventHandler_h
#include "graphics/MLEventHandler.h"
#endif

#ifndef __MLActionHandler_h
#include "MLActionHandler.h"
#endif

#ifndef __MLAllocObject_h
#include "MLAllocObject.h"
#endif

#ifndef __MLCommand_h
#include "MLCommand.h"
#endif

#ifndef __MLNode_h
#include "kernel/MLNode.h"
#endif

class SoftWindow;
class SystemWindow;
class MLModule;
class MLEngine;
class MLFont;
class MLCanvasAction;
class MLCanvasPopup;
class MLCanvasButton;
class MLMode;
class MLWFileBlock;
class MLRFileBlock;

/////////////////////////////////////////////////////////////////////////////

class MLCanvas: public MLNode,
                public MLEventHandler,
                public MLActionHandler
{
  friend class MLCanvasActionsPool;
  friend class MLCanvasPopup;
  friend class MLCanvasButton;
  friend class MLEngine;
  friend class MLModule;

protected:
  MLModule* module;

  int init_where, where;

  MLFont* titleFont;
  int titleHeight;

  SoftWindow* mainWindow;
  SoftWindow* titleWindow;
  SystemWindow* gfxWindow;
  int blackMargin;

  MLCanvasPopup* enginesPopup;
  MLCanvasButton* minmaxButton;

  MLCanvasActionsPool* actions; // not owner

  MLEngine* engine;

  MLEngine** engines;
  int nEngines;

public:
  MLCanvas( MLModule* _module );

  virtual ~MLCanvas();

  virtual MLNodeType::Type getNodeType() const;

  virtual void create( int _where );

  int getModuleID() const;

  MLModule* getModule() const
    {
    return module;
    }
  MLEngine* getEngine() const
    {
    return engine;
    }

  SystemWindow* getGfxWindow() const
    {
    return gfxWindow;
    }

  void map();
  void unmap();

  void titleDraw();
  void gfxDraw();

  void post( MLCommand* command ) const
    {
    command->module= getModuleID();
    command->canvas= getID();
    command->post();
    }
  void postRefresh();

  void setActionsPool( MLCanvasActionsPool* _actions );

  virtual void handleCommand( MLCommand* cmd );

protected:
  virtual boolean eventHandler( MLWindow*, MLEvent& );

  virtual boolean mainWindowEventHandler( MLEvent& );

  virtual boolean gfxWindowEventHandler( MLEvent& );

public:
  boolean mapped() const;

protected:
  void position( int _where );
  void positionActions();

  virtual boolean handleAction( MLAction* action );

  virtual void enterMode( MLMode* mm );
  virtual void leaveMode( MLMode* mm );

  virtual void write( MLWFileBlock& block );
  virtual void read( MLRFileBlock& block );

  virtual void resetAll();

public:
  void activateEngine( int engineID );
  void addEngine( MLEngine* _engine );
  void removeEngine( MLEngine* _engine );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLCanvas_h
