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
  popupCommand.C

  void ModelModule::handlePopupCommand( int label );

  Stephane Rehel
  January 12 1996
*/

#include "ModelModule.h"

#include "kw/KWDialog.h"

#include "interface/MLInterfaceCommand.h"
#include "interface/MLPopup.h"

#include "scene/MLScene.h"
#include "scene/MLObject.h"
#include "scene/MLMesh.h"
#include "scene/MLObjectsIterator.h"
#include "scene/MLAbstractMesh.h"
#include "scene/lighting/MLLight.h"
#include "scene/lighting/MLLightPool.h"
#include "scene/lighting/MLPoolTable.h"

#include "ModelScene.h"
#include "ModelCommand.h"
#include "ModelCommands.h"
#include "ModelFlags.h"

#include "ModelPopups.h"

#include "gdialogs/file/MLLoadGeometry.h"
#include "gdialogs/file/MLSaveGeometry.h"
#include "gdialogs/file/MLLoadHierarchy.h"
#include "gdialogs/file/MLSaveHierarchy.h"
#include "gdialogs/file/MLLoadScene.h"
#include "gdialogs/file/MLLoadProject.h"
#include "gdialogs/file/MLSaveProject.h"
#include "gdialogs/file/MLDLoadPackaged.h"
#include "gdialogs/file/MLDSavePackaged.h"
#include "gdialogs/file/MLDExportAs.h"
#include "gdialogs/MLDSceneInfo.h"
#include "gdialogs/MLDMaterial.h"
#include "gdialogs/MLDRendererOptions.h"
#include "gdialogs/MLDMessagesLogs.h"

#include "scene/file/MLDatabasesList.h"

#include "render/ModelRender.h"

#include "commands/MLCSetVariable.h"
#include "commands/MLCPolygonize.h"

/////////////////////////////////////////////////////////////////////////////

