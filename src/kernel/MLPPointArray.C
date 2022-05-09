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
  MLPPointArray.h

  Stephane Rehel
  April 15 1998
*/

#include "tools/xdr/XDRFile.h"

#include "MLPPointArray.h"

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPPointArray::_serialWrite( XDRFile* file )
{
  if( file == 0 )
    return false;

  if( ! file->putInteger(length()) )
    return false;

  for( int i= 1; i <= length(); ++i )
    {
    const Point& v= array.get(i);
    if( ! file->putDouble(v.x()) )
      return false;
    if( ! file->putDouble(v.y()) )
      return false;
    if( ! file->putDouble(v.z()) )
      return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPPointArray::_serialRead( XDRFile* file )
{
  if( file == 0 )
    return false;

  int _length= 0;
  if( ! file->getInteger(&_length) )
    return false;

  array.realloc(_length);

  for( int i= 1; i <= _length; ++i )
    {
    double x, y, z;
    if( ! file->getDouble(&x) )
      return false;
    if( ! file->getDouble(&y) )
      return false;
    if( ! file->getDouble(&z) )
      return false;

    array[i]= Point(x,y,z);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return the size (in bytes) that will be read/written by
// serialRead()/serialWrite() on this property
int MLPPointArray::_serialSize() const
{
  return XDROp::getIntegerSize() + XDROp::getIntegerSize() * 3 * length();
}

/////////////////////////////////////////////////////////////////////////////

MLProperty* MLPPointArray::clone() const
{
  MLPPointArray* a= new MLPPointArray(0);
  a->array= array;
  return a;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLPPointArray::elementToString( const Point& v ) const
{
  return MLString("<") + MLString::valueOf(v.x())
                       + MLString(",")
                       + MLString::valueOf(v.y())
                       + MLString(",")
                       + MLString::valueOf(v.z())
       + MLString(">");
}

/////////////////////////////////////////////////////////////////////////////
