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
  MLKernelModule.h

  Stephane Rehel
  April 19 1998
*/

#ifndef __MLKernelModule_h
#define __MLKernelModule_h

#include "tools/boolean.h"

#include "MLNode.h"

class MLString;
class MLKernel;

/////////////////////////////////////////////////////////////////////////////

class MLKernelModule: public MLNode
{
public:
  MLKernelModule( MLKernel* kernel, MLNode* data );

  virtual ~MLKernelModule();

//  virtual const MLString& getName() const = 0;

  virtual MLAppRoot* init( MLKernel* kernel, MLNode* data ) = 0;
};

/////////////////////////////////////////////////////////////////////////////

#ifdef __CYGWIN32__

#define KERNEL_MODULE_INIT(MODULE_NAME,MODULE_CLASS_NAME) \
 extern "C"                                                      \
 MLAppRoot* MODULE_NAME##_main( MLKernel* kernel, MLNode* data )     \
 {                                                               \
   MLKernelModule* m= new MODULE_CLASS_NAME(kernel,data);        \
                                                                 \
   return m->init( kernel, data );                               \
 }
#define LOAD_KERNEL_MODULE(MODULE_NAME,KERNEL,DATA,ERROR_MSG) \
  MODULE_NAME##_main(KERNEL,DATA)

/*  extern "C" MLAppRoot* MODULE_NAME##_main( MLKernel*, MLNode* ); */

#else

#define KERNEL_MODULE_INIT(MODULE_NAME,MODULE_CLASS_NAME) \
 extern "C"                                                      \
 MLAppRoot* ml_module_main( MLKernel* kernel, MLNode* data )     \
 {                                                               \
   MLKernelModule* m= new MODULE_CLASS_NAME(kernel,data);        \
                                                                 \
   return m->init( kernel, data );                               \
 }

#define LOAD_KERNEL_MODULE(MODULE_NAME,KERNEL,DATA,ERROR_MSG) \
  (KERNEL)->loadModule(#MODULE_NAME,DATA,ERROR_MSG)

#endif

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLKernelModule_h
