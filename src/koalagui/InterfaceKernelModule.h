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
  InterfaceKernelModule.h

  Stephane Rehel

  May 31 1998
*/

#ifndef __InterfaceKernelModule_h
#define __InterfaceKernelModule_h

#include "tools/MLString.h"
#include "kernel/MLKernelModule.h"

/////////////////////////////////////////////////////////////////////////////

class InterfaceKernelModule: public MLKernelModule
{
protected:
  MLString name;

public:
  InterfaceKernelModule( MLKernel* kernel, MLNode* data );

  virtual ~InterfaceKernelModule();

  virtual const MLString& getName() const
    {
    return name;
    }

  virtual MLAppRoot* init( MLKernel* kernel, MLNode* data );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __InterfaceKernelModule_h
