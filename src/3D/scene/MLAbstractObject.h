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
  MLAbstractObject.h

  Stephane Rehel

  February 8 1998
*/

#ifndef __MLAbstractObject_h
#define __MLAbstractObject_h

#ifndef __MLName_h
#include "MLName.h"
#endif

#ifndef __MLReference_h
#include "tools/MLReference.h"
#endif

class MLScene;
class MLObject;

/////////////////////////////////////////////////////////////////////////////

class MLAbstractObject: public MLReference
{
  friend class MLSceneIO; // set name.filename when reading

protected:
  MLScene* scene;

  MLName name;

  int* instances;
  int nInstances;
  int max_instances;

public:
  typedef unsigned int Flag;
  Flag flags;
  enum FLAG
    {
    SELECTED= (1<<0)
    };

public:
  MLAbstractObject( MLScene* _scene );

  virtual ~MLAbstractObject();

  const MLName& getName() const
    {
    return name;
    }

  void setName( const MLString& _name );

  MLScene* getScene() const
    {
    return scene;
    }

protected:
  boolean _addReference( MLObject* instance );
  boolean _removeReference( MLObject* instance );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLAbstractObject_h
