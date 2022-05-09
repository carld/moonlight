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
  ModelPopups.C

  Stephane Rehel
  January 12 1997
*/

#include "interface/MLPopup.h"
#include "interface/MLButtonsPanel.h"

#include "ModelModule.h"
#include "ModelScene.h"
#include "ModelFlags.h"

#include "ModelPopups.h"

#ifndef NDEBUG
  #define PRIVATE_MENU
#endif

//  #define PRIVATE_MENU

/////////////////////////////////////////////////////////////////////////////

ModelPopups::ModelPopups( ModelModule* _model )
{
  model= _model;
}

/////////////////////////////////////////////////////////////////////////////

ModelPopups::~ModelPopups()
{}

/////////////////////////////////////////////////////////////////////////////

// called by ModelModule::init()
void ModelPopups::init( MLButtonsPanel* buttons )
{
  MLPopup* p;
  MLPopup* p1;

  // Scene
  p= new MLPopup(model);
  popups.file= p;

  clear_all= p->add("Clear all");
  clear_scene= p->add("Clear scene");
  load_project= p->add("Load project...");
  save_project= p->add("Save project");
  save_project_as= p->add("Save project as...");
  p1= new MLPopup(model);
  get= p->add("Get",p1);
    load_packaged= p1->add("Packaged...");
    load_scene= p1->add("Scene...");
    load_hierarchy= p1->add("Hierarchy...");
    load_geometry= p1->add("Geometry...");
  p1= new MLPopup(model);
  put= p->add("Put",p1);
    save_packaged= p1->add("Packaged...");
    save_hierarchy= p1->add("Hierarchy...");
    save_geometry= p1->add("Geometry...");
  export_as= p->add("Export as...");
#ifdef PRIVATE_MENU
  snap_shot= p->add("Snapshot");
#else
  snap_shot= 0;
#endif
  scene_info= p->add("Scene Info");
  print_render_time= p->add("Render Time");
  refresh_screen= p->add("Refresh screen\tr");

  // Edit
  p= new MLPopup(model);
  popups.edit= p;
  undo= p->add("Undo\tu");
  redo= p->add("Redo\tu");
  duplicate_selection= p->add("Duplicate\td");
  delete_selection= p->add("Delete\tBackspace");
  instantiate_selection= p->add("Instantiate\talt-d");

  // Primitives
  p= new MLPopup(model);
  popups.primitives= p;
  create_box= p->add("Box");
  create_null_object= p->add("Null object");

  p1= new MLPopup(model);
  create_param= p->add("Parametric",p1);
    {
    create_param_face= p1->add("Face");
    create_param_cylinder= p1->add("Cylinder");
    create_param_sphere= p1->add("Sphere");
    create_param_cone= p1->add("Cone");
    create_param_torus= p1->add("Torus");
    }

  p1= new MLPopup(model);
  create_param= p->add("NURBS",p1);
    {
    create_nurbs_bilinear= p1->add("Bilinear");
    create_nurbs_cylinder= p1->add("Cylinder");
    create_nurbs_sphere= p1->add("Sphere");
    create_nurbs_cone= p1->add("Cone");
    create_nurbs_torus= p1->add("Torus");
    }

  p1= new MLPopup(model);
  create_polyhedra= p->add("Polyhedra",p1);
    {
    create_tetrahedron= p1->add("Tetrahedron");
    create_cube= p1->add("Cube");
    create_octahedron= p1->add("Octahedron");
    create_icosahedron= p1->add("Icosahedron");
    create_dodecahedron= p1->add("Dodecahedron");
    create_cuboctahedron= p1->add("Cuboctahedron");
    create_icosidodecahedron= p1->add("Icosidodecahedron");
    create_rhombicuboctahedron= p1->add("Rhombicuboctahedron");
    create_rhombicosidodecahedron= p1->add("Rhombicosidodecahedron (buggy)");
    create_rhombitruncated_cubeoctahedron= p1->add("Rhombitruncated_cubeoctahedron (buggy)");
    create_rhombitruncated_icosidodecahedron= p1->add("Rhombitruncated_icosidodecahedron (buggy)");
    create_snub_cube= p1->add("Snub_cube");
    create_snub_dodecahedron= p1->add("Snub_dodecahedron");
    create_truncated_tetrahedron= p1->add("Snub_dodecahedron");
    create_truncated_cube= p1->add("Truncated_cube (buggy)");
    create_truncated_octahedron= p1->add("Truncated_octahedron (buggy)");
    create_truncated_icosahedron= p1->add("Truncated_icosahedron (buggy)");
    create_truncated_dodecahedron= p1->add("Truncated_dodecahedron (buggy)");
    }

#ifdef PRIVATE_MENU
  create_foo_object= p->add("Foo Object");
#else
  create_foo_object= 0;
#endif

  create_plain_text= p->add("Plain Text");

  _dialog_primitives_list= p->add("Primitives List");
  convert_to_polygons= p->add("Convert to polygons");

  // Lights
  p= new MLPopup(model);
  popups.lights= p;
  create_point_light= p->add("Point Light");
  create_dir_light= p->add("Directional Light");
  create_spot_light= p->add("Spot Light");
  _dialog_light= p->add("Light Dialog");
  _dialog_pool= p->add("Pool Manager Dialog");
  p1= new MLPopup(model);
  p->add("Default lights are:",p1);
  default_lights_direct= p1->addCheck("Direct");
  default_lights_global= p1->addCheck("Global");

  // Materials
  p= new MLPopup(model);
  popups.materials= p;
  edit_material= p->add("Edit object material");

  // Curves
  p= new MLPopup(model);
  popups.curves= p;
  create_square= p->add("Square");
  p1= new MLPopup(model);
  create_param_curve= p->add("Parametric",p1);
    {
    create_param_segment= p1->add("Segment");
    create_param_circle= p1->add("Circle");
    }
  p1= new MLPopup(model);
  create_nurbs_curve= p->add("NURBS",p1);
    {
    create_nurbs_segment= p1->add("Segment");
    create_nurbs_circle= p1->add("Circle");
    }

  // Selection
  p= new MLPopup(model);
  popups.selection= p;
  single_selection= p->addCheck("Single");
  select_all= p->add("Select all");
  unselect_all= p->add("Unselect all");
  select_all_lights= p->add("Select all lights");

  // Show
  p= new MLPopup(model);
  popups.show= p;
  show_view_axes= p->addCheck("View axes");
  show_object_axes= p->addCheck("Object axes");
  show_camera= p->addCheck("Camera");
  show_hier_struct= p->addCheck("Hierarchy");
  show_tesselation= p->addCheck("Tesselation");
  show_normals= p->addCheck("Normals");
#ifdef PRIVATE_MENU
  show_hashbox= p->addCheck("HashBox");
#else
  show_hashbox= 0;
#endif
  p1= new MLPopup(model);
  popups.show_object= p1;
  popups.show->add("Object",p1);
  hide_object= p1->add("Hide\th");

  unhide_object= p1->add("Unhide\th");
  hide_all_objects= p1->add("Hide all");
  unhide_all_objects= p1->add("Unhide all");

  // Radiosity
  p= new MLPopup(model);
  popups.radiosity= p;
  initiate= p->add("Reset all");
  rad_reset_objects= p->add("Reset selected objects");
  shoot_direct= p->addCheck("Shoot direct lighting");
  diffuse_indirect= p->addCheck("Diffuse indirect lighting");
  update_shadows= p->addCheck("Interactive shadows updating");
  _dialog_radiosity= p->add("Process dialog");

  // Render
  p= new MLPopup(model);
  popups.render= p;
  do_render= p->add("Render scene");
  render_options= p->add("Set renderer options");
  show_prev_render= p->add("Show previous rendering");

  // Dialogs
  p= new MLPopup(model);
  popups.dialogs= p;
  dialog_transform= p->add("Transformation");
  dialog_camera= 0;
  dialog_object_info= p->add("Object Info");
  dialog_mesh_tess= p->add("Mesh Tesselation");
  dialog_curve_tess= p->add("Curve Tesselation");
  dialog_plain_text= p->add("Plain Text Params");
  dialog_primitives_list= p->add("Primitives list");
  dialog_light= p->add("Light");
  dialog_pool= p->add("Light Pool Manager");
  dialog_radiosity= p->add("Radiosity");
  dialog_modes_list= p->add("Modes list");
  dialog_welcome= p->add("Welcome");

  // Quit
  p= new MLPopup(model);
  popups.quit= p;
  quit= p->getPopupLabel();

  buttons->setButton(  1, "File",      popups.file );
  buttons->setButton(  2, "Edit",      popups.edit );
  buttons->setButton(  3, "Primitives", popups.primitives );
  buttons->setButton(  4, "Lights",    popups.lights );
  buttons->setButton(  5, "Materials", popups.materials );
  buttons->setButton(  6, "Curves",    popups.curves );
  buttons->setButton(  7, "Selection", popups.selection );
  buttons->setButton(  8, "Show",      popups.show );
  buttons->setButton(  9, "Radiosity", popups.radiosity );
  buttons->setButton( 10, "Render",    popups.render );
  buttons->setButton( 11, "Dialogs",   popups.dialogs );
  buttons->setButton( 20, "Quit",      popups.quit );
}

/////////////////////////////////////////////////////////////////////////////

void ModelPopups::updateFlags()
{
  const ModelFlags* flags= model->getFlags();

  popups.selection->check( single_selection, flags->singleSelection );

  popups.show->check( show_view_axes, flags->showViewAxes );
  popups.show->check( show_object_axes, flags->showObjectAxes );
  popups.show->check( show_camera, flags->showCamera );
  popups.show->check( show_hier_struct, flags->showHierStruct );
  popups.show->check( show_tesselation, flags->showTesselation );
  popups.show->check( show_normals, flags->showNormals );

#ifdef PRIVATE_MENU
  popups.show->check( show_hashbox, flags->showHashBox );
#endif

  popups.radiosity->check( shoot_direct, flags->computeDirectLighting );
  popups.radiosity->check( diffuse_indirect, flags->computeIndirectLighting );
  popups.radiosity->check( update_shadows, flags->updateShadows);

  popups.render->enable( show_prev_render, model->modelRender != 0 );

  popups.lights->check( default_lights_direct, flags->defaultLightsDirect );
  popups.lights->check( default_lights_global, flags->defaultLightsGlobal );
}

/////////////////////////////////////////////////////////////////////////////

