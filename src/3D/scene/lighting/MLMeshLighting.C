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
  MLMeshLighting.C

  Stephane Rehel

  March 10 1997
*/

#include "tools/3D/MLPolygon.h"

#include "MLMeshLighting.h"

#include "scene/material/MLMaterial.h"

#include "scene/MeshRep.h"
#include "scene/MLMesh.h"
#include "scene/MLScene.h"

#include "MLLight.h"
#include "MLPoolTable.h"

#include "MLLightPool.h"
#include "MLMeshPoolLighting.h"
#include "MLPolygonEnergySource.h"

/////////////////////////////////////////////////////////////////////////////

MLMeshLighting::MLMeshLighting( MLMesh* _mesh )
{
  mesh= _mesh;
  dirty= true;
  dirtyFinal= true;
  final_is_only_indirect= false;

  indirectPES= 0;

  MLScene* scene= mesh->getScene();
  if( scene != 0 )
    {
    MLPoolTable* table= scene->getPoolTable();
    if( table != 0 )
      {
      for( int i= 1; i <= table->getNPools(); ++i )
        {
        MLLightPool* pool= table->getPool(i);
        if( pool == 0 )
          continue;
        addPool(pool);
        }
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

MLMeshLighting::~MLMeshLighting()
{
  for( int i= 1; i <= lightings.getSize(); ++i )
    {
    MLMeshPoolLighting* sl= lightings[i];
    lightings[i]= 0;
    delete sl;
    }

  unref(indirectPES);
  indirectPES= 0;
}

/////////////////////////////////////////////////////////////////////////////

int MLMeshLighting::getSizeOf() const
{
  int s= sizeof(*this)
        +finalLighting.getSizeOf() - sizeof(finalLighting)
        +lightings.getSizeOf() - sizeof(lightings);

  for( int i= 1; i <= lightings.getSize(); ++i )
    {
    const MLMeshPoolLighting* sl= lightings.get(i);
    if( sl != 0 )
      s+= sl->getSizeOf();
    }

  return s;
}

/////////////////////////////////////////////////////////////////////////////

void MLMeshLighting::setDirty()
{
  dirty= true;
  mesh->getScene()->dirtyObjectsLighting= true;
  reset();
}

/////////////////////////////////////////////////////////////////////////////

void MLMeshLighting::setDirtyPool( MLLightPool* pool )
{
  if( pool == 0 )
    return;

  dirty= true;
  mesh->getScene()->dirtyObjectsLighting= true;

  resetPool(pool);
}

/////////////////////////////////////////////////////////////////////////////

void MLMeshLighting::reset()
{
  MLPoolTable* pools= mesh->getScene()->getPoolTable();

  if( pools->getNPools() == 0 )
    return;

  for( int i= 1; i <= lightings.getSize(); ++i )
    {
    MLLightPool* pool= pools->getPool(i);
    if( pool == 0 )
      continue;

    resetPool(pool);
    }

  unref(indirectPES);
  indirectPES= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLMeshLighting::resetPool( MLLightPool* pool )
{
  if( pool == 0 )
    return;

  int i= pool->getPoolIndex();
  if( i < 1 || i > lightings.getSize() )
    return;

  if( pool->getNLights() == 0 ||
      pool->getPoolType() == MLLightPool::DIRECT_POOL )
    {
    delete lightings[i];
    lightings[i]= 0;
    return;
    }

  MLMeshPoolLighting* pl= lightings[i];

  if( pl == 0 )
    {
    pl= new MLMeshPoolLighting(mesh,pool);
    lightings[i]= pl;
    }

  pl->reset();

  unref(indirectPES);
  indirectPES= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLMeshLighting::resetIndirect()
{
  unref(indirectPES);
  indirectPES= 0;

  for( int i= 1; i <= lightings.getSize(); ++i )
    {
    MLMeshPoolLighting* sl= lightings[i];
    if( sl != 0 )
      sl->resetIndirect();
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLMeshLighting::removePool( MLLightPool* pool )
{
  if( pool == 0 )
    return;

  int index= pool->getPoolIndex();

  if( index < 1 || index > lightings.getSize() )
    return;

  MLMeshPoolLighting* sl= lightings[index];
  lightings[index]= 0;

  delete sl;

  lightings.remove(index);
}

/////////////////////////////////////////////////////////////////////////////

void MLMeshLighting::addPool( MLLightPool* pool )
{
  if( pool == 0 )
    return;

  int i1= lightings.getSize() + 1;
  int i2= pool->getPoolIndex();

  lightings.realloc(max(i2,lightings.getSize()));
  int i;
  for( i= i1; i < i2; ++i )
    lightings[i]= 0;

  if( pool->getPoolType() == MLLightPool::DIRECT_POOL )
    lightings[i2]= 0;
   else
    lightings[i2]= new MLMeshPoolLighting(mesh,pool);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLMeshLighting::hasContributionOfLight( MLLight* light )
{
  if( light == 0 )
    return false;

  MLLightPool* pool= light->getPool();
  if( pool == 0 )
    return false;

  int index= pool->getPoolIndex();
  if( index < 1 || index > lightings.getSize() )
    return false;

  MLMeshPoolLighting* sl= lightings[index];
  if( sl == 0 )
    return false;

  return sl->ffDirect.getSize() > 0 ||
         sl->eDirect.getSize() > 0  ||
         sl->indirect.getSize() > 0 ||
         sl->unshotEnergy.getSize() > 0 ;
}

/////////////////////////////////////////////////////////////////////////////

MLMeshPoolLighting* MLMeshLighting::prepareForShooting(
                                                         MLLightPool* pool )
{
  if( pool == 0 )
    return 0;

  int index= pool->getPoolIndex();

  if( index < 1 || index > lightings.getSize() )
    return 0;

  if( pool->getPoolType() == MLLightPool::DIRECT_POOL )
    {
    delete lightings[index];
    lightings[index]= 0;
    return 0;
    }

  if( lightings[index] == 0 )
    {
    lightings[index]= new MLMeshPoolLighting(mesh,pool);
    lightings[index]->reset();
    }

  return lightings[index];
}

/////////////////////////////////////////////////////////////////////////////

MLMeshPoolLighting* MLMeshLighting::prepareForShooting( MLLight* light )
{
  if( light == 0 )
    return 0;

  return prepareForShooting( light->getPool() );
}

/////////////////////////////////////////////////////////////////////////////

// might be an expensive function (depends on dirty flag)
const MLSpectrumArray& MLMeshLighting::getFinalLighting(
                                        boolean only_indirect /* = false */ )
{
  if( ! mesh->getMaterial()->isRadiosityContributor() )
    {
    dirtyFinal= false;
    finalLighting.clear();
    return finalLighting;
    }

  if( !dirtyFinal && ((final_is_only_indirect?1:0) == (only_indirect?1:0)) )
    {
    finalLighting.realloc(mesh->getSVerticesSize()); // safe
    return finalLighting;
    }

  dirtyFinal= false;
  final_is_only_indirect= only_indirect;

  int nSVertices= mesh->getSVerticesSize();
  if( nSVertices < 1 )
    return finalLighting;

  int i;

  if( finalLighting.getSize() == 0 )
    finalLighting.realloc(nSVertices);
   else
    {
    finalLighting.realloc(nSVertices);
    for( i= 1; i <= nSVertices; ++i )
      finalLighting[i].zero();
    }

  MLPoolTable* pools= mesh->getScene()->getPoolTable();

  for( i= 1; i <= lightings.getSize(); ++i )
    {
    MLMeshPoolLighting* sl= lightings[i];
    if( sl == 0 )
      continue;

    MLLightPool* pool= pools->getPool(i);
    if( pool == 0 )
      continue;
    if( pool->getNLights() == 0 )
      continue;
    if( pool->getPoolType() == MLLightPool::DIRECT_POOL )
      continue;

    // first get direct lighting
    MLLight* light= pool->getSingleGlobalLight();

    if( only_indirect )
      goto indirect;

    if( light == 0 )
      {
      MLSpectrumArray& direct= sl->eDirect;
      if( direct.getSize() < nSVertices )
        goto indirect;

      MLMaterial* material= mesh->getMaterial();
      Spectrum diffuse= material->diffuse.get() & pool->getEnergy();

      // get static direct lighting
      for( int j= 1; j <= nSVertices; ++j )
        finalLighting[j]+= direct[j] & diffuse;
      }
     else
      {
      MLFormFactorArray& direct= sl->ffDirect;
      if( direct.getSize() < nSVertices )
        goto indirect;

      MLMaterial* material= mesh->getMaterial();
      Spectrum energy= material->diffuse.get() & light->getEnergy();
      energy &= pool->getEnergy();

      // get dynamic direct lighting
      for( int j= 1; j <= nSVertices; ++j )
        finalLighting[j]+= direct[j] * energy;
      }

indirect:

    MLSpectrumArray& indirect= sl->indirect;
    if( indirect.getSize() < nSVertices )
      continue; // no indirect lighting for this pool/object

    Spectrum energy= pool->getEnergy();
    if( light != 0 )
      energy &= light->getEnergy();

    MLMaterial* material= mesh->getMaterial();
    energy &= material->diffuse.get();

    // sum indirect lighting
    for( int j= 1; j <= nSVertices; ++j )
      finalLighting[j]+= indirect[j] & energy;
    }

  return finalLighting;
}

/////////////////////////////////////////////////////////////////////////////

Spectrum MLMeshLighting::getSVertexFinalLighting( MLLightPool* pool,
                                                    int svi )
{
  Spectrum s(0,0,0);
  if( pool == 0 )
    return s;

  int index= pool->getPoolIndex();
  if( index < 1 || index > lightings.getSize() )
    return s;

  MLMeshPoolLighting* opl= lightings[index];
  if( opl == 0 )
    return s;

  return opl->getSVertexFinalLighting(svi);
}

/////////////////////////////////////////////////////////////////////////////

// lighting process
boolean MLMeshLighting::shoot( MLLightingControl& control )
{
  if( ! dirty )
    return true;

  MLPoolTable* poolTable= mesh->getScene()->getPoolTable();
  if( poolTable == 0 )
    return true;

  for( int i= 1; i <= poolTable->getNPools(); ++i )
    {
    MLLightPool* pool= poolTable->getPool(i);
    if( pool == 0 )
      continue;

    MLMeshPoolLighting* lighting= prepareForShooting(pool);
    if( lighting == 0 )
      continue;

    if( ! lighting->dirty )
      continue;

    if( ! lighting->shoot(control) )
      return false;
    }

  dirty= false;
  return true;
}

/////////////////////////////////////////////////////////////////////////////

/*
// private
Spectrum MLMeshLighting::getPolygonUnshotEnergy( int polygon_index,
                                            const Spectrum* poolsEnergy )
{
  Spectrum total(0,0,0);

  for( int i= 1; i <= lightings.getSize(); ++i )
    {
    MLMeshPoolLighting* sl= lightings[i];
    if( sl == 0 )
      continue;

    register MLSpectrumArray& ue= sl->unshotEnergy;
    if( ue.getSize() < polygon_index )
      continue;
    total+= ue[ polygon_index ] & poolsEnergy[i];
    }

  return total;
}
*/

/////////////////////////////////////////////////////////////////////////////

// called by the lighting process
boolean MLMeshLighting::getShootingPolygon( MLPolygonEnergySource& pes )
{
  int nPolygons= mesh->getPolygonsSize();
  if( nPolygons < 1 )
    return false;

  if( ! mesh->getMaterial()->isRadiosityContributor() )
    return false;

  int i;

  MLPoolTable* pools= mesh->getScene()->getPoolTable();

  Spectrum* poolsEnergy= new Spectrum [ lightings.getSize() + 1 ];

  for( i= 1; i <= lightings.getSize(); ++i )
    {
    MLMeshPoolLighting* sl= lightings[i];
    if( sl == 0 )
      continue;

    MLLightPool* pool= pools->getPool(i);
    if( pool == 0 )
      continue;
    if( pool->getNLights() == 0 )
      continue;

    MLLight* light= pool->getSingleGlobalLight();

    if( light == 0 )
      poolsEnergy[i]= pool->getEnergy();
     else
      poolsEnergy[i]= pool->getEnergy() & light->getEnergy();
    }

  double max_abs_power= 0;
  int max_abs_index= 0;

  MLPolygonArray& polygons= mesh->getMeshRep()->polygons;
  for( i= 1; i <= nPolygons; ++i )
    {
    MLPolygon& p= polygons[i];
    if( ! p.isShootingPolygon() )
      continue;

//    Spectrum total= getPolygonUnshotEnergy(i,poolsEnergy);
    Spectrum total(0,0,0);
    for( int j= 1; j <= lightings.getSize(); ++j )
      {
      register MLMeshPoolLighting* sl= lightings[j];
      if( sl == 0 )
        continue;

      register MLSpectrumArray& ue= sl->unshotEnergy;
      if( ue.getSize() < i )
        continue;

      total+= ue[i] & poolsEnergy[j];
      }

    double abs_power= fabs(total.power());

    if( abs_power > max_abs_power )
      {
      max_abs_power= abs_power;
      max_abs_index= i;
      }
    }

  delete poolsEnergy;
  poolsEnergy= 0;

  if( max_abs_index == 0 )
    return false;

  if( max_abs_power > pes.max_abs_power )
    {
    pes.max_abs_power= max_abs_power;
    pes.mesh= mesh;
    pes.object_index= mesh->getID();
    pes.polygon= max_abs_index;
    pes.polygonArea= mesh->getPolygonArea(polygons[max_abs_index]);

    for( i= 1; i <= lightings.getSize(); ++i )
      {
      pes.energy[i].zero();
      MLMeshPoolLighting* sl= lightings[i];

      if( sl == 0 )
        continue;

      register MLSpectrumArray& ue= sl->unshotEnergy;
      if( ue.getSize() < max_abs_index )
        continue;
      pes.energy[i]= ue[ max_abs_index ];
      }
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLMeshLighting::retrieveShootingPolygon( MLPolygonEnergySource* pes )
{
  if( pes == 0 )
    return;
  if( pes->mesh != mesh )
    return;

  if( pes->getPolygon() == 0 )
    return;

  int polygon_index= pes->polygon;
  for( int i= 1; i <= lightings.getSize(); ++i )
    {
    MLMeshPoolLighting* poolLighting= lightings[i];
    if( poolLighting == 0 )
      continue;

    register MLSpectrumArray& ue= poolLighting->unshotEnergy;
    if( ue.getSize() < polygon_index )
      continue;
    ue[ polygon_index ].zero();
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLMeshLighting::setIndirectPES( MLPolygonEnergySource* pes )
{
  ref(pes);

  removeIndirectPES();

  indirectPES= pes;
}

/////////////////////////////////////////////////////////////////////////////

void MLMeshLighting::removeIndirectPES()
{
  unref(indirectPES);
  indirectPES= 0;
}

/////////////////////////////////////////////////////////////////////////////

// ff[] is nSVertices long
void MLMeshLighting::indirectShoot( MLFormFactor* ff,
                                      MLPolygonEnergySource* pes )
{
  if( ff == 0 || pes == 0 )
    return;

  int nSVertices= mesh->getSVerticesSize();
  if( nSVertices < 1 )
    return;

  if( ! mesh->getMaterial()->isRadiosityContributor() )
    return;

  MeshRep* meshrep= mesh->getMeshRep();
  if( meshrep == 0 )
    return;

  MLPoolTable* pools= mesh->getScene()->getPoolTable();
  if( pools->getNPools() != pes->nPools )
    return;

  Spectrum* materialEnergies= new Spectrum [ lightings.getSize() + 1 ];

  MLPolygonArray& polygons= meshrep->polygons;
  int nPolygons= polygons.getSize();

  Spectrum diffuse(mesh->getMaterial()->diffuse.get());

  for( int i= 1; i <= lightings.getSize(); ++i )
    {
    Spectrum e= pes->energy[i];

    MLMeshPoolLighting* poolLighting= lightings[i];
    if( poolLighting == 0 )
      continue;

    MLSpectrumArray& indirect= poolLighting->indirect;
    if( indirect.getSize() < nSVertices )
      indirect.realloc(nSVertices);

    for( int j= 1; j <= nSVertices; ++j )
      indirect[j]+= ff[j] * e;

    materialEnergies[i]= e & diffuse;

    MLSpectrumArray& unshotEnergy= poolLighting->unshotEnergy;
    if( unshotEnergy.getSize() < nPolygons )
      unshotEnergy.realloc(nPolygons);
    }

  for( int j= 1; j <= nPolygons; ++j )
    {
    MLPolygon& p= polygons[j];
    if( ! p.isShootingPolygon() )
      continue;
    if( p.nVertices < 3 )
      continue;

    register double total_ff= 0.;
    register MLPolygon::Element* elements= p.elements;
    for( int k= p.nVertices; k; --k, ++elements )
      {
      register int svertex= elements->svertex;
      if( svertex == 0 )
        continue;
      total_ff+= ff[ svertex ];
      }

    // getPolygonArea may be expensive
    total_ff *= mesh->getPolygonArea(p) / double(p.nVertices);

    for( int i= 1; i <= lightings.getSize(); ++i )
      {
      register MLMeshPoolLighting* poolLighting= lightings[i];
      if( poolLighting == 0 )
        continue;

      register MLSpectrumArray& unshotEnergy= poolLighting->unshotEnergy;

      unshotEnergy[j]+= total_ff * materialEnergies[i];
      }
    }

  delete materialEnergies;
  materialEnergies= 0;

  dirtyFinal= true;
}

/////////////////////////////////////////////////////////////////////////////

void MLMeshLighting::getShotUnshot( Spectrum& direct_shot,
                                      Spectrum& indirect_shot,
                                      Spectrum& unshot )
{
  direct_shot.zero();
  indirect_shot.zero();
  unshot.zero();

  MLMaterial* material= mesh->getMaterial();

  if( ! material->isRadiosityContributor() )
    return;

  MeshRep* meshrep= mesh->getMeshRep();
  if( meshrep == 0 )
    return;

  int nPolygons= mesh->getPolygonsSize();
  if( nPolygons < 1 )
    return;

  int i;

  MLPoolTable* pools= mesh->getScene()->getPoolTable();

  Spectrum diffuse(material->diffuse.get());

  Spectrum* poolsEnergy= new Spectrum [ lightings.getSize() + 1 ];

  for( i= 1; i <= lightings.getSize(); ++i )
    {
    MLMeshPoolLighting* pl= lightings[i];
    if( pl == 0 )
      continue;

    MLLightPool* pool= pools->getPool(i);
    if( pool == 0 )
      continue;
    if( pool->getNLights() == 0 )
      continue;

    MLLight* light= pool->getSingleGlobalLight();

    if( light == 0 )
      poolsEnergy[i]= pool->getEnergy();
     else
      poolsEnergy[i]= pool->getEnergy() & light->getEnergy();

    poolsEnergy[i] &= diffuse;
    }

  MLPolygonArray& polygons= meshrep->polygons;
  for( i= 1; i <= nPolygons; ++i )
    {
    MLPolygon& p= polygons[i];
    if( ! p.isShootingPolygon() )
      continue;
    if( p.nVertices < 3 )
      continue;

    // getPolygonArea() could be expensive
    double area1= mesh->getPolygonArea(p) / double(p.nVertices);

    for( int j= 1; j <= lightings.getSize(); ++j )
      {
      MLMeshPoolLighting* sl= lightings[j];
      if( sl == 0 )
        continue;

        {
        register MLSpectrumArray& ue= sl->unshotEnergy;
        if( i <= ue.getSize() )
          unshot+= ue[i] & poolsEnergy[j];
        }

      Spectrum indirect(0,0,0);
      if( sl->pool->getSingleGlobalLight() == 0 )
        {
        Spectrum direct(0,0,0);
        register MLPolygon::Element* elements= p.elements;
        for( register int k= p.nVertices; k; --k, ++elements )
          {
          register int svertex= elements->svertex;
          if( svertex == 0 )
            continue;
          if( svertex <= sl->eDirect.getSize() )
            direct+= sl->eDirect[ svertex ];
          if( svertex <= sl->indirect.getSize() )
            indirect+= sl->indirect[ svertex ];
          }

        direct_shot+= area1 * (direct & poolsEnergy[j]);
        }
       else
        {
        register double total_ff= 0.;
        register MLPolygon::Element* elements= p.elements;
        for( register int k= p.nVertices; k; --k, ++elements )
          {
          register int svertex= elements->svertex;
          if( svertex == 0 )
            continue;
          if( svertex <= sl->ffDirect.getSize() )
            total_ff+= sl->ffDirect[ svertex ];
          if( svertex <= sl->indirect.getSize() )
            indirect+= sl->indirect[ svertex ];
          }

        direct_shot+= (total_ff * area1) * poolsEnergy[j];
        }

      indirect_shot+= area1 * (indirect & poolsEnergy[j]);
      }

    }

  delete poolsEnergy;
  poolsEnergy= 0;
}

/////////////////////////////////////////////////////////////////////////////
