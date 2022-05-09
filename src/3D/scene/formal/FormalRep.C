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
   FormalRep.C

   November 10 1996
   Stephane Rehel
*/

#include "FormalRep.h"

#include "scene/MLScene.h"
#include "scene/MeshRep.h"

#include "FormalBox.h"
#include "FormalParamCylinder.h"
#include "FormalParamSphere.h"
#include "FormalParamFace.h"
#include "FormalParamCone.h"
#include "FormalParamTorus.h"
#include "FormalPolyhedron.h"
#include "FormalNURBS.h"
#include "FormalText.h"

#include "FormalFoo.h"

#include "tools/file/MLRFileBlock.h"
#include "tools/file/MLWFileBlock.h"

/////////////////////////////////////////////////////////////////////////////

FormalRep::FormalRep( MLScene* _scene )
{
  scene= _scene;
}

/////////////////////////////////////////////////////////////////////////////

FormalRep::~FormalRep()
{}

/////////////////////////////////////////////////////////////////////////////

int FormalRep::getSizeOf() const
{
  return sizeof(*this);
}

/////////////////////////////////////////////////////////////////////////////

// static
FormalRep* FormalRep::newFormal( MLScene* scene, TYPE type )
{
  FormalRep* fr= 0;

  switch( type )
    {
    case NULL_TYPE: break;
    case LAST_TYPE: break;
    case BOX           : fr= new FormalBox(scene); break;
    case MESH          : fr= 0; break;
    case POLYGONS      : fr= 0; break;
    case PARAM_CYLINDER: fr= new FormalParamCylinder(scene); break;
    case PARAM_SPHERE  : fr= new FormalParamSphere(scene); break;
    case PARAM_FACE    : fr= new FormalParamFace(scene); break;
    case PARAM_CONE    : fr= new FormalParamCone(scene); break;
    case PARAM_TORUS   : fr= new FormalParamTorus(scene); break;
    case POLYHEDRON    : fr= new FormalPolyhedron(scene); break;
    case NURBS         : fr= new FormalNURBS(scene); break;
    case TEXT          : fr= new FormalText(scene); break;
    case FOO           : fr= new FormalFoo(scene); break;
    }

  assert( fr != 0 );

  return fr;
}

/////////////////////////////////////////////////////////////////////////////

boolean FormalRep::isParamSurf() const
{
  return false;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString FormalRep::getName( FormalRep::TYPE t )
{
  switch(t)
    {
    case NULL_TYPE     : return "";
    case BOX           : return "Box";
    case MESH          : return "Mesh";
    case POLYGONS      : return "Polygons";
    case PARAM_CYLINDER: return "Cylinder";
    case PARAM_SPHERE  : return "Sphere";
    case PARAM_FACE    : return "Face";
    case PARAM_CONE    : return "Cone";
    case PARAM_TORUS   : return "Torus";
    case POLYHEDRON    : return "Polyhedron";
    case NURBS         : return "NURBS";
    case TEXT          : return "Text";
    case FOO           : return "";
    case LAST_TYPE     : return "";
    }

  return "";
}

/////////////////////////////////////////////////////////////////////////////

// static
FormalRep::TYPE FormalRep::getTypeFromName( const MLString& name )
{
  if( name == "Box"            ) return BOX;
  if( name == "Mesh"           ) return MESH;
  if( name == "Polygons"       ) return POLYGONS;
  if( name == "Cylinder"       ) return PARAM_CYLINDER;
  if( name == "Sphere"         ) return PARAM_SPHERE;
  if( name == "Face"           ) return PARAM_FACE;
  if( name == "Cone"           ) return PARAM_CONE;
  if( name == "Torus"          ) return PARAM_TORUS;
  if( name == "Polyhedron"     ) return POLYHEDRON;
  if( name == "NURBS"          ) return NURBS;
  if( name == "Text"           ) return TEXT;

  return NULL_TYPE;
}

/////////////////////////////////////////////////////////////////////////////

// static
FormalRep* FormalRep::read( MLScene* scene, MLRFileBlock* block )
{
  assert( block != 0 );

  int type;
  *block >> type;

  FormalRep* fr= newFormal(scene,TYPE(type));

  fr->_read(block);

  return fr;
}

/////////////////////////////////////////////////////////////////////////////

// static
void FormalRep::write( MLWFileBlock* block, FormalRep* formal )
{
  assert( formal != 0 );
  assert( block != 0 );

  *block << int(formal->getType());

  formal->_write(block);
}

/////////////////////////////////////////////////////////////////////////////
