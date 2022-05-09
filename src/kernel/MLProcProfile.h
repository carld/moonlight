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
  MLProcProfile.h

  Stephane Rehel
  April 6 1998
*/

#ifndef __MLProcProfile_h
#define __MLProcProfile_h

#include <stdarg.h>

#include "tools/boolean.h"
#include "tools/MLArray.h"
#include "tools/MLString.h"

#include "MLNode.h"

class MLProcArgs;
class MLProcedure;

/////////////////////////////////////////////////////////////////////////////

class MLProcProfile: public MLNode
{
protected:
  MLProcedure* procedure; // not owner

  struct Param
    {
    MLString name;
    int type;
    };

  MLArray<Param*> params;

public:
  MLProcProfile( MLProcedure* _procedure );

  virtual ~MLProcProfile();

  MLProcedure* getProcedure() const
    {
    return procedure;
    }

  virtual MLNodeType::Type getNodeType() const
    {
    return MLNodeType::PROC_PROFILE;
    }

  int getNParams() const;

  void addParam( const MLString& name, int type );

  MLProcArgs* buildArgs( va_list ap );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLProcProfile_h
