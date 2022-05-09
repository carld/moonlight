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
   FormalRep.h

   November 10 1996
   Stephane Rehel
*/

#ifndef __FormalRep_h
#define __FormalRep_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

class MeshRep;
class MLRFileBlock;
class MLWFileBlock;
class MLScene;

/////////////////////////////////////////////////////////////////////////////

class FormalRep
{
public:
  enum TYPE
    {
    NULL_TYPE      = 0,
    BOX            = 1,
    MESH           = 2,
    POLYGONS       = 3,
    PARAM_CYLINDER = 4,
    PARAM_SPHERE   = 5,
    PARAM_FACE     = 6,
    PARAM_CONE     = 7,
    PARAM_TORUS    = 8,
    POLYHEDRON     = 9,
    NURBS          = 10,
    TEXT           = 11,
    FOO            = 1000,
    LAST_TYPE
    };

protected:
  MLScene* scene;

public:
  FormalRep( MLScene* _scene );

  virtual ~FormalRep();

  virtual int getSizeOf() const;

  virtual TYPE getType() const = 0;

  virtual void generate( MeshRep* mesh ) const = 0;

  virtual FormalRep* duplicate() = 0;

  static FormalRep* newFormal( MLScene* scene, TYPE type );

  static FormalRep* read( MLScene* scene, MLRFileBlock* block );
  static void write( MLWFileBlock* block, FormalRep* formal );

  virtual boolean isParamSurf() const;

  static MLString getName( TYPE t );
  static TYPE getTypeFromName( const MLString& name );

protected:
  virtual void _read( MLRFileBlock* block ) = 0;
  virtual void _write( MLWFileBlock* block ) = 0;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __FormalRep_h
