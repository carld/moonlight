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
  MLCmdLineConvert.h

  Stephane Rehel

  August 6 1997
*/

#ifndef __MLCmdLineConvert_h
#define __MLCmdLineConvert_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

class MLCommandLine;
class MLConfig;
class MLKernel;

/////////////////////////////////////////////////////////////////////////////

class MLCmdLineConvert
{
protected:
  MLConfig* config;

public:
  MLCmdLineConvert( MLConfig* _config );

  virtual ~MLCmdLineConvert();

  int run( MLKernel* kernel, MLCommandLine& command );
};

/////////////////////////////////////////////////////////////////////////////

#endif // __MLCmdLineConvert_h
