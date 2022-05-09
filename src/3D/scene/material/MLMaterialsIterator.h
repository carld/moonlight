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
  MLMaterialsIterator.h

  Stephane Rehel

  January 5 1998
*/

#ifndef __MLMaterialsIterator_h
#define __MLMaterialsIterator_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

class MLScene;
class MLMaterial;

/////////////////////////////////////////////////////////////////////////////

class MLMaterialsIterator
{
protected:
  MLScene* scene;
  MLMaterial** materials;
  int max_materials;
  int i;

public:
  MLMaterialsIterator( MLScene* _scene );

  virtual ~MLMaterialsIterator() {}

  void reset();

  boolean eol() const
    {
    return i > max_materials;
    }

  MLMaterial* next()
    {
    if( i > max_materials )
      return 0;

    MLMaterial* m= materials[i];
    ++i;
    follow();
    return m;
    }

  virtual MLMaterial* follow()
    {
    register MLMaterial* m= 0;
    for(;;)
      {
      if( i > max_materials )
        return 0;

      m= materials[i];
      if( m != 0 )
        break;

      ++i;
      }

    return m;
    }

  MLMaterial* operator ++ (int)
    {
    return next();
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLMaterialsIterator_h
