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
  ModelKernelModule.h

  Stephane Rehel

  April 16 1998
*/

#ifndef __ModelKernelModule_h
#define __ModelKernelModule_h

#include "tools/MLString.h"
#include "kernel/MLKernelModule.h"

/////////////////////////////////////////////////////////////////////////////

class ModelKernelModule: public MLKernelModule
{
protected:
  MLString name;

public:
  ModelKernelModule( MLKernel* kernel, MLNode* data );

  virtual ~ModelKernelModule();

  virtual const MLString& getName() const
    {
    return name;
    }

  virtual MLAppRoot* init( MLKernel* kernel, MLNode* data );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelKernelModule_h
