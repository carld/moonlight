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
  XDROp.C

  Stephane Rehel
  August 4 1996
*/

#include "xdrinc.h"

#include <stdio.h>

#include "tools/MLString.h"

#include "XDROp.h"


/////////////////////////////////////////////////////////////////////////////

XDROp::XDROp()
{
  xdr= 0;
  error= true;
}

/////////////////////////////////////////////////////////////////////////////

XDROp::~XDROp()
{}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::putByte( char b )
{
  if( xdr == 0 )
    return false;

  if( ! xdr_opaque( (XDR*)xdr, &b, 1 ) )
    {
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::putChar( char ch )
{
  if( xdr == 0 )
    return false;

  if( ! xdr_char( (XDR*)xdr, &ch ) )
    {
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::putUChar( unsigned char ch )
{
  if( xdr == 0 )
    return false;

  if( ! xdr_u_char( (XDR*)xdr, &ch ) )
    {
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::putInteger( int i )
{
  if( xdr == 0 )
    return false;

  if( ! xdr_int( (XDR*)xdr, &i ) )
    {
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::putUInteger( unsigned int i )
{
  if( xdr == 0 )
    return false;

  if( ! xdr_u_int( (XDR*)xdr, &i ) )
    {
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::putShort( short i )
{
  if( xdr == 0 )
    return false;

  if( ! xdr_short( (XDR*)xdr, &i ) )
    {
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::putUShort( unsigned short i )
{
  if( xdr == 0 )
    return false;

  if( ! xdr_u_short( (XDR*)xdr, &i ) )
    {
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::putLong( long l )
{
  if( xdr == 0 )
    return false;

  if( ! xdr_long( (XDR*)xdr, &l ) )
    {
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::putULong( unsigned long l )
{
  if( xdr == 0 )
    return false;

  if( ! xdr_u_long( (XDR*)xdr, &l ) )
    {
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::putFloat( float f )
{
  if( xdr == 0 )
    return false;

  if( ! xdr_float( (XDR*)xdr, &f ) )
    {
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::putDouble( double d )
{
  if( xdr == 0 )
    return false;

  if( ! xdr_double( (XDR*)xdr, &d ) )
    {
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::putString( const MLString& s )
{
  if( xdr == 0 )
    return false;

  if( ! putInteger(s.length()) )
    return false;

  for( int i= 0; i < s.length(); ++i )
    if( ! putChar( s.charAt(i) ) )
      return false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::getByte( char* b )
{
  if( xdr == 0 || b == 0 )
    return false;

  if( ! xdr_opaque( (XDR*)xdr, b, 1 ) )
    {
    *b= char(0);
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::getChar( char* ch )
{
  if( xdr == 0 || ch == 0 )
    return false;

  if( ! xdr_char( (XDR*)xdr, ch ) )
    {
    *ch= char(0);
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::getUChar( unsigned char* ch )
{
  if( xdr == 0 || ch == 0 )
    return false;

  if( ! xdr_u_char( (XDR*)xdr, ch ) )
    {
    *ch= (unsigned char)(0);
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::getInteger( int* i )
{
  if( xdr == 0 || i == 0 )
    return false;

  if( ! xdr_int( (XDR*)xdr, i ) )
    {
    *i= int(0);
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::getUInteger( unsigned int* i )
{
  if( xdr == 0 || i == 0 )
    return false;

  if( ! xdr_u_int( (XDR*)xdr, i ) )
    {
    *i= (unsigned int)(0);
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::getShort( short* i )
{
  if( xdr == 0 || i == 0 )
    return false;

  if( ! xdr_short( (XDR*)xdr, i ) )
    {
    *i= short(0);
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::getUShort( unsigned short* i )
{
  if( xdr == 0 || i == 0 )
    return false;

  if( ! xdr_u_short( (XDR*)xdr, i ) )
    {
    *i= (unsigned short)(0);
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::getLong( long* l )
{
  if( xdr == 0 || l == 0 )
    return false;

  if( ! xdr_long( (XDR*)xdr, l ) )
    {
    *l= long(0);
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::getULong( unsigned long* l )
{
  if( xdr == 0 || l == 0 )
    return false;

  if( ! xdr_u_long( (XDR*)xdr, l ) )
    {
    *l= (unsigned long)(0);
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::getFloat( float* f )
{
  if( xdr == 0 || f == 0 )
    return false;

  if( ! xdr_float( (XDR*)xdr, f ) )
    {
    *f= float(0);
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::getDouble( double* d )
{
  if( d == 0 )
    return false;

  if( ! xdr_double( (XDR*)xdr, d ) )
    {
    *d= double(0);
    error= true;
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean XDROp::getString( MLString* s )
{
  if( xdr == 0 || s == 0 )
    return false;

  int length= 0;
  if( ! getInteger(&length) )
    return false;

  *s= MLString(length);

  for( int i= 0; i < length; ++i )
    {
    char ch;
    if( ! getChar(&ch) )
      return false;

    s->getBytes()[i]= ch;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
