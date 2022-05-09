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
  SystemWindow.C

  Abstract System Window

  Stephane Rehel
  June 28, 1996
*/

#include "GL/opengl.h"

#include "SystemWindow.h"
#include "SoftWindow.h"
#include "MLEventManager.h"

#include "ScreenBlockBuffer.h"
#include "GraphicsSystem.h"
#include "WindowDriver.h"
#include "MLCursors.h"

/////////////////////////////////////////////////////////////////////////////

SystemWindow::SystemWindow()
{
  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  driver= graphicsSystem->newWindowDriver(this);
  parent= 0;
  firstRedraw= true;

  saveUnder= false;
  saveUnderBuffer= 0;
}

/////////////////////////////////////////////////////////////////////////////

SystemWindow::~SystemWindow()
{
  while( ! softChildren.empty() )
    {
    IListElement<SoftWindow>* ile= softChildren.getLastElement();
    SoftWindow* sw= ile->element;
    softChildren.remove(ile);
    delete sw;
    }

  while( ! systemWindows.empty() )
    {
    IListElement<SystemWindow>* ile= systemWindows.getLastElement();
    SystemWindow* sw= ile->element;
    systemWindows.remove(ile);
    delete sw;
    }

  map(false);

  delete saveUnderBuffer;
  saveUnderBuffer= 0;

  if( parent != 0 )
    {
    parent->systemWindows.remove(this);
    parent= 0;
    }

  delete driver;
  driver= 0;
}

/////////////////////////////////////////////////////////////////////////////

void SystemWindow::setSaveUnder()
{
  saveUnder= true;
}

/////////////////////////////////////////////////////////////////////////////

