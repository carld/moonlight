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
  MLMeshPoolLighting.h

  Direct, indirect, and unshot lighting of an object, from a pool.

  Stephane Rehel

  March 10 1997
*/

#ifndef __MLMeshPoolLighting_h
#define __MLMeshPoolLighting_h

#ifndef __MLSpectrumArray_h
#include "MLSpectrumArray.h"
#endif

#ifndef __MLFormFactorArray_h
#include "MLFormFactorArray.h"
#endif

class MLMesh;
class MLLight;
class MLLightPool;
class MLLightingControl;

/////////////////////////////////////////////////////////////////////////////

class MLMeshPoolLighting
{
  friend class MLMeshLighting;
  friend class MLLightPool;
  friend class MLSceneIO;
protected:
  MLMesh* mesh;
  MLLightPool* pool;

  // nSVertices long
  MLFormFactorArray ffDirect; // pool size > 1
  MLSpectrumArray    eDirect;  // pool size == 1

  // nSVertices long
  MLSpectrumArray indirect;

  // nPolygons long (it's too large, but no matter)
  MLSpectrumArray unshotEnergy;
  boolean dirty;

public:
  MLMeshPoolLighting( MLMesh* _mesh, MLLightPool* _pool );

  virtual ~MLMeshPoolLighting();

  int getSizeOf() const;

  void reset();

  void resetIndirect();

  // ff[] is nSVertices long
  // energy is used in case of single light pool lighting
  void directShoot( MLFormFactor* ff, const Spectrum& energy );

  // lighting process
  boolean shoot( MLLightingControl& control );

  Spectrum getSVertexFinalLighting( int svi );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLMeshPoolLighting_h
