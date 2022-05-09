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
  MLPVector.C

  Stephane Rehel
  April 15 1998
*/

#include "tools/xdr/XDRFile.h"
#include "MLPVector.h"

/////////////////////////////////////////////////////////////////////////////

void MLPVector::setKey( int _frame )
{
  // anim != 0
//***  anim->setKey(_frame,v);
  frame= _frame;
}

/////////////////////////////////////////////////////////////////////////////

// return true is this property has changed
boolean MLPVector::setFrame( int _frame )
{
  if( anim == 0 )
    return false; // not animated

  if( _frame == frame )
    return false; // already set

  frame= _frame;
//***  Vector _v= anim->get(frame);
Vector _v;//DUMMY
  return set(_v);
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPVector::_serialWrite( XDRFile* file )
{
  if( file == 0 )
    return false;

  if( ! file->putDouble( v.x() ) )
    return false;
  if( ! file->putDouble( v.y() ) )
    return false;
  if( ! file->putDouble( v.z() ) )
    return false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPVector::_serialRead( XDRFile* file )
{
  if( file == 0 )
    return false;

  double x, y, z;
  if( ! file->getDouble(&x) )
    return false;
  if( ! file->getDouble(&y) )
    return false;
  if( ! file->getDouble(&z) )
    return false;

  v[0]= x;
  v[1]= y;
  v[2]= z;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return the size (in bytes) that will be read/written by
// serialRead()/serialWrite() on this property
int MLPVector::_serialSize() const
{
  return 3 * XDROp::getDoubleSize();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLPVector::copy( MLProperty* p ) const
{
  if( p->getType() != getType() )
    return false;

  ((MLPVector*)p)->v= v;
  return true;
}

/////////////////////////////////////////////////////////////////////////////

MLProperty* MLPVector::clone() const
{
  MLPVector* p= new MLPVector(0);
  p->v= v;
  return p;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLPVector::toString() const
{
  return MLString("<") + MLString::valueOf(v.x())
                       + MLString(",")
                       + MLString::valueOf(v.y())
                       + MLString(",")
                       + MLString::valueOf(v.z())
       + MLString(">");
}

/////////////////////////////////////////////////////////////////////////////
