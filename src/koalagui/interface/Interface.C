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
  Interface.C

  Stephane Rehel

  July 8 1996
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"

#include "tools/MLCommandLine.h"
#include "tools/MLGammaCorrection.h"

#include "kernel/MLKernel.h"
#include "kernel/MLAppRoot.h"

#include "graphics/mlgl.h"
#include "graphics/GraphicsSystem.h"
#include "graphics/SystemWindow.h"
#include "graphics/Key.h"
#include "graphics/KeyCodes.h"
#include "graphics/MLEvent.h"
#include "graphics/MLEventManager.h"

#include "koala/VScrollingDialogs.h"
#include "koala/VScrollingCartouche.h"
#include "koala/VDialogManager.h"

#include "Interface.h"
#include "InterfaceAppRoot.h"
#include "KoalaNodeTypes.h"

#include "MLAllocator.h"
#include "MLInterfaceCommand.h"

#include "MLCommandList.h"
#include "MLUndoRedoStack.h"

#include "MLCanvas.h"

#include "MLButtonsPanel.h"
#include "MLModulesBar.h"
#include "MLScrollingDialog.h"
#include "MLStatusBars.h"
#include "MLModule.h"

#include "MLInterfaceVersion.h"

#include "startup/MLStartup.h"

#include "dialogs/MLDQuitInterface.h"

/////////////////////////////////////////////////////////////////////////////

Interface* interface= 0;

// static
int Interface::buttonsPanelWidth= 0;
int Interface::scrollingDialogsWidth= 0;
int Interface::modulesBarHeight= 0;
int Interface::statusLinesHeight= 0;

static const int max_modules= 100;

static const int max_pressed_keys= 256;
static Key pressedKeys[max_pressed_keys];
static int nPressedKeys= 0;
static Key currentPressedKey;

static const int max_refresh_canvas= 50;
static int refresh_canvas[max_refresh_canvas];
static int n_refresh_canvas= 0;

static const int max_refresh_dialogs= 100;
static int refresh_dialogs[max_refresh_dialogs];
static int n_refresh_dialogs= 0;

static Key iconifyingKey;

/////////////////////////////////////////////////////////////////////////////

Interface::Interface( MLKernel* _kernel )
{
  kernel= _kernel;

  interfaceAppRoot= new InterfaceAppRoot(kernel,this);

  exclusiveModeWindow= 0;

  nPressedKeys= 0;
  currentPressedKey= Key::NULL_KEY;

  cmdList= new MLCommandList;
  cmdList->setMTSafe();

  MLAllocator::init(this);

  // alloc interface commands base
  MLInterfaceCommand::initInterfaceCommands();

  iconifyingKey= KeyCodes::ALT_F9;

  GraphicsSystem* graphicsSystem= getGraphicsSystem();
  assert( graphicsSystem != 0 );

  window= graphicsSystem->getRootWindow();
  screenSize= graphicsSystem->getScreenSize();
  graphicsSystem->autoRepeat(false);

  window->setEventHandler(this);

  modulesBar= 0;

  // printed in modules bar
  programName=  MLString("ML ")
              + MLString(MLInterfaceVersion::version())
              + MLString(" ")
              + MLString(MLInterfaceVersion::build_cpu())
              + MLString("-")
              + MLString(MLInterfaceVersion::build_vendor())
              + MLString("-")
              + MLString(MLInterfaceVersion::build_os());

  graphicsSystem->setApplicationName("Moonlight-Creator");

  currentModule= 0;

  modules= new int [ max_modules + 1 ];
  for( int i= 0; i <= max_modules; ++i )
    modules[i]= 0;
  nModules= 0;
}

/////////////////////////////////////////////////////////////////////////////

