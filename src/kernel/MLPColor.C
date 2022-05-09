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
  MLPColor.C

  Stephane Rehel
  April 15 1998
*/

#include "tools/xdr/XDRFile.h"
#include "MLPColor.h"

/////////////////////////////////////////////////////////////////////////////

void MLPColor::setKey( int _frame )
{
  // anim != 0
//***  anim->setKey(_frame,c);
  frame= _frame;
}

/////////////////////////////////////////////////////////////////////////////

// return true is this property has changed
boolean MLPColor::setFrame( int _frame )
{
  if( anim == 0 )
    return false; // not animated

  if( _frame == frame )
    return false; // already set

  frame= _frame;
//***  Color _c= anim->get(frame);
Color _c;//DUMMY
  return set(_c);
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPColor::_serialWrite( XDRFile* file )
{
  if( file == 0 )
    return false;

  if( ! file->putDouble( c.r() ) )
    return false;
  if( ! file->putDouble( c.g() ) )
    return false;
  if( ! file->putDouble( c.b() ) )
    return false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPColor::_serialRead( XDRFile* file )
{
  if( file == 0 )
    return false;

  double r, g, b;
  if( ! file->getDouble(&r) )
    return false;
  if( ! file->getDouble(&g) )
    return false;
  if( ! file->getDouble(&b) )
    return false;

  c[0]= r;
  c[1]= g;
  c[2]= b;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return the size (in bytes) that will be read/written by
// serialRead()/serialWrite() on this property
int MLPColor::_serialSize() const
{
  return 3 * XDROp::getDoubleSize();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLPColor::copy( MLProperty* p ) const
{
  if( p->getType() != getType() )
    return false;

  ((MLPColor*)p)->c= c;
  return true;
}

/////////////////////////////////////////////////////////////////////////////

MLProperty* MLPColor::clone() const
{
  MLPColor* p= new MLPColor(0);
  p->c= c;
  return p;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLPColor::toString() const
{
  return MLString("rgb(") + MLString::valueOf(c.r())
                          + MLString(",")
                          + MLString::valueOf(c.g())
                          + MLString(",")
                          + MLString::valueOf(c.b())
       + MLString(")");
}

/////////////////////////////////////////////////////////////////////////////
