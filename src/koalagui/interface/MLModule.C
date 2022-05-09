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
  MLModule.C

  Stephane Rehel

  November 15 1996
*/

#include "tools/MLCommandLine.h"
#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"

#include "kernel/MLAppRoot.h"

#include "graphics/GraphicsSystem.h"
#include "graphics/SystemWindow.h"
#include "graphics/MLEvent.h"
#include "graphics/KeyCodes.h"
#include "graphics/MLCursors.h"

#include "koala/VScrollingDialogs.h"
#include "kw/KWDialog.h"

#include "MLAllocator.h"
#include "MLInterfaceCommand.h"

#include "Interface.h"
#include "InterfaceAppRoot.h"
#include "KoalaNodeTypes.h"

#include "MLCanvas.h"
#include "MLButtonsPanel.h"
#include "MLStatusBars.h"
#include "MLPopup.h"
#include "MLScrollingDialog.h"
#include "MLScrollingDialogs.h"
#include "MLGlobalDialog.h"
#include "MLUndoRedoStack.h"

#include "MLMode.h"
#include "MLModes.h"
#include "MLFunctionsManager.h"
#include "MLFunction.h"
#include "MLFunctions.h"

#include "MLModule.h"

/////////////////////////////////////////////////////////////////////////////

static int max_canvas= 16;

/////////////////////////////////////////////////////////////////////////////

MLModule::MLModule( Interface* _interface ):
  MLNode( _interface->getKernel() )
{
  MLNode::setName("KoalaModule");

  interface= _interface;

  rc_group= "";

  koalaNodeTypes= interface->getKoalaNodeTypes();

  scrollingDialogs= 0;

  root= 0;
  window= 0;
  buttonsPanel= 0;
  statusBars= 0;
  scrollingDialogs= 0;
  dialogs= 0;
  moduleKeyboardHandler= 0;
  modes= 0;
  functionsManager= 0;
  functions= 0;
  currentMode= 0;
  undoStack= redoStack= 0;
  canvas= 0;
  name= "";

//  MLAllocator::allocModule(this);

  undoStack= new MLUndoRedoStack;
  redoStack= new MLUndoRedoStack;

  dialogs= new MLScrollingDialogs(this);

  canvas= new MLCanvas* [ max_canvas + 1 ];
  nCanvas= 0;
  for( int i= 0; i <= max_canvas; ++i )
    canvas[i]= 0;

  mainArea= IRectangle( IPoint( Interface::buttonsPanelWidth,
                                Interface::modulesBarHeight ),
                        IPoint( interface->screenSize.x()-2
                                - Interface::scrollingDialogsWidth,
                                interface->screenSize.y()-1
                                - Interface::statusLinesHeight ) );
  root= interface->window;
  window= 0;
  _mapped= false;

  buttonsPanel= 0;
  statusBars= 0;
  scrollingDialogs= 0;

  name= MLString("NoNameModule");
  color= Color(0.05,0.05,0.25);

  currentMode= 0;
  modes= new MLModes(this);
  functions= new MLFunctions(this);
  functionsManager= new MLFunctionsManager(this);
}

/////////////////////////////////////////////////////////////////////////////

