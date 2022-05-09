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
  MLMaterialAllocator.h

  Stephane Rehel

  March 7 1997
*/

#ifndef __MLMaterialAllocator_h
#define __MLMaterialAllocator_h

class MLMaterial;
class MLScene;

/////////////////////////////////////////////////////////////////////////////

class MLMaterialAllocator
{
  friend class MLMaterialsIterator;
protected:
  MLScene* scene;

  MLMaterial** materials;
  int max_materials;
  int nMaterials;
  MLMaterial* defaultMaterial;

public:
  MLMaterialAllocator( MLScene* _scene );

  virtual ~MLMaterialAllocator();

  int allocID( MLMaterial* );
  void freeID( int id );
  void freeID( MLMaterial* );

  // never NULL
  MLMaterial* getDefault() const
    {
    return materials[0];
    }

  MLMaterial* getMaterial( int id ) const
    {
    if( id < 0 || id > max_materials )
      return 0;

    return materials[id];
    }

  int getNMaterials() const
    {
    return nMaterials;
    }

  void clear();
};

/////////////////////////////////////////////////////////////////////////////

#endif // __MLMaterialAllocator_h
