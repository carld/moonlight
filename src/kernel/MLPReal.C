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
  MLPReal.C

  Stephane Rehel
  April 15 1998
*/

#include "MLPReal.h"
#include "tools/xdr/XDRFile.h"

/////////////////////////////////////////////////////////////////////////////

void MLPReal::setKey( int _frame )
{
  // anim != 0
//***  anim->setKey(_frame,r);
  frame= _frame;
}

/////////////////////////////////////////////////////////////////////////////

// return true is this property has changed
boolean MLPReal::setFrame( int _frame )
{
  if( anim == 0 )
    return false; // not animated

  if( _frame == frame )
    return false; // already set

  frame= _frame;

//***  double _d= anim->get(frame);
double _r=0.;//DUMMY
  return set(_r);
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPReal::_serialWrite( XDRFile* file )
{
  if( file == 0 )
    return false;

  return file->putDouble(r);
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPReal::_serialRead( XDRFile* file )
{
  if( file == 0 )
    return false;

  return file->getDouble(&r);
}

/////////////////////////////////////////////////////////////////////////////

// return the size (in bytes) that will be read/written by
// serialRead()/serialWrite() on this property
int MLPReal::_serialSize() const
{
  return XDROp::getDoubleSize();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLPReal::copy( MLProperty* p ) const
{
  if( p->getType() != getType() )
    return false;

  ((MLPReal*)p)->r= r;
  return true;
}

/////////////////////////////////////////////////////////////////////////////

MLProperty* MLPReal::clone() const
{
  MLPReal* rp= new MLPReal(0);
  rp->r= r;

  return rp;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLPReal::toString() const
{
  return MLString::valueOf(r);
}

/////////////////////////////////////////////////////////////////////////////
