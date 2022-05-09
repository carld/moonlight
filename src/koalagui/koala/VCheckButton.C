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
   VCheckButton.C

   Creation: SR, August 10th, 1995
   Revisions:
     + SR July 5 1996

*/

#include "graphics/MLPixmaps.h"

#include "VCheckButton.h"


/////////////////////////////////////////////////////////////////////////////

VCheckButton::VCheckButton( VWindow* _parent ): VBlankButton(_parent)
{}

/////////////////////////////////////////////////////////////////////////////

VCheckButton::~VCheckButton()
{}

/////////////////////////////////////////////////////////////////////////////

void VCheckButton::drawButton()
{
  if( ! beginDraw() )
    return;

  gray1();
  drawHorizLine(0,width-1,0);
  drawHorizLine(1,width-2,1);
  drawVertLine( 0, 0, height-2 );
  drawVertLine( 1, 1, height-3 );
  gray2();
  drawHorizLine( 1, width-2, height-2 );
  drawVertLine( width-2, 2, height-2 );
  gray3();
  drawHorizLine( 0, width-1, height-1 );
  drawVertLine( width-1, 1, height-1 );
  fillRectangle( IPoint(2,2), IPoint(width-3,height-3) );

  if( VButtonBase::state == VButtonBase::PRESSED )
    drawPixmap( MLPixmaps::check, 3, 2 );

  endDraw();
}

/////////////////////////////////////////////////////////////////////////////