// _parent may be NULL
boolean SystemWindow::create( SystemWindow* _parent,
                              int x, int y, int width, int height )
{
  if( parent != 0 || driver == 0 )
    return false;

  if( ! driver->create( _parent, x, y, width, height ) )
    return false;
  firstRedraw= true;
  isMapped= false;

  if( ! MLWindow::create( x, y, width, height ) )
    return false;

  parent= _parent;

  if( parent != 0 )
    parent->systemWindows.append(this);

  if( parent == 0 )
    setDefaultCursor();
   else
    {
    setCursor(parent->driver->cursor);

    driver->cursor= parent->driver->cursor;
    driver->pushedWaitCursor= parent->driver->pushedWaitCursor;
    driver->saveCursor= parent->driver->saveCursor;
    }

  if( MLEventManager::getEventManager() != 0 )
    MLEventManager::getEventManager()->newWindow(this);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

SystemWindow* SystemWindow::newSystemChild( int x, int y,
                                            int width, int height )
{
  SystemWindow* sw= new SystemWindow;

  if( ! sw->create( this, x, y, width, height ) )
    {
    delete sw;
    return 0;
    }

  return sw;
}

/////////////////////////////////////////////////////////////////////////////

void SystemWindow::map( boolean yes /* = true */ )
{
  if( (yes?1:0) == (isMapped?1:0) )
    return;

  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  if( yes && saveUnder && !graphicsSystem->supportSaveUnder() )
    {
    delete saveUnderBuffer;
    saveUnderBuffer= new ScreenBlockBuffer;
    saveUnderBuffer->get(this);
    }

  boolean no_need_to_redraw= false;
  if( !yes )
    {
    if( saveUnderBuffer != 0 )
      {
      saveUnderBuffer->put(this);
      delete saveUnderBuffer;
      saveUnderBuffer= 0;
      no_need_to_redraw= true;
      }
     else
      {
      if( saveUnder )
        no_need_to_redraw= true;
      }
    }

  isMapped= yes;
  driver->map(yes);

  MLEventManager* eventManager= MLEventManager::getEventManager();

  if( eventManager != 0 )
    eventManager->removeRedraw(this);
  if( yes )
    firstRedraw= true;

  if( ! yes && ! no_need_to_redraw )
    graphicsSystem->invalidateRect(this);

  IListIterator<SoftWindow> li1(softChildren);
  while( ! li1.eol() )
    li1.next()->map(yes);

  IListIterator<SystemWindow> li2(systemWindows);
  while( ! li2.eol() )
    li2.next()->map(yes);

  if( eventManager != 0 )
    eventManager->checkForEnterLeaveEvents();
}

/////////////////////////////////////////////////////////////////////////////

void SystemWindow::createContext()
{
  driver->createContext();
}

/////////////////////////////////////////////////////////////////////////////

void SystemWindow::destroyContext()
{
  driver->destroyContext();
}

/////////////////////////////////////////////////////////////////////////////

void SystemWindow::current()
{
  driver->current();

  glViewport( 0, 0, size.x(), size.y() );
}

/////////////////////////////////////////////////////////////////////////////

void SystemWindow::setCursor( MLCursor* cursor,
                              boolean children_too /* = false */ )
{
  driver->setCursor(cursor);

  if( children_too )
    {
    IListIterator<SystemWindow> li(systemWindows);
    while( ! li.eol() )
      (li++)->setCursor(cursor,children_too);
    }
}

/////////////////////////////////////////////////////////////////////////////

void SystemWindow::setDefaultCursor()
{
  setCursor( MLCursors::normal );
}

/////////////////////////////////////////////////////////////////////////////

void SystemWindow::pushWaitCursor()
{
  if( driver != 0 )
    driver->pushWaitCursor();

  IListIterator<SystemWindow> li(systemWindows);
  while( ! li.eol() )
    (li++)->pushWaitCursor();

  glFlush();
}

/////////////////////////////////////////////////////////////////////////////

void SystemWindow::popWaitCursor()
{
  IListIterator<SystemWindow> li(systemWindows);
  while( ! li.eol() )
    (li++)->popWaitCursor();

  if( driver != 0 )
    driver->popWaitCursor();

  glFlush();
}

/////////////////////////////////////////////////////////////////////////////

void SystemWindow::swapBuffers()
{
  if( driver != 0 )
    driver->swapBuffers();
}

/////////////////////////////////////////////////////////////////////////////

void SystemWindow::setPosition( const IPoint& p )
{
  if( p == MLWindow::position )
    return;

  driver->setPosition(p);

  MLWindow::position= p;
}

/////////////////////////////////////////////////////////////////////////////

void SystemWindow::setSize( const IVector& s )
{
  if( s == MLWindow::size )
    return;

  driver->setSize(s);

  MLWindow::size= s;
}

/////////////////////////////////////////////////////////////////////////////

void SystemWindow::drawXORFrame( int x1, int y1, int x2, int y2,
                                 boolean undo )
{
  if( x1 == x2 && y1 == y1 )
    return;
  driver->drawXORFrame(x1,y1,x2,y2,undo);
}

/////////////////////////////////////////////////////////////////////////////

MLWindow* SystemWindow::findCursorWindow( const IPoint& p )
{
  if( ! mapped() )
    return 0;

  if( ! into(p) )
    return 0;

  IListElement<SystemWindow>* current= systemWindows.getLastElement();
  while( current != 0 )
    {
    SystemWindow* sw= current->element;
    MLWindow* w= sw->findCursorWindow( IPoint( p.x() - sw->x1(),
                                       p.y() - sw->y1() ) );

    if( w != 0 )
      return w;

    current= current->prev;
    }

  MLWindow* w= MLWindow::findCursorWindow(p);
  if( w != 0 )
    return w;

  return this;
}

/////////////////////////////////////////////////////////////////////////////

IPoint SystemWindow::getAbsPosition() const
{
  IPoint p= position;

  SystemWindow* _parent= parent;
  while( _parent != 0 )
    {
    p[0] += parent->position.x();
    p[1] += parent->position.y();
    _parent= _parent->parent;
    }

  return p;
}

/////////////////////////////////////////////////////////////////////////////
