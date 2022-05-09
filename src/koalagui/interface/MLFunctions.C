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
  MLFunctions.C

  Stephane Rehel
  August 2 1997
*/

#include "MLFunctions.h"

#include "MLFunction.h"

/////////////////////////////////////////////////////////////////////////////

static const int max_functions= 256;

/////////////////////////////////////////////////////////////////////////////

MLFunctions::MLFunctions( MLModule* _module )
{
  module= _module;

  functions= new FunctionKey [ max_functions + 1 ];

  for( int i= 0; i <= max_functions; ++i )
    {
    functions[i].key= Key::NULL_KEY;
    functions[i].f= 0;
    }

  nFunctions= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLFunctions::~MLFunctions()
{
  if( functions != 0 )
    {
    for( int i= 0; i <= max_functions; ++i )
      {
      FunctionKey& fk= functions[i];
      delete fk.f;
      }
    delete functions;
    functions= 0;
    }

  nFunctions= 0;
}

/////////////////////////////////////////////////////////////////////////////

// MLFunctions becomes the owner of *f
void MLFunctions::add( int id, const Key& key, MLFunction* f )
{
  if( key == Key::NULL_KEY || f == 0 )
    return;

  if( get(key) != 0 )
    return;

  if( nFunctions >= max_functions )
    return;

  for( int i= 1; i <= nFunctions; ++i )
    if( functions[i].f == f )
      return;

  ++nFunctions;
  FunctionKey& fk= functions[nFunctions];
  fk.key= key;
  fk.f= f;

  f->function_id= id;
}

/////////////////////////////////////////////////////////////////////////////

// return function associated with the specified key
MLFunction* MLFunctions::get( const Key& key ) const
{
  for( int i= 1; i <= nFunctions; ++i )
    {
    FunctionKey& fk= functions[i];
    if( fk.key == key )
      return fk.f;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

MLFunction* MLFunctions::find( int id ) const
{
  for( int i= 1; i <= nFunctions; ++i )
    {
    FunctionKey& fk= functions[i];
    if( fk.f == 0 )
      continue;
    if( fk.f->getID() == id )
      return fk.f;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////
