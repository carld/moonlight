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
  InterfaceAppRoot.h

  Stephane Rehel

  May 28 1998
*/

#ifndef __InterfaceAppRoot_h
#define __InterfaceAppRoot_h

#ifndef __MLAppRoot_h
#include "kernel/MLAppRoot.h"
#endif

class Interface;
class KoalaNodeTypes;

/////////////////////////////////////////////////////////////////////////////

class InterfaceAppRoot: public MLAppRoot
{
protected:
  Interface* interface;
  KoalaNodeTypes* koalaNodeTypes;

public:
  InterfaceAppRoot( MLKernel* _kernel, Interface* _interface );

  virtual ~InterfaceAppRoot();

  virtual boolean init();

  virtual MLNodeType::Type getNodeType() const;

  Interface* getInterface()
    {
    return interface;
    }

  KoalaNodeTypes* getNodeTypes() const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __InterfaceAppRoot_h
