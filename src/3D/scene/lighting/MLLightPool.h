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
  MLLightPool.h

  Stephane Rehel

  March 11 1997
*/

#ifndef __MLLightPool_h
#define __MLLightPool_h

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __Color_h
#include "tools/Color.h"
#endif

#ifndef __Spectrum_h
#include "tools/Spectrum.h"
#endif

class MLScene;
class MLLight;
class MLLightingControl;

/////////////////////////////////////////////////////////////////////////////

class MLLightPool
{
  friend class MLPoolTable; // sets pool_index
  friend class MLSceneIO;

public:
  static const char* defaultPoolName;

private:
  int pool_index;

protected:
  MLScene* scene;

  MLLight** lights;
  int max_lights;
  int nLights;

public:
  enum POOL_TYPE
    {
    DIRECT_POOL= 1,     // n (>=0) direct lights
    SINGLE_GLOBAL_POOL, // only one global light and n (>=0) direct lights
    GLOBAL_POOL         // some global and n (>=0) direct lights
    };

public:
  Color color;
  double intensity;

protected:
  MLString name;
  boolean dirty;
  POOL_TYPE poolType;

  // for SINGLE_GLOBAL_POOL type
  MLLight* singleGlobalLight;

public:
  boolean dirtyIndirect; // set by MLMeshPoolLighting shooting

public:
  MLLightPool( MLScene* _scene );

  virtual ~MLLightPool();

private:
  int getIndexOfLight( MLLight* light );

public:
  POOL_TYPE getPoolType() const
    {
    return poolType;
    }
  void updatePoolType();

  // for SINGLE_GLOBAL_POOL type
  MLLight* getSingleGlobalLight() const
    {
    return singleGlobalLight;
    }
  void addLight( MLLight* light );
  void removeLight( MLLight* light );

  int getPoolIndex() const
    {
    return pool_index;
    }
  const MLString& getName() const
    {
    return name;
    }
  void setName( const MLString& _name );

  Spectrum getEnergy() const
    {
    return Spectrum(color,intensity);
    }

  int getNLights() const
    {
    return nLights;
    }
  MLLight* getLight( int i ) const
    {
    return (i>=1 && i<=nLights) ? lights[i] : 0;
    }

  boolean shoot( MLLightingControl& control );

  boolean isDirty() const
    {
    return dirty;
    }
  void setDirty();
  void setDirtyFinal();
  void initiateIndirect();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLLightPool_h
