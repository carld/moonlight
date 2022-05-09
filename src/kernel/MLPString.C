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
  MLPString.C

  Stephane Rehel
  April 15 1998
*/

#include "tools/xdr/XDRFile.h"
#include "MLPString.h"

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPString::_serialWrite( XDRFile* file )
{
  if( file == 0 )
    return false;

  return file->putString(s);
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPString::_serialRead( XDRFile* file )
{
  if( file == 0 )
    return false;

  s= "";
  return file->getString(&s);
}

/////////////////////////////////////////////////////////////////////////////

// return the size (in bytes) that will be read/written by
// serialRead()/serialWrite() on this property
int MLPString::_serialSize() const
{
  return XDROp::getIntegerSize() + s.length() * XDROp::getCharSize();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLPString::copy( MLProperty* p ) const
{
  if( p->getType() != getType() )
    return false;

  ((MLPString*)p)->s= s;
  return true;
}

/////////////////////////////////////////////////////////////////////////////

MLProperty* MLPString::clone() const
{
  MLPString* p= new MLPString(0);
  p->s= s;
  return p;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLPString::toString() const
{
  return MLString("\"") + s + MLString("\"");
}

/////////////////////////////////////////////////////////////////////////////
