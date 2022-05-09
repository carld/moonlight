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
  MLSceneIDs.h

  Stephane Rehel
  May 2 1997
*/

#ifndef __MLSceneIDs_h
#define __MLSceneIDs_h

/////////////////////////////////////////////////////////////////////////////

class MLSceneID
{
public:

  static const unsigned int signature;

  enum {
       FIRST                  = 0,
       DRIVER_VERSION         = 1,
       SCENE_PARAMS           = 2,
       SCENE_HIER             = 3,
       SCENE_ABSTRACTS        = 4,
       SCENE_POOLS            = 5,
       SCENE_LIGHTING         = 6,
       SCENE_RENDERER_OPTIONS = 7,
       SCENE_INFO             = 8,
       LAST
       };
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLSceneIDs_h
