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
  MLProcArgs.h

  Stephane Rehel
  March 30 1998
*/

#ifndef __MLProcArgs_h
#define __MLProcArgs_h

#include <stdarg.h>

#include "MLNode.h"

class MLProcedure;

/////////////////////////////////////////////////////////////////////////////

class MLProcArgs: public MLNode
{
protected:
  MLNode* base_node;

public:
  MLProcArgs( MLKernel* kernel );

  virtual ~MLProcArgs();

  void setBaseNode( MLNode* _base_node );

  MLNode* getBaseNode() const
    {
    return base_node;
    }

  MLString toString();

  static MLProcArgs* create( MLProcedure* procedure, va_list ap );

  static MLProcArgs* create( MLProcedure* procedure, ... );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLProcArgs_h
