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
  XEventManager.h

  Stephane Rehel
  June 29, 1996
*/

#ifndef __XEventManager_h
#define __XEventManager_h

#ifndef __MLEventManager_h
#include "MLEventManager.h"
#endif

#include <X11/Xlib.h>

class SystemWindow;

/////////////////////////////////////////////////////////////////////////////

class XEventManager: public MLEventManager
{
public:
  XEventManager();
  virtual ~XEventManager();

  // return false if timed out
  boolean wait();

  void get();

protected:
  SystemWindow* findWindow( Window windowID );

  void handleEvent( XEvent* event );

  // Called by any thread for unblocking the main thread
//  void unblockWaiting();

private:
  SystemWindow* _findWindow( SystemWindow* sw, Window windowID );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __XEventManager_h