MLModule::~MLModule()
{
  moduleKeyboardHandler= 0;

  clearUndosRedos();

  delete undoStack; undoStack= 0;
  delete redoStack; redoStack= 0;

  for( int i= nCanvas; i; --i )
    {
    MLCanvas* c= canvas[i];
    canvas[i]= 0;
    delete c;
    }

  delete canvas;
  nCanvas= 0;

  delete dialogs;
  dialogs= 0;

  delete functions;
  functions= 0;

  delete functionsManager;
  functionsManager= 0;

  delete modes;
  modes= 0;

  delete window;
  window= 0;

  delete buttonsPanel;
  buttonsPanel= 0;

  delete statusBars;
  statusBars= 0;

  delete scrollingDialogs;
  scrollingDialogs= 0;

  koalaNodeTypes= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLModule::init( MLAppRoot* _approot )
{
  if( _approot == 0 )
    return false;

  approot= _approot;

  MLNode::attachToParent(approot);

//  MLCommandLine& command= getKernel()->getCommandLine();

  if( window != 0 )
    return false;

  window= new SystemWindow;
  window->create( root,
                  mainArea.x(), mainArea.y(),
                  mainArea.getSize().x(), mainArea.getSize().y() );
  window->setEventHandler(this);

  buttonsPanel= new MLButtonsPanel(this);
  buttonsPanel->create( IPoint(0,Interface::modulesBarHeight),
                        IVector( Interface::buttonsPanelWidth,
                                 mainArea.getSize().y()) );
  buttonsPanel->map(false);

  statusBars= new MLStatusBars;
  statusBars->create( root,
                      IPoint( Interface::buttonsPanelWidth+1,
                              interface->screenSize.y() -
                              Interface::statusLinesHeight ),
                      IVector( mainArea.getSize().x()-2,
                               Interface::statusLinesHeight ) );
  statusBars->map(false);

  scrollingDialogs= new VScrollingDialogs;
  scrollingDialogs->create( root,
            interface->screenSize.x() - Interface::scrollingDialogsWidth,
            Interface::modulesBarHeight+1,
            Interface::scrollingDialogsWidth
              - VScrollingDialogs::getScrollBarWidth(),
            interface->screenSize.y() - Interface::modulesBarHeight
                                      - Interface::statusLinesHeight-2 );
  scrollingDialogs->map(false);

  _mapped= false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

KoalaNodeTypes* MLModule::getKoalaNodeTypes() const
{
  if( interface == 0 )
    return 0;

  return interface->getKoalaNodeTypes();
}

/////////////////////////////////////////////////////////////////////////////

MLNodeType::Type MLModule::getNodeType() const
{
  KoalaNodeTypes* types= getKoalaNodeTypes();

  return types->module;
}

/////////////////////////////////////////////////////////////////////////////

const MLString& MLModule::getName() const
{
  return name;
}

/////////////////////////////////////////////////////////////////////////////

const Color& MLModule::getColor() const
{
  return color;
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::map()
{
  if( window == 0 || _mapped )
    return;

  window->map();
  window->postRedraw();

  if( buttonsPanel != 0 )
    buttonsPanel->map();

  if( statusBars != 0 )
    statusBars->map();

  if( scrollingDialogs != 0 )
    {
    scrollingDialogs->map();
    scrollingDialogs->postRedraw();
    }

  _mapped= true;
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::unmap()
{
  if( window == 0 || !_mapped )
    return;

  window->map(false);

  if( buttonsPanel != 0 )
    buttonsPanel->map(false);

  if( statusBars != 0 )
    statusBars->map(false);

  if( scrollingDialogs != 0 )
    scrollingDialogs->map(false);

  _mapped= false;
}

/////////////////////////////////////////////////////////////////////////////

MLConfig* MLModule::getConfig() const
{
  return (interface==0) ? 0 : interface->getConfig();
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::setModuleKeyboardHandler( MLKeyboardHandler*
                                                    _moduleKeyboardHandler )
{
  moduleKeyboardHandler= _moduleKeyboardHandler;
}

/////////////////////////////////////////////////////////////////////////////

// protected
boolean MLModule::eventHandler( MLWindow* w, MLEvent& event )
{
  if( event.type == MLEvent::REDRAW )
    {
    draw();
    return true;
    }

  return w->handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::post( MLCommand* command ) const
{
  command->module= MLNode::getID(); //MLAllocObject::getID();
  command->post();
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::postRefreshCanvas( int canvasID )
{
  interface->postRefreshCanvas(canvasID);
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::refreshScreen( unsigned int what )
{
  if( dialogs != 0 )
    dialogs->refreshScreen(what);
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::refreshInterface()
{
  if( interface != 0 )
    interface->refreshInterface();
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::clearUndosRedos()
{
  if( undoStack != 0 )
    undoStack->clear();
  if( redoStack != 0 )
    redoStack->clear();
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::postCmdStop()
{
  MLInterfaceCommand* icmd= new MLInterfaceCommand;
  icmd->type= MLInterfaceCommand::CMD_STOP;
  post(icmd);
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::postUndoRedo( boolean undo_it )
{
  interface->sendCommands();

  boolean found= false;

  MLInterfaceCommand* icmd;

  icmd= new MLInterfaceCommand;
  icmd->type= MLInterfaceCommand::CMD_STOP;
  icmd->isUndo= undo_it ? 1 : 0;
  post(icmd);

  for(;;)
    {
    MLCommand* undo;
    if( undo_it )
      undo= undoStack->get();
     else
      undo= redoStack->get();

    if( undo == 0 )
      break;

    boolean isStop= undo->getBase() == MLInterfaceCommand::BASE &&
                    undo->getType() == MLInterfaceCommand::CMD_STOP;
    if( isStop )
      {
      ::unref(undo);
      if( found )
        break;
      continue;
      }

    undo->isUndo= undo_it ? 1 : 0;
    post(undo);

    ::unref(undo);

    found= true;
    }

  icmd= new MLInterfaceCommand;
  icmd->type= MLInterfaceCommand::CMD_STOP;
  icmd->isUndo= undo_it ? 1 : 0;
  post(icmd);
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::handleCommand( MLCommand* cmd )
{
  if( cmd == 0 )
    return;

  MLInterfaceCommand* icmd= 0;

  if( cmd->getBase() != MLInterfaceCommand::BASE )
    goto to_canvas; // not an interface command?!

  icmd= (MLInterfaceCommand*) cmd;

  switch( icmd->getType() )
    {
    case MLInterfaceCommand::REFRESH_MODULE:
      {
      if( buttonsPanel != 0 )
        buttonsPanel->postRedraw();
      if( statusBars != 0 )
        statusBars->postRedraw();
      if( scrollingDialogs != 0 )
          scrollingDialogs->postDrawScrollBar();
      break;
      }

    case MLInterfaceCommand::REFRESH_CANVAS_i:
      {
      if( ! mapped() )
        break;
      MLCanvas* c= MLAllocator::getCanvas(icmd->index);
      if( c != 0 )
        c->gfxDraw();

      break;
      }

    case MLInterfaceCommand::KEY_PRESS:
    case MLInterfaceCommand::KEY_RELEASE:
      {
      handleKey( icmd->key,
                 icmd->getType() == MLInterfaceCommand::KEY_PRESS );
      break;
      }

    case MLInterfaceCommand::REMOVE_DIALOG:
      {
      if( dialogs != 0 )
        dialogs->removeDialog(icmd->string);
      break;
      }

    case MLInterfaceCommand::REFRESH_DIALOG_i:
      {
      MLScrollingDialog* sd= MLAllocator::getScrollingDialog(icmd->index);
      if( sd != 0 )
        sd->update();

      break;
      }

    default:
      break;
    }

to_canvas:
  if( cmd->canvas != 0 )
    {
    MLCanvas* c= MLAllocator::getCanvas(cmd->canvas);
    if( c != 0 )
      c->handleCommand(cmd);
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::getCanvasConfig( int where,
                                IPoint& position, IVector& size,
                                int& margins )
{
  int margin= 2; // even number please

  IPoint p1= IPoint(0,0);
  IPoint p2= IPoint( mainArea.getSize().x()-1,
                     mainArea.getSize().y()-1 );

  IPoint pm= (p1+p2)/2 ;//- IVector( margin / 2 - 1, margin / 2 - 1 );
  IPoint pM= pm + IPoint(1,1) ;//IPoint(1+margin,1+margin);

  IPoint& P1= position;
  IPoint P2;
  switch( where )
    {
    case int(CANVAS_1):
      P1= p1;
      P2= pm;
      break;
    case int(CANVAS_2):
      P1= IPoint(pM.x(),p1.y());
      P2= IPoint(p2.x(),pm.y());
      break;
    case int(CANVAS_3):
      P1= IPoint(p1.x(),pM.y());
      P2= IPoint(pm.x(),p2.y());
      break;
    case int(CANVAS_4):
      P1= pM;
      P2= p2;
      break;
    case int(CANVAS_12):
      P1= p1;
      P2= IPoint(p2.x(),pm.y());
      break;
    case int(CANVAS_34):
      P1= IPoint(p1.x(),pM.y());
      P2= p2;
      break;
    case int(CANVAS_1234):
      P1= p1;
      P2= p2;
      break;
    }

  size= P2 - P1 + IVector(1,1);
  margins= margin / 2;
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::printMessage( const MLString& /*message*/,
                             int /* color = 1 */ )
{}

/////////////////////////////////////////////////////////////////////////////

void MLModule::draw()
{
  if( ! _mapped )
    return;
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::popupCommand( MLPopup* popup, int label )
{
  if( popup == 0 || label <= 0 )
    return;

  if( label == MLPopup::OPEN || label == MLPopup::CLOSE )
    return;

  MLInterfaceCommand* cmd= new MLInterfaceCommand;
  cmd->type= MLInterfaceCommand::POPUP;
  cmd->index= label;
  post(cmd);
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::addCanvas( MLCanvas* c )
{
  if( c == 0 || nCanvas >= max_canvas )
    return;

  for( int i= 1; i <= nCanvas; ++i )
    if( canvas[i] == c )
      return;

  ++nCanvas;
  canvas [ nCanvas ]= c;
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::removeCanvas( MLCanvas* c )
{
  if( c == 0 )
    return;

  int i;
  for( i= 1; i <= nCanvas; ++i )
    if( canvas[i] == c )
      break;

  if( i > nCanvas )
    return;

  while( i+1 <= nCanvas )
    {
    canvas[i]= canvas[i+1];
    ++i;
    }

  --nCanvas;
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::unpressButtons() // called by Interface
{
  if( buttonsPanel != 0 )
    buttonsPanel->unpress();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLModule::handleKey( Key key, boolean pressed )
{
  if( key.getBase() >= 'A' && key.getBase() <= 'Z' )
    key= key.unshift();

  if( key == KeyCodes::ESC )
    {
    // ok, cancel current function if any
    if( pressed )
      leaveFunction();
    return true;
    }

  MLFunction* f= functions->get(key);
  if( f == 0 )
    return false; // no function associated with this key

  if( ! pressed )
    {
    MLFunction* current= functionsManager->getCurrentFunction();

    if( current != 0 )
      if( current->getID() == f->getID() )
        {
        if( ! pressed )
          leaveFunction();
        }

    return true;
    }

  enterFunction(f);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::leaveFunction()
{
  if( functionsManager == 0 )
    return;

  enteringFunction(0);

  functionsManager->cancelCurrentFunction();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLModule::enterFunction( int function_id )
{
  return enterFunction( functions->find(function_id) );
}

/////////////////////////////////////////////////////////////////////////////

boolean MLModule::enterFunction( MLFunction* f )
{
  if( f == 0 )
    {
    leaveFunction();
    return true;
    }

  MLFunction* current= functionsManager->getCurrentFunction();
  if( current != 0 )
    return false;

  enteringFunction(f);

  // ok, run this function
  printMessage(""); // clear message
  MLFunction* executed_function= f->newFunction();
  functionsManager->call( executed_function );
  delete executed_function;
  executed_function= 0;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// f==0 if leaving current
void MLModule::enteringFunction( MLFunction* )
{}

/////////////////////////////////////////////////////////////////////////////

MLMode* MLModule::getMode( int matching_id /* = 0 */ ) const
{
  if( currentMode == 0 )
    return 0;

  if( matching_id == 0 )
    return currentMode;

  if( currentMode->getID() != matching_id )
    return 0;

  return currentMode;
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::enterMode( int id )
{
  if( modes == 0 )
    return;
  MLMode* mm= modes->get(id);
  if( mm == 0 )
    return;

  enterMode(mm);
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::enterMode( MLMode* mm )
{
  if( currentMode == mm )
    return;

  for( int i= 1; i <= nCanvas; ++i )
    if( canvas[i] != 0 )
      canvas[i]->enterMode(mm);

  root->setCursor(MLCursors::highlighted_normal,true);
  currentMode= mm;
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::leaveMode( MLMode* mm /* = 0 */ )
{
  if( currentMode == 0 )
    return;

  if( mm == 0 )
    mm= currentMode;

  for( int i= 1; i <= nCanvas; ++i )
    if( canvas[i] != 0 )
      canvas[i]->leaveMode(mm);

  root->setCursor(0,true);
  currentMode= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLFunctionsManager* MLModule::getFunctionsManager() const
{
  return functionsManager;
}

/////////////////////////////////////////////////////////////////////////////

MLFunctions* MLModule::getFunctions() const
{
  return functions;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLModule::inFunction() const
{
  if( functionsManager == 0 )
    return false;

  return functionsManager->getCurrentFunction() != 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLModule::last_function_must_return() const
{
  if( functionsManager == 0 )
    return false;

  return functionsManager->last_function_must_return();
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::validateFunction()
{
  if( functionsManager == 0 )
    return;

  MLFunction* f= functionsManager->getCurrentFunction();

  if( f == 0 )
    return;

  f->validate();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLModule::isIconic() const
{
  if( interface == 0 )
    return true;

  return interface->isIconic();
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::pushWaitCursor()
{
  if( interface == 0 )
    return;

  interface->pushWaitCursor();
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::popWaitCursor()
{
  if( interface == 0 )
    return;

  interface->popWaitCursor();
}

/////////////////////////////////////////////////////////////////////////////

MLGammaCorrection* MLModule::getGammaCorrection() const
{
  if( interface == 0 )
    return 0;

  return interface->getGammaCorrection();
}

/////////////////////////////////////////////////////////////////////////////

double MLModule::getGammaCorrectionValue() const
{
  if( interface == 0 )
    return 1.;

  return interface->getGammaCorrectionValue();
}

/////////////////////////////////////////////////////////////////////////////

MLEventManager* MLModule::getEventManager() const
{
  if( interface == 0 )
    return 0;

  return interface->getEventManager();
}

/////////////////////////////////////////////////////////////////////////////

GraphicsSystem* MLModule::getGraphicsSystem() const
{
  if( interface == 0 )
    return 0;

  return interface->getGraphicsSystem();
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::write( MLWFileBlock& block )
{
  int i;

  block << nCanvas;
  for( i= 1; i <= nCanvas; ++i )
    {
    if( canvas[i] == 0 )
      {
      block << int(0);
      continue;
      }
    block << int(1);
    canvas[i]->write(block);
    }

  block << int(dialogs->getNScrollingDialogs());
  for( i= 1; i <= dialogs->getNScrollingDialogs(); ++i )
    {
    MLScrollingDialog* sd= dialogs->getScrollingDialog(i);
    if( sd == 0 )
      block << MLString("");
     else
      block << sd->getName();
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::read( MLRFileBlock& block )
{
  int i;

  block >> nCanvas;
  assert( nCanvas == 4 );
  for( i= 1; i <= nCanvas; ++i )
    {
    assert( canvas[i] != 0 );
    int hasCanvas;
    block >> hasCanvas;
    canvas[i]->read(block);
    }

  dialogs->removeAllDialogs();

  int nDialogs;
  block >> nDialogs;
  for( i= 1; i <= nDialogs; ++i )
    {
    MLString name;
    block >> name;
    if( name.length() == 0 )
      continue;

    createDialog(name);
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLModule::resetAll()
{
  clearUndosRedos();

  for( int i= 1; i <= nCanvas; ++i )
    {
    if( canvas[i] != 0 )
      canvas[i]->resetAll();
    }

  MLGlobalDialog::resetAll();

  dialogs->removeAllDialogs();
}

/////////////////////////////////////////////////////////////////////////////
