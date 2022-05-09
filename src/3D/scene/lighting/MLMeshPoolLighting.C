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
  MLMeshPoolLighting.C

  Direct, indirect, and unshot lighting of an object, from a pool.

  Stephane Rehel

  March 10 1997
*/

#include "tools/3D/MLPolygon.h"

#include "MLMeshPoolLighting.h"

#include "scene/material/MLMaterial.h"

#include "scene/MeshRep.h"
#include "scene/MLMesh.h"

#include "MLLightPool.h"
#include "MLLight.h"

#include "MLMeshLighting.h"
#include "MLLightingControl.h"

#include "MLShooter.h"

/////////////////////////////////////////////////////////////////////////////

MLMeshPoolLighting::MLMeshPoolLighting( MLMesh* _mesh,
                                            MLLightPool* _pool )
{
  mesh= _mesh;
  pool= _pool;
  dirty= true;
}

/////////////////////////////////////////////////////////////////////////////

MLMeshPoolLighting::~MLMeshPoolLighting()
{
  pool= 0;
  mesh= 0;
}

/////////////////////////////////////////////////////////////////////////////

int MLMeshPoolLighting::getSizeOf() const
{
  return sizeof(this)
         + ffDirect.getSizeOf() - sizeof(ffDirect)
         + eDirect.getSizeOf() - sizeof(eDirect)
         + indirect.getSizeOf() - sizeof(indirect)
         + unshotEnergy.getSizeOf() - sizeof(unshotEnergy);
}

/////////////////////////////////////////////////////////////////////////////

void MLMeshPoolLighting::reset()
{
/*
  if( ffDirect.getSize() == 0 &&
      eDirect.getSize() == 0 &&
      indirect.getSize() == 0 &&
      unshotEnergy.getSize() == 0 )
    return;
*/

  ffDirect.clear();
  eDirect.clear();
  indirect.clear();
  unshotEnergy.clear();

  dirty= true;
  pool->dirtyIndirect= true;

//***  mesh->getLighting()->dirtyFinal= true;
}

/////////////////////////////////////////////////////////////////////////////

void MLMeshPoolLighting::resetIndirect()
{
  MeshRep* meshrep= mesh->getMeshRep();

  boolean setFinalDirty= false;

  if( indirect.getSize() > 0 )
    {
    setFinalDirty= true;
    indirect.clear();
    }

  int nSVertices= mesh->getSVerticesSize();

  if( pool->getNLights() == 0 ||
      pool->getPoolType() == MLLightPool::DIRECT_POOL )
    {
clear_exit:
    if( unshotEnergy.getSize() > 0 )
      {
      setFinalDirty= true;
      unshotEnergy.clear();
      }
//***    mesh->getLighting()->dirtyFinal= setFinalDirty;
    return;
    }

  MLMaterial* material= mesh->getMaterial();

  if( ! material->isRadiosityContributor() )
    goto clear_exit;

  boolean single= (pool->getPoolType() == MLLightPool::SINGLE_GLOBAL_POOL);

  if( ( single && ffDirect.getSize() < nSVertices ) ||
      (!single && eDirect.getSize() < nSVertices ) ||
      meshrep == 0 )
    {
    if( unshotEnergy.getSize() > 0 )
      setFinalDirty= true;

    unshotEnergy.clear();
//***    mesh->getLighting()->dirtyFinal= setFinalDirty;
    return;
    }

  setFinalDirty= true;
  MLPolygonArray& polygons= meshrep->polygons;
  int nPolygons= polygons.getSize();
  if( nPolygons < 1 )
    {
    unshotEnergy.clear();
    return;
    }

  unshotEnergy.realloc(nPolygons);

  MLLight* light= pool->getSingleGlobalLight();

  if( light != 0 )
    {
    Spectrum diffuse(material->diffuse.get());

    for( int i= 1; i <= nPolygons; ++i )
      {
      MLPolygon& p= polygons[i];
      if( ! p.isShootingPolygon() )
        continue;

      if( p.nVertices < 3 )
        continue;

      register MLFormFactor ff= 0.;
      register MLPolygon::Element* elements= p.elements;
      for( int j= p.nVertices; j; --j, ++elements )
        {
        register int svertex= elements->svertex;
        if( svertex == 0 )
          continue;
        ff+= ffDirect [ svertex ];
        }

      ff *= mesh->getPolygonArea(p) / double(p.nVertices);
      unshotEnergy[i]= diffuse * ff;
      }
    }
   else
    {
    Spectrum diffuse(material->diffuse.get());

    for( int i= 1; i <= nPolygons; ++i )
      {
      MLPolygon& p= polygons[i];
      if( ! p.isShootingPolygon() )
        continue;

      if( p.nVertices < 3 )
        continue;

      Spectrum energy(0,0,0);
      register MLPolygon::Element* elements= p.elements;
      for( int j= p.nVertices; j; --j, ++elements )
        {
        register int svertex= elements->svertex;
        if( svertex == 0 )
          continue;
        energy+= eDirect [ svertex ];
        }

      energy *= mesh->getPolygonArea(p) / double(p.nVertices);
      unshotEnergy[i]= energy & diffuse;
      }
    }

//***  mesh->getLighting()->dirtyFinal= setFinalDirty;
}

