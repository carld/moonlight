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
  ModelCommand.h

  Stephane Rehel

  December 15 1996
*/

#ifndef __ModelCommand_h
#define __ModelCommand_h

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

#ifndef __IntegerArray_h
#include "tools/IntegerArray.h"
#endif

#ifndef __MLCommand_h
#include "interface/MLCommand.h"
#endif

class ModelModule;

/////////////////////////////////////////////////////////////////////////////

class ModelCommand: public MLCommand
{
public:
  static int BASE;

public:
  int index;

  Vector v;
  MLString string;

  IntegerArray indices;
  IntegerArray indices2;

  ModelCommand* undo;

public:
  ModelCommand()
    {
    MLCommand::base= BASE;

    string= "";
    undo= 0;

    index= 0;
    }

  virtual ~ModelCommand()
    {
    if( undo != 0 )
      {
      unref(undo);
      undo= 0;
      }
    }

  virtual ModelCommand* createUndo( ModelCommand* cmd = 0 );

  virtual MLCommand* getUndo()
    {
    return undo;
    }

  void swapIndices();

  static void initModelCommands();

  // return true if implemented
  virtual boolean doit( ModelModule* model );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelCommand_h
