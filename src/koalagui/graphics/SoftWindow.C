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
  SoftWindow.C

  Stephane Rehel
  June 28, 1996
*/

#include "GL/opengl.h"

#include "SystemWindow.h"
#include "WindowDriver.h"
#include "SoftWindow.h"
#include "MLEventManager.h"

/////////////////////////////////////////////////////////////////////////////

SoftWindow::SoftWindow()
{
  parent= 0;
  absPosition= IPoint(0,0);
  systemParent= 0;
}

/////////////////////////////////////////////////////////////////////////////

SoftWindow::~SoftWindow()
{
  while( ! softChildren.empty() )
    {
    IListElement<SoftWindow>* ile= softChildren.getLastElement();
    SoftWindow* sw= ile->element;
    softChildren.remove(ile);
    delete sw;
    }

  if( parent != 0 )
    {
    parent->softChildren.remove(this);
    parent= 0;
    }

  systemParent= 0;
}

/////////////////////////////////////////////////////////////////////////////

// _parent != 0
boolean SoftWindow::create( MLWindow* _parent,
                            int x, int y, int width, int height )
{
  if( parent != 0 || _parent == 0 )
    return false;

  if( ! MLWindow::create( x, y, width, height ) )
    return false;

  parent= _parent;

  if( parent->isSystemWindow() )
    {
    absPosition= IPoint(x,y);
    systemParent= (SystemWindow*) parent;
    }
   else
    {
    SoftWindow* sw= (SoftWindow*) parent;
    absPosition= sw->absPosition + IVector(x,y);
    systemParent= sw->systemParent;
    }

  parent->softChildren.append(this);
  isMapped= true;//parent->mapped();

  if( MLEventManager::getEventManager() != 0 )
    MLEventManager::getEventManager()->newWindow(this);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void SoftWindow::map( boolean yes /* = true */ )
{
  if( (yes?1:0) == (isMapped?1:0) )
    return;

  isMapped= yes;

  IListIterator<SoftWindow> li(softChildren);
  while( ! li.eol() )
    li.next()->map(yes);

  MLEventManager* eventManager= MLEventManager::getEventManager();

  if( eventManager != 0 )
    eventManager->checkForEnterLeaveEvents();
}

/////////////////////////////////////////////////////////////////////////////

void SoftWindow::current() // current for opengl-drawing
{
  if( parent != 0 )
    {
    systemParent->driver->current();

    glViewport( absPosition.x(),
                systemParent->size.y()-1-(absPosition.y()+size.y()-1),
                size.x(), size.y() );
    }
}

/////////////////////////////////////////////////////////////////////////////

void SoftWindow::setPosition( const IPoint& newPosition )
{
  if( newPosition == MLWindow::position )
    return;

  IVector delta= newPosition - MLWindow::position;

  MLWindow::position= newPosition;

  adjustAbsPosition(delta);

  MLWindow::postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void SoftWindow::adjustAbsPosition( const IVector& delta )
{
  absPosition+= delta;

  IListIterator<SoftWindow> li(softChildren);
  while( ! li.eol() )
    {
    SoftWindow* child= li.next();
    child->adjustAbsPosition(delta);
    }
}

/////////////////////////////////////////////////////////////////////////////

void SoftWindow::setSize( const IVector& newSize )
{
  if( newSize == MLWindow::size )
    return;

  MLWindow::size= newSize;

  MLWindow::postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void SoftWindow::scissor()
{
  glScissor( absPosition.x(),
             systemParent->size.y()-1-(absPosition.y()+size.y()-1),
             size.x(), size.y() );
  glEnable(GL_SCISSOR_TEST);
}

/////////////////////////////////////////////////////////////////////////////

void SoftWindow::unscissor()
{
  glDisable(GL_SCISSOR_TEST);
}

/////////////////////////////////////////////////////////////////////////////
