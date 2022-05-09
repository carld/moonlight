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
  MLFunction.C

  Stephane Rehel
  August 2 1997
*/

#include "MLFunction.h"

#include "MLFunctionsManager.h"
#include "MLModule.h"

/////////////////////////////////////////////////////////////////////////////

MLFunction::MLFunction( MLModule* _module )
{
  module= _module;

  function_id= 0;

  please_return= false;
  cancelled= false;
  exitFunction= false;
}

/////////////////////////////////////////////////////////////////////////////

MLFunction::~MLFunction()
{
  module= 0;
}

/////////////////////////////////////////////////////////////////////////////

// return false if cancelled
boolean MLFunction::call()
{
  return module->getFunctionsManager()->call(this);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLFunction::isBaseFunction() const
{
  return false;
}

/////////////////////////////////////////////////////////////////////////////

// set function validated
// return true if successully completed
boolean MLFunction::validate()
{
  please_return= true;
  cancelled= false;
  exitFunction= false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// set function completed
// return true if successully completed
boolean MLFunction::complete()
{
  please_return= true;
  cancelled= false;
  exitFunction= true;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// set function cancelled
// return true if successully cancelled
boolean MLFunction::cancel()
{
  please_return= true;
  cancelled= true;
  exitFunction= true;

  return true;
}

/////////////////////////////////////////////////////////////////////////////
