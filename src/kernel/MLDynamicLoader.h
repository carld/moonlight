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
  MLDynamicLoader.h

  Stephane Rehel
  April 21 1998
*/

#ifndef __MLDynamicLoader_h
#define __MLDynamicLoader_h

#include "tools/boolean.h"
#include "tools/MLString.h"

class MLDynamicLibrary;
class MLKernel;

/////////////////////////////////////////////////////////////////////////////

class MLDynamicLoader
{
protected:
  MLDynamicLibrary* dl;
  MLString errorMessage;

  /**
   * with full path information,
   * for instance: "/usr/local/lib/libmoon_kmodel.so"
   */
  MLString full_lib_filename;

public:
  MLDynamicLoader();

  virtual ~MLDynamicLoader();

  /**
   * try to open the .so-file as referenced by _la_filename,
   * use kernel to get the standard pathnames
   */
  boolean load( MLKernel* kernel, const MLString a_filename );

  /** if there was an error, get the error string */
  virtual MLString getErrorMessage();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLDynamicLoader_h
