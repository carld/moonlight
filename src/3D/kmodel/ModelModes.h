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
  ModelModes.h

  Stephane Rehel

  December 9 1996
*/

#ifndef __ModelModes_h
#define __ModelModes_h

class MLModes;
class ModelModule;

/////////////////////////////////////////////////////////////////////////////

class ModelModes
{
public:
  static void init( ModelModule* model, MLModes* m );

  enum MODES_TYPE
    {
    FIRST= 0,

    SELECT= 1,
    DRAG,
    CLICK,
    CAMERA_ORBIT,
    CAMERA_ZOOM,
    CAMERA_WALK,

    RAY,
    RAY_SELECTED,

    TAG_VERTEX,
    TAG_POLYGON,

    LAST
    };
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelModes_h

