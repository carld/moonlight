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
  MLPropertyType.h

  Stephane Rehel
  March 24 1998
*/

#ifndef __MLPropertyType_h
#define __MLPropertyType_h

#include "tools/MLString.h"

class MLProperty;

/////////////////////////////////////////////////////////////////////////////

class MLPropertyType
{
public:
  enum Type
       {
       NULL_TYPE = 0,

       BOOLEAN = 1,
       INTEGER,
       REAL,
       VECTOR,
       POINT,
       COLOR,
       STRING,

       BYTE_ARRAY,
       INTEGER_ARRAY,
       REAL_ARRAY,
       VECTOR_ARRAY,
       POINT_ARRAY,
       COLOR_ARRAY,
       VERTEX_ARRAY,
       EDGE_ARRAY,
       POLYGON_ARRAY,
       SVERTEX_ARRAY,

       NODE_REF,
       NODE_STRUCT,

       LAST_TYPE
       };

  static MLString getName( Type t );

  static MLProperty* newProperty( Type t );

  static boolean isArray( Type t );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLPropertyType_h
