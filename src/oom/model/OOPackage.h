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
  OOPackage.h

  Stephane Rehel

  December 2 1997
*/

#ifndef __OOPackage_h
#define __OOPackage_h

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __SIList_h
#include "tools/SIList.h"
#endif

class OOModel;
class OOClass;
class OORelation;

/////////////////////////////////////////////////////////////////////////////

class OOPackage
{
protected:
  OOModel* model;
  MLString name;

  SIList<OOClass> classes;
  SIList<OORelation> relations;

public:
  OOPackage( OOModel* _model, const MLString& _name );

  virtual ~OOPackage();

  const MLString& getName() const
    {
    return name;
    }

  OOClass* findClass( const MLString& name );

  OOClass* newClass( const MLString& name );

  OORelation* newRelation( int n );

  SIList<OOClass>& getClasses()
    {
    return classes;
    }

  SIList<OORelation>& getRelations()
    {
    return relations;
    }

};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __OOPackage_h
