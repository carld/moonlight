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
  ModelColors.C

  Stephane Rehel

  August 5 1997
*/

#include "tools/MLGammaCorrection.h"

#include "ModelColors.h"

#include "ModelModule.h"

/////////////////////////////////////////////////////////////////////////////

// static
Color ModelColors::wire_background;
Color ModelColors::grid;
Color ModelColors::highlighted;
Color ModelColors::selected;
Color ModelColors::unselected;
Color ModelColors::disc_edge;
Color ModelColors::border_edge;
Color ModelColors::null_object;
Color ModelColors::light;
Color ModelColors::light_direct;
Color ModelColors::light_global;
Color ModelColors::light_direct_global;
Color ModelColors::camera;
Color ModelColors::tesselation;
Color ModelColors::normals;
Color ModelColors::hierarchy_link;
Color ModelColors::X_Axis;
Color ModelColors::Y_Axis;
Color ModelColors::Z_Axis;
Color ModelColors::tag_vertex;
Color ModelColors::tag_polygon;
Color ModelColors::nurbs_control_net; // selected
Color ModelColors::selected_nurbs_control_point;
Color ModelColors::unselected_nurbs_control_point;

/////////////////////////////////////////////////////////////////////////////

// static
void ModelColors::init( ModelModule* model )
{
#define GColor(r,g,b) Color(gc->do_f(r),gc->do_f(g),gc->do_f(b))
  MLGammaCorrection* gc= model->getGammaCorrection();

  double w_bk= gc->do_f(128.5/255.);

  wire_background= Color(w_bk,w_bk,w_bk);
  grid= GColor(0,0,0);

  highlighted=         GColor(1,0,0);
  selected=            GColor(1,1,1);
  unselected=          GColor(0,0,0);
  disc_edge=           GColor(0,1,1);
  border_edge=         GColor(0,1,0);
  null_object=         selected;
  light=               selected;
  light_direct=        GColor(1,1,0);
  light_global=        GColor(1,.3,0);
  light_direct_global= GColor(.5,1,0);
  camera=              GColor(0,0,.5);
  tesselation=         GColor(0,.5,0);
  normals=             GColor(0.8,0,0);
  hierarchy_link=      GColor(.8,0,.5);
  X_Axis=              GColor(1,0,0);
  Y_Axis=              GColor(0,1,0);
  Z_Axis=              GColor(0,0,1);
  tag_vertex=          GColor(1,0,0);
  tag_polygon=         GColor(.3,0,0);

  nurbs_control_net=              GColor(1,1,1); // selected
  selected_nurbs_control_point=   GColor(1,0,0);
  unselected_nurbs_control_point= GColor(1,1,0.5);

#undef GColor
}

/////////////////////////////////////////////////////////////////////////////
