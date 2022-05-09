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
  WindowDriver.C

  Abstract Window Driver Class

  Stephane Rehel
  July 4, 1996
*/

#include "GraphicsSystem.h"
#include "SystemWindow.h"

#include "WindowDriver.h"

#include "MLCursors.h"

/////////////////////////////////////////////////////////////////////////////

WindowDriver::WindowDriver( SystemWindow* _systemWindow )
{
  systemWindow= _systemWindow;
  cursor= saveCursor= 0;
  pushedWaitCursor= 0;
}

/////////////////////////////////////////////////////////////////////////////

WindowDriver::~WindowDriver()
{}

/////////////////////////////////////////////////////////////////////////////

boolean WindowDriver::hasSaveUnder() const
{
  if( systemWindow == 0 )
    return false;

  return systemWindow->saveUnder;
}

/////////////////////////////////////////////////////////////////////////////

void WindowDriver::setCursor( MLCursor* _cursor )
{
  if( _cursor == 0 )
    _cursor= MLCursors::normal;

  if( _cursor == cursor )
    return;

  cursor= _cursor;

  _setCursor();
}

/////////////////////////////////////////////////////////////////////////////

void WindowDriver::pushWaitCursor()
{
  ++pushedWaitCursor;
  if( pushedWaitCursor == 1 )
    {
    saveCursor= cursor;
    setCursor( MLCursors::hourglass );
    }
}

/////////////////////////////////////////////////////////////////////////////

void WindowDriver::popWaitCursor()
{
  if( pushedWaitCursor == 1 )
    {
    setCursor( saveCursor );
    saveCursor= 0;
    }
  --pushedWaitCursor;
}

/////////////////////////////////////////////////////////////////////////////
