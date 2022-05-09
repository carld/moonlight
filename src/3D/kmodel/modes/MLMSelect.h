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
  MLMSelect.h

  Stephane Rehel

  August 2 1997
*/

#ifndef __MLMSelect_h
#define __MLMSelect_h

#ifndef __IntegerArray_h
#include "tools/IntegerArray.h"
#endif

#ifndef __MLMode_h
#include "interface/MLMode.h"
#endif

class ModelModule;

/////////////////////////////////////////////////////////////////////////////

class MLMSelect: public MLMode
{
public:
  int button;
  IntegerArray objects;

public:
  MLMSelect( ModelModule* _model );

  virtual ~MLMSelect();

  virtual void enter();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLMSelect_h
