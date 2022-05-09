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
   VCircleButton.C

   Creation: SR, August 9th, 1995
   Revisions:
     + SR July 5 1996
*/

#include "graphics/MLPixmap.h"
#include "graphics/MLPixmaps.h"

#include "VCircleButton.h"


/////////////////////////////////////////////////////////////////////////////

VCircleButton::VCircleButton( VWindow* _parent ): VButtonBase(_parent)
{}

/////////////////////////////////////////////////////////////////////////////

VCircleButton::~VCircleButton()
{}

/////////////////////////////////////////////////////////////////////////////

void VCircleButton::create( int upperleftCorner_x, int upperleftCorner_y )
{
  VButtonBase::create( upperleftCorner_x, upperleftCorner_y,
                       MLPixmaps::emptyCircle->getWidth(),
                       MLPixmaps::emptyCircle->getHeight() );
}

/////////////////////////////////////////////////////////////////////////////

void VCircleButton::create( const IPoint& upperleftCorner )
{
  create( upperleftCorner.x(), upperleftCorner.y() );
}

/////////////////////////////////////////////////////////////////////////////

void VCircleButton::drawButton()
{
  if( ! beginDraw() )
    return;

  if( state == RELEASED )
    drawPixmap( MLPixmaps::emptyCircle, 0, 0 );
   else
    drawPixmap( MLPixmaps::filledCircle, 0, 0 );

  endDraw();
}

/////////////////////////////////////////////////////////////////////////////
