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
  MLAbstractCurveAllocator.C

  Stephane Rehel

  January 21 1998
*/

#include "MLAbstractCurveAllocator.h"

#include "tools/IArray.h"

#include "MLName.h"

#include "MLAbstractCurve.h"

/////////////////////////////////////////////////////////////////////////////

MLAbstractCurveAllocator::MLAbstractCurveAllocator()
{
  curves= new IArray;
  nCurves= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractCurveAllocator::~MLAbstractCurveAllocator()
{
  delete curves;
  curves= 0;
  nCurves= 0;
}

/////////////////////////////////////////////////////////////////////////////

int MLAbstractCurveAllocator::allocID( MLAbstractCurve* curve )
{
  if( curve == 0 )
    return 0;

  int hole= 0;

  for( int i= 1; i <= curves->getSize(); ++i )
    {
    register void* c= (*curves)[i];
    if( c == (void*) curve )
      return i; // already allocated
    if( c == 0 && hole == 0 )
      hole= i;
    }

  ++nCurves;
  if( hole == 0 )
    {
    curves->append( (void*) curve );
    return curves->getSize();
    }

  (*curves)[ hole ]= (void*) curve;

  return hole;
}

/////////////////////////////////////////////////////////////////////////////

void MLAbstractCurveAllocator::freeID( int id )
{
  if( id < 1 || id > curves->getSize() )
    return;
  if( (*curves)[id] == 0 )
    return;

  (*curves)[id]= 0;
  --nCurves;
}

/////////////////////////////////////////////////////////////////////////////

void MLAbstractCurveAllocator::freeID( MLAbstractCurve* curve )
{
  if( curve == 0 )
    return;

  int id= curve->abstract_curve_id;
  if( id < 1 || id > curves->getSize() )
    return;

  for( int i= 1; i <= curves->getSize(); ++i )
    {
    register void*& c= (*curves)[i];
    if( c == (void*) curve )
      {
      c= 0;
      --nCurves;
      return;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractCurve* MLAbstractCurveAllocator::getAbstract( int id ) const
{
  if( id < 1 || id > curves->getSize() )
    return 0;

  return (MLAbstractCurve*) (*curves)[id];
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractCurve*
MLAbstractCurveAllocator::getAbstract( const MLName& name ) const
{
  if( name.name.length() == 0 )
    return 0;

  for( int i= 1; i <= curves->getSize(); ++i )
    {
    register MLAbstractCurve* ac= (MLAbstractCurve*) (*curves)[i];
    if( ac == 0 )
      continue;

    if( ac->getName() == name )
      return ac;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractCurve*
MLAbstractCurveAllocator::getAbstractByFilename( const MLString& filename )
const
{
  for( int i= 1; i <= curves->getSize(); ++i )
    {
    register MLAbstractCurve* ac= (MLAbstractCurve*) (*curves)[i];
    if( ac == 0 )
      continue;

    if( ac->getName().filename == filename )
      return ac;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLAbstractCurveAllocator::adjustNameIndices( MLAbstractCurve* newAC )
{
  if( newAC == 0 )
    return;

  const MLName& name= newAC->getName();

  for( int i= 1; i <= curves->getSize(); ++i )
    {
    register MLAbstractCurve* ac= (MLAbstractCurve*) (*curves)[i];
    if( ac == 0 )
      continue;

    MLName& n= ac->name;
    if( n.name != name.name ||
        n.name_index != name.name_index )
      continue;

    if( ac == newAC )
      continue;

    // we found an abstract that points to newAO name
    // Let's change its name_index
    int ni= MLName::default_name_index;
    MLName guess= name;

    for(;;)
      {
      guess.name_index= ni;
      MLAbstractCurve* ac1= getAbstract(guess);
      if( ac1 == 0 )
        break;
      ++ni;
      }

    n.name_index= ni;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLAbstractCurveAllocator::clear()
{
  for( int i= 1; i <= curves->getSize(); ++i )
    {
    register MLAbstractCurve* ac= (MLAbstractCurve*) (*curves)[i];
    if( ac == 0 )
      continue;
    (*curves)[i]= 0;

    unref(ac);
    }

  nCurves= 0;
}

/////////////////////////////////////////////////////////////////////////////
