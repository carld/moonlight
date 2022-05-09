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
  MLPSVertexArray.C

  Stephane Rehel

  April 7 1998
*/

#include "tools/xdr/XDRFile.h"

#include "MLPSVertexArray.h"

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPSVertexArray::_serialWrite( XDRFile* file )
{
  if( file == 0 )
    return false;

  if( ! file->putInteger(length()) )
    return false;

  for( int i= 1; i <= length(); ++i )
    {
    MLSVertex& v= array[i];

    if( ! v.serialWrite(file) )
      return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPSVertexArray::_serialRead( XDRFile* file )
{
  if( file == 0 )
    return false;

  int _length= 0;
  if( ! file->getInteger(&_length) )
    return false;

  array.realloc(_length);

  for( int i= 1; i <= _length; ++i )
    {
    MLSVertex& v= array[i];

    if( ! v.serialRead(file) )
      return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return the size (in bytes) that will be read/written by
// serialRead()/serialWrite() on this property
int MLPSVertexArray::_serialSize() const
{
  int s= 0;

  // length
  s+= XDROp::getIntegerSize();

  for( int i= 1; i <= array.length(); ++i )
    s+= array.get(i).serialSize();

  return s;
}

/////////////////////////////////////////////////////////////////////////////

MLProperty* MLPSVertexArray::clone() const
{
  MLPSVertexArray* a= new MLPSVertexArray(0);
  a->array= array;
  return a;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLPSVertexArray::elementToString( const MLSVertex& v ) const
{
  return MLString::printf( "MLSVertex@%p", &v );
}

/////////////////////////////////////////////////////////////////////////////
