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
  MLFTagPolygon.h

  Stephane Rehel

  November 23 1997
*/

#ifndef __MLFTagPolygon_h
#define __MLFTagPolygon_h

#ifndef __MLBaseFunction_h
#include "interface/MLBaseFunction.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLFTagPolygon: public MLBaseFunction
{
public:
  MLFTagPolygon( MLModule* _module );

  virtual ~MLFTagPolygon();

  virtual boolean _call();

protected:
  virtual MLFunction* newFunction() const
    {
    return set_function_id(new MLFTagPolygon(module));
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLFTagPolygon_h
