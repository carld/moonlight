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
  MLRunnableProc.h

  Stephane Rehel
  May 13 1998
*/

#ifndef __MLRunnableProc_h
#define __MLRunnableProc_h

#include "tools/boolean.h"
#include "tools/MLReference.h"

class MLProcedure;
class MLProcArgs;
class MLNode;

/////////////////////////////////////////////////////////////////////////////

class MLRunnableProc: public MLReference
{
protected:
  MLProcedure* body;
  MLProcArgs* args;

  MLRunnableProc* undo_proc;
  boolean is_undo;

public:
  MLRunnableProc( MLProcedure* _body= 0, MLProcArgs* _args= 0 );

  virtual ~MLRunnableProc();

  MLProcedure* getProcedure() const
    {
    return body;
    }

  MLProcArgs* getArguments() const
    {
    return args;
    }

  void setProcedure( MLProcedure* _body );
  void setArguments( MLProcArgs* _args );

  MLRunnableProc* getUndo() const
    {
    return undo_proc;
    }

  void setUndo( MLRunnableProc* _undo_proc );

  boolean isUndo() const
    {
    return is_undo;
    }

  // create an undo proc without any argument
  MLRunnableProc* createUndo();

  MLNode* run();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLRunnableProc_h
