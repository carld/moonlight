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
  MLTagVertexArray.C

  Stephane Rehel

  October 28 1997
*/

#include "scene/MLVertexID.h"
#include "scene/MLControlPointID.h"
#include "scene/MLScene.h"

#include "MLTagVertexArray.h"

/////////////////////////////////////////////////////////////////////////////

MLTagVertexArray::MLTagVertexArray( MLScene* _scene )
{
  scene= _scene;
}

/////////////////////////////////////////////////////////////////////////////

MLTagVertexArray& MLTagVertexArray::operator = ( const MLTagVertexArray& tva )
{
  scene= tva.scene;
  IntegerArray::operator = ( tva );

  return *this;
}

/////////////////////////////////////////////////////////////////////////////

// i >= 0
MLMesh* MLTagVertexArray::getMesh( int i )
{
  i *= 2;

  if( i < 0 || i+1 >= IntegerArray::getSize() )
    return 0;

  MLVertexID vid( IntegerArray::get(i), IntegerArray::get(i+1) );

  return vid.getMesh(scene);
}

/////////////////////////////////////////////////////////////////////////////

// i >= 0
MLVertex* MLTagVertexArray::getVertex( int i )
{
  i *= 2;

  if( i < 0 || i+1 >= IntegerArray::getSize() )
    return 0;

  MLVertexID vid( IntegerArray::get(i), IntegerArray::get(i+1) );

  return vid.getVertex(scene);
}

/////////////////////////////////////////////////////////////////////////////

// i >= 0
boolean MLTagVertexArray::isControlPoint( int i )
{
  i *= 2;

  if( i < 0 || i+1 >= IntegerArray::getSize() )
    return false;

  MLControlPointID cpid( IntegerArray::get(i), IntegerArray::get(i+1) );

  return cpid.getNURBS(scene) != 0;
}

/////////////////////////////////////////////////////////////////////////////

// i >= 0
MLNURBS_ControlPoint* MLTagVertexArray::getControlPoint( int i )
{
  i *= 2;

  if( i < 0 || i+1 >= IntegerArray::getSize() )
    return 0;

  MLControlPointID cpid( IntegerArray::get(i), IntegerArray::get(i+1) );

  return cpid.getControlPoint(scene);
}

/////////////////////////////////////////////////////////////////////////////
