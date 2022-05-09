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
  MLRunnableProc.C

  Stephane Rehel
  May 13 1998
*/

#include "MLRunnableProc.h"

#include "MLProcedure.h"
#include "MLProcArgs.h"

/////////////////////////////////////////////////////////////////////////////

MLRunnableProc::MLRunnableProc( MLProcedure* _body /* = 0 */,
                                MLProcArgs* _args /* = 0 */ )
{
  body= _body;
  ref(body);

  args= _args;
  ref(args);

  undo_proc= 0;
  is_undo= false;
}

/////////////////////////////////////////////////////////////////////////////

MLRunnableProc::~MLRunnableProc()
{
  unref(body);
  body= 0;

  unref(args);
  args= 0;

  unref(undo_proc);
  undo_proc= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLRunnableProc::setProcedure( MLProcedure* _body )
{
  ref(_body);
  unref(body);
  body= _body;
}

/////////////////////////////////////////////////////////////////////////////

void MLRunnableProc::setArguments( MLProcArgs* _args )
{
  ref(_args);
  unref(args);
  args= _args;
}

/////////////////////////////////////////////////////////////////////////////

MLNode* MLRunnableProc::run()
{
  if( body == 0 )
    return 0;

  return body->run(args);
}

/////////////////////////////////////////////////////////////////////////////

void MLRunnableProc::setUndo( MLRunnableProc* _undo_proc )
{
  ref(_undo_proc);
  unref(undo_proc);
  undo_proc= _undo_proc;
}

/////////////////////////////////////////////////////////////////////////////

// create an undo proc without any argument
MLRunnableProc* MLRunnableProc::createUndo()
{
  MLRunnableProc* up= new MLRunnableProc(body);
  up->is_undo= ! is_undo;

  up->setUndo(this);

  setUndo(up);

  return up;
}

/////////////////////////////////////////////////////////////////////////////
