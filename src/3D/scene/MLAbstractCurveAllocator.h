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
  MLAbstractCurveAllocator.h

  Stephane Rehel

  January 21 1998
*/

#ifndef __MLAbstractCurveAllocator_h
#define __MLAbstractCurveAllocator_h

class IArray;
class MLAbstractCurve;
class MLName;
class MLString;

/////////////////////////////////////////////////////////////////////////////

class MLAbstractCurveAllocator
{
  friend class MLAbstractCurvesIterator;
protected:
  IArray* curves; // of MLAbstractCurve
  int nCurves;

public:
  MLAbstractCurveAllocator();

  virtual ~MLAbstractCurveAllocator();

  int allocID( MLAbstractCurve* );
  void freeID( int id );
  void freeID( MLAbstractCurve* );

  MLAbstractCurve* getAbstract( int id ) const;
  MLAbstractCurve* getAbstract( const MLName& name ) const;

  MLAbstractCurve* getAbstractByFilename( const MLString& filename ) const;

  int getNAbstracts() const
    {
    return nCurves;
    }

  void adjustNameIndices( MLAbstractCurve* newAC );

  void clear();
};

/////////////////////////////////////////////////////////////////////////////

#endif // __MLAbstractCurveAllocator_h
