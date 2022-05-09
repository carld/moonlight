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
  MLCTagPolygon.h

  Stephane Rehel

  November 23 1997
*/

#ifndef __MLCTagPolygon_h
#define __MLCTagPolygon_h

#ifndef __ModelCommand_h
#include "kmodel/ModelCommand.h"
#endif

#ifndef __IToggle_h
#include "tools/IToggle.h"
#endif

class ModelModule;

/////////////////////////////////////////////////////////////////////////////

// ModelCommand::type can be TAG_VERTEX_SET or TAG_VERTEX_TOGGLE
class MLCTagPolygon: public ModelCommand
{
public:
  IToggle oper;
  int mesh_id;
  int polygon_id;

public:
  MLCTagPolygon();

  virtual ~MLCTagPolygon();

  virtual boolean doit( ModelModule* model );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLCTagPolygon_h
