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
  MLInterfaceCommand.h

  Stephane Rehel

  March 3 1997
*/

#ifndef __MLInterfaceCommand_h
#define __MLInterfaceCommand_h

#ifndef __MLCommand_h
#include "MLCommand.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __Key_h
#include "graphics/Key.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLInterfaceCommand: public MLCommand
{
public:
  static int BASE;

public:
  // general commands
  enum COMMON
    {
    FIRST= 0,

    CMD_STOP, // used to isolate lonely commands

    ACTIVATE_MODULE,

    REFRESH_MODULE,
    REFRESH_CANVAS_i,

    MIN_MAX_CANVAS,
    MINIMIZE_CANVAS,
    MAXIMIZE_CANVAS,

    ACTIVATE_ENGINE,
    POPUP,
    KEY_PRESS,
    KEY_RELEASE,

    REMOVE_DIALOG,
    REFRESH_DIALOG_i,

    QUIT,

    LAST
    };

  int index;
  MLString string;
  Key key;

  MLInterfaceCommand()
    {
    index= 0;
    key= Key::NULL_KEY;
    string= "";
    base= BASE;
    canUndo= 0;
    }

  virtual ~MLInterfaceCommand() {}

  virtual MLCommand* getUndo();

  static void initInterfaceCommands();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLInterfaceCommand_h
