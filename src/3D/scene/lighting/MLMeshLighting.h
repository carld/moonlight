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
  MLMeshLighting.h

  Stephane Rehel

  March 10 1997
*/

#ifndef __MLMeshLighting_h
#define __MLMeshLighting_h

#ifndef __MLMeshPoolLightingArray_h
#include "MLMeshPoolLightingArray.h"
#endif

#ifndef __MLSpectrumArray_h
#include "MLSpectrumArray.h"
#endif

#ifndef __MLFormFactor_h
#include "MLFormFactor.h"
#endif

class MLMesh;
class MLLight;
class MLLightPool;
class MLLightingControl;
class MLPolygonEnergySource;

/////////////////////////////////////////////////////////////////////////////

class MLMeshLighting
{
  friend class MLMeshPoolLighting; // for setting dirtyFinal
  friend class MLLightPool; // for setting dirtyFinal
  friend class MLShooter; // for accessing/setting indirectPES
  friend class MLSceneIO;

protected:
  MLMesh* mesh;

  // indirectPES = the indirect source that is to be shot to this
  //               object.
  MLPolygonEnergySource* indirectPES;

  // nPools long: one lighting for one pool
  MLMeshPoolLightingArray lightings; // of MLMeshPoolLighting*

  // nSVertices long
  MLSpectrumArray finalLighting;

  boolean final_is_only_indirect;

protected:
  boolean dirty;      // lighting of this object is fully dirty
  boolean dirtyFinal; // set by MLMeshPoolLighting

public:
  MLMeshLighting( MLMesh* _mesh );

  virtual ~MLMeshLighting();

  int getSizeOf() const;

  void setDirty();
  void setDirtyPool( MLLightPool* pool );

  boolean isDirty() const
    {
    return dirty;
    }

  void reset();
  void resetPool( MLLightPool* pool );
  void resetIndirect();

  void removePool( MLLightPool* pool );
  void addPool( MLLightPool* pool );

  boolean hasContributionOfLight( MLLight* light );

  MLMeshPoolLighting* prepareForShooting( MLLightPool* pool );
  MLMeshPoolLighting* prepareForShooting( MLLight* light );

  // might be an expensive function (depends on dirty flag)
  const MLSpectrumArray& getFinalLighting( boolean only_indirect = false );

  Spectrum getSVertexFinalLighting( MLLightPool* pool, int svi );

  // no check! be careful.
  const Spectrum& peekFinal( int i ) const
    {
    return finalLighting.get(i);
    }

  // lighting process
  boolean shoot( MLLightingControl& control );

//private:
//  Spectrum getPolygonUnshotEnergy( int polygon_index,
//                                   const Spectrum* poolsEnergy );

public:
  // called by the lighting process
  // return false if unable to find any
  boolean getShootingPolygon( MLPolygonEnergySource& pes );
  void retrieveShootingPolygon( MLPolygonEnergySource* pes );

  void setIndirectPES( MLPolygonEnergySource* pes );
  void removeIndirectPES();

  // ff[] is nSVertices long
  void indirectShoot( MLFormFactor* ff, MLPolygonEnergySource* pes );

  void getShotUnshot( Spectrum& direct_shot,
                      Spectrum& indirect_shot,
                      Spectrum& unshot );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLMeshLighting_h
