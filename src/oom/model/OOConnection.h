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
  OOConnection.h

  Stephane Rehel

  December 4 1997
*/

#ifndef __OOConnection_h
#define __OOConnection_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

#ifndef __OOComponent_h
#include "OOComponent.h"
#endif

class OOClass;
class OORelation;

/////////////////////////////////////////////////////////////////////////////

class OOConnection: public OOComponent
{
protected:
  OORelation* relation;
  int i; // index in relation

public:
  OOClass* the_class;
  double position; // position of connection in class

  IPoint point;
  IVector normal;

public:
  OOConnection( OORelation* _relation, int _i );

  virtual ~OOConnection();

  void computePoint();

  void connect( OOClass* c );
  void unconnect();

  boolean connected() const
    {
    return the_class != 0;
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __OOConnection_h
