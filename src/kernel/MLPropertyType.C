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
  MLPropertyType.C

  Stephane Rehel
  March 24 1998
*/

#include "MLPropertyType.h"

#include "MLPBoolean.h"
#include "MLPInteger.h"
#include "MLPReal.h"
#include "MLPVector.h"
#include "MLPPoint.h"
#include "MLPColor.h"
#include "MLPString.h"
#include "MLPNodeRef.h"
#include "MLPNodeStruct.h"
#include "MLPByteArray.h"
#include "MLPIntegerArray.h"
#include "MLPRealArray.h"
#include "MLPVectorArray.h"
#include "MLPPointArray.h"
#include "MLPColorArray.h"
#include "MLPVertexArray.h"
#include "MLPEdgeArray.h"
#include "MLPPolygonArray.h"
#include "MLPSVertexArray.h"

/////////////////////////////////////////////////////////////////////////////

// static
MLString MLPropertyType::getName( Type t )
{
  switch(t)
    {
    case NULL_TYPE:
      return "NULL";

    case BOOLEAN:
      return "boolean";

    case INTEGER:
      return "integer";

    case REAL:
      return "real";

    case VECTOR:
      return "vector";

    case POINT:
      return "point";

    case COLOR:
      return "color";

    case STRING:
      return "string";

    case NODE_REF:
      return "node-ref";

    case NODE_STRUCT:
      return "node-struct";

    case BYTE_ARRAY:
      return "byte-array";

    case INTEGER_ARRAY:
      return "integer-array";

    case REAL_ARRAY:
      return "real-array";

    case VECTOR_ARRAY:
      return "vector-array";

    case COLOR_ARRAY:
      return "color-array";

    case VERTEX_ARRAY:
      return "vertexr-array";

    case EDGE_ARRAY:
      return "edge-array";

    case POLYGON_ARRAY:
      return "polygon-array";

    default:
      break;
    }

  return "!BUGGY-PROPERTY-TYPE!";
}

/////////////////////////////////////////////////////////////////////////////

// static
MLProperty* MLPropertyType::newProperty( Type t )
{
  switch(t)
    {
    case NULL_TYPE:
      return 0;

    case BOOLEAN:
      return new MLPBoolean(0);

    case INTEGER:
      return new MLPInteger(0);

    case REAL:
      return new MLPReal(0);

    case VECTOR:
      return new MLPVector(0);

    case POINT:
      return new MLPPoint(0);

    case COLOR:
      return new MLPColor(0);

    case STRING:
      return new MLPString(0);

    case NODE_REF:
      return new MLPNodeRef(0);

    case NODE_STRUCT:
      return new MLPNodeStruct(0);

    case INTEGER_ARRAY:
      return new MLPIntegerArray(0);

    case BYTE_ARRAY:
      return new MLPByteArray(0);

    case REAL_ARRAY:
      return new MLPRealArray(0);

    case VECTOR_ARRAY:
      return new MLPVectorArray(0);

    case COLOR_ARRAY:
      return new MLPColorArray(0);

    case VERTEX_ARRAY:
      return new MLPVertexArray(0);

    case EDGE_ARRAY:
      return new MLPEdgeArray(0);

    case POLYGON_ARRAY:
      return new MLPPolygonArray(0);

    default:
      break;
    }

  return 0; // !BUGGY-PROPERTY-TYPE!
}

/////////////////////////////////////////////////////////////////////////////

// static
boolean MLPropertyType::isArray( Type t )
{
  return t == MLPropertyType::BYTE_ARRAY ||
         t == MLPropertyType::INTEGER_ARRAY ||
         t == MLPropertyType::REAL_ARRAY ||
         t == MLPropertyType::VECTOR_ARRAY ||
         t == MLPropertyType::POINT_ARRAY ||
         t == MLPropertyType::COLOR_ARRAY ||
         t == MLPropertyType::VERTEX_ARRAY ||
         t == MLPropertyType::EDGE_ARRAY ||
         t == MLPropertyType::POLYGON_ARRAY ||
         t == MLPropertyType::SVERTEX_ARRAY;
}

/////////////////////////////////////////////////////////////////////////////
