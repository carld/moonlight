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
  ModelFunctions.h

  Stephane Rehel

  August 2 1997
*/

#ifndef __ModelFunctions_h
#define __ModelFunctions_h

class ModelModule;
class MLFunctions;

/////////////////////////////////////////////////////////////////////////////

class ModelFunctions
{
public:
  static void init( ModelModule* model );

  enum {
       FIRST = 0,

       SELECT,
       GET_INFO,
       HIDE_UNHIDE,
       DUPLICATE,
       INSTANTIATE,
       DELETE,
       TRANSLATE,
       TRANSLATE_XYZ,
       SCALE,
       SCALE_XYZ,
       ROTATE,
       ROTATE_XYZ,
       REPARENT,

       CAMERA_ORBIT,
       CAMERA_ZOOM,
       CAMERA_WALK,

       UNDO_REDO,
       REFRESH_SCREEN,

       ORIENT_OBJECT,

       TAG_VERTEX,
       TAG_POLYGON,

       EDIT_MATERIAL,

       LAST
       };
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelFunctions_h
