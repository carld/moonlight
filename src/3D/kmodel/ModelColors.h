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
  ModelColors.h

  Stephane Rehel

  August 5 1997
*/

#ifndef __ModelColors_h
#define __ModelColors_h

#ifndef __Color_h
#include "tools/Color.h"
#endif

class ModelModule;

/////////////////////////////////////////////////////////////////////////////

class ModelColors
{
public:
  static void init( ModelModule* model );

  static Color wire_background;
  static Color grid;

  static Color highlighted;
  static Color selected;
  static Color unselected;
  static Color disc_edge;
  static Color border_edge;
  static Color null_object;
  static Color light;
  static Color light_direct;
  static Color light_global;
  static Color light_direct_global;
  static Color camera;
  static Color tesselation;
  static Color normals;
  static Color hierarchy_link;
  static Color X_Axis;
  static Color Y_Axis;
  static Color Z_Axis;
  static Color tag_vertex;
  static Color tag_polygon;
  static Color nurbs_control_net; // selected
  static Color selected_nurbs_control_point;
  static Color unselected_nurbs_control_point;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelColors_h
