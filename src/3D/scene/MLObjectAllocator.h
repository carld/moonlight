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
  MLObjectAllocator.h

  Stephane Rehel

  December 11 1996
*/

#ifndef __MLObjectAllocator_h
#define __MLObjectAllocator_h

class IArray;
class MLObject;
class MLName;
class MLString;

/////////////////////////////////////////////////////////////////////////////

class MLObjectAllocator
{
  friend class MLObjectsIterator;
protected:
  IArray* objects;
  int nObjects;

public:
  MLObjectAllocator();
  virtual ~MLObjectAllocator();

  int allocID( MLObject* );
  void freeID( int id );
  void detach( int id );
  void attach( int id );
  void freeID( MLObject* );

  MLObject* getObject( int id ) const;
  MLObject* getObject( const MLName& name ) const;
  MLObject* getObjectByFilename( const MLString& filename ) const;
  MLObject* getDetachedObject( int id ) const;

  int getNObjects() const
    {
    return nObjects;
    }

  boolean isValidObject( int index, MLObject* o ) const;

  void adjustNameIndices( MLObject* newO );

  void clear();
};

/////////////////////////////////////////////////////////////////////////////

#endif // __MLObjectAllocator_h
