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
  MLFunctionsManager.h

  Stephane Rehel
  August 2 1997
*/

#ifndef __MLFunctionsManager_h
#define __MLFunctionsManager_h

#ifndef __IStack_h
#include "tools/IStack.h"
#endif

class MLFunction;
class MLFunctions;
class MLModule;

/////////////////////////////////////////////////////////////////////////////

class MLFunctionsManager
{
protected:
  MLModule* module;

  // stack of currently called functions
  IStack<MLFunction> functions;

public:
  MLFunctionsManager( MLModule* _module );

  virtual ~MLFunctionsManager();

  boolean call( int id );
  boolean call( MLFunction* f );

  MLFunction* getCurrentFunction() const;

  // return true if successfully validated
  boolean validateCurrentFunction();

  // return true if successfully completed
  boolean completeCurrentFunction();

  // return true if successfully cancelled
  boolean cancelCurrentFunction();

  // called by MLModule
  boolean last_function_must_return() const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLFunctionsManager_h
