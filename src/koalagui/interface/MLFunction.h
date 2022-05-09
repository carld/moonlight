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
  MLFunction.h

  Stephane Rehel
  August 2 1997
*/

#ifndef __MLFunction_h
#define __MLFunction_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

class MLModule;

/////////////////////////////////////////////////////////////////////////////

class MLFunction
{
  friend class MLFunctionsManager; // calls complete() & cancel()
  friend class MLFunctions; // set function id
private:
  int function_id;
protected:
  MLFunction* set_function_id( MLFunction* f ) const
    {
    f->function_id= function_id;
    return f;
    }
protected:
  MLModule* module;
  boolean please_return; // return from interface loop
  boolean cancelled;
  boolean exitFunction; // cancelled or completed

public:
  MLFunction( MLModule* _module );

  virtual ~MLFunction();

  int getID() const
    {
    return function_id;
    }

  // return false if cancelled
  boolean call();

  virtual boolean isBaseFunction() const;

protected:
  // return false if cancelled
  virtual boolean _call() = 0;

public:
  // set function validated
  // return true if successully completed
  virtual boolean validate();

  // set function completed
  // return true if successully completed
  virtual boolean complete();

  // set function cancelled
  // return true if successully cancelled
  virtual boolean cancel();

public:
  virtual MLFunction* newFunction() const = 0;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLFunction_h
