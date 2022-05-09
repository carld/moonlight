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
  MLFunctions.h

  Stephane Rehel

  August 2 1997
*/

#ifndef __MLFunctions_h
#define __MLFunctions_h

#ifndef __Key_h
#include "graphics/Key.h"
#endif

class MLModule;
class MLFunction;

/////////////////////////////////////////////////////////////////////////////

class MLFunctions
{
protected:
  MLModule* module;

  struct FunctionKey
    {
    Key key;
    MLFunction* f; // owner!
    };

  FunctionKey* functions;
  int nFunctions;

public:
  MLFunctions( MLModule* _module );

  virtual ~MLFunctions();

  // MLFunctions becomes the owner of *f
  void add( int id, const Key& key, MLFunction* f );

  // return function associated with the specified key
  MLFunction* get( const Key& key ) const;

  MLFunction* find( int id ) const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLFunctions_h
