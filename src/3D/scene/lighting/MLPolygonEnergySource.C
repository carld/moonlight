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
  MLPolygonEnergySource.C

  Stephane Rehel
  April 4 1997
*/

#include "tools/Spectrum.h"
#include "tools/3D/MLPolygon.h"

#include "MLPolygonEnergySource.h"
#include "MLPoolTable.h"

#include "scene/MeshRep.h"
#include "scene/MLMesh.h"
#include "scene/MLObjectAllocator.h"
#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

MLPolygonEnergySource::MLPolygonEnergySource( MLScene* _scene )
{
  scene= _scene;

  MLPoolTable* table= scene->getPoolTable();
  nPools= table->getNPools();

  energy= new Spectrum [ nPools + 1 ];
  max_abs_power= 0.;
  mesh= 0;
  polygon= 0;
  polygonArea= 0.;
}

/////////////////////////////////////////////////////////////////////////////

MLPolygonEnergySource::~MLPolygonEnergySource()
{
  delete energy; energy= 0;
  mesh= 0;
  polygon= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLPolygonEnergySource::isValid()
{
  if( energy == 0 || mesh == 0 )
    return false;

  if( scene->getPoolTable()->getNPools() != nPools )
    return false;

  if( ! scene->getObjectAllocator()->isValidObject( object_index, mesh ) )
    return false;

  MeshRep* meshrep= mesh->getMeshRep();
  if( meshrep == 0 )
    return false;

  if( polygon < 1 || polygon > meshrep->polygons.getSize() )
    return false;

  MLPolygon& p= meshrep->polygons[polygon];

  if( ! p.isShootingPolygon() )
    return false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

MLPolygon* MLPolygonEnergySource::getPolygon()
{
  if( energy == 0 || mesh == 0 )
    return 0;

  if( scene->getPoolTable()->getNPools() != nPools )
    return 0;

  if( ! scene->getObjectAllocator()->isValidObject( object_index, mesh ) )
    return 0;

  MeshRep* meshrep= mesh->getMeshRep();
  if( meshrep == 0 )
    return 0;

  if( polygon < 1 || polygon > meshrep->polygons.getSize() )
    return 0;

  MLPolygon& p= meshrep->polygons[polygon];

  if( ! p.isShootingPolygon() )
    return 0;

  return &p;
}

/////////////////////////////////////////////////////////////////////////////

