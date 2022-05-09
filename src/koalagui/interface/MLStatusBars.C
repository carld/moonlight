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
  MLStatusBars.C

  Stephane Rehel

  December 8 1996
*/

#include "MLStatusBars.h"

#include "graphics/mlgl.h"
#include "graphics/SoftWindow.h"
#include "graphics/SystemWindow.h"
#include "graphics/MLEvent.h"

#include "MLStatusLine.h"

/////////////////////////////////////////////////////////////////////////////

MLStatusBars::MLStatusBars()
{
  window= 0;
  lines= 0;
  nLines= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLStatusBars::~MLStatusBars()
{
  for( int i= 0; i < nLines; ++i )
    {
    MLStatusLine* line= lines[i];
    lines[i]= 0;
    delete line;
    }

  delete lines;
  lines= 0;
  nLines= 0;

  delete window;
  window= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusBars::create( SystemWindow* root,
                           const IPoint& position, const IVector& size )
{
  if( window != 0 )
    return; // nimporte koi

  int i;

  nLines= 2;
  lines= new MLStatusLine* [nLines];
  for( i= 0; i < nLines; ++i )
    lines[i]= 0;

  window= new SoftWindow;
  window->create( root, position.x(), position.y(), size.x(), size.y() );
  window->setEventHandler(this);

  int lineHeight= MLStatusLine::getHeight();

  for( i= 0; i < nLines; ++i )
    {
    lines[i]= new MLStatusLine( window,
                                IPoint(2,2+(lineHeight+1)*i),
                                size.x()-3 );
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusBars::map( boolean yes /* = true */ )
{
  if( window != 0 )
    window->map(yes);
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusBars::postRedraw()
{
  if( window == 0 )
    return;
  if( ! window->mapped() )
    return;

  window->postRedraw();

  for( int i= 0; i < nLines; ++i )
    lines[i]->postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusBars::draw()
{
  if( window == 0 )
    return;
  if( ! window->mapped() )
    return;

  const IVector& size= window->getSize();

  window->currentPixel();
  mlFront();

  mlDarkGray();
  mlRect( 1, 0, size.x()-1, size.y()-2 );
  int lineHeight= MLStatusLine::getHeight();
  mlLine( 1, 1+lineHeight, size.x()-1, 1+lineHeight );

  mlLightGray();
  mlLine( 0, size.y()-1, size.x()-1, size.y()-1 );
  mlLine( 0, 0, 0, size.y()-1 );
}

/////////////////////////////////////////////////////////////////////////////

boolean MLStatusBars::eventHandler( MLWindow* w, MLEvent& e )
{
  if( w != window || window == 0 )
    return false;

  if( e.type == MLEvent::REDRAW )
    {
    draw();
    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

// sent by Interface::handleMLKeyboard()
// return false if the key event must be handle by the interface
boolean MLStatusBars::handleMLKeyboard( MLEvent& event )
{
  for( int i= 0; i < nLines; ++i )
    {
    MLStatusLine* line= lines[i];
    if( line == 0 )
      continue;
    if( line->handleMLKeyboard(event) )
      return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////
