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
  MLCommand.h

  Stephane Rehel

  November 28 1996
*/

#ifndef __MLCommand_h
#define __MLCommand_h

#ifndef __MLReference_h
#include "tools/MLReference.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLCommand: public MLReference
{
public:
  int isUndo; // set by Interface
  int isUserCommand; // set by MLModule
  int lonelyCommand; // =1 if this command can be isolated in an undo/redo
                     // ex: draging doesn't generate lonely commands
  int canUndo;

  int module; // to which module
  int canvas; // to which canvas
  int engine; // to which engine

  int base;
  int type;

  MLCommand()
    {
    isUndo= 0;
    isUserCommand= 1;
    lonelyCommand= 1;
    canUndo= 1;
    base= module= canvas= engine= type= 0;
    }

  virtual ~MLCommand() {}

  void post();

  int getType() const
    {
    return type & 0xffff; // remove base
    }
  int getBase() const
    {
    return base;
    }

  // init an undo command for the current command
  virtual void initUndo( MLCommand* undo ) const;

  // return an unreferenced undo
  virtual MLCommand* getUndo() = 0;

protected:
  static int allocBase();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLCommand_h
