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
  OOMCommand.h

  Stephane Rehel

  October 19 1997
*/

#ifndef __OOMCommand_h
#define __OOMCommand_h

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

#ifndef __IntegerArray_h
#include "tools/IntegerArray.h"
#endif

#ifndef __MLCommand_h
#include "interface/MLCommand.h"
#endif

class OOMModule;

/////////////////////////////////////////////////////////////////////////////

class OOMCommand: public MLCommand
{
public:
  static int BASE;

public:
  int index;

  Vector v;

  IntegerArray indices;

  OOMCommand* undo;

public:
  OOMCommand()
    {
    MLCommand::base= BASE;

    undo= 0;

    index= 0;
    }

  virtual ~OOMCommand()
    {
    if( undo != 0 )
      {
      unref(undo);
      undo= 0;
      }
    }

  virtual OOMCommand* createUndo( OOMCommand* cmd = 0 );

  virtual MLCommand* getUndo()
    {
    return undo;
    }

  static void initOOMCommands();

  // return true if implemented
  virtual boolean doit( OOMModule* oom );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __OOMCommand_h
