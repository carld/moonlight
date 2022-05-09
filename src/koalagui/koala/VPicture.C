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
  VPicture.C

  Stephane Rehel
  August 16 1996
*/

#include "graphics/MLEvent.h"
#include "graphics/MLPictures.h"
#include "graphics/MLPicture.h"

#include "VPicture.h"

/////////////////////////////////////////////////////////////////////////////

VPicture::VPicture( VWindow* _parent ): VWidget(_parent)
{
  picture= 0;
}

/////////////////////////////////////////////////////////////////////////////

VPicture::~VPicture()
{
  MLPictures::unref(picture);
}

/////////////////////////////////////////////////////////////////////////////

void VPicture::create( int x, int y, MLPicture* _picture )
{
  picture= _picture;

  int width, height;
  if( picture == 0 )
    width= height= 20;
   else
    {
    width= picture->getWidth();
    height= picture->getHeight();
    }

  MLPictures::ref(picture);

  VWindow::create( x, y, width, height );
}

/////////////////////////////////////////////////////////////////////////////

void VPicture::drawWidget()
{
  removeRedraw();
  drawPicture();
}

/////////////////////////////////////////////////////////////////////////////

void VPicture::drawPicture()
{
  if( picture == 0 )
    return;

  if( ! beginDraw() )
    return;

  picture->draw(0,0);

  endDraw();
}

/////////////////////////////////////////////////////////////////////////////

boolean VPicture::handleEvent( MLEvent& event )
{
  if( event.type == MLEvent::REDRAW )
    {
    drawPicture();
    return true;
    }

  return VWindow::handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////