/////////////////////////////////////////////////////////////////////////////

// ff[] is nSVertices long
void MLMeshPoolLighting::directShoot( MLFormFactor* ff,
                                        const Spectrum& energy )
{
  if( ff == 0 )
    return;

  int nSVertices= mesh->getSVerticesSize();
  if( nSVertices < 1 ||
      pool->getNLights() == 0 ||
      pool->getPoolType() == MLLightPool::DIRECT_POOL ||
      !mesh->getMaterial()->isRadiosityContributor() )
    {
    unshotEnergy.clear();
    indirect.clear();
    pool->dirtyIndirect= true;
    return;
    }

  MLLight* light= pool->getSingleGlobalLight();

  if( light != 0 )
    {
    eDirect.clear();

    if(ffDirect.getSize()<nSVertices)
      ffDirect.realloc(nSVertices);

    for( int i= 1; i <= nSVertices; ++i )
      ffDirect[i]= ff[i];
    }
   else
    {
    ffDirect.clear();

    eDirect.realloc(nSVertices);
    for( int i= 1; i <= nSVertices; ++i )
      eDirect[i]+= ff[i] * energy;
    }

  unshotEnergy.clear();
  indirect.clear();
  pool->dirtyIndirect= true;

  mesh->getLighting()->dirtyFinal= true;
}

/////////////////////////////////////////////////////////////////////////////

// lighting process
boolean MLMeshPoolLighting::shoot( MLLightingControl& control )
{
  if( pool->getPoolType() == MLLightPool::DIRECT_POOL )
    return true;

  if( ! mesh->getMaterial()->isRadiosityContributor() )
    return true;

  reset();

  for( int i= 1; i <= pool->getNLights(); ++i )
    {
    if( control.cancelled() )
      return false; // remains dirty

    MLLight* light= pool->getLight(i);
    if( light == 0 )
      continue;

    MLShooter shooter(mesh->getScene(),&control);

    shooter.shoot(light,mesh);
    }

  dirty= false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

Spectrum MLMeshPoolLighting::getSVertexFinalLighting( int svi )
{
  Spectrum s(0,0,0);

  MeshRep* meshrep= mesh->getMeshRep();
  if( meshrep == 0 )
    return s;

  MLSVertexArray& svertices= meshrep->svertices;
  if( svi < 1 || svi > svertices.getSize() )
    return s;

  MLSVertex& sv= svertices[svi];
  if( ! sv.allocated() )
    return s;

  if( pool->getNLights() == 0 )
    return s;
  if( pool->getPoolType() == MLLightPool::DIRECT_POOL )
    return s;

  MLLight* light= pool->getSingleGlobalLight();

  if( light == 0 )
    {
    MLSpectrumArray& direct= eDirect;
    if( svi > direct.getSize() )
      goto indirect;

    MLMaterial* material= mesh->getMaterial();
    Spectrum diffuse= material->diffuse.get() & pool->getEnergy();

    // get static direct lighting
    s+= direct[svi] & diffuse;
    }
   else
    {
    MLFormFactorArray& direct= ffDirect;
    if( svi > direct.getSize())
      goto indirect;

    MLMaterial* material= mesh->getMaterial();
    Spectrum energy= material->diffuse.get() & light->getEnergy();
    energy &= pool->getEnergy();

    // get dynamic direct lighting
    s+= direct[svi] * energy;
    }

indirect:

  if( svi > indirect.getSize() )
    return s; // no indirect lighting for this pool/object

  Spectrum energy= pool->getEnergy();
  if( light != 0 )
    energy &= light->getEnergy();

  MLMaterial* material= mesh->getMaterial();
  energy &= material->diffuse.get();

  s+= indirect[svi] & energy;

  return s;
}

/////////////////////////////////////////////////////////////////////////////
