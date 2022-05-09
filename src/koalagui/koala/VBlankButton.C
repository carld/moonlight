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
   VBlankButton.C

   Creation: SR, July 28th, 1995
   Revisions:
     + SR July 5 1996

*/

#include "VBlankButton.h"

/////////////////////////////////////////////////////////////////////////////

VBlankButton::VBlankButton( VWindow* _parent ): VButtonBase(_parent)
{}

/////////////////////////////////////////////////////////////////////////////

VBlankButton::~VBlankButton()
{}

/////////////////////////////////////////////////////////////////////////////

void VBlankButton::create( int x, int y, int width, int height )
{
  VButtonBase::create( x, y, width, height );
}

/////////////////////////////////////////////////////////////////////////////

void VBlankButton::create( const IPoint& p1, const IPoint& p2 )
{
  VButtonBase::create( p1, p2 );
}

/////////////////////////////////////////////////////////////////////////////

void VBlankButton::drawButton()
{
  if( ! beginDraw() )
    return;

  _drawButton();
  endDraw();
}

/////////////////////////////////////////////////////////////////////////////

void VBlankButton::_drawButton()
{
  VButtonBase::_drawButtonBase();
}

/////////////////////////////////////////////////////////////////////////////
