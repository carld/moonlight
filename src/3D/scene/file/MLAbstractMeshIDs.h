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
  MLAbstractMeshIDs.h

  Stephane Rehel
  May 2 1997
*/

#ifndef __MLAbstractMeshIDs_h
#define __MLAbstractMeshIDs_h

/////////////////////////////////////////////////////////////////////////////

class MLAbstractMeshID
{
public:

  static const unsigned int signature;

  enum {
       FIRST         = 0,
       DRIVER_VERSION= 1,
       NAME          = 2,
       ABSTRACT      = 3,
       FORMAL_REP    = 4,
       MESH_PARAMS   = 5,
       MESH_VERTICES = 6,
       MESH_EDGES    = 7,
       MESH_POLYGONS = 8,
       MESH_SVERTICES= 9,
       MESH_HASH_BOX = 10,
       INFO          = 11,
       LAST
       };
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLSceneIDs_h
