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
  OOClass.h

  Stephane Rehel

  December 2 1997
*/

#ifndef __OOClass_h
#define __OOClass_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __SIList_h
#include "tools/SIList.h"
#endif

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

#ifndef __OOComponent_h
#include "OOComponent.h"
#endif

class OOPackage;
class OORelation;

/////////////////////////////////////////////////////////////////////////////

class OOClass: public OOComponent
{
protected:
  OOPackage* package;

  MLString name;

  // connected relations
  SIList<OORelation> relations;

public:
  struct
    {
    int x1, y1, x2, y2;
    } draw;

public:
  OOClass( OOPackage* _package, const MLString& _name );

  virtual ~OOClass();

  const MLString& getName() const
    {
    return name;
    }

  boolean connected( OORelation* relation );

  void connect( OORelation* r );
  void unconnect( OORelation* r );

  IVector getConnectionNormal( double s ) const;
  IPoint getConnectionPoint( double s ) const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __OOClass_h
