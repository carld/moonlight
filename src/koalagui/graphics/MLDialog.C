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
  MLDialog.C

  Stephane Rehel
  July 1 1996
*/

#include "mlgl.h"

#include "MLDialog.h"
#include "MLEvent.h"
#include "MLFont.h"
#include "MLFonts.h"

#include "SystemWindow.h"

/////////////////////////////////////////////////////////////////////////////

MLDialog::MLDialog()
{
  systemWindow= 0;
  position= IPoint(0,0);
  size= IVector(0,0);
  title= MLString("");
}

/////////////////////////////////////////////////////////////////////////////

MLDialog::~MLDialog()
{
  delete systemWindow;
  systemWindow= 0;
}

/////////////////////////////////////////////////////////////////////////////

// _parent != 0
boolean MLDialog::create( SystemWindow* _parent,
                        int x, int y, int width, int height,
                        const MLString& _title )
{
  if( systemWindow != 0 )
    return false;

  title= _title;

  systemWindow= _parent->newSystemChild(x,y,width,height);

  if( systemWindow == 0 )
    return false;

  systemWindow->setEventHandler(this);

  position= IPoint(x,y);
  size= IVector(width,height);

  systemWindow->map();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLDialog::redraw()
{
  if( systemWindow == 0 )
    return;

  systemWindow->current();
  systemWindow->pixelMode();
  mlBack();
  systemWindow->clear(.5,.5,.5);

mlLightGray();
mlRect( 0, 0,
        systemWindow->getSize().x()-1, systemWindow->getSize().y()-1 );

  drawFrame();

  // redraw widgets
  // ...

  systemWindow->swapBuffers();
}

/////////////////////////////////////////////////////////////////////////////

// private
void MLDialog::drawFrame()
{
  mlBlack();
  mlFrame( 0, 0, size.x()-1, size.y()-1 );

  int fh= normalFont->getHeight();

  mlRect( 0, size.y()-1-fh, size.x()-1, size.y()-1 );

  mlMiddleGray();
  normalFont->print( 1+normalFont->getCharWidth(' '),
                     size.y()-1-fh+normalFont->getDescent(),
                     title );
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDialog::eventHandler( MLWindow* dialog, MLEvent& event )
{
  if( dialog != systemWindow )
    return false;

  if( event.type == MLEvent::REDRAW )
    {
    redraw();
    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////
