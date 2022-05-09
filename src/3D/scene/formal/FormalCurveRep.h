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
   FormalCurveRep.h

   Stephane Rehel

   January 21 1998
*/

#ifndef __FormalCurveRep_h
#define __FormalCurveRep_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

class CurveRep;
class MLRFileBlock;
class MLWFileBlock;
class MLScene;

/////////////////////////////////////////////////////////////////////////////

class FormalCurveRep
{
public:
  enum TYPE
    {
    NULL_TYPE      = 0,
    SQUARE         = 1,
    PARAM_SEGMENT  = 2,
    PARAM_CIRCLE   = 3,
    NURBS          = 4,
    LAST_TYPE
    };

protected:
  MLScene* scene;

public:
  FormalCurveRep( MLScene* _scene );

  virtual ~FormalCurveRep();

  virtual int getSizeOf() const;

  virtual TYPE getType() const = 0;

  virtual void generate( CurveRep* curve ) const = 0;

  virtual FormalCurveRep* duplicate() = 0;

  static FormalCurveRep* newFormal( MLScene* scene, TYPE type );

  static FormalCurveRep* read( MLScene* scene, MLRFileBlock* block );
  static void write( MLWFileBlock* block, FormalCurveRep* formal );

  virtual boolean isParamCurve() const;

  static MLString getName( TYPE t );
  static TYPE getTypeFromName( const MLString& name );

protected:
  virtual void _read( MLRFileBlock* block ) = 0;
  virtual void _write( MLWFileBlock* block ) = 0;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __FormalCurveRep_h