void ModelModule::handlePopupCommand( int label )
{
  if( label == popups->_dialog_light )     label= popups->dialog_light;
  if( label == popups->_dialog_pool )      label= popups->dialog_pool;
  if( label == popups->_dialog_radiosity ) label= popups->dialog_radiosity;
  if( label == popups->_dialog_primitives_list ) label= popups->dialog_primitives_list;

  if( label == popups->clear_all )
    {
    stopLightingProcess();
    if( KWWarningOkCancel( "The whole project is going to be cleared.\n"
                           "Continue?" ) )
      {
      resetAll();
      }
    return;
    }

  if( label == popups->clear_scene )
    {
    stopLightingProcess();
    if( KWWarningOkCancel( "The scene will be cleared.\n"
                           "Continue?" ) )
      {
      MLModule::clearUndosRedos();
      modelScene->resetAll();
      refreshScreen(REFRESH_ALL);
      }
    }

  if( label == popups->load_geometry )
    {
    MLLoadGeometry load(this);
    MLAbstractMesh* ao= load.run();
    if( ao == 0 )
      return;

    ModelCommand* cmd= new ModelCommand;
    cmd->type= ModelCommands::CREATE_GEOMETRY;
    cmd->index= ao->getID();
    post(cmd);
    return;
    }

  if( label == popups->save_geometry )
    {
    int object_id= getSingleSelected();
    if( object_id == 0 )
      {
      printMessage( "Please select one object", 2 );
      return;
      }

    MLMesh* mesh= (MLMesh*) scene->getObject(object_id,MLObject::MESH);
    if( mesh == 0 )
      {
      printMessage( "Please select a mesh object", 2 );
      return;
      }

    MLAbstractMesh* ao= mesh->getAbstract();
    if( ao == 0 )
      return;

    MLSaveGeometry save(this);
    save.run(ao);
    return;
    }

  if( label == popups->load_hierarchy )
    {
    MLLoadHierarchy load(this);

    // CAREFUL: NO UNDO!
    load.run();

    refreshScreen(ModelModule::REFRESH_ALL);
    return;
    }

  if( label == popups->save_hierarchy )
    {
    int object_id= getSingleSelected();
    if( object_id == 0 )
      {
      printMessage( "Please select one father object", 2 );
      return;
      }

    MLObject* object= scene->getObject(object_id);
    if( object == 0 )
      return;
    if( object->getFather() != scene->getRootObject() )
      {
      printMessage( "Please select a hierarchy base object", 2 );
      return;
      }

    MLSaveHierarchy save(this);
    save.run(object);
    return;
    }

  if( label == popups->load_scene )
    {
    MLLoadScene load(this);

    // CAREFUL: NO UNDO!
    load.run();

    refreshScreen(ModelModule::REFRESH_ALL);
    return;
    }

  if( label == popups->load_project )
    {
    MLLoadProject load(this);

    // CAREFUL: NO UNDO!
    load.run();

    refreshScreen(ModelModule::REFRESH_ALL);
    return;
    }

  if( label == popups->save_project )
    {
    MLDatabasesList dblist;
    dblist.build();
    MLDatabase* database= dblist.getDefaultDatabase();
    if( scene->getName().name == "noname" || database == 0 )
      label= popups->save_project_as;
     else
      {
      MLSaveProject::save( database, this );
      return;
      }
    }

  if( label == popups->save_project_as )
    {
    MLSaveProject save(this);
    // CAREFUL: NO UNDO!
    save.run();
    return;
    }

  if( label == popups->load_packaged )
    {
    stopLightingProcess();

    MLDLoadPackaged load(this);

    // CAREFUL: NO UNDO!
    load.run();

    refreshScreen(ModelModule::REFRESH_ALL);
    return;
    }

  if( label == popups->save_packaged )
    {
    stopLightingProcess();

    MLDSavePackaged save(this);

    save.run();
    return;
    }

  if( label == popups->export_as )
    {
    MLDExportAs e(this);
    e.run();
    return;
    }


  if( label == popups->quit )
    {
    MLInterfaceCommand* icmd= new MLInterfaceCommand;
    icmd->type= MLInterfaceCommand::QUIT;
    icmd->post();
    return;
    }

  if( label == popups->snap_shot )
    {
    ModelCommand* cmd= new ModelCommand;
    cmd->type= ModelCommands::SNAPSHOT;
    post(cmd);
    return;
    }

  if( label == popups->scene_info )
    {
    MLDSceneInfo si(this);
    si.run();
    return;
    }

  if( label == popups->print_render_time )
    {
    printRenderTime();
    return;
    }

  if( label == popups->refresh_screen )
    {
    refreshInterface();
    return;
    }

  if( label == popups->create_null_object )
    {
    ModelCommand* cmd= new ModelCommand;
    cmd->type= ModelCommands::CREATE_NULL_OBJECT;
    post(cmd);
    return;
    }

  if( label == popups->create_box                               ||
      label == popups->create_param_face                        ||
      label == popups->create_param_cylinder                    ||
      label == popups->create_param_sphere                      ||
      label == popups->create_param_cone                        ||
      label == popups->create_param_torus                       ||
      label == popups->create_foo_object                        ||
      label == popups->create_plain_text                        ||
      label == popups->create_tetrahedron                       ||
      label == popups->create_cube                              ||
      label == popups->create_octahedron                        ||
      label == popups->create_icosahedron                       ||
      label == popups->create_dodecahedron                      ||
      label == popups->create_cuboctahedron                     ||
      label == popups->create_icosidodecahedron                 ||
      label == popups->create_rhombicuboctahedron               ||
      label == popups->create_rhombicosidodecahedron            ||
      label == popups->create_rhombitruncated_cubeoctahedron    ||
      label == popups->create_rhombitruncated_icosidodecahedron ||
      label == popups->create_snub_cube                         ||
      label == popups->create_snub_dodecahedron                 ||
      label == popups->create_truncated_tetrahedron             ||
      label == popups->create_truncated_cube                    ||
      label == popups->create_truncated_octahedron              ||
      label == popups->create_truncated_icosahedron             ||
      label == popups->create_truncated_dodecahedron )
    {
    ModelCommand* cmd= new ModelCommand;
    cmd->type= ModelCommands::CREATE_PRIM;
    cmd->index= label;
    post(cmd);
    return;
    }

  if( label == popups->create_nurbs_bilinear ||
      label == popups->create_nurbs_cylinder ||
      label == popups->create_nurbs_sphere   ||
      label == popups->create_nurbs_cone     ||
      label == popups->create_nurbs_torus    )
    {
    ModelCommand* cmd= new ModelCommand;
    cmd->type= ModelCommands::CREATE_NURBS_OBJECT;
    cmd->index= label;
    post(cmd);
    return;
    }

  if( label == popups->create_point_light ||
      label == popups->create_spot_light ||
      label == popups->create_dir_light )
    {
    ModelCommand* cmd= new ModelCommand;

    if( label == popups->create_spot_light )
      cmd->index= MLLight::SPOT;
    else
    if( label == popups->create_dir_light )
      cmd->index= MLLight::DIRECTIONAL;
    else
    cmd->index= MLLight::POINT;

    cmd->type= ModelCommands::CREATE_LIGHT;
    post(cmd);
    return;
    }

  if( label == popups->default_lights_direct )
    {
    post( new MLCSetVariable( ModelFlags::DEFAULT_LIGHTS_DIRECT,
                              !flags->defaultLightsDirect ) );
    return;
    }

  if( label == popups->default_lights_global )
    {
    post( new MLCSetVariable( ModelFlags::DEFAULT_LIGHTS_GLOBAL,
                              !flags->defaultLightsGlobal ) );
    return;
    }

  if( label == popups->edit_material )
    {
    MLDMaterial mat_edit(this);
    mat_edit.run();
    return;
    }

  if( label == popups->create_square        ||
      label == popups->create_param_segment ||
      label == popups->create_param_circle  )
    {
    ModelCommand* cmd= new ModelCommand;
    cmd->type= ModelCommands::CREATE_CURVE;
    cmd->index= label;
    post(cmd);
    return;
    }

  if( label == popups->create_nurbs_segment ||
      label == popups->create_nurbs_circle  )
    {
    ModelCommand* cmd= new ModelCommand;
    cmd->type= ModelCommands::CREATE_NURBS_CURVE;
    cmd->index= label;
    post(cmd);
    return;
    }

  if( label == popups->undo )
    {
    postUndo();
    return;
    }

  if( label == popups->redo )
    {
    postRedo();
    return;
    }

  if( label == popups->delete_selection )
    {
    ModelCommand* cmd= new ModelCommand;
    cmd->type= ModelCommands::DELETE;
    modelScene->getSelectedObjects(cmd);
    post(cmd);
    return;
    }

  if( label == popups->duplicate_selection )
    {
    ModelCommand* cmd= new ModelCommand;
    cmd->type= ModelCommands::DUPLICATE;
    modelScene->getSelectedObjects(cmd);
    post(cmd);
    return;
    }

  if( label == popups->instantiate_selection )
    {
    ModelCommand* cmd= new ModelCommand;
    cmd->type= ModelCommands::INSTANTIATE;
    modelScene->getSelectedObjects(cmd);
    post(cmd);
    return;
    }

  if( label == popups->convert_to_polygons )
    {
    MLCPolygonize* cmd= new MLCPolygonize;
    cmd->type= ModelCommands::POLYGONIZE;
    modelScene->getSelectedObjects(cmd);
    post(cmd);
    return;
    }

  if( label == popups->single_selection )
    {
    post( new MLCSetVariable( ModelFlags::SINGLE_SELECTION,
                              !flags->singleSelection ) );
    return;
    }

  if( label == popups->select_all )
    {
    ModelCommand* cmd= new ModelCommand;
    cmd->type= ModelCommands::SET_SELECTED;
    modelScene->getAllObjects(cmd);
    post(cmd);
    return;
    }

  if( label == popups->select_all_lights )
    {
    ModelCommand* cmd= new ModelCommand;
    cmd->type= ModelCommands::SET_SELECTED;
    if( modelScene->getAllLights(cmd) > 0 )
      post(cmd);
     else
      {
      delete cmd;
      cmd= 0;
      }
    return;
    }

  if( label == popups->unselect_all )
    {
    ModelCommand* cmd= new ModelCommand;
    cmd->type= ModelCommands::SET_SELECTED;
    cmd->indices.alloc(0);
    post(cmd);
    return;
    }

  if( label == popups->dialog_transform )
    {
    createDialog("Transformation");
    return;
    }

  if( label == popups->dialog_camera )
    {
    createDialog("Camera");
    return;
    }

  if( label == popups->dialog_object_info )
    {
    createDialog("Object Info");
    return;
    }

  if( label == popups->dialog_mesh_tess )
    {
    createDialog("MeshTesselation");
    return;
    }

  if( label == popups->dialog_curve_tess )
    {
    createDialog("CurveTesselation");
    return;
    }

  if( label == popups->dialog_plain_text )
    {
    createDialog("PlainText");
    return;
    }

  if( label == popups->dialog_primitives_list )
    {
    createDialog("PrimitivesList");
    return;
    }

  if( label == popups->dialog_light )
    {
    createDialog("Light");
    return;
    }

  if( label == popups->dialog_pool )
    {
    createDialog("Light Pool Manager");
    return;
    }

  if( label == popups->dialog_radiosity )
    {
    createDialog("Radiosity");
    return;
    }

  if( label == popups->dialog_modes_list )
    {
    createDialog("ModesList");
    return;
    }

  if( label == popups->dialog_welcome )
    {
    createDialog("Welcome");
    return;
    }

  if( label == popups->show_view_axes )
    {
    post( new MLCSetVariable( ModelFlags::SHOW_VIEW_AXES,
                              !flags->showViewAxes ) );
    return;
    }

  if( label == popups->show_object_axes )
    {
    post( new MLCSetVariable( ModelFlags::SHOW_OBJECT_AXES,
                              !flags->showObjectAxes ) );
    return;
    }

  if( label == popups->show_camera )
    {
    post( new MLCSetVariable( ModelFlags::SHOW_CAMERA,
                              !flags->showCamera ) );
    return;
    }

  if( label == popups->show_hier_struct )
    {
    post( new MLCSetVariable( ModelFlags::SHOW_HIER_STRUCT,
                              !flags->showHierStruct ) );
    return;
    }

  if( label == popups->show_tesselation )
    {
    post( new MLCSetVariable( ModelFlags::SHOW_TESSELATION,
                              !flags->showTesselation ) );
    return;
    }

  if( label == popups->show_normals )
    {
    post( new MLCSetVariable( ModelFlags::SHOW_NORMALS,
                              !flags->showNormals ) );
    return;
    }

  if( label == popups->show_hashbox )
    {
    post( new MLCSetVariable( ModelFlags::SHOW_HASHBOX,
                              !flags->showHashBox ) );
    return;
    }

  if( label == popups->hide_object || label == popups->hide_all_objects )
    {
    ModelCommand* cmd= new ModelCommand;
    cmd->type= ModelCommands::HIDE;
    if( label == popups->hide_all_objects )
      modelScene->getAllObjects(cmd);
     else
      modelScene->getSelectedObjects(cmd);
    post(cmd);
    return;
    }

  if( label == popups->unhide_object || label == popups->unhide_all_objects )
    {
    ModelCommand* cmd= new ModelCommand;
    cmd->type= ModelCommands::UNHIDE;
    if( label == popups->unhide_all_objects )
      modelScene->getAllObjects(cmd);
     else
      modelScene->getSelectedObjects(cmd);
    post(cmd);
    return;
    }

  // RADIOSITY

  if( label == popups->initiate )
    {
    // NO UNDO
    stopLightingProcess();
    MLPoolTable* table= scene->getPoolTable();
    for( int i= 1; i <= table->getNPools(); ++i )
      {
      MLLightPool* pool= table->getPool(i);
      if( pool == 0 )
        continue;
      pool->setDirty();
      pool->setDirtyFinal();
      }
    refreshScreen(REFRESH_ILLUM);
    return;
    }

  if( label == popups->rad_reset_objects )
    {
    stopLightingProcess();
    modelScene->resetObjectsRadiosity();
    return;
    }

  if( label == popups->shoot_direct )
    {
    post( new MLCSetVariable( ModelFlags::COMPUTE_DIRECT_LIGHTING,
                              !flags->computeDirectLighting ) );
    return;
    }

  if( label == popups->diffuse_indirect )
    {
    post( new MLCSetVariable( ModelFlags::COMPUTE_INDIRECT_LIGHTING,
                              !flags->computeIndirectLighting ) );
    return;
    }

  if( label == popups->update_shadows )
    {
    post( new MLCSetVariable( ModelFlags::UPDATE_SHADOWS,
                              !flags->updateShadows ) );
    return;
    }

  if( label == popups->do_render )
    {
    stopLightingProcess();

    pushWaitCursor();

    // Check for boundings/hashboxes to update
    modelScene->updateBoundingHashBox();

    popWaitCursor();

    if( modelRender == 0 )
      modelRender= new ModelRender(this);

    updatePopupsFlags();
    modelRender->init();
    if( ! modelRender->run() )
      {
      delete modelRender;
      modelRender= 0;
      }

    return;
    }

  if( label == popups->render_options )
    {
    MLDRendererOptions ro(this);
    ro.run();
    return;
    }

  if( label == popups->show_prev_render )
    {
    if( modelRender == 0 )
      return;
    stopLightingProcess();
    modelRender->wakeup();
    return;
    }
}

/////////////////////////////////////////////////////////////////////////////
