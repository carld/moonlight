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
  MLPoolTable.h

  Stephane Rehel

  March 11 1997
*/

#ifndef __MLPoolTable_h
#define __MLPoolTable_h

class MLScene;
class MLLightPool;
class MLLight;
class MLString;

/////////////////////////////////////////////////////////////////////////////

class MLPoolTable
{
protected:
  MLScene* scene;

  MLLightPool** pools;
  int nPools; // >= 1
  int max_pools;

public:
  MLPoolTable( MLScene* _scene );

  virtual ~MLPoolTable();

  void addPool( MLLightPool* pool );
  void removePool( MLLightPool* p );

  int getNPools() const
    {
    return nPools;
    }

  MLLightPool* getPool( int pool_index ) const
    {
    if( pool_index < 1 || pool_index > nPools )
      return 0;

    return pools[pool_index];
    }

  MLLightPool* getPool( const MLString& _name ) const;

  // set pool=0 for default pool
  void addLightToPool( MLLight* light, MLLightPool* pool );

  void clear();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLPoolTable_h
