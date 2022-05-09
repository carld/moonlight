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
  MLCanvas.C

  Stephane Rehel

  November 15 1996
*/

#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"

#include "graphics/MLFont.h"
#include "graphics/MLFonts.h"
#include "graphics/MLEvent.h"
#include "graphics/mlgl.h"

#include "graphics/SoftWindow.h"
#include "graphics/SystemWindow.h"
#include "graphics/MLPixmap.h"

#include "MLAllocator.h"
#include "MLInterfaceCommand.h"

#include "MLCanvas.h"
#include "MLEngine.h"
#include "MLModule.h"

#include "KoalaNodeTypes.h"

#include "MLCanvasAction.h"
#include "MLCanvasActionsPool.h"
#include "MLCanvasPopup.h"
#include "MLCanvasButton.h"
#include "MLPopup.h"

/////////////////////////////////////////////////////////////////////////////

static const int max_engines= 30;

static MLPixmap* maximize_pixmap= 0;
static MLPixmap* maximize_pixmap_highlighted= 0;
static MLPixmap* minimize_pixmap= 0;
static MLPixmap* minimize_pixmap_highlighted= 0;

/////////////////////////////////////////////////////////////////////////////

MLCanvas::MLCanvas( MLModule* _module ): MLNode(_module)
{
  module= _module;

  MLNode::setName( MLString("canvas-") + MLString::valueOf(getID()) );

//  MLAllocator::allocCanvas(this);

  mainWindow= 0;
  titleWindow= 0;
  gfxWindow= 0;
  blackMargin= 0;
  titleHeight= 0;
  titleFont= smallFont;

  engines= new MLEngine* [ max_engines ];
  for( int i= 0; i < max_engines; ++i )
    engines[i]= 0;
  nEngines= 0;
  engine= 0;

  actions= 0;

  enginesPopup= 0;
  minmaxButton= 0;

#define begin_pixmap(name) { char* _##name[]= {
#define end_pixmap(name) };name= new MLPixmap; \
                  (name)->make( (const unsigned char**)_##name, \
                                 sizeof(_##name)/sizeof(char*) ); }

  if( maximize_pixmap == 0 )
    {
    begin_pixmap(maximize_pixmap)
      "2222222222222",
      "2220000000002",
      "2220000000002",
      "2220222222202",
      "2220222222202",
      "2110222222202",
      "2110222222202",
      "21 0222222202",
      "21 0000000002",
      "21     122222",
      "2111111122222",
      "2222222222222"
    end_pixmap(maximize_pixmap)

    begin_pixmap(maximize_pixmap_highlighted)
      "3333333333333",
      "3330000000003",
      "3330000000003",
      "3330333333303",
      "3330333333303",
      "3110333333303",
      "3110333333303",
      "31 0333333303",
      "31 0000000003",
      "31     133333",
      "3111111133333",
      "3333333333333"
    end_pixmap(maximize_pixmap_highlighted)

    begin_pixmap(minimize_pixmap)
      "2222222222222",
      "2221111111112",
      "2221111111112",
      "2221       12",
      "2221       12",
      "20000000   12",
      "20000000   12",
      "20222220   12",
      "2022222011112",
      "2022222022222",
      "2000000022222",
      "2222222222222"
    end_pixmap(minimize_pixmap)

    begin_pixmap(minimize_pixmap_highlighted)
      "3333333333333",
      "3331111111113",
      "3331111111113",
      "3331       13",
      "3331       13",
      "30000000   13",
      "30000000   13",
      "30333330   13",
      "3033333011113",
      "3033333033333",
      "3000000033333",
      "3333333333333"
    end_pixmap(minimize_pixmap_highlighted)
    }
#undef end_pixmap
#undef begin_pixmap

}

/////////////////////////////////////////////////////////////////////////////

MLCanvas::~MLCanvas()
{
  for( int i= 0; i < nEngines; ++i )
    {
    MLEngine* e= engines[i];
    engines[i]= 0;
    delete e;
    }

  delete enginesPopup; enginesPopup= 0;
  delete minmaxButton; minmaxButton= 0;

  actions= 0;
  engine= 0;

  delete gfxWindow;
  gfxWindow= 0;

  delete titleWindow;
  titleWindow= 0;

  delete mainWindow;
  mainWindow= 0;

  module->removeCanvas(this);
  module= 0;

//  MLAllocator::freeCanvas(this);
}

/////////////////////////////////////////////////////////////////////////////

MLNodeType::Type MLCanvas::getNodeType() const
{
  return module->getKoalaNodeTypes()->canvas;
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvas::create( int _where )
{
  if( mainWindow != 0 || gfxWindow != 0 )
    return;

  where= _where;
  init_where= _where;

  module->addCanvas(this);

  enginesPopup= new MLCanvasPopup(this);
  enginesPopup->create( "Engines" );
/*
  enginesPopup->getPopup()->add("PersPective");
  enginesPopup->getPopup()->add("Ortho X");
  enginesPopup->getPopup()->add("Ortho Y");
  enginesPopup->getPopup()->add("Ortho Z");
*/

  minmaxButton= new MLCanvasButton(this);
  minmaxButton->create( maximize_pixmap, maximize_pixmap_highlighted );

  position(where);

//  engine= new MLEngine3D(this);
}

/////////////////////////////////////////////////////////////////////////////

int MLCanvas::getModuleID() const
{
  return module->getID();
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvas::position( int _where )
{
  if( where==_where && mainWindow != 0 )
    return;

  where= _where;

  if( where == int(MLModule::CANVAS_1234) && minmaxButton != 0 )
    minmaxButton->set( minimize_pixmap, minimize_pixmap_highlighted );
   else
    minmaxButton->set( maximize_pixmap, maximize_pixmap_highlighted );

  IPoint position;
  IVector size;

  module->getCanvasConfig(where,position,size,blackMargin);

//  if( actions != 0 )
//    actions->destroyActionsWindows();

  // destroy all canvas actions windows for all engines defined
  // for this canvas. ough!
  for( int i= 0; i < nEngines; ++i )
    {
    MLEngine* engine= engines[i];
    if( engine == 0 )
      continue;
    if( engine->actions == 0 )
      continue;
    engine->actions->destroyActionsWindows();
    }

  if( enginesPopup != 0 )
    enginesPopup->destroyWindow();
  if( minmaxButton!= 0 )
    minmaxButton->destroyWindow();

  delete titleWindow; titleWindow= 0;
  delete mainWindow; mainWindow= 0;
  delete gfxWindow; gfxWindow= 0;

  SystemWindow* root= module->window;

  mainWindow= new SoftWindow;
  mainWindow->create( root,
                      position.x(), position.y(),
                      size.x(), size.y() );
  mainWindow->setEventHandler(this);

  int bm= blackMargin;
  titleHeight= 2 + titleFont->getHeight();
  titleWindow= new SoftWindow;
  titleWindow->create( mainWindow,
                       bm+1, bm+1,
                       size.x()-2*(bm+1), titleHeight );

  int y2= position.y() + (1 + titleHeight + 1);
  gfxWindow= new SystemWindow;
  gfxWindow->create( root,
                     position.x()+1 + blackMargin,
                     y2 + blackMargin,
                     size.x()-2 - 2*blackMargin,
                     size.y() - (1+y2-position.y()) - 2*blackMargin
                   );
  gfxWindow->map();
  gfxWindow->setEventHandler(this);

  if( enginesPopup != 0 )
    {
    enginesPopup->left= 1;
    enginesPopup->x= 0;
    enginesPopup->createWindow();
    if( enginesPopup->window != 0 )
      enginesPopup->window->removeRedraw();
    }

  if( minmaxButton != 0 )
    {
    minmaxButton->left= -1;
    minmaxButton->x= titleWindow->getSize().x();
    minmaxButton->createWindow();
    if( minmaxButton->window != 0 )
      minmaxButton->window->removeRedraw();
    }

  if( actions != 0 )
    actions->reconfigure();
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvas::map()
{
  if( mainWindow != 0 )
    mainWindow->map();

  if( gfxWindow != 0 )
    {
    gfxWindow->map();
    gfxWindow->postRedraw();
    }

  if( enginesPopup != 0 ) enginesPopup->map(true);
  if( minmaxButton != 0 ) minmaxButton->map(true);

  if( actions != 0 ) actions->map();
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvas::unmap()
{
  if( actions != 0 ) actions->unmap();

  if( enginesPopup != 0 ) enginesPopup->map(false);
  if( minmaxButton != 0 ) minmaxButton->map(false);

  if( gfxWindow != 0 )
    gfxWindow->map(false);

  if( mainWindow != 0 )
    mainWindow->map(false);
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvas::titleDraw()
{
  if( mainWindow == 0 )
    return;

  if( ! mainWindow->mapped() )
    return;

  mainWindow->currentPixel();
  mlFront();

  IVector size= mainWindow->getSize();

  if( blackMargin > 0 )
    {
    mlBlack();
    mlRect( 0, 0, blackMargin-1, size.y()-1 );
    mlRect( 0, 0, size.x()-1, blackMargin-1 );
    mlRect( 0, size.y()-blackMargin, size.x()-1, size.y()-1 );
    mlRect( size.x()-blackMargin, 0, size.x()-1, size.y()-1 );
    }

  int bm= blackMargin;

  mlDarkGray();
  mlFrame( bm, bm, size.x()-1-bm, size.y()-1-bm );

  IPoint p1(1+bm,size.y()-1-1-titleHeight+1-bm);
  IPoint p2(size.x()-1-1-bm,size.y()-1-1-bm);

  glBegin(GL_LINES);
    glVertex2i(p1.x(),p1.y()-1);
    glVertex2i(p2.x(),p1.y()-1);
  glEnd();

  mlLightGray();
  mlRect( p1.x(), p1.y(), p2.x(), p2.y() );

  if( enginesPopup != 0 )
    enginesPopup->draw();

  if( minmaxButton != 0 )
    minmaxButton->draw();

  if( actions != 0 )
    actions->draw();
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvas::gfxDraw()
{
  if( gfxWindow == 0 )
    return;

  if( ! gfxWindow->mapped() )
    return;

  if( engine == 0 )
    {
    gfxWindow->currentPixel();
    mlFront();
    mlBlack();
    mlRect( 0, 0, gfxWindow->getSize().x()-1, gfxWindow->getSize().x()-1 );
    return;
    }

  engine->draw();
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvas::postRefresh()
{
  if( module == 0 )
    return;
  module->postRefreshCanvas(getID());
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvas::setActionsPool( MLCanvasActionsPool* _actions )
{
  if( actions == _actions )
    return;

  actions= _actions;
  actions->reconfigure();
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvas::handleCommand( MLCommand* cmd )
{
  if( cmd == 0 )
    return;

  if( cmd->engine != 0 )
    {
    MLEngine* e= MLAllocator::getEngine(cmd->engine);
    if( e != 0 )
      e->handleCommand(cmd);

    return;
    }

  // ok, this command is for me

  if( cmd->getBase() != MLInterfaceCommand::BASE )
    return;

  MLInterfaceCommand* icmd= (MLInterfaceCommand*) cmd;

  switch( icmd->getType() )
    {
    case MLInterfaceCommand::MIN_MAX_CANVAS:
      {
      MLInterfaceCommand* cmd= new MLInterfaceCommand;
      if( where == init_where )
        cmd->type= MLInterfaceCommand::MAXIMIZE_CANVAS;
       else
        cmd->type= MLInterfaceCommand::MINIMIZE_CANVAS;
      post(cmd);
      break;
      }
    case MLInterfaceCommand::MINIMIZE_CANVAS:
      {
      if( where != MLModule::CANVAS_1234 )
        break;
      position(init_where);
//      titleDraw();
      gfxWindow->postRedraw();
      break;
      }
    case MLInterfaceCommand::MAXIMIZE_CANVAS:
      {
      if( where == MLModule::CANVAS_1234 )
        break;
      position(MLModule::CANVAS_1234);
//      titleDraw();
      gfxWindow->postRedraw();
      break;
      }

    case MLInterfaceCommand::ACTIVATE_ENGINE:
      {
      MLEngine* e= MLAllocator::getEngine(icmd->index);
      if( e == 0 )
        break;
      int i;
      for( i= 0; i < nEngines; ++i )
        if( engines[i] != 0 )
          if( engines[i]->getID() == icmd->index )
            break;

      if( i >= nEngines )
        break;
      if( e == engine )
        break;

      if( actions != 0 )
        actions->unmap();

      engine= e;
      actions= e->actions;

      // change popup title
      if( enginesPopup != 0 )
        {
        if( engine != 0 )
          enginesPopup->setTitle( engine->getName() );
        }

      actions->map();
      actions->reconfigure();

      titleDraw();
      gfxDraw();

      break;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLCanvas::eventHandler( MLWindow* window, MLEvent& event )
{
  if( window == mainWindow )
    return mainWindowEventHandler(event);

  if( window == gfxWindow )
    return gfxWindowEventHandler(event);

  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLCanvas::gfxWindowEventHandler( MLEvent& event )
{
  if( engine != 0 )
    {
    if( engine->engineEventHandler(event) )
      return true;
    }

  if( event.type == MLEvent::REDRAW )
    {
    gfxDraw();
    return true;
    }

  return gfxWindow->handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLCanvas::mainWindowEventHandler( MLEvent& event )
{
  switch( event.type )
    {
    case MLEvent::REDRAW:
      {
//printf("mainWindow redraw\n");
      return true;
      }

    default:
      break;
    }

  return mainWindow->handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLCanvas::mapped() const
{
  if( gfxWindow == 0 )
    return false;
  if( ! gfxWindow->mapped() )
    return false;

  return module->mapped();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLCanvas::handleAction( MLAction* action )
{
  if( action == minmaxButton )
    {
    MLInterfaceCommand* cmd= new MLInterfaceCommand;
    cmd->type= MLInterfaceCommand::MIN_MAX_CANVAS;
    post(cmd);
    return true;
    }

  if( action == enginesPopup )
    {
    int i;
    for( i= 0; i < nEngines; ++i )
      {
      MLEngine* e= engines[i];
      if( e == 0 )
        continue;
      if( e->_canvas_popup_label == enginesPopup->selected_label )
        break;
      }

    enginesPopup->selected_label= 0;

    if( i < nEngines )
      activateEngine( engines[i]->getID() );

    return true;
    }

  if( engine != 0 && actions != 0 )
    {
    // check if this action does belong to an engine

    if( actions->find(action) )
      return engine->handleAction(action);
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvas::enterMode( MLMode* mm )
{
  if( engine != 0 )
    engine->enterMode(mm);
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvas::leaveMode( MLMode* mm )
{
  if( engine != 0 )
    engine->leaveMode(mm);
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvas::activateEngine( int engineID )
{
  // check if it's a valid engine
  int i;
  for( i= 0; i < nEngines; ++i )
    if( engines[i]->getID() == engineID )
      break; // found

  if( i == nEngines )
    return; // unknown engine?

  MLInterfaceCommand* cmd= new MLInterfaceCommand;
  cmd->type= MLInterfaceCommand::ACTIVATE_ENGINE;
  cmd->index= engineID;
  post(cmd);
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvas::addEngine( MLEngine* _engine )
{
  if( _engine == 0 )
    return;
  if( nEngines >= max_engines )
    return;

  for( int i= 0; i < nEngines; ++i )
    if( engines[i] == _engine )
      return;

  engines[ nEngines ]= _engine;
  ++nEngines;

  if( enginesPopup != 0 )
    {
    int label= enginesPopup->getPopup()->add( _engine->getName() );
    _engine->_canvas_popup_label= label;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvas::removeEngine( MLEngine* _engine )
{
  if( _engine == 0 )
    return;

  int i;
  for( i= 0; i < nEngines; ++i )
    if( engines[i] == _engine )
      break;
  if( i >= nEngines )
    return; // not found

  while( i+1 < nEngines )
    {
    engines[i]= engines[i+1];
    ++i;
    }

  --nEngines;
  engines[ nEngines ]= 0;

  if( engine == _engine ) // ugh
    {
    actions= 0;
    engine= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvas::write( MLWFileBlock& block )
{
  block << nEngines;
  for( int i= 0; i < nEngines; ++i )
    {
    MLEngine* e= engines[i];
    block << e->getName();
    e->write(block);
    }

  block << engine->getName();

  block << int(where);
  block << ((gfxWindow==0) ? int(0) : int(gfxWindow->mapped()));

  block << int(0);
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvas::read( MLRFileBlock& block )
{
  int n;
  MLString name;

  block >> n;
  for( int i= 0; i < n; ++i )
    {
    block >> name;
    int j;
    for( j= 0; j < nEngines; ++j )
      {
      if( engines[j]->getName() == name )
        break;
      }
    assert( j < nEngines );

    engines[j]->read(block);
    }

  block >> name;
  int current_engine= 0;
  for( int j= 0; j < nEngines; ++j )
    {
    if( engines[j]->getName() == name )
      {
      current_engine= engines[j]->getID();
      break;
      }
    }
  if( current_engine != 0 )
    activateEngine(current_engine);

  int _where, _mapped;
  block >> _where;
  block >> _mapped;

  position(_where);

  if( ! _mapped )
    unmap();
   else
    map();

//  gfxWindow->postRedraw();

  if( block.getInteger() == 0 )
    return;
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvas::resetAll()
{
  for( int i= 0; i < nEngines; ++i )
    engines[i]->resetAll();

  position(init_where);
  map();
}

/////////////////////////////////////////////////////////////////////////////
