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
  MLFunctionsManager.C

  Stephane Rehel
  August 2 1997
*/

#include "MLModule.h"

#include "MLFunctionsManager.h"

#include "MLFunction.h"
#include "MLFunctions.h"

/////////////////////////////////////////////////////////////////////////////

MLFunctionsManager::MLFunctionsManager( MLModule* _module )
{
  module= _module;
}

/////////////////////////////////////////////////////////////////////////////

MLFunctionsManager::~MLFunctionsManager()
{
  while( ! functions.isEmpty() )
    {
    MLFunction* f= functions.pop();
    delete f;
    }
}

/////////////////////////////////////////////////////////////////////////////

MLFunction* MLFunctionsManager::getCurrentFunction() const
{
  if( functions.isEmpty() )
    return 0;

  return functions.peek();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLFunctionsManager::call( int id )
{
  MLFunctions* functions= module->getFunctions();
  if( functions == 0 )
    return false;

  MLFunction* f= functions->find(id);
  if( f == 0 )
    return false;

  return call(f);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLFunctionsManager::call( MLFunction* f )
{
  if( f == 0 )
    return false;

  functions.push(f);

  f->please_return= false;
  f->cancelled= false;
  f->exitFunction= false;

  boolean outcome= f->_call();

  functions.pop();

  return outcome;
}

/////////////////////////////////////////////////////////////////////////////

// return true if successfully completed
boolean MLFunctionsManager::validateCurrentFunction()
{
  MLFunction* f= getCurrentFunction();
  if( f == 0 )
    return false;

  return f->validate();
}

/////////////////////////////////////////////////////////////////////////////

// return true if successfully completed
boolean MLFunctionsManager::completeCurrentFunction()
{
  MLFunction* f= getCurrentFunction();
  if( f == 0 )
    return false;

  return f->complete();
}

/////////////////////////////////////////////////////////////////////////////

// return true if successfully cancelled
boolean MLFunctionsManager::cancelCurrentFunction()
{
  MLFunction* f= getCurrentFunction();
  if( f == 0 )
    return false;

  return f->cancel();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLFunctionsManager::last_function_must_return() const
{
  MLFunction* f= getCurrentFunction();
  if( f == 0 )
    return false;

  return f->please_return;
}

/////////////////////////////////////////////////////////////////////////////
