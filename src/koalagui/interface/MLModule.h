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
  MLModule.h

  Stephane Rehel

  November 15 1996
*/

#ifndef __MLModule_h
#define __MLModule_h

#ifndef __IRectangle_h
#include "tools/IRectangle.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __Color_h
#include "tools/Color.h"
#endif

#ifndef __MLEventHandler_h
#include "graphics/MLEventHandler.h"
#endif

#ifndef __Key_h
#include "graphics/Key.h"
#endif

#ifndef __MLCommand_h
#include "MLCommand.h"
#endif

#ifndef __MLPopupHandler_h
#include "MLPopupHandler.h"
#endif

#ifndef __MLNode_h
#include "kernel/MLNode.h"
#endif

class MLCommandLine;
class SystemWindow;
class MLCanvas;
class Interface;
class MLButtonsPanel;
class MLStatusBars;
class VScrollingDialogs;
class MLModes;
class MLMode;
class MLFunctionsManager;
class MLFunctions;
class MLScrollingDialog;
class MLScrollingDialogs;
class MLUndoRedoStack;
class MLPicture;
class MLGammaCorrection;
class MLKeyboardHandler;
class GraphicsSystem;
class MLEventManager;
class MLFunction;

class MLWFileBlock;
class MLRFileBlock;

class MLConfig;

class MLKernel;
class MLAppRoot;
class KoalaNodeTypes;

/////////////////////////////////////////////////////////////////////////////

class MLModule: public MLNode,
                public MLEventHandler,
                public MLPopupHandler
{
  friend class MLCanvas;
  friend class Interface;
  friend class MLGlobalDialog;
  friend class MLScrollingDialogs;

public:
  //
  //    +----+ +----+   +----+ +----+   +-----------+   +-----------+
  //    |  1 | |  2 |   |  1 | |  2 |   |     12    |   |           |
  //    +----+ +----+   +----+ +----+   +-----------+   |    1234   |
  //    +----+ +----+   +-----------+   +----+ +----+   |           |
  //    |  3 | |  4 |   |     34    |   |  3 | |  4 |   |           |
  //    +----+ +----+   +-----------+   +----+ +----+   +-----------+
  //

  enum CANVAS_POSITION
    {
    CANVAS_1= 1,
    CANVAS_2= 2,
    CANVAS_3= 3,
    CANVAS_4= 4,
    CANVAS_12= 5,
    CANVAS_34= 6,
    CANVAS_1234= 7
    };

protected:
  MLString rc_group;

  Interface* interface;
  SystemWindow* root;
  SystemWindow* window;
  IRectangle mainArea;
  boolean _mapped;
  MLButtonsPanel* buttonsPanel;
  MLStatusBars* statusBars;
  VScrollingDialogs* scrollingDialogs;
  MLKeyboardHandler* moduleKeyboardHandler;

  MLModes* modes;
  MLFunctionsManager* functionsManager;
  MLFunctions* functions;

  MLMode* currentMode;

private:
  // updated by the interface, used by this
  MLUndoRedoStack* undoStack;
  MLUndoRedoStack* redoStack;

protected:
  MLString name;
  Color color;

  MLCanvas** canvas;
  int nCanvas;

  MLScrollingDialogs* dialogs;

  KoalaNodeTypes* koalaNodeTypes; // not owner

public:
  MLModule( Interface* _interface );

  virtual ~MLModule();

  virtual boolean init( MLAppRoot* _approot );

  virtual MLNodeType::Type getNodeType() const;

  KoalaNodeTypes* getKoalaNodeTypes() const;

  const MLString& getName() const;

  const Color& getColor() const;

  virtual void idle()
    {}

  virtual void map();
  virtual void unmap();
  Interface* getInterface() const
    {
    return interface;
    }

  MLConfig* getConfig() const;
  const MLString getConfigGroup() const
    {
    return rc_group;
    }

  void setModuleKeyboardHandler( MLKeyboardHandler* _moduleKeyboardHandler );

  virtual void draw();

  boolean mapped() const
    {
    return _mapped;
    }

  void post( MLCommand* command ) const;

  void postRefreshCanvas( int canvasID );

  virtual boolean createDialog( const MLString& name );

  virtual void refreshScreen( unsigned int what );

  void refreshInterface();

  void clearUndosRedos();
  void postCmdStop();
private:
  void postUndoRedo( boolean undo_it );

public:
  // return the undo command
  void postUndo()
    {
    postUndoRedo(true);
    }

  // return the redo command
  void postRedo()
    {
    postUndoRedo(false);
    }

  virtual void handleCommand( MLCommand* cmd );

  virtual boolean handleKey( Key key, boolean pressed );

  virtual void leaveFunction();
  virtual boolean enterFunction( int function_id );
  virtual boolean enterFunction( MLFunction* f );

  // f==0 if leaving current
  virtual void enteringFunction( MLFunction* f );

  MLMode* getMode( int matching_id = 0 ) const;

  void enterMode( int id );

  virtual void enterMode( MLMode* mm );
  virtual void leaveMode( MLMode* mm = 0 );

  MLFunctionsManager* getFunctionsManager() const;
  MLFunctions* getFunctions() const;

  // return true if currently executing a function
  boolean inFunction() const;
  boolean last_function_must_return() const;

  void validateFunction();

  // return a 256x192 picture of the screen
  MLPicture* getSnapshotOfScreen(); // in getSnapshot.C

  boolean isIconic() const;
  void pushWaitCursor();
  void  popWaitCursor();

  // from graphicsSystem
  MLGammaCorrection* getGammaCorrection() const;
  double getGammaCorrectionValue() const;

  MLEventManager* getEventManager() const;
  GraphicsSystem* getGraphicsSystem() const;

protected:
  void addCanvas( MLCanvas* c );
  void removeCanvas( MLCanvas* c );

  void unpressButtons(); // called by Interface

  void getCanvasConfig( int where, IPoint& position, IVector& size,
                        int& margins );

  virtual void printMessage( const MLString& message, int color = 1 );

  virtual boolean eventHandler( MLWindow*, MLEvent& );

  virtual void popupCommand( MLPopup* popup, int label );

  virtual void write( MLWFileBlock& block );
  virtual void read( MLRFileBlock& block );
  virtual void resetAll();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLModule_h
