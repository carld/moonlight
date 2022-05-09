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
  Interface.h

  Stephane Rehel

  July 8 1996
*/

#ifndef __Interface_h
#define __Interface_h

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __MLEventHandler_h
#include "graphics/MLEventHandler.h"
#endif

#ifndef __MLKeyboardHandler_h
#include "graphics/MLKeyboardHandler.h"
#endif

#ifndef __MLEventGrabber_h
#include "graphics/MLEventGrabber.h"
#endif

#ifndef __MLThread_h
#include "tools/thread/MLThread.h"
#endif

class MLString;
class MLCommandLine;
class SystemWindow;
class SoftWindow;
class MLModulesBar;
class MLModule;
class MLCanvas;
class MLEngine;
class Key;
class MLCommand;
class MLCommandList;
class MLEventManager;
class GraphicsSystem;
class MLGammaCorrection;
class MLConfig;

class MLKernel;
class MLAppRoot;
class InterfaceAppRoot;
class KoalaNodeTypes;

/////////////////////////////////////////////////////////////////////////////

// Well, there's a thread for waiting either an X event, either
// a refresh screen request (from the lighting thread)
// Our thread waits for an X event

class Interface: public MLEventHandler, public MLKeyboardHandler,
                                        public MLEventGrabber
{
  friend class MLModule;
  friend class MLModulesBar;

protected:
  MLString programName;

  IVector screenSize;
  SystemWindow* window;

  int currentModule;

  int* modules;
  int nModules;

  MLModulesBar* modulesBar;

  static int buttonsPanelWidth;
  static int scrollingDialogsWidth;
  static int modulesBarHeight;
  static int statusLinesHeight;

  MLCommandList* cmdList;

  MLMutex postRefreshMutex;

  SystemWindow* exclusiveModeWindow;

  MLKernel* kernel;

  InterfaceAppRoot* interfaceAppRoot;

public:
  Interface( MLKernel* _kernel );

  virtual ~Interface();

  boolean init();

  InterfaceAppRoot* getInterfaceAppRoot() const;
  MLAppRoot* getAppRoot() const;
  MLKernel* getKernel() const;
  KoalaNodeTypes* getKoalaNodeTypes() const;

  MLConfig* getConfig() const;

  // return 0 if error
  MLModule* queryAppModule( const MLString& name, MLString* errorMsg = 0 );

  void post( MLCommand* command );

  void postRefreshCanvas( int canvasID );
  void postRefreshDialog( int dialogID );

  void callIdleModulesFunction();

protected:
  boolean last_function_must_return() const;

public:
  // check for refresh, and post refresh commands if necessary
  // return true if last_function_must_return
  boolean checkForRefresh();

  // get events, send commands, and return
  // return true if last_function_must_return
  boolean runloop();

  // constantly call runloop()
  void mainloop();

  // return true if last_function_must_return
  boolean sendCommands();

  boolean inExclusiveMode() const;
  SystemWindow* enterExclusiveMode();
  void leaveExclusiveMode();

protected:
  void handleCommand( MLCommand* cmd );

public:
  void refreshInterface();

protected:
  virtual boolean eventHandler( MLWindow*, MLEvent& );
  virtual boolean eventGrabber( MLWindow*, MLEvent& );

public:
  void newModule( int id );
  void activateModule( int id );
  MLModule* getCurrentModule() const;
  boolean currentModuleInFunction() const;

protected:
  virtual boolean handleMLKeyboard( MLEvent& event );

public:
  // used by exclusive modes
  boolean testIconifyingKey( const Key& key );

  MLEventManager* getEventManager() const;
  GraphicsSystem* getGraphicsSystem() const;

  boolean isIconic() const;
  void pushWaitCursor();
  void popWaitCursor();
  MLGammaCorrection* getGammaCorrection() const;
  double getGammaCorrectionValue() const;

private:
  void pressKey( const Key& key );
  void unpressKey( const Key& key );
};

/////////////////////////////////////////////////////////////////////////////

extern Interface* interface;

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __Interface_h
