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
  handleModelCommand.C

  void ModelModule::handleModelCommand( ModelCommand* cmd );

  Stephane Rehel

  March 5 1997
*/

#include <stdio.h>

#include "tools/IToggle.h"

#include "interface/MLPopup.h"

#include "scene/MLCamera.h"
#include "scene/material/MLMaterial.h"
#include "scene/MLObject.h"
#include "scene/MLMesh.h"
#include "scene/MLCurve.h"
#include "scene/MLNullObject.h"
#include "scene/MLObjectsIterator.h"

#include "scene/lighting/MLLight.h"
#include "scene/lighting/MLLightPool.h"
#include "scene/lighting/MLPoolTable.h"
#include "scene/lighting/MLGlobalLighting.h"

#include "scene/MLScene.h"

#include "scene/formal/FormalRep.h"
#include "scene/formal/FormalPolyhedron.h"
#include "scene/formal/FormalNURBS.h"
#include "scene/formal/FormalCurveRep.h"
#include "scene/formal/FormalNURBSCurve.h"

#include "ModelPrim.h"
#include "ModelCommand.h"
#include "ModelCommands.h"
#include "ModelScene.h"
#include "ModelFlags.h"
#include "ModelPopups.h"
#include "ModelStatusBars.h"
#include "ModelModule.h"

/////////////////////////////////////////////////////////////////////////////

