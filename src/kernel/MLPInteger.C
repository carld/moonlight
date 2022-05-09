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
  MLPInteger.C

  Stephane Rehel
  April 15 1998
*/

#include "MLPInteger.h"
#include "tools/xdr/XDRFile.h"

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPInteger::_serialWrite( XDRFile* file )
{
  if( file == 0 )
    return false;

  return file->putInteger(i);
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPInteger::_serialRead( XDRFile* file )
{
  if( file == 0 )
    return false;

  return file->getInteger(&i);
}

/////////////////////////////////////////////////////////////////////////////

// return the size (in bytes) that will be read/written by
// serialRead()/serialWrite() on this property
int MLPInteger::_serialSize() const
{
  return XDROp::getIntegerSize();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLPInteger::copy( MLProperty* p ) const
{
  if( p->getType() != getType() )
    return false;

  ((MLPInteger*)p)->i= i;
  return true;
}

/////////////////////////////////////////////////////////////////////////////

MLProperty* MLPInteger::clone() const
{
  MLPInteger* ip= new MLPInteger(0);
  ip->i= i;
  return ip;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLPInteger::toString() const
{
  return MLString::valueOf(i);
}

/////////////////////////////////////////////////////////////////////////////
