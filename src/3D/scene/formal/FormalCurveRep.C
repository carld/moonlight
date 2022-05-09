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
   FormalCurveRep.C

   Stephane Rehel

   January 21 1998
*/

#include "FormalCurveRep.h"

#include "scene/CurveRep.h"

#include "FormalSquare.h"

#include "FormalParamSegment.h"
#include "FormalParamCircle.h"

#include "FormalNURBSCurve.h"

#include "tools/file/MLRFileBlock.h"
#include "tools/file/MLWFileBlock.h"

/////////////////////////////////////////////////////////////////////////////

FormalCurveRep::FormalCurveRep( MLScene* _scene )
{
  scene= _scene;
}

/////////////////////////////////////////////////////////////////////////////

FormalCurveRep::~FormalCurveRep()
{}

/////////////////////////////////////////////////////////////////////////////

int FormalCurveRep::getSizeOf() const
{
  return sizeof(*this);
}

/////////////////////////////////////////////////////////////////////////////

// static
FormalCurveRep* FormalCurveRep::newFormal( MLScene* scene, TYPE type )
{
  FormalCurveRep* fr= 0;

  switch( type )
    {
    case NULL_TYPE     : break;
    case SQUARE        : fr= new FormalSquare(scene); break;
    case PARAM_SEGMENT : fr= new FormalParamSegment(scene); break;
    case PARAM_CIRCLE  : fr= new FormalParamCircle(scene); break;
    case NURBS         : fr= new FormalNURBSCurve(scene); break;
    case LAST_TYPE     : break;
    }

  assert( fr != 0 );

  return fr;
}

/////////////////////////////////////////////////////////////////////////////

boolean FormalCurveRep::isParamCurve() const
{
  return false;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString FormalCurveRep::getName( FormalCurveRep::TYPE t )
{
  switch(t)
    {
    case NULL_TYPE     : return "";
    case SQUARE        : return "Square";
    case PARAM_SEGMENT : return "Segment";
    case PARAM_CIRCLE  : return "Circle";
    case NURBS         : return "NURBS";
    case LAST_TYPE     : return "";
    }

  return "";
}

/////////////////////////////////////////////////////////////////////////////

// static
FormalCurveRep::TYPE FormalCurveRep::getTypeFromName( const MLString& name )
{
  if( name == "Square"  ) return SQUARE;
  if( name == "NURBS"   ) return NURBS;
  if( name == "Segment" ) return PARAM_SEGMENT;
  if( name == "Circle"  ) return PARAM_CIRCLE;
  return NULL_TYPE;
}

/////////////////////////////////////////////////////////////////////////////

// static
FormalCurveRep* FormalCurveRep::read( MLScene* scene, MLRFileBlock* block )
{
  assert( block != 0 );

  int type;
  *block >> type;

  FormalCurveRep* fr= newFormal(scene,TYPE(type));

  fr->_read(block);

  return fr;
}

/////////////////////////////////////////////////////////////////////////////

// static
void FormalCurveRep::write( MLWFileBlock* block, FormalCurveRep* formal )
{
  assert( formal != 0 );
  assert( block != 0 );

  *block << int(formal->getType());

  formal->_write(block);
}

/////////////////////////////////////////////////////////////////////////////