void ModelModule::handleModelCommand( ModelCommand* cmd )
{
  if( cmd == 0 )
    return;

  if( cmd->doit(this) )
    return; // implemented into the command

  switch( cmd->getType() )
    {
    case ModelCommands::CREATE_GEOMETRY:
      {
      MLAbstractMesh* ao= scene->getAbstractObject(cmd->index);
      if( ao == 0 )
        return;

      MLMesh* mesh= new MLMesh( scene->getRootObject(), ao );
      ref(mesh);
      scene->updateTransformations();

      ModelCommand* undo= cmd->createUndo();
      undo->type= ModelCommands::DELETE;

      modelScene->getSelectedObjects(undo);
      undo->indices2= undo->indices;
      undo->indices.clear();
      undo->index= 1;

      undo->indices.alloc(1);
      undo->indices[0]= mesh->getID();

      modelScene->selectAll(IUNSET);
      modelScene->selectObjects(undo,ISET);

      mesh->updateMaterials();

      scene->getGlobalLighting()->object_is_transformed(mesh);

      refreshScreen( REFRESH_SELECTION | REFRESH_GEOMETRY | REFRESH_HIER |
                     REFRESH_ILLUM | REFRESH_DIALOGS );
      return;
      }

    case ModelCommands::SET_SELECTED:
      {
      ModelCommand* undo= cmd->createUndo();
      modelScene->getSelectedObjects(undo);

      modelScene->selectAll(IUNSET);
      modelScene->selectObjects(cmd,ISET);

      refreshScreen(REFRESH_SELECTION);
      return;
      }

    case ModelCommands::SELECT_TOGGLE:
    case ModelCommands::SELECT:
    case ModelCommands::UNSELECT:
      {
      ModelCommand* undo= cmd->createUndo();
      undo->type= ModelCommands::SET_SELECTED;
      modelScene->getSelectedObjects(undo);

      IToggle toggle= ITOGGLE;
      if( cmd->getType() == ModelCommands::SELECT )
        toggle= ISET;
       else
      if( cmd->getType() == ModelCommands::UNSELECT )
        toggle= IUNSET;

      int i;
      for( i= 0; i < cmd->indices.getSize(); ++i )
        {
        MLObject* o= scene->getObject(cmd->indices[i]);
        if( o == 0 )
          continue;

        if( (o->selected()?ISET:IUNSET) == toggle )
          cmd->indices[i]= 0; // already selected or unselected
        }

      if( flags->singleSelection && toggle != IUNSET )
        {
        // i choose the first unselected object in single selection
        int id= 0;
        for( i= 0; i < cmd->indices.getSize(); ++i )
          {
          MLObject* o= scene->getObject(cmd->indices[i]);
          if( o == 0 )
            continue;

          if( ! o->selected() )
            {
            // i choose this one
            id= cmd->indices[i];
            break;
            }
          }
        if( id == 0 )
          {
          cmd->indices.clear();
          }
         else
          {
          cmd->indices.alloc(1);
          cmd->indices[0]= id;
          }

        modelScene->selectAll(IUNSET);
        }

      modelScene->selectObjects(cmd,toggle);

      refreshScreen(REFRESH_SELECTION);
      return;
      }

    case ModelCommands::REPARENT:
      {
      if( cmd->indices.getSize() != 2 )
        return;
      MLObject* child= scene->getObject(cmd->indices[0]);
      MLObject* new_parent;
      if( cmd->indices[1] == 0 )
        new_parent= scene->getRootObject();
       else
        new_parent= scene->getObject(cmd->indices[1]);
      if( child == 0 || new_parent == 0 || child == new_parent )
        return;

      cmd->createUndo();
      cmd->undo->indices[1]= child->getFather()->getID();

      modelScene->reparent( child, new_parent );

      refreshScreen(REFRESH_HIER);
      return;
      }

    case ModelCommands::CREATE_LIGHT:
      {
      stopLightingProcess();

      MLAbstractMesh* a= ModelPrim::getPointLightAbstract(scene);

      ModelCommand* undo= cmd->createUndo();
      undo->type= ModelCommands::DELETE;

      modelScene->getSelectedObjects(undo);

      undo->indices2= undo->indices;
      undo->indices.clear();
      undo->index= 1;

      if( a != 0 )
        a->generateMesh();

      // add this light in default pool
      MLLight* l= new MLLight( scene->getRootObject(),
                               0, // pool
                               getFlags()->defaultLightsDirect,
                               getFlags()->defaultLightsGlobal );

//      if( a != 0 )
//        l->setAbstract(a);
      ref(l);
      l->setLightType( MLLight::LightType(cmd->index) );

      scene->updateTransformations();

      modelScene->selectAll(IUNSET);

      undo->indices.alloc(1);
      undo->indices[0]= l->getID();

      modelScene->selectObjects(undo,ISET);

      scene->getGlobalLighting()->update_light(l);

      refreshScreen(REFRESH_SELECTION | REFRESH_GEOMETRY | REFRESH_HIER);
      return;
      }

    case ModelCommands::CREATE_NULL_OBJECT:
      {
      stopLightingProcess();

      ModelCommand* undo= cmd->createUndo();
      undo->type= ModelCommands::DELETE;

      modelScene->getSelectedObjects(undo);
      undo->indices2= undo->indices;
      undo->indices.clear();
      undo->index= 1;

      MLNullObject* no= new MLNullObject( scene->getRootObject() );
      ref(no);

      scene->updateTransformations();

      modelScene->selectAll(IUNSET);

      undo->indices.alloc(1);
      undo->indices[0]= no->getID();

      modelScene->selectObjects(undo,ISET);

      refreshScreen(REFRESH_SELECTION | REFRESH_GEOMETRY | REFRESH_HIER);

      return;
      }

    case ModelCommands::CREATE_PRIM:
      {
      stopLightingProcess();

      MLAbstractMesh* a= 0;

#define TEST(popup_prim,prim) \
if( cmd->index == popup_prim ) { a= ModelPrim::getFormal(scene,prim); } else
#define TEST_PH(popup_prim,ph) \
if( cmd->index == popup_prim ) { a= ModelPrim::getPolyhedron(scene,ph); } else

      TEST(popups->create_box,FormalRep::BOX)
      TEST(popups->create_param_face,FormalRep::PARAM_FACE)
      TEST(popups->create_param_cylinder,FormalRep::PARAM_CYLINDER)
      TEST(popups->create_param_sphere,FormalRep::PARAM_SPHERE)
      TEST(popups->create_param_cone,FormalRep::PARAM_CONE)
      TEST(popups->create_param_torus,FormalRep::PARAM_TORUS)
      TEST(popups->create_foo_object,FormalRep::FOO)
      TEST(popups->create_plain_text,FormalRep::TEXT)
      TEST_PH(popups->create_tetrahedron,FormalPolyhedron::tetrahedron)
      TEST_PH(popups->create_cube,FormalPolyhedron::cube)
      TEST_PH(popups->create_octahedron,FormalPolyhedron::octahedron)
      TEST_PH(popups->create_icosahedron,FormalPolyhedron::icosahedron)
      TEST_PH(popups->create_dodecahedron,FormalPolyhedron::dodecahedron)
      TEST_PH(popups->create_cuboctahedron,FormalPolyhedron::cuboctahedron)
      TEST_PH(popups->create_icosidodecahedron,FormalPolyhedron::icosidodecahedron)
      TEST_PH(popups->create_rhombicuboctahedron,FormalPolyhedron::rhombicuboctahedron)
      TEST_PH(popups->create_rhombicosidodecahedron,FormalPolyhedron::rhombicosidodecahedron)
      TEST_PH(popups->create_rhombitruncated_cubeoctahedron,FormalPolyhedron::rhombitruncated_cubeoctahedron)
      TEST_PH(popups->create_rhombitruncated_icosidodecahedron,FormalPolyhedron::rhombitruncated_icosidodecahedron)
      TEST_PH(popups->create_snub_cube,FormalPolyhedron::snub_cube)
      TEST_PH(popups->create_snub_dodecahedron,FormalPolyhedron::snub_dodecahedron)
      TEST_PH(popups->create_truncated_tetrahedron,FormalPolyhedron::truncated_tetrahedron)
      TEST_PH(popups->create_truncated_cube,FormalPolyhedron::truncated_cube)
      TEST_PH(popups->create_truncated_octahedron,FormalPolyhedron::truncated_octahedron)
      TEST_PH(popups->create_truncated_icosahedron,FormalPolyhedron::truncated_icosahedron)
      TEST_PH(popups->create_truncated_dodecahedron,FormalPolyhedron::truncated_dodecahedron)
        { a = 0; }

#undef TEST
#undef TEST_PH

      if( a == 0 )
        return;

      ModelCommand* undo= cmd->createUndo();
      undo->type= ModelCommands::DELETE;

      modelScene->getSelectedObjects(undo);
      undo->indices2= undo->indices;
      undo->indices.clear();
      undo->index= 1;

      a->generateMesh();

      MLMesh* mesh= new MLMesh( scene->getRootObject(), a );
      ref(mesh);

      scene->updateTransformations();

      modelScene->selectAll(IUNSET);

      undo->indices.alloc(1);
      undo->indices[0]= mesh->getID();

      modelScene->selectObjects(undo,ISET);

      scene->getGlobalLighting()->object_is_transformed(mesh);

      refreshScreen(REFRESH_SELECTION | REFRESH_GEOMETRY | REFRESH_HIER);
      return;
      }

    case ModelCommands::CREATE_NURBS_OBJECT:
      {
      stopLightingProcess();

      MLAbstractMesh* a= ModelPrim::getFormal(scene,FormalRep::NURBS);
      FormalNURBS* nurbs= (FormalNURBS*) a->getFormalRep();

      if( cmd->index == popups->create_nurbs_bilinear )
        {
        nurbs->createBilinear( Point(-0.5,-0.5,0)*5.,
                               Point(+0.5,-0.5,0)*5.,
                               Point(+0.5,+0.5,0)*5.,
                               Point(-0.5,+0.5,0)*5. );
        goto end_nurbs;
        }

      if( cmd->index == popups->create_nurbs_cylinder )
        {
        nurbs->createCylinder();
        goto end_nurbs;
        }

      if( cmd->index == popups->create_nurbs_sphere )
        {
        nurbs->createSphere();
        goto end_nurbs;
        }

      if( cmd->index == popups->create_nurbs_cone )
        {
        nurbs->createCone();
        goto end_nurbs;
        }

      if( cmd->index == popups->create_nurbs_torus )
        {
        nurbs->createTorus();
        goto end_nurbs;
        }

      delete a;
      return;

end_nurbs:

      ModelCommand* undo= cmd->createUndo();
      undo->type= ModelCommands::DELETE;

      modelScene->getSelectedObjects(undo);
      undo->indices2= undo->indices;
      undo->indices.clear();
      undo->index= 1;

      a->generateMesh();

      MLMesh* mesh= new MLMesh( scene->getRootObject(), a );
      ref(mesh);

      scene->updateTransformations();

      modelScene->selectAll(IUNSET);

      undo->indices.alloc(1);
      undo->indices[0]= mesh->getID();

      modelScene->selectObjects(undo,ISET);

      scene->getGlobalLighting()->object_is_transformed(mesh);

      refreshScreen(REFRESH_SELECTION | REFRESH_GEOMETRY | REFRESH_HIER);
      return;
      }

    case ModelCommands::CREATE_CURVE:
      {
      MLAbstractCurve* a= 0;

#define TEST(popup_prim,prim) \
if( cmd->index == popup_prim ) \
  { a= ModelPrim::getFormalCurve(scene,prim); } else

      TEST(popups->create_square,FormalCurveRep::SQUARE)
      TEST(popups->create_param_segment,FormalCurveRep::PARAM_SEGMENT)
      TEST(popups->create_param_circle,FormalCurveRep::PARAM_CIRCLE)
        { a = 0; }
#undef TEST

      if( a == 0 )
        return;

      ModelCommand* undo= cmd->createUndo();
      undo->type= ModelCommands::DELETE;

      modelScene->getSelectedObjects(undo);
      undo->indices2= undo->indices;
      undo->indices.clear();
      undo->index= 1;

      a->generateCurve();

      MLCurve* curve= new MLCurve( scene->getRootObject(), a );
      ref(curve);

      scene->updateTransformations();

      modelScene->selectAll(IUNSET);

      undo->indices.alloc(1);
      undo->indices[0]= curve->getID();

      modelScene->selectObjects(undo,ISET);

      refreshScreen(REFRESH_SELECTION | REFRESH_GEOMETRY | REFRESH_HIER);
      return;
      }

    case ModelCommands::CREATE_NURBS_CURVE:
      {
      MLAbstractCurve* a= ModelPrim::getFormalCurve( scene,
                                                     FormalCurveRep::NURBS);
      FormalNURBSCurve* nurbs= (FormalNURBSCurve*) a->getFormalRep();

      if( cmd->index == popups->create_nurbs_segment )
        {
        nurbs->createSegment( Point(-0.5,0.,0), Point(+0.5,0.,0) );
        goto end_nurbs_curve;
        }

      if( cmd->index == popups->create_nurbs_circle )
        {
        nurbs->createCircle();
        goto end_nurbs_curve;
        }

      delete a;
      return;

end_nurbs_curve:

      ModelCommand* undo= cmd->createUndo();
      undo->type= ModelCommands::DELETE;

      modelScene->getSelectedObjects(undo);
      undo->indices2= undo->indices;
      undo->indices.clear();
      undo->index= 1;

      a->generateCurve();

      MLCurve* curve= new MLCurve( scene->getRootObject(), a );
      ref(curve);

      scene->updateTransformations();

      modelScene->selectAll(IUNSET);

      undo->indices.alloc(1);
      undo->indices[0]= curve->getID();

      modelScene->selectObjects(undo,ISET);

      refreshScreen(REFRESH_SELECTION | REFRESH_GEOMETRY | REFRESH_HIER);
      return;
      }

    case ModelCommands::DUPLICATE:
      {
      if( cmd->indices.getSize() == 0 )
        return;

      stopLightingProcess();

      ModelCommand* undo= cmd->createUndo();
      undo->type= ModelCommands::DELETE;

      undo->swapIndices();
      modelScene->getSelectedObjects(undo);
      undo->swapIndices();
      undo->index= 1;

      modelScene->selectAll(IUNSET);
      modelScene->duplicateObjects(cmd);

      scene->updateTransformations();

      modelScene->getSelectedObjects(undo);

      refreshScreen(REFRESH_SELECTION | REFRESH_GEOMETRY | REFRESH_HIER);
      return;
      }

    case ModelCommands::INSTANTIATE:
      {
      if( cmd->indices.getSize() == 0 )
        return;

      stopLightingProcess();

      ModelCommand* undo= cmd->createUndo();
      undo->type= ModelCommands::DELETE;

      undo->swapIndices();
      modelScene->getSelectedObjects(undo);
      undo->swapIndices();
      undo->index= 1;

      modelScene->selectAll(IUNSET);
      modelScene->instantiateObjects(cmd);

      scene->updateTransformations();

      modelScene->getSelectedObjects(undo);

      refreshScreen(REFRESH_SELECTION | REFRESH_GEOMETRY | REFRESH_HIER);
      return;
      }

    case ModelCommands::DELETE:
      {
      if( cmd->indices.getSize() == 0 )
        return;

      stopLightingProcess();

      ModelCommand* undo= cmd->createUndo();
      undo->type= ModelCommands::WAKEUP_OBJECTS;
      undo->swapIndices();
      modelScene->getSelectedObjects(undo);
      undo->swapIndices();

      modelScene->deleteObjects(cmd);

      if( cmd->index != 0 )
        {
        modelScene->selectAll(IUNSET);

        cmd->swapIndices();
        modelScene->selectObjects(cmd,ISET);
        cmd->swapIndices();

        refreshScreen(REFRESH_SELECTION);
        }

      refreshScreen(REFRESH_GEOMETRY | REFRESH_HIER);
      return;
      }

    case ModelCommands::WAKEUP_OBJECTS:
      {
      if( cmd->indices.getSize() == 0 )
        return;

      stopLightingProcess();

      ModelCommand* undo= cmd->createUndo();
      undo->type= ModelCommands::DELETE;
      undo->swapIndices();
      modelScene->getSelectedObjects(undo);
      undo->swapIndices();
      undo->index= 1;

      modelScene->selectAll(IUNSET);
      modelScene->wakeupObjects(cmd);

      cmd->swapIndices();
      modelScene->selectObjects(cmd,ISET);
      cmd->swapIndices();

      refreshScreen(REFRESH_SELECTION | REFRESH_GEOMETRY | REFRESH_HIER);

      return;
      }

    case ModelCommands::GET_INFO:
      {
      if( cmd->indices.getSize() <= 0 )
        {
        printMessage("");
        return;
        }

      int index= cmd->indices[0];
      MLObject* object= scene->getObject(index);
      if( object == 0 )
        return;

      switch(object->getObjectType())
        {
        case MLObject::MESH:
          {
          MLMesh* mesh= (MLMesh*) object;

          int nVertices, nSVertices, nEdges,
              nPolygons, nTessTriangles, nTriangles;

          mesh->getInfo( nVertices, nSVertices, nEdges,
                         nPolygons, nTessTriangles, nTriangles );

          if( nVertices == 0 )
            {
            printMessage("Empty object");
            return;
            }

          int size= mesh->getSizeOf() / 1024;
          const MLString& materialName= mesh->getMaterial()->getName().name;

          MLString buf= MLString::printf(
             "%s: Vert(%d) SVert(%d) Edges(%d) Polys(%d) TessTri(%d) Mem(%d Kb) Material(%s)",
                   mesh->getName().name.get(),
                   nVertices, nEdges, nSVertices, nPolygons,
                   nTriangles,
                   size,
                   materialName.get() );

          printMessage(buf);

          return;
          }

        case MLObject::NULL_OBJECT:
          {
          MLString buf= MLString::printf( "%s: Null Object",
                                          object->getName().name.get() );
          printMessage(buf);
          return;
          }

        case MLObject::LIGHT:
          {
          MLLight* light= (MLLight*) object;

          MLString type("");
          switch(light->getLightType())
            {
            case MLLight::POINT:       type= "Point"; break;
            case MLLight::DIRECTIONAL: type= "Directional"; break;
            case MLLight::SPOT:        type= "Spot"; break;
            default:
              break;
            }

          MLString buf= MLString::printf( "%s: %s light",
                                          object->getName().name.get(),
                                          type.get() );

          printMessage(buf);

          return;
          }

        default:
          break;
        }

      break;
      }

    case ModelCommands::HIDE_UNHIDE:
    case ModelCommands::HIDE:
    case ModelCommands::UNHIDE:
      {
      if( cmd->indices.getSize() == 0 )
        return;

      IToggle toggle= ITOGGLE;

      ModelCommand* undo= cmd->createUndo();

      if( cmd->getType() == ModelCommands::HIDE )
        {
        toggle= ISET;
        undo->type= ModelCommands::UNHIDE;
        }
       else
      if( cmd->getType() == ModelCommands::UNHIDE )
        {
        toggle= IUNSET;
        undo->type= ModelCommands::HIDE;
        }

      for( int i= 0; i < cmd->indices.getSize(); ++i )
        {
        MLObject* o= scene->getObject(cmd->indices[i]);
        if( o != 0 )
          if( (((o->flags & MLObject::SHOW_BOX)==0)?IUNSET:ISET) == toggle )
            cmd->indices[i]= 0; // already hidden or unhidden
        }

      modelScene->hideObjects( cmd, toggle );
      refreshScreen(REFRESH_GEOMETRY);
      return;
      }

    case ModelCommands::RENAME_OBJECT:
      {
      MLObject* o= scene->getObject(cmd->index);
      if( o == 0 )
        return;

      ModelCommand* undo= cmd->createUndo();
      undo->string= o->getName().name;

      o->setName(cmd->string);

      refreshScreen(REFRESH_NAMES);
      break;
      }

    case ModelCommands::RENAME_POOL:
      {
      MLLightPool* pool= scene->getPool(cmd->string);
      if( pool == 0 )
        return;

      ModelCommand* undo= cmd->createUndo();
      undo->string= pool->getName();

      pool->setName(cmd->string);

      refreshScreen(REFRESH_NAMES);
      break;
      }

    case ModelCommands::ADD_POOL:
      {
      stopLightingProcess();

      const MLString& poolName= cmd->string;
      if( scene->getPool(poolName) != 0 )
        return; // already exists?!

      MLLightPool* pool= new MLLightPool(scene);
      pool->setName(poolName);

      ModelCommand* undo= cmd->createUndo();
      undo->type= ModelCommands::REMOVE_POOL;
      undo->string= pool->getName();

      refreshScreen(REFRESH_NAMES);
      break;
      }

    case ModelCommands::REMOVE_POOL:
      {
      MLLightPool* pool= scene->getPool(cmd->string);
      if( pool == 0 )
        return;

      if( pool->getNLights() > 0 )
        return; // this pool is not empty?!

      stopLightingProcess();

      ModelCommand* undo= cmd->createUndo();
      undo->type= ModelCommands::ADD_POOL;
      undo->string= pool->getName();

      MLPoolTable* poolTable= scene->getPoolTable();
      poolTable->removePool(pool);
      delete pool;
      pool= 0;

      refreshScreen(REFRESH_NAMES);
      break;
      }

    default:
      break;
    }
}

/////////////////////////////////////////////////////////////////////////////
