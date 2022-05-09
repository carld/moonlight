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
  MLWindow.C

  Abstract Window (either a SoftWindow or a SystemWindow)

  Stephane Rehel
  June 28, 1996
*/

#include "mlgl.h"

#include "GraphicsSystem.h"
#include "MLWindow.h"
#include "SystemWindow.h"
#include "SoftWindow.h"
#include "MLEvent.h"
#include "MLEventManager.h"
#include "MLFont.h"

/////////////////////////////////////////////////////////////////////////////

MLWindow::MLWindow()
{
  position= IPoint(0,0);
  size= IVector(0,0);
  _eventHandler= 0;
  isMapped= false;
  mousePointerInto= false;
}

/////////////////////////////////////////////////////////////////////////////

MLWindow::~MLWindow()
{
  MLEventManager* eventManager= MLEventManager::getEventManager();

  if( eventManager != 0 )
    eventManager->destroyWindow(this);
}

/////////////////////////////////////////////////////////////////////////////

void MLWindow::setEventHandler( MLEventHandler* __eventHandler )
{
  _eventHandler= __eventHandler;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLWindow::handleEvent( const MLEvent& event )
{
  if( event.type == MLEvent::REDRAW )
    {
    event.window->redraw(); // default action
    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

SystemWindow* MLWindow::getSystemWindow() const
{
  if( isSystemWindow() )
    return (SystemWindow*) this;
   else
    return ((SoftWindow*) this)->getSystemParent();
}

/////////////////////////////////////////////////////////////////////////////

// protected
boolean MLWindow::create( int x, int y, int width, int height )
{
  position= IPoint(x,y);
  size= IVector(width,height);

  MLEventManager* eventManager= MLEventManager::getEventManager();

  if( eventManager != 0 )
    postRedraw();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

IPoint MLWindow::getAbsolutePosition() const
{
  SystemWindow* sw;

  IPoint pos(0,0);

  if( isSoftWindow() )
    {
    SoftWindow* soft= (SoftWindow*) this;
    pos= soft->getAbsPosition();
    sw= soft->getSystemParent();
    }
   else
    sw= (SystemWindow*) this;

  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  while( sw != graphicsSystem->getRootWindow() )
    {
    pos[0]+= sw->getPosition().x();
    pos[1]+= sw->getPosition().y();
    sw= sw->getParent();
    }

  return pos;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLWindow::into( const IPoint& p ) const
{
  return p.x() >= 0 && p.x() < size.x() &&
         p.y() >= 0 && p.y() < size.y() ;
}

/////////////////////////////////////////////////////////////////////////////

void MLWindow::pixelMode()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho( 0., GLfloat(size.x()), 0., GLfloat(size.y()), -1., 1. );

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

#ifdef __WINDOWS__
  glTranslatef( 0.5, 0.375, 0. );
#else
  glTranslatef( 0.375, 0.375, 0. );
#endif

  glShadeModel(GL_FLAT);
//  glShadeModel(GL_SMOOTH);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
}

/////////////////////////////////////////////////////////////////////////////

void MLWindow::clear( float r, float g, float b )
{
  mlColor3f( r, g, b );
  mlRect( 0, 0, size.x()-1, size.y()-1 );
}

/////////////////////////////////////////////////////////////////////////////

SoftWindow* MLWindow::newSoftChild( int x, int y, int width, int height )
{
  SoftWindow* sw= new SoftWindow;

  if( ! sw->create( this, x, y, width, height ) )
    {
    delete sw;
    return 0;
    }

  return sw;
}

/////////////////////////////////////////////////////////////////////////////

void MLWindow::postRedraw( boolean ignore_unmapped /* = false */ )
{
  MLEventManager* eventManager= MLEventManager::getEventManager();

  if( eventManager == 0 )
    return;

  eventManager->removeTyped( this, MLEvent::REDRAW );

  MLEvent* event= new MLEvent;
  event->window= this;
  event->time= 0;
  event->privateEvent= true;
  event->type= MLEvent::REDRAW;
  event->position= IPoint(0,0);
  event->size= size;
  event->button= 0;
  event->state= 0;

  if( isSoftWindow() )
    {
    event->position[0] += ((SoftWindow*)this)->absPosition[0];
    event->position[1] += ((SoftWindow*)this)->absPosition[1];
    }

  eventManager->postEvent(event,ignore_unmapped);
}

/////////////////////////////////////////////////////////////////////////////

// return true if found any
boolean MLWindow::removeRedraw()
{
  MLEventManager* eventManager= MLEventManager::getEventManager();
  if( eventManager == 0 )
    return false;

  return eventManager->removeTyped( this, MLEvent::REDRAW );
}

/////////////////////////////////////////////////////////////////////////////

MLWindow* MLWindow::findCursorWindow( const IPoint& p )
{
  if( ! mapped() )
    return 0;

  if( ! into(p) )
    return 0;

  IListIterator<SoftWindow> li(softChildren);
  while( ! li.eol() )
    {
    SoftWindow* sw= li++;
    MLWindow* w= sw->findCursorWindow( IPoint( p.x() - sw->x1(),
                                               p.y() - sw->y1() ) );
    if( w != 0 )
      return w;
    }

  return this;
}

/////////////////////////////////////////////////////////////////////////////
