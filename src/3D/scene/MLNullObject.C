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
  MLNullObject.C

  Stephane Rehel

  October 5 1997
*/

#include "MLScene.h"
#include "MLNullObject.h"

/////////////////////////////////////////////////////////////////////////////

MLNullObject::MLNullObject( MLObject* _father ): MLObject(_father)
{}

/////////////////////////////////////////////////////////////////////////////

MLNullObject::~MLNullObject()
{}

/////////////////////////////////////////////////////////////////////////////

int MLNullObject::getSizeOf() const
{
  return MLObject::getSizeOf();
}

/////////////////////////////////////////////////////////////////////////////

// same father. Dont duplicate children.
MLObject* MLNullObject::duplicate() const
{
  if( isRoot() )
    return 0; // why do you want to duplicate root??!!

  MLNullObject* n= new MLNullObject(getFather());

  ::ref(n);

  copy(n);

  return n;
}

/////////////////////////////////////////////////////////////////////////////

// protected
void MLNullObject::copy( MLNullObject* n ) const
{
  if( n == 0 )
    return;
  if( father == 0 )
    return; // why do you want to duplicate root??!!

  MLObject::copy( (MLObject*) n );
}

/////////////////////////////////////////////////////////////////////////////

// return false if not bounded
boolean MLNullObject::getLocalBBox( Box& /*bbox*/ ) const
{
  return false;
}

/////////////////////////////////////////////////////////////////////////////

