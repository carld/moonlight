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
  MLPBoolean.C

  Stephane Rehel
  April 15 1998
*/

#include "MLPBoolean.h"
#include "tools/xdr/XDRFile.h"

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPBoolean::_serialWrite( XDRFile* file )
{
  if( file == 0 )
    return false;

  return file->putInteger( int(b) );
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPBoolean::_serialRead( XDRFile* file )
{
  if( file == 0 )
    return false;

  int i;
  if( ! file->getInteger(&i) )
    return false;

  b= boolean(i);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return the size (in bytes) that will be read/written by
// serialRead()/serialWrite() on this property
int MLPBoolean::_serialSize() const
{
  return XDROp::getIntegerSize();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLPBoolean::copy( MLProperty* p ) const
{
  if( p->getType() != getType() )
    return false;

  ((MLPBoolean*)p)->b= b;
  return true;
}

/////////////////////////////////////////////////////////////////////////////

MLProperty* MLPBoolean::clone() const
{
  MLPBoolean* bp= new MLPBoolean(0);
  bp->b= b;

  return bp;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLPBoolean::toString() const
{
  return b ? "true" : "false";
}

/////////////////////////////////////////////////////////////////////////////