Interface::~Interface()
{
  leaveExclusiveMode();

  postRefreshMutex.unlock(); // safe unlock

  GraphicsSystem* graphicsSystem= getGraphicsSystem();
  if( graphicsSystem != 0 )
    graphicsSystem->autoRepeat(true);

  if( modules != 0 )
    {
    const int _nModules= nModules;
    for( int i= _nModules; i >= 1; --i )
      {
      MLModule* m= MLAllocator::getModule(modules[i]);
      modules[i]= 0;
      --nModules;
      delete m;
      }
    delete modules;
    modules= 0;
    }

  currentModule= 0;
  nModules= 0;

  delete modulesBar;
  modulesBar= 0;

  delete cmdList;
  cmdList= 0;

  delete interfaceAppRoot;
  interfaceAppRoot= 0;

  kernel= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean Interface::init()
{
  if( kernel == 0 || interfaceAppRoot == 0 )
    return false;

  if( ! interfaceAppRoot->init() )
    return false;

//  MLCommandLine& command= kernel->getCommandLine();

  window->currentPixel();
  mlFront();
  window->clear(0,0,0);
  glFlush();

  GraphicsSystem* graphicsSystem= getGraphicsSystem();

  if( graphicsSystem->isReallyFullScreen() )
    {
    MLStartup* startup= new MLStartup(this);
    startup->run();
    delete startup;
    startup= 0;
    }

  int dialogsWidth= VScrollingCartouche::probeCartoucheSize
                                           ( IVector(200,200), true ).x();

  dialogsWidth+= VScrollingDialogs::getScrollBarWidth();

  Interface::buttonsPanelWidth= 90;
  Interface::scrollingDialogsWidth= dialogsWidth;
  Interface::modulesBarHeight= 30;
  Interface::statusLinesHeight= 38;

  modulesBar= new MLModulesBar(this);
  modulesBar->create();

  window->postRedraw();

  MLEventManager* eventManager= getEventManager();
  eventManager->setKeyboardFocus(this);
  eventManager->setEventGrabber(this);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

InterfaceAppRoot* Interface::getInterfaceAppRoot() const
{
  return interfaceAppRoot;
}

/////////////////////////////////////////////////////////////////////////////

MLAppRoot* Interface::getAppRoot() const
{
  return interfaceAppRoot;
}

/////////////////////////////////////////////////////////////////////////////

MLKernel* Interface::getKernel() const
{
  return kernel;
}

/////////////////////////////////////////////////////////////////////////////

KoalaNodeTypes* Interface::getKoalaNodeTypes() const
{
  if( interfaceAppRoot == 0 )
    return 0;

  return interfaceAppRoot->getNodeTypes();
}

/////////////////////////////////////////////////////////////////////////////

MLConfig* Interface::getConfig() const
{
  if( kernel == 0 )
    return 0;

  return kernel->getConfig();
}

/////////////////////////////////////////////////////////////////////////////

// return 0 if error
MLModule* Interface::queryAppModule( const MLString& name,
                                     MLString* errorMsg /*= 0*/ )
{
  if( kernel == 0 || interfaceAppRoot == 0 )
    return 0;

  MLAppRoot* _approot= kernel->loadModule( name, interfaceAppRoot,
                                           errorMsg );
  if( _approot == 0 )
    return 0;

/*
  KoalaNodeTypes* types= interfaceAppRoot->getNodeTypes();

  if( ! ar->nodeOf(types->module) )
    {
    if( errorMsg != 0 )
      *errorMsg= MLString::printf( "Plug-in %s is not a Koala Module!",
                                   name.get() );

    return 0;
    }
*/

  MLNode* _module= _approot->getChild("KoalaModule");
  if( _module == 0 )
    return 0;

  MLModule* module= (MLModule*) _module;

  newModule(module->getID());

  return module;
}

/////////////////////////////////////////////////////////////////////////////

void Interface::post( MLCommand* command )
{
  if( command == 0 )
    return;

  boolean isInterfaceCmd= command->getBase() == MLInterfaceCommand::BASE;
  boolean isStop= isInterfaceCmd &&
                  command->getType() == MLInterfaceCommand::CMD_STOP;

  if( isStop )
    {
    // are we posting a CMD_STOP after another CMD_STOP for the same module??
    MLCommand* last= cmdList->peek();
    if( last != 0 )
      {
      if( last->getBase() == MLInterfaceCommand::BASE &&
          last->getType() == MLInterfaceCommand::CMD_STOP &&
          last->module == command->module &&
          last->isUndo == command->isUndo &&
          last->isUserCommand == command->isUserCommand &&
          last->lonelyCommand == command->lonelyCommand &&
          last->canUndo == command->canUndo )
        return; // unnecessary
      }
    }

  cmdList->post(command);

  if( !isInterfaceCmd && !isStop && command->lonelyCommand )
    {
    // we've got to post an CMD_STOP command
    MLInterfaceCommand* icmd= new MLInterfaceCommand;
    icmd->module= command->module;
    icmd->type= MLInterfaceCommand::CMD_STOP;
    cmdList->post(icmd);
    }
}

/////////////////////////////////////////////////////////////////////////////

void Interface::postRefreshCanvas( int canvasID )
{
  if( MLAllocator::getCanvas(canvasID) == 0 )
    return;

  if( n_refresh_canvas >= max_refresh_canvas )
    return;

  postRefreshMutex.lock();
  for( int i= 0; i < n_refresh_canvas; ++i )
    if( refresh_canvas[i] == canvasID )
      {
      postRefreshMutex.unlock();
      return;
      }

  refresh_canvas[ n_refresh_canvas++ ]= canvasID;

  postRefreshMutex.unlock();
}

/////////////////////////////////////////////////////////////////////////////

void Interface::postRefreshDialog( int dialogID )
{
  if( MLAllocator::getScrollingDialog(dialogID) == 0 )
    return;

  if( n_refresh_dialogs >= max_refresh_dialogs )
    return;

  postRefreshMutex.lock();
  for( int i= 0; i < n_refresh_dialogs; ++i )
    if( refresh_dialogs[i] == dialogID )
      {
      postRefreshMutex.unlock();
      return;
      }

  refresh_dialogs[ n_refresh_dialogs++ ]= dialogID;

  postRefreshMutex.unlock();
}

/////////////////////////////////////////////////////////////////////////////

// this is called when there aren't any more events, and just
// before waiting for other events
void Interface::callIdleModulesFunction()
{
  for( int i= 1; i <= nModules; ++i  )
    {
    MLModule* module= MLAllocator::getModule(modules[i]);
    if( module == 0 )
      continue;

    module->idle();
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean Interface::last_function_must_return() const
{
  MLModule* current= getCurrentModule();
  if( current == 0 )
    return false;

  return current->last_function_must_return();
}

/////////////////////////////////////////////////////////////////////////////

boolean Interface::checkForRefresh()
{
  postRefreshMutex.lock();
  if( n_refresh_canvas > 0 )
    {
    for( int i= 0; i < n_refresh_canvas; ++i )
      {
      MLCanvas* c= MLAllocator::getCanvas( refresh_canvas[i] );
      if( c == 0 )
        continue;

      MLInterfaceCommand* icmd= new MLInterfaceCommand;
      icmd->type= MLInterfaceCommand::REFRESH_CANVAS_i;
      icmd->module= c->getModuleID();
      icmd->index= refresh_canvas[i];
      icmd->post();
      }
    n_refresh_canvas= 0;

    postRefreshMutex.unlock();
    if( sendCommands() )
      return true;
    }
   else
    postRefreshMutex.unlock();

  postRefreshMutex.lock();
  if( n_refresh_dialogs > 0 )
    {
    for( int i= 0; i < n_refresh_dialogs; ++i )
      {
      MLScrollingDialog* sd=
                     MLAllocator::getScrollingDialog( refresh_dialogs[i] );
      if( sd == 0 )
        continue;

      MLInterfaceCommand* icmd= new MLInterfaceCommand;
      icmd->type= MLInterfaceCommand::REFRESH_DIALOG_i;
      icmd->module= sd->getModuleID();
      icmd->index= refresh_dialogs[i];
      icmd->post();
      }
    n_refresh_dialogs= 0;

    postRefreshMutex.unlock();
    if( sendCommands() )
      return true;
    }
   else
    postRefreshMutex.unlock();

  return false;
}

/////////////////////////////////////////////////////////////////////////////

// get events, send commands, and return
// return true if last_function_must_return
boolean Interface::runloop()
{
  MLEventManager* eventManager= getEventManager();

  eventManager->get();

  boolean idle;

  idle= eventManager->empty() &&
        cmdList->empty() &&
        n_refresh_dialogs == 0 &&
        n_refresh_canvas == 0;

  if( idle )
    {
    callIdleModulesFunction();

    if( last_function_must_return() )
      return true;
    }

  idle= eventManager->empty() &&
        cmdList->empty() &&
        n_refresh_dialogs == 0 &&
        n_refresh_canvas == 0;

  if( idle )
    {
    // ok, really idle time!!!
    eventManager->wait();
    }

  while( eventManager->dispatch(true) )
    {
    if( last_function_must_return() )
      return true;
    }

  if( last_function_must_return() )
    return true;

  if( sendCommands() )
    return true;

  return checkForRefresh();
}

/////////////////////////////////////////////////////////////////////////////

void Interface::mainloop()
{
  for(;;)
    {
    if( runloop() )
      break;
    }
}

/////////////////////////////////////////////////////////////////////////////

// return true if last_function_must_return
boolean Interface::sendCommands()
{
  MLModule* m= 0;

  for(;;)
    {
    MLCommand* cmd= cmdList->get();

    if( cmd == 0 )
      break;

    if( cmd->module == 0 )
      {
      // it's for me
      handleCommand(cmd);
      goto end_it;
      }

    m= MLAllocator::getModule(cmd->module);

    if( m != 0 )
      {
      boolean isStop= cmd->getBase() == MLInterfaceCommand::BASE &&
                      cmd->getType() == MLInterfaceCommand::CMD_STOP;
      if( ! isStop )
        m->handleCommand(cmd);

      if( cmd->isUserCommand && cmd->canUndo )
        m->redoStack->clear();

      MLCommand* undo= cmd->getUndo();
      if( undo != 0 )
        {
        undo->isUserCommand= 0;

        if( cmd->isUndo )
          m->redoStack->push(undo);
         else
          m->undoStack->push(undo);
        }

      if( cmd->getBase() == MLInterfaceCommand::BASE &&
          cmd->getType() == MLInterfaceCommand::POPUP )
        m->unpressButtons();
      }

end_it:
    ::unref(cmd);

    if( last_function_must_return() )
      return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean Interface::inExclusiveMode() const
{
  return exclusiveModeWindow != 0;
}

/////////////////////////////////////////////////////////////////////////////

SystemWindow* Interface::enterExclusiveMode()
{
  if( exclusiveModeWindow != 0 )
    return exclusiveModeWindow;

  MLModule* module= getCurrentModule();
  if( module != 0 )
    module->unmap();

  GraphicsSystem* graphicsSystem= getGraphicsSystem();

  exclusiveModeWindow= graphicsSystem->newFullScreenWindow();
  exclusiveModeWindow->map();

  return exclusiveModeWindow;
}

/////////////////////////////////////////////////////////////////////////////

void Interface::leaveExclusiveMode()
{
  if( exclusiveModeWindow == 0 )
    return;

  delete exclusiveModeWindow;
  exclusiveModeWindow= 0;

  MLModule* module= getCurrentModule();
  if( module != 0 )
    module->map();
}

/////////////////////////////////////////////////////////////////////////////

void Interface::handleCommand( MLCommand* cmd )
{
  if( cmd == 0 )
    return;

  if( cmd->getBase() != MLInterfaceCommand::BASE )
    return;

  MLInterfaceCommand* icmd= (MLInterfaceCommand*) cmd;

  if( icmd->getType() == MLInterfaceCommand::QUIT )
    {
    MLDQuitInterface qi(this);
    int i= qi.run();

    if( i == MLDQuitInterface::ICONIFY )
      {
      GraphicsSystem* graphicsSystem= getGraphicsSystem();
      graphicsSystem->iconify(true);
      return;
      }

    if( i == MLDQuitInterface::REALLY_QUIT )
      {
      GraphicsSystem* graphicsSystem= getGraphicsSystem();
      graphicsSystem->autoRepeat(true);
      exit(0);
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

void Interface::refreshInterface()
{
  GraphicsSystem* graphicsSystem= getGraphicsSystem();

  if( graphicsSystem != 0 )
    graphicsSystem->refreshScreen();
}

/////////////////////////////////////////////////////////////////////////////

// protected
boolean Interface::eventHandler( MLWindow* w, MLEvent& event )
{
  switch( event.type )
    {
    case MLEvent::REDRAW:
      {
//      MLInterfaceCommand* icmd= new MLInterfaceCommand;
//      icmd->type= MLInterfaceCommand::REFRESH_MODULE;
//      icmd->module= currentModule;
//      icmd->post();

      window->currentPixel();

      mlFront();
      mlBlack();
      //glColor3f(0,1,0);
      mlRect( 0,
              screenSize.y() - Interface::modulesBarHeight - 1,
              Interface::buttonsPanelWidth - 1,
              screenSize.y() - 1 );
      //glColor3f(1,0,0);
      mlRect( screenSize.x() - Interface::scrollingDialogsWidth - 1,
              screenSize.y() - Interface::modulesBarHeight - 1,
              screenSize.x() - 1,
              screenSize.y() - 1 );
      //glColor3f(1,0,1);
      mlRect( 0, 0,
              Interface::buttonsPanelWidth-1 + 1,
              Interface::statusLinesHeight-1 );
      //glColor3f(1,1,0);
      mlRect( screenSize.x() - Interface::scrollingDialogsWidth - 2,
              0,
              screenSize.x() - 1,
              Interface::statusLinesHeight-1 + 1 );
      //glColor3f(0,1,1);
      mlRect( screenSize.x() - Interface::scrollingDialogsWidth - 1,
              screenSize.y() - Interface::modulesBarHeight - 2,
              screenSize.x() - Interface::scrollingDialogsWidth - 1,
              Interface::statusLinesHeight );
      //glColor3f(0,1,0);
      //mlRect(0,0,screenSize.x()-1,screenSize.y()-1);

      if( modulesBar != 0 )
        modulesBar->postRedraw();

      MLModule* current= getCurrentModule();
      if( current != 0 )
        {
        if( current->buttonsPanel != 0 )
          current->buttonsPanel->postRedraw();
        if( current->statusBars != 0 )
          current->statusBars->postRedraw();
        if( current->scrollingDialogs != 0 )
          current->scrollingDialogs->postDrawScrollBar();
        }

      break;
      }

    default:
      return w->handleEvent(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean Interface::eventGrabber( MLWindow* w, MLEvent& event )
{
  if( event.type == MLEvent::KEY_PRESS ||
      event.type == MLEvent::KEY_RELEASE )
    return handleMLKeyboard(event);

  MLModule* current= getCurrentModule();

  if( current != 0 && event.type != MLEvent::REDRAW )
    {
    if( current->inFunction() )
      {
      // no event for dialogs if we are in a function
      MLWindow* ww= w;
      while( ww != 0 )
        {
        if( current->scrollingDialogs == ww )
          return true;
        if( ww->isSystemWindow() )
          ww= ((SystemWindow*)ww)->getParent();
         else
          ww= ((SoftWindow*)ww)->getSystemParent();
        }
      }
    }

  MLEventManager* eventManager= getEventManager();

  return eventManager->sendEvent(w,event,false);
}

/////////////////////////////////////////////////////////////////////////////

void Interface::newModule( int id )
{
  if( MLAllocator::getModule(id) == 0 )
    return;

  if( nModules >= ::max_modules )
    {
    fprintf( stderr, "panic: too many modules (%d)\n", ::max_modules );
    return;
    }

  ++nModules;
  modules [ nModules ]= id;

  if( modulesBar != 0 )
    modulesBar->appendModule(id);
}

/////////////////////////////////////////////////////////////////////////////

void Interface::activateModule( int id )
{
  if( currentModule == id )
    return;
  if( MLAllocator::getModule(id) == 0 )
    return;

  if( currentModuleInFunction() )
    return; // this module should terminate the function first

  // flush pressed keys
  while( nPressedKeys > 0 )
    unpressKey(pressedKeys[0]);

  int oldModule= currentModule;
  currentModule= id;

  if( modulesBar != 0 )
    modulesBar->postRedraw();

  if( MLAllocator::getModule(oldModule) != 0 )
    MLAllocator::getModule(oldModule)->unmap();

  MLAllocator::getModule(currentModule)->map();
}

/////////////////////////////////////////////////////////////////////////////

MLModule* Interface::getCurrentModule() const
{
  return MLAllocator::getModule(currentModule);
}

/////////////////////////////////////////////////////////////////////////////

boolean Interface::currentModuleInFunction() const
{
  MLModule* current= getCurrentModule();
  if( current == 0 )
    return false;

  return current->inFunction();
}

/////////////////////////////////////////////////////////////////////////////

boolean Interface::testIconifyingKey( const Key& key )
{
  if( key == iconifyingKey )
    {
    GraphicsSystem* graphicsSystem= getGraphicsSystem();
    graphicsSystem->iconify(true);
    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

MLEventManager* Interface::getEventManager() const
{
  return MLEventManager::getEventManager();
}

/////////////////////////////////////////////////////////////////////////////

GraphicsSystem* Interface::getGraphicsSystem() const
{
  return GraphicsSystem::getGraphicsSystem();
}

/////////////////////////////////////////////////////////////////////////////

boolean Interface::isIconic() const
{
  GraphicsSystem* graphicsSystem= getGraphicsSystem();

  if( graphicsSystem == 0 )
    return true;

  return graphicsSystem->isIconic();
}

/////////////////////////////////////////////////////////////////////////////

void Interface::pushWaitCursor()
{
  GraphicsSystem* graphicsSystem= getGraphicsSystem();

  if( graphicsSystem == 0 )
    return;
  SystemWindow* root= graphicsSystem->getRootWindow();
  if( root == 0 )
    return;

  root->pushWaitCursor();
}

/////////////////////////////////////////////////////////////////////////////

void Interface::popWaitCursor()
{
  GraphicsSystem* graphicsSystem= getGraphicsSystem();

  if( graphicsSystem == 0 )
    return;
  SystemWindow* root= graphicsSystem->getRootWindow();
  if( root == 0 )
    return;

  root->popWaitCursor();
}

/////////////////////////////////////////////////////////////////////////////

MLGammaCorrection* Interface::getGammaCorrection() const
{
  GraphicsSystem* gs= getGraphicsSystem();

  if( gs == 0 )
    return 0;

  return gs->getGammaCorrection();
}

/////////////////////////////////////////////////////////////////////////////

double Interface::getGammaCorrectionValue() const
{
  GraphicsSystem* gs= getGraphicsSystem();

  if( gs == 0 )
    return 1.;

  return gs->getGammaCorrection()->getGamma();
}

/////////////////////////////////////////////////////////////////////////////

void Interface::pressKey( const Key& key )
{
  if( key == iconifyingKey )
    return;

  int base= key.upperCase().getBase();

  if( base == 0 )
    return;

  // find this key
  int i;
  for( i= 0; i < nPressedKeys; ++i )
    if( pressedKeys[i].upperCase().getBase() == base )
      break;

  boolean found= (i < nPressedKeys);
  if( found )
    return;

  if( nPressedKeys >= max_pressed_keys )
    return;

  pressedKeys[nPressedKeys]= key;
  ++nPressedKeys;

  if( nPressedKeys == 1 )
    {
    currentPressedKey= key;
    MLInterfaceCommand* icmd= new MLInterfaceCommand;
    icmd->type= MLInterfaceCommand::KEY_PRESS;
    icmd->key= key;
    icmd->module= currentModule;
    post(icmd);
    }
}

/////////////////////////////////////////////////////////////////////////////

void Interface::unpressKey( const Key& key )
{
  if( key == iconifyingKey )
    return;

  int base= key.upperCase().getBase();

  if( base == 0 )
    return;

  // find this key
  int i;
  for( i= 0; i < nPressedKeys; ++i )
    {
    if( pressedKeys[i].upperCase().getBase() == base )
      break;
    }

  boolean found= (i < nPressedKeys);
  if( ! found )
    return;

  Key unpressed= pressedKeys[i];

  while( i+1 < nPressedKeys )
    {
    pressedKeys[i]= pressedKeys[i+1];
    ++i;
    }
  --nPressedKeys;

  if( base == currentPressedKey.upperCase().getBase() )
    {
    MLInterfaceCommand* icmd= new MLInterfaceCommand;
    icmd->type= MLInterfaceCommand::KEY_RELEASE;
    icmd->key= unpressed;
    icmd->module= currentModule;

    post(icmd);
    currentPressedKey= Key::NULL_KEY;
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean Interface::handleMLKeyboard( MLEvent& event )
{
#ifndef NDEBUG
if( event.type == MLEvent::KEY_PRESS &&
    event.key == Key('0').control().alt() )
  {
  GraphicsSystem* graphicsSystem= getGraphicsSystem();
  graphicsSystem->autoRepeat(true);
  exit(1);
  }
#endif

  if( event.type != MLEvent::KEY_PRESS &&
      event.type != MLEvent::KEY_RELEASE )
    return false;

  if( event.type == MLEvent::KEY_PRESS )
    {
    if( testIconifyingKey(event.key) )
      return true;
    }

  if( nPressedKeys == 0 && event.key != KeyCodes::ESC )
    {
    if( currentModuleInFunction() )
      return true;

    IRectangle dialogsRect( IPoint( screenSize.x()-scrollingDialogsWidth,
                                    modulesBarHeight ),
                            IPoint( screenSize.x()-1,
                                    screenSize.y()-1-statusLinesHeight ) );

    if( dialogsRect.into(event.position) )
      return VDialogManager::handleKey(event);
    }

  MLModule* module= getCurrentModule();
  if( module != 0 )
    {
    if( module->moduleKeyboardHandler != 0 )
      if( module->moduleKeyboardHandler->handleMLKeyboard(event) )
        return true;

    if( module->statusBars != 0 )
      if( module->statusBars->handleMLKeyboard(event) )
        return true;
    }

  if( event.type == MLEvent::KEY_PRESS )
    {
    pressKey(event.key);
    return true;
    }

  if( event.type == MLEvent::KEY_RELEASE )
    {
    unpressKey(event.key);
    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////
