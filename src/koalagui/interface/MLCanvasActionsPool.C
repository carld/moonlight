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
  MLCanvasActionsPool.C

  Stephane Rehel
  December 1 1996
*/

#include "graphics/SoftWindow.h"

#include "MLCanvas.h"
#include "MLCanvasAction.h"
#include "MLCanvasPopup.h"
#include "MLCanvasButton.h"

#include "MLCanvasActionsPool.h"

/////////////////////////////////////////////////////////////////////////////

static const int max_actions= 30;

/////////////////////////////////////////////////////////////////////////////

MLCanvasActionsPool::MLCanvasActionsPool( MLCanvas* _canvas )
{
  canvas= _canvas;

  actions= new MLCanvasAction* [ max_actions ];
  for( int i= 0; i < max_actions; ++i )
    actions[i]= 0;

  nActions= 0;
  _mapped= true;
}

/////////////////////////////////////////////////////////////////////////////

MLCanvasActionsPool::~MLCanvasActionsPool()
{
  for( int i= 0; i < nActions; ++i )
    {
    MLCanvasAction* a= actions[i];
    actions[i]= 0;
    delete a;
    }

  delete actions;
  actions= 0;
  nActions= 0;
  canvas= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasActionsPool::map()
{
  for( int i= 0; i < nActions; ++i )
    {
    MLCanvasAction* a= actions[i];
    if( a == 0 )
      continue;
    a->map();
    }

  _mapped= true;
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasActionsPool::unmap()
{
  for( int i= 0; i < nActions; ++i )
    {
    MLCanvasAction* a= actions[i];
    if( a == 0 )
      continue;
    a->map(false);
    }

  _mapped= false;
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasActionsPool::addLeftAction( MLCanvasAction* action )
{
  if( action == 0 )
    return;

  action->left= 1;
  addAction(action);
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasActionsPool::addRightAction( MLCanvasAction* action )
{
  if( action == 0 )
    return;

  action->left= -1;
  addAction(action);
}

/////////////////////////////////////////////////////////////////////////////

// protected
void MLCanvasActionsPool::addAction( MLCanvasAction* action )
{
  if( action == 0 )
    return;

  if( nActions >= max_actions )
    return;

  for( int i= 0; i < nActions; ++i )
    if( actions[i] == action )
      return; // crazy?

  actions[nActions]= action;
  ++nActions;
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasActionsPool::removeAction( MLCanvasAction* action )
{
  if( action == 0 )
    return;

  int i;
  for( i= 0; i < nActions; ++i )
    if( actions[i] == action )
      break;

  if( i >= nActions )
    return;

  while( i+1 < nActions )
    {
    actions[i]= actions[i+1];
    ++i;
    }

  --nActions;
  actions[nActions]= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasActionsPool::draw()
{
  if( ! mapped() )
    return;

  if( canvas == 0 )
    return;
  if( ! canvas->mapped() )
    return;

  SoftWindow* window= canvas->titleWindow;
  if( window == 0 )
    return;
  if( ! window->mapped() )
    return;

  for( int i= 0; i < nActions; ++i )
    {
    MLCanvasAction* a= actions[i];
    if( a == 0 )
      continue;

    a->draw();
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasActionsPool::destroyActionsWindows()
{
  for( int i= 0; i < nActions; ++i )
    {
    MLCanvasAction* a= actions[i];
    if( a == 0 )
      continue;
    a->destroyWindow();
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLCanvasActionsPool::reconfigure()
{
  if( ! mapped() )
    return;

  if( canvas == 0 )
    return;

  if( ! canvas->mapped() )
    return;

  SoftWindow* window= canvas->titleWindow;
  if( window == 0 )
    return;
  if( ! window->mapped() )
    return;

  int left= 0;
  int right= window->getSize().x();

  if( canvas->enginesPopup != 0 )
    {
    MLCanvasAction* a= canvas->enginesPopup;
    if( a->window == 0 )
      a->createWindow();
     else
      {
      int x= left;
      int y_pos= a->window->getPosition().y();
      a->window->setPosition( IPoint(x,y_pos) );
      int y_size= a->window->getSize().y();
      a->window->setSize(IVector(a->getWidth(),y_size));
      }

    left+= canvas->enginesPopup->getWidth();
    }

  if( canvas->minmaxButton != 0 )
    {
    right-= canvas->minmaxButton->getWidth();

    MLCanvasAction* a= canvas->minmaxButton;
    if( a->window == 0 )
      a->createWindow();
     else
      {
      int x= right;
      int y_pos= a->window->getPosition().y();
      a->window->setPosition( IPoint(x,y_pos) );
      int y_size= a->window->getSize().y();
      a->window->setSize(IVector(a->getWidth(),y_size));
      }
    }

  for( int i= 0; i < nActions; ++i )
    {
    MLCanvasAction* a= actions[i];
    if( a == 0 )
      continue;

    a->x= (a->left==1) ? left : right;
    if( a->window == 0 )
      a->createWindow();
     else
      {
      int x;
      if( a->left == 1 )
        x= left;
       else
        x= right - a->getWidth();

      int y_pos= a->window->getPosition().y();
      a->window->setPosition( IPoint(x,y_pos) );
      int y_size= a->window->getSize().y();
      a->window->setSize(IVector(a->getWidth(),y_size));
      }

    if( a->window != 0 )
      {
      a->window->removeRedraw();
      if( a->left == 1 )
        left+= a->getWidth();
       else
        right-= a->getWidth();
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLCanvasActionsPool::find( MLAction* action ) const
{
  if( action == 0 )
    return false;

  for( int i= 0; i < nActions; ++i )
    {
    MLCanvasAction* a= actions[i];
    if( a == action )
      return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////
