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
  MLAbstractMeshAllocator.h

  Stephane Rehel

  December 12 1996
*/

#ifndef __MLAbstractMeshAllocator_h
#define __MLAbstractMeshAllocator_h

class IArray;
class MLAbstractMesh;
class MLName;
class MLString;

/////////////////////////////////////////////////////////////////////////////

class MLAbstractMeshAllocator
{
  friend class MLAbstractMeshesIterator;
protected:
  IArray* objects; // of MLAbstractMesh
  int nObjects;

public:
  MLAbstractMeshAllocator();
  virtual ~MLAbstractMeshAllocator();

  int allocID( MLAbstractMesh* );
  void freeID( int id );
  void freeID( MLAbstractMesh* );

  MLAbstractMesh* getAbstract( int id ) const;
  MLAbstractMesh* getAbstract( const MLName& name ) const;

  MLAbstractMesh* getAbstractByFilename( const MLString& filename ) const;

  int getNAbstracts() const
    {
    return nObjects;
    }

  void adjustNameIndices( MLAbstractMesh* newAO );

  void clear();
};

/////////////////////////////////////////////////////////////////////////////

#endif // __MLAbstractMeshAllocator_h
