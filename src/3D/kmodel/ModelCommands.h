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
  ModelCommands.h

  Stephane Rehel

  August 6 1997
*/

#ifndef __ModelCommands_h
#define __ModelCommands_h

/////////////////////////////////////////////////////////////////////////////

class ModelCommands
{
public:
  enum MODEL_COMMAND
    {
    FIRST_MODEL_COMMAND= 0,

    SNAPSHOT,

    SET_VARIABLE,

    SET_SELECTED,
    SELECT,
    UNSELECT,
    SELECT_TOGGLE,

    REPARENT,

    SCALE,
    ROTATE,
    TRANSLATE,
    SET_TRANSFORMATION, // in MLCSetXForm.C
    SET_TRANSFORMATIONS, // in MLCSetXForms.C
    WORLD_ROTATE, // in MLCRotate.C

    CREATE_PRIM,
    CREATE_NULL_OBJECT,
    CREATE_NURBS_OBJECT,
    CREATE_LIGHT,
    DUPLICATE,
    DELETE,
    WAKEUP_OBJECTS,
    INSTANTIATE,

    CREATE_CURVE,
    CREATE_NURBS_CURVE,

    HIDE,
    UNHIDE,
    HIDE_UNHIDE,

    GET_INFO,

    RENAME_OBJECT,
    RENAME_POOL,
    ADD_POOL,
    REMOVE_POOL,

    EDIT_LIGHT, // custom command, in MLCEditLight.C
    EDIT_POOL, // custom command, in MLCEditPool.C

    CREATE_GEOMETRY, // index == abstract object id
    CREATE_OBJECT,

    SET_MATERIAL, // (set or create) in MLCSetMaterial.C

    TAG_VERTEX_TOGGLE,
    TAG_VERTEX_SET,

    SCALE_VERTICES,
    ROTATE_VERTICES,
    TRANSLATE_VERTICES,

    TAG_POLYGON,

    SET_PARAM_POLYGONS,
    SET_PARAM_SEGMENTS,

    POLYGONIZE,
    UNDO_POLYGONIZE,

    SET_PLAIN_TEXT,

    LAST_MODEL_COMMAND
    };
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelCommands_h
