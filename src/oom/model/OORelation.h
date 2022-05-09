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
  OORelation.h

  Stephane Rehel

  December 2 1997
*/

#ifndef __OORelation_h
#define __OORelation_h

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __OOComponent_h
#include "OOComponent.h"
#endif

class OOPackage;
class OOClass;
class OOConnection;

/////////////////////////////////////////////////////////////////////////////

class OORelation: public OOComponent
{
protected:
  OOPackage* package;

  MLString name;

  int n; // number of connections (default if 2)
  OOConnection** connections;

public:
  OORelation( OOPackage* _package, int _n = 2 );

  virtual ~OORelation();

  const MLString& getName() const
    {
    return name;
    }
  int getNConnections() const
    {
    return n;
    }
  OOConnection* getConnection( int i ) const
    {
    if( i < 1 || i > n )
      return 0;
    return connections[i];
    }

  // 1 <= i <= n
  void connect( int i, OOClass* _class );

  // 1 <= i <= n
  void unconnect( int i );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __OORelation_h
