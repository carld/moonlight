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
  modelPopups.h

  Stephane Rehel
  January 12 1997
*/

#ifndef __ModelPopups_h
#define __ModelPopups_h

class MLPopup;
class MLButtonsPanel;
class ModelModule;

/////////////////////////////////////////////////////////////////////////////

class ModelPopups
{
protected:
  ModelModule* model;

public:
  // File
  int clear_all;
  int clear_scene;
  int load_project;
  int save_project;
  int save_project_as;
  int get;
  int put;
  int load_packaged;
  int save_packaged;
  int load_scene;
  int load_hierarchy;
  int save_hierarchy;
  int load_geometry;
  int save_geometry;
  int export_as;
  int snap_shot;
  int scene_info;
  int print_render_time;
  int refresh_screen;

  // Edit
  int undo;
  int redo;
  int delete_selection;
  int duplicate_selection;
  int instantiate_selection;

  // Primitive
  int create_box;
  int create_null_object;
  int create_param;
    int create_param_face;
    int create_param_cylinder;
    int create_param_sphere;
    int create_param_cone;
    int create_param_torus;
  int create_nurbs;
    int create_nurbs_bilinear;
    int create_nurbs_cylinder;
    int create_nurbs_sphere;
    int create_nurbs_cone;
    int create_nurbs_torus;
  int create_foo_object;
  int create_polyhedra;
    int create_tetrahedron;
    int create_cube;
    int create_octahedron;
    int create_icosahedron;
    int create_dodecahedron;
    int create_cuboctahedron;
    int create_icosidodecahedron;
    int create_rhombicuboctahedron;
    int create_rhombicosidodecahedron;
    int create_rhombitruncated_cubeoctahedron;
    int create_rhombitruncated_icosidodecahedron;
    int create_snub_cube;
    int create_snub_dodecahedron;
    int create_truncated_tetrahedron;
    int create_truncated_cube;
    int create_truncated_octahedron;
    int create_truncated_icosahedron;
    int create_truncated_dodecahedron;
  int create_plain_text;
  int _dialog_primitives_list;
  int convert_to_polygons;

  // Light
  int create_point_light;
  int create_dir_light;
  int create_spot_light;
  int _dialog_light;
  int _dialog_pool;
  int default_lights_direct;
  int default_lights_global;

  // Material
  int edit_material;

  // Curve
  int create_square;
  int create_param_curve;
    int create_param_segment;
    int create_param_circle;
  int create_nurbs_curve;
    int create_nurbs_segment;
    int create_nurbs_circle;

  // Selection
  int single_selection;
  int select_all;
  int unselect_all;
  int select_all_lights;

  // Show
  int show_view_axes;
  int show_object_axes;
  int show_camera;
  int show_hier_struct;
  int show_tesselation;
  int show_normals;
  int show_hashbox;
  int hide_object;
  int hide_all_objects;
  int unhide_object;
  int unhide_all_objects;

  // Radiosity
  int initiate;
  int rad_reset_objects;
  int shoot_direct;
  int diffuse_indirect;
  int update_shadows;
  int _dialog_radiosity;

  // Render
  int do_render;
  int render_options;
  int show_prev_render;

  // Dialogs
  int dialog_transform;
  int dialog_camera;
  int dialog_object_info;
  int dialog_mesh_tess;
  int dialog_curve_tess;
  int dialog_plain_text;
  int dialog_primitives_list;
  int dialog_light;
  int dialog_pool;
  int dialog_radiosity;
  int dialog_modes_list;
  int dialog_welcome;

  // Quit
  int quit;

  struct
    {
    MLPopup* file;
    MLPopup* edit;
    MLPopup* primitives;
    MLPopup* lights;
    MLPopup* materials;
    MLPopup* selection;
    MLPopup* curves;
    MLPopup* show;
    MLPopup* show_object;
    MLPopup* radiosity;
    MLPopup* render;
    MLPopup* dialogs;
    MLPopup* quit;
    } popups;

public:
  ModelPopups( ModelModule* _model );

  virtual ~ModelPopups();

  void init( MLButtonsPanel* buttons );

  void updateFlags();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelPopups_h
